/*
 * Connection.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "Connection.h"
#include "../ARK2D.h"
#include "../Util/Log.h"

namespace ARK {
	namespace Net { 
		
		Connection::Connection(unsigned int protocolId, float timeout):
			m_protocolId(protocolId),
			m_timeout(timeout),
			m_mode(MODE_NONE),
			m_running(false)
		{
			clearData();
		}  
		
		bool Connection::start( int port ) 
		{
			assert(!m_running);
			ARK2D::getLog()->v(StringUtil::append("start connection on port ", port));
			if (!m_socket.open(port)) { 
				return false;
			}
			m_running = true;
			onStart();
			return true;
		}
		
		void Connection::stop()
		{
			assert( m_running );
			ARK2D::getLog()->v( "stop connection" );
			bool connected = isConnected();
			clearData();
			m_socket.close();
			m_running = false;
			if ( connected ) {
				onDisconnect();
			}
			onStop();
		}
		
		bool Connection::isRunning() const {
			return m_running;
		}
		
		void Connection::listen() // server calls this
		{
			ARK2D::getLog()->v( "server listening for connection" );
			bool connected = isConnected();
			clearData();
			if ( connected ) { 
				onDisconnect();
			}
			m_mode = MODE_SERVER;
			m_state = STATE_LISTENING;
		}
		
		void Connection::connect( const Address & address ) // clients call this.
		{
			printf( "client connecting to %d.%d.%d.%d:%d\n", address.getA(), address.getB(), address.getC(), address.getD(), address.getPort() );
			bool connected = isConnected();
			clearData();
			if ( connected ) { 
				onDisconnect();
			}
			m_mode = MODE_CLIENT;
			m_state = STATE_CONNECTING;
//			m_address = address;
			addAddress(address);
		}
		
		bool Connection::isConnecting() const { 
			return m_state == STATE_CONNECTING; 
		}
		
		bool Connection::connectFailed() const { 
			return m_state == STATE_CONNECT_FAIL; 
		}
		
		bool Connection::isConnected() const { 
			return m_state == STATE_CONNECTED; 
		}
		
		bool Connection::isListening() const { 
			return m_state == STATE_LISTENING; 
		}
		
		//Connection::Mode Connection::getMode() const {
		unsigned int Connection::getMode() const {
			return m_mode;
		}
		
		void Connection::update( float deltaTime )
		{
			assert( m_running );
			m_timeoutAccumulator += deltaTime;
			if ( m_timeoutAccumulator > m_timeout )
			{
				if ( m_state == STATE_CONNECTING )
				{
					ARK2D::getLog()->v( "connect timed out\n" );
					clearData();
					m_state = STATE_CONNECT_FAIL;
					onDisconnect();
				}
				else if ( m_state == STATE_CONNECTED )
				{
					ARK2D::getLog()->v( "connection timed out\n" );
					clearData();
					if ( m_state == STATE_CONNECTING ) { 
						m_state = STATE_CONNECT_FAIL;
					}
					onDisconnect();
				}
			}
		}
		
		bool Connection::sendPacketAll( const unsigned char data[], int size )
		{
			assert( m_running );
			if ( m_addresses.size() == 0 ||
				(m_addresses.size() == 1 && m_addresses[0].getAddress() == 0 )) {
				return false;
			}
			#ifdef ARK2D_WINDOWS_VS
				unsigned char* packet = (unsigned char*)alloca(size+4); // headersize 4 bytes (protocol id)
			#else
				unsigned char packet[size+4];
			#endif
			packet[0] = (unsigned char) ( m_protocolId >> 24 );
			packet[1] = (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF );
			packet[2] = (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF );
			packet[3] = (unsigned char) ( ( m_protocolId ) & 0xFF );
			memcpy( &packet[4], data, size );
			
			bool returnValue = true;
			for(unsigned int i = 0; i < m_addresses.size(); i++) { 
				bool b = m_socket.send( m_addresses[i], packet, size + 4 );
				if (!b) {
					returnValue = false;
				}
			}
			return returnValue; // investigate!
		}
		bool Connection::sendPacket( unsigned int addressIndex, const unsigned char data[], int size )
		{
			assert( m_running );
			if ( m_addresses.size() == 0 || m_addresses[addressIndex].getAddress() == 0 ) {
				return false;
			}
			
			#ifdef ARK2D_WINDOWS_VS
				unsigned char* packet = (unsigned char*)alloca(size+4);
			#else
				unsigned char packet[size+4];
			#endif
			packet[0] = (unsigned char) ( m_protocolId >> 24 );
			packet[1] = (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF );
			packet[2] = (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF );
			packet[3] = (unsigned char) ( ( m_protocolId ) & 0xFF );
			memcpy( &packet[4], data, size );
			return m_socket.send( m_addresses[addressIndex], packet, size + 4 );
		}
				
		
		
		int Connection::receivePacket(Address& sender, unsigned char data[], int size )
		{
			assert( m_running );
#ifdef ARK2D_WINDOWS_VS
			unsigned char* packet = (unsigned char*)alloca(size + 4);
#else
			unsigned char packet[size + 4];
#endif
			//Address sender;
			int bytes_read = m_socket.receive( sender, packet, size + 4 );
			if ( bytes_read == 0 ) {
				return 0;
			}
			if ( bytes_read <= 4 ) { 
				return 0;
			}
			if ( packet[0] != (unsigned char) ( m_protocolId >> 24 ) ||
				 packet[1] != (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF ) ||
				 packet[2] != (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF ) ||
				 packet[3] != (unsigned char) ( m_protocolId & 0xFF ) ) {
				return 0;
			}
			if ( m_mode == MODE_SERVER && !isConnected() )
			{
				printf( "server accepts connection from client %d.%d.%d.%d:%d\n", sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort() );
				m_state = STATE_CONNECTED;
				//m_address = sender;
				addAddress(sender);
				onConnect(0);
			} else if (m_mode == MODE_SERVER && isConnected() && !hasAddress(sender)) {
				// second/third/fourth client
				printf( "server accepts connection from another client %d.%d.%d.%d:%d\n", sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort() );
				onConnect(m_addresses.size());
				addAddress(sender);
			}

			bool foundSender = hasAddress(sender);
			if ( foundSender )
			//if ( sender == m_address )
			{
				if ( m_mode == MODE_CLIENT && m_state == STATE_CONNECTING )
				{
					ARK2D::getLog()->v( "client completes connection with server\n" );
					m_state = STATE_CONNECTED;
					onConnect(0);
				}
				m_timeoutAccumulator = 0.0f;
				memcpy( data, &packet[4], bytes_read - 4 );
				return bytes_read - 4;
			}
			return 0;
		}
		
		int Connection::getHeaderSize() const
		{
			return 4;
		}
		
		void Connection::addAddress(Address addr) {
			m_addresses.push_back(addr);
		}
		bool Connection::hasAddress(Address& sender) 
		{
			return getAddressIndex(sender) >= 0;
		}
		signed int Connection::getAddressIndex(Address& sender) {
			for(unsigned int i = 0; i < m_addresses.size(); ++i) {
				Address& addr = m_addresses[i]; 
				if (sender.getA() == addr.getA() && 
					sender.getB() == addr.getB() &&
					sender.getC() == addr.getC() &&
					sender.getD() == addr.getD() &&
					sender.getPort() == addr.getPort()) 
				{
					return (signed int) i;
				}
			}
			return -1;
		}
			
		
		void Connection::clearData()
		{
			m_state = STATE_DISCONNECTED;
			m_timeoutAccumulator = 0.0f;
			//m_address = Address();
			m_addresses.clear();
		}

		Connection::~Connection()
		{
			if ( isRunning() ) { 
				stop();
			}
		}

	}
}
