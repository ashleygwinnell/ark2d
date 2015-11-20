/*
 * ReliableConnection.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "ReliableConnection.h"
#include "../ARK2D.h"
#include "../Util/Log.h"

namespace ARK {
	namespace Net { 
		ReliableConnection::ReliableConnection( unsigned int protocolId, float timeout, unsigned int max_sequence ):
			Connection( protocolId, timeout ),
			m_max_sequence(max_sequence)//reliabilitySystem( max_sequence )
		{
			clearData();
			#ifdef NET_UNIT_TEST
				packet_loss_mask = 0;
			#endif
		}

		void ReliableConnection::addAddress(Address addr) {
			Connection::addAddress(addr);

			ReliabilitySystem rs = ReliabilitySystem( m_max_sequence );
			reliabilitySystems.push_back(rs);
		}
	
		
		// overriden functions from "Connection"
				
		bool ReliableConnection::sendPacket( unsigned int addressIndex, const unsigned char data[], int size )
		{
			if (addressIndex >= m_addresses.size()) {
				ARK2D::getLog()->e("Could not send packet because addressIndex was out of bounds. ");
				return;
			}
			#ifdef NET_UNIT_TEST
				if ( reliabilitySystems[addressIndex].GetLocalSequence() & packet_loss_mask )
				{
					reliabilitySystems[addressIndex].PacketSent( size );
					return true;
				}
			#endif
			const int header = 12;
#ifdef ARK2D_WINDOWS_VS
			unsigned char* packet = (unsigned char*) alloca(header + size);
#else
			unsigned char packet[header+size];
#endif
			unsigned int seq = reliabilitySystems[addressIndex].getLocalSequence();
			unsigned int ack = reliabilitySystems[addressIndex].getRemoteSequence();
			unsigned int ack_bits = reliabilitySystems[addressIndex].generateAckBits();
			writeHeader( packet, seq, ack, ack_bits );
			memcpy( packet + header, data, size );
 			if ( !Connection::sendPacket( addressIndex, packet, size + header ) )
				return false;
			reliabilitySystems[addressIndex].packetSent( size );
			return true;
		}				
		bool ReliableConnection::sendPacketAll( const unsigned char data[], int size )
		{
			bool returnValue = true;
			for(unsigned int i = 0; i < m_addresses.size(); ++i) {
				bool b = sendPacket(i, data, size);
				if (!b) {
					returnValue = false;
				}
			}
			return returnValue;
		}	
		
		int ReliableConnection::receivePacket( unsigned char data[], int size )
		{
			const int header = 12;
			if ( size <= header )
				return false;
#ifdef ARK2D_WINDOWS_VS
			unsigned char* packet = (unsigned char*)alloca(header + size);
#else
			unsigned char packet[header + size];
#endif
			Address sender;
			int received_bytes = Connection::receivePacket(sender, packet, size + header );
			if ( received_bytes == 0 )
				return false;
			if ( received_bytes <= header )
				return false;
			unsigned int addressIndex = getAddressIndex(sender);
			unsigned int packet_sequence = 0;
			unsigned int packet_ack = 0;
			unsigned int packet_ack_bits = 0;
			readHeader( packet, packet_sequence, packet_ack, packet_ack_bits );
			reliabilitySystems[addressIndex].packetReceived( packet_sequence, received_bytes - header );
			reliabilitySystems[addressIndex].processAck( packet_ack, packet_ack_bits );
			memcpy( data, packet + header, received_bytes - header );
			return received_bytes - header;
		}
		
		void ReliableConnection::update( float deltaTime )
		{
			Connection::update( deltaTime );
			for(unsigned int i = 0; i < reliabilitySystems.size(); ++i) {
				reliabilitySystems[i].update( deltaTime );
			}
		}
		
		int ReliableConnection::getHeaderSize() const
		{
			return Connection::getHeaderSize() + reliabilitySystems[0].getHeaderSize();
		}
		
		ReliabilitySystem& ReliableConnection::getReliabilitySystem(unsigned int index)
		{
			return reliabilitySystems[index];
		}

		// unit test controls
		#ifdef NET_UNIT_TEST
			void ReliableConnection::setPacketLossMask( unsigned int mask )
			{
				packet_loss_mask = mask;
			}
		#endif
		
		void ReliableConnection::writeInteger( unsigned char * data, unsigned int value )
		{
			data[0] = (unsigned char) ( value >> 24 );
			data[1] = (unsigned char) ( ( value >> 16 ) & 0xFF );
			data[2] = (unsigned char) ( ( value >> 8 ) & 0xFF );
			data[3] = (unsigned char) ( value & 0xFF );
		}

		void ReliableConnection::writeHeader( unsigned char * header, unsigned int sequence, unsigned int ack, unsigned int ack_bits )
		{
			writeInteger( header, sequence );
			writeInteger( header + 4, ack );
			writeInteger( header + 8, ack_bits );
		}
		
		void ReliableConnection::readInteger( const unsigned char * data, unsigned int & value )
		{
 			value = ( ( (unsigned int)data[0] << 24 ) | ( (unsigned int)data[1] << 16 ) | 
				      ( (unsigned int)data[2] << 8 )  | ( (unsigned int)data[3] ) );				
		}
		
		void ReliableConnection::readHeader( const unsigned char * header, unsigned int & sequence, unsigned int & ack, unsigned int & ack_bits )
		{
			readInteger( header, sequence );
			readInteger( header + 4, ack );
			readInteger( header + 8, ack_bits );
		}

		void ReliableConnection::onStop()
		{
			clearData();
		}
		
		void ReliableConnection::clearData() {
			//reliabilitySystem.reset();
			reliabilitySystems.clear();
		}
		void ReliableConnection::onConnect(unsigned int num) {
			
		}
		void ReliableConnection::onDisconnect(unsigned int num) {
			Connection::onDisconnect(num);
			reliabilitySystems.erase(reliabilitySystems.begin() + num);
			//clearData();
		}

		unsigned int ReliableConnection::getTotalSentPackets() const {
			unsigned int count = 0;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getSentPackets();
			}
			return count;
		}
		unsigned int ReliableConnection::getTotalReceivedPackets() const {
			unsigned int count = 0;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getReceivedPackets();
			}
			return count;
		}
		unsigned int ReliableConnection::getTotalLostPackets() const {
			unsigned int count = 0;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getLostPackets();
			}
			return count;
		}
		unsigned int ReliableConnection::getTotalAckedPackets() const {
			unsigned int count = 0;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getAckedPackets();
			}
			return count;
		}
		float ReliableConnection::getAverageRoundTripTime() const {
			float count = 0.0f;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getRoundTripTime();
			}
			return count / float(reliabilitySystems.size());
		}
		float ReliableConnection::getTotalSentBandwidth() const {
			float count = 0.0f;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getSentBandwidth();
			}
			return count;
		}

		float ReliableConnection::getTotalAckedBandwidth() const {
			float count = 0.0f;
			for(unsigned int i = 0; i < reliabilitySystems.size(); i++) {
				count += reliabilitySystems[i].getAckedBandwidth();
			}
			return count;
		}

		ReliableConnection::~ReliableConnection()
		{
			if ( isRunning() ) { 
				stop();
			}
		}
		

	}
}
