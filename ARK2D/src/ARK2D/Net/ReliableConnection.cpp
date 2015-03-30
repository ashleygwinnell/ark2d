/*
 * ReliableConnection.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "ReliableConnection.h"

namespace ARK {
	namespace Net { 
		ReliableConnection::ReliableConnection( unsigned int protocolId, float timeout, unsigned int max_sequence ):
			Connection( protocolId, timeout ), 
			reliabilitySystem( max_sequence )
		{
			clearData();
			#ifdef NET_UNIT_TEST
				packet_loss_mask = 0;
			#endif
		}
	
		
		// overriden functions from "Connection"
				
		bool ReliableConnection::sendPacket( const unsigned char data[], int size )
		{
			#ifdef NET_UNIT_TEST
			if ( reliabilitySystem.GetLocalSequence() & packet_loss_mask )
			{
				reliabilitySystem.PacketSent( size );
				return true;
			}
			#endif
			const int header = 12;
#ifdef ARK2D_WINDOWS_VS
			unsigned char* packet = (unsigned char*) alloca(header + size);
#else
			unsigned char packet[header+size];
#endif
			unsigned int seq = reliabilitySystem.getLocalSequence();
			unsigned int ack = reliabilitySystem.getRemoteSequence();
			unsigned int ack_bits = reliabilitySystem.generateAckBits();
			writeHeader( packet, seq, ack, ack_bits );
			memcpy( packet + header, data, size );
 			if ( !Connection::sendPacket( packet, size + header ) )
				return false;
			reliabilitySystem.packetSent( size );
			return true;
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
			int received_bytes = Connection::receivePacket( packet, size + header );
			if ( received_bytes == 0 )
				return false;
			if ( received_bytes <= header )
				return false;
			unsigned int packet_sequence = 0;
			unsigned int packet_ack = 0;
			unsigned int packet_ack_bits = 0;
			readHeader( packet, packet_sequence, packet_ack, packet_ack_bits );
			reliabilitySystem.packetReceived( packet_sequence, received_bytes - header );
			reliabilitySystem.processAck( packet_ack, packet_ack_bits );
			memcpy( data, packet + header, received_bytes - header );
			return received_bytes - header;
		}
		
		void ReliableConnection::update( float deltaTime )
		{
			Connection::update( deltaTime );
			reliabilitySystem.update( deltaTime );
		}
		
		int ReliableConnection::getHeaderSize() const
		{
			return Connection::getHeaderSize() + reliabilitySystem.getHeaderSize();
		}
		
		ReliabilitySystem& ReliableConnection::getReliabilitySystem()
		{
			return reliabilitySystem;
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
		
		void ReliableConnection::onDisconnect()
		{
			clearData();
		}
		

		void ReliableConnection::clearData()
		{
			reliabilitySystem.reset();
		}

		ReliableConnection::~ReliableConnection()
		{
			if ( isRunning() ) { 
				stop();
			}
		}
		

	}
}
