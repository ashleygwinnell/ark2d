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
			m_mode(None),
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
			m_mode = Server;
			m_state = Listening;
		}
		
		void Connection::connect( const Address & address ) // clients call this.
		{
			printf( "client connecting to %d.%d.%d.%d:%d\n", address.getA(), address.getB(), address.getC(), address.getD(), address.getPort() );
			bool connected = isConnected();
			clearData();
			if ( connected ) { 
				onDisconnect();
			}
			m_mode = Client;
			m_state = Connecting;
			m_address = address;
		}
		
		bool Connection::isConnecting() const { 
			return m_state == Connecting; 
		}
		
		bool Connection::connectFailed() const { 
			return m_state == ConnectFail; 
		}
		
		bool Connection::isConnected() const { 
			return m_state == Connected; 
		}
		
		bool Connection::isListening() const { 
			return m_state == Listening; 
		}
		
		Connection::Mode Connection::getMode() const {
			return m_mode;
		}
		
		void Connection::update( float deltaTime )
		{
			assert( m_running );
			m_timeoutAccumulator += deltaTime;
			if ( m_timeoutAccumulator > m_timeout )
			{
				if ( m_state == Connecting )
				{
					ARK2D::getLog()->v( "connect timed out\n" );
					clearData();
					m_state = ConnectFail;
					onDisconnect();
				}
				else if ( m_state == Connected )
				{
					ARK2D::getLog()->v( "connection timed out\n" );
					clearData();
					if ( m_state == Connecting ) { 
						m_state = ConnectFail;
					}
					onDisconnect();
				}
			}
		}
		
		bool Connection::sendPacket( const unsigned char data[], int size )
		{
			assert( m_running );
			if ( m_address.getAddress() == 0 ) {
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
			return m_socket.send( m_address, packet, size + 4 );
		}
		
		int Connection::receivePacket( unsigned char data[], int size )
		{
			assert( m_running );
#ifdef ARK2D_WINDOWS_VS
			unsigned char* packet = (unsigned char*)alloca(size + 4);
#else
			unsigned char packet[size + 4];
#endif
			Address sender;
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
			if ( m_mode == Server && !isConnected() )
			{
				printf( "server accepts connection from client %d.%d.%d.%d:%d\n", sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort() );
				m_state = Connected;
				m_address = sender;
				m_addresses.push_back(sender);
				onConnect();
			} else if (m_mode == Server && isConnected()) {
				// second/third/fourth client
				printf( "server accepts connection from another client %d.%d.%d.%d:%d\n", sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort() );
				m_addresses.push_back(sender);

			}

			bool foundSender = false;
			for(unsigned int i = 0; i < m_addresses.size(); ++i) {
				Address& addr = m_addresses[0]; 
				if (sender.getA() == addr.getA() && 
					sender.getB() == addr.getB() &&
					sender.getC() == addr.getC() &&
					sender.getD() == addr.getD() &&
					sender.getPort() == addr.getPort()) 
				{
					foundSender = true;
					break;
				}

			}
			if ( foundSender )
			//if ( sender == m_address )
			{
				if ( m_mode == Client && m_state == Connecting )
				{
					ARK2D::getLog()->v( "client completes connection with server\n" );
					m_state = Connected;
					onConnect();
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
		
	
			
		
		void Connection::clearData()
		{
			m_state = Disconnected;
			m_timeoutAccumulator = 0.0f;
			m_address = Address();
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
