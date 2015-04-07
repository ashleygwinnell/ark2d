/*
 * Discovery.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "Discovery.h"
#include "../ARK2D.h"
#include "../Util/Log.h"
#include "../Util/FileUtil.h"
#include "../Core/GameTimer.h"

namespace ARK {
	namespace Net { 
		
		Discovery::Discovery(unsigned int protocolId):
			m_protocolId(protocolId),
			m_mode(MODE_NONE),
			m_searchTimer(1.0f),
			m_searchDuration(1.0f)
		{
			//clearData();
		}  
		
		bool Discovery::start( int port ) {
			m_port = port;
			ARK2D::getLog()->v(StringUtil::append("start connection on port ", port));
			if (!m_socket.open(port)) { return false; }

			m_broadcastAddress = Address(255,255,255,255,31000); // 31000 = server, 31001 = client
			m_broadcastAddress.setBroadcast();

			return true;
		}
		
		void Discovery::stop() {
			m_socket.close();
		}
		
		void Discovery::search() {
			m_mode = MODE_CLIENT;
		}

		void Discovery::listen() {
			m_mode = MODE_SERVER;
		}
		
		unsigned int Discovery::getMode() const {
			return m_mode;
		}
		
		void Discovery::update( float deltaTime )
		{
			if (m_mode == MODE_CLIENT) {
				m_searchTimer += deltaTime;
				if (m_searchTimer >= m_searchDuration) {
					// do another search. 
					bool b = call();
					if (!b) {
						ARK2D::getLog()->e("did not send all 9 bytes?");
					}
					m_searchTimer = 0.0f;
				}

				// check for out of date servers
				for(unsigned int i = 0; i < m_servers.size(); ++i) {
					const DiscoveryAddress& da = m_servers[i];
					if (da.lastresponsetime < ARK2D::getTimer()->unixTimestamp() - 4) {
						m_servers.erase(m_servers.begin() + i);
						i = 0;
					}
				}
			} 

			while(true) {
				unsigned char packet[128];
				unsigned int receivedBytes = receive(packet, 128);
				if (receivedBytes == 0) {
					break; 
				}

				// Parse packet data.
				if (packet[0] == EVENT_SEARCH && m_mode == MODE_SERVER) {
					unsigned short port;
					memcpy(&port, &packet[5], 2);
					Address a(packet[1], packet[2], packet[3], packet[4], port);
					printf( "responding to client %d.%d.%d.%d:%d\n", a.getA(), a.getB(), a.getC(), a.getD(), a.getPort() );
					bool b = respond(a);
					if (!b) {
						ARK2D::getLog()->e("did not respond with all 9 bytes?");
					}
				} else if (packet[0] == EVENT_RESPONSE && m_mode == MODE_CLIENT) {
					ARK2D::getLog()->i("got address response from server");
					
					unsigned int namelen = packet[1];
					char name[255];
					memcpy(name, &packet[2], namelen);

					unsigned int addressStart = 2 + namelen;
					unsigned short port;
					memcpy(&port, &packet[addressStart+4], 2);
					Address serverAddress = Address(packet[addressStart], packet[addressStart+1], packet[addressStart+2], packet[addressStart+3], port); 

					bool foundServer = false;
					signed int foundServerIndex = -1;
					for(unsigned int i = 0; i < m_servers.size(); ++i) {
						if (serverAddress == m_servers[i].ipv4address) {
							foundServer = true;
							foundServerIndex = (signed int) i;
							break;
						}
					}
					if (!foundServer) {
						DiscoveryAddress da;
						da.name = StringUtil::append(name, "'s server"); //"test name";
						da.ipv4address = serverAddress;
						da.lastresponsetime = ARK2D::getTimer()->unixTimestamp();
						m_servers.push_back(da);
					} else { 
						m_servers[foundServerIndex].lastresponsetime = ARK2D::getTimer()->unixTimestamp();
					}
					
				} else {
					ARK2D::getLog()->i("received *something*");
				}
			}
		}
 
		bool Discovery::call() {
			ARK2D::getLog()->i("Discovery::call");
			unsigned int packetLen = 4 + 1;
			unsigned char* packet = (unsigned char*)alloca(packetLen);
			packet[0] = (unsigned char) ( m_protocolId >> 24 );
			packet[1] = (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF );
			packet[2] = (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF );
			packet[3] = (unsigned char) ( ( m_protocolId ) & 0xFF );
			packet[4] = EVENT_SEARCH;
			return m_socket.send( m_broadcastAddress, packet, packetLen);
		}
		bool Discovery::respond(Address& a) {
			ARK2D::getLog()->i("Discovery::respond");
			string username = FileUtil::getOSUsername();

			unsigned int packetLen = 4 + 1 + 1 + username.length();
			unsigned char* packet = (unsigned char*)alloca(packetLen);
			packet[0] = (unsigned char) ( m_protocolId >> 24 );
			packet[1] = (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF );
			packet[2] = (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF );
			packet[3] = (unsigned char) ( ( m_protocolId ) & 0xFF );
			packet[4] = EVENT_RESPONSE;
			packet[5] = username.length();
			memcpy(&packet[6], username.c_str(), packet[5]);

			return m_socket.send( a, packet, packetLen);
		}
		
		
		int Discovery::receive(unsigned char data[], int size )
		{
			Address sender;
			unsigned char* packet = (unsigned char*)alloca(size + 4);
			int bytes_read = m_socket.receive( sender, packet, size + 4);
			if ( bytes_read == 0 ) { return 0; }
			if ( bytes_read <= 4 ) { return 0; }
			if ( packet[0] != (unsigned char) ( m_protocolId >> 24 ) ||
				 packet[1] != (unsigned char) ( ( m_protocolId >> 16 ) & 0xFF ) ||
				 packet[2] != (unsigned char) ( ( m_protocolId >> 8 ) & 0xFF ) ||
				 packet[3] != (unsigned char) ( m_protocolId & 0xFF ) ) {
				return 0;
			}
			unsigned int startIndex = bytes_read;

			if (m_mode == MODE_SERVER) 
			{
				printf( "server accepts connection from client %d.%d.%d.%d:%d\n", sender.getA(), sender.getB(), sender.getC(), sender.getD(), sender.getPort() );
				printf( "bytes read: %d\n", bytes_read );
				//packet[4] = EVENT TYPE
				packet[startIndex] = sender.getA(); // client info
				packet[startIndex+1] = sender.getB();
				packet[startIndex+2] = sender.getC();
				packet[startIndex+3] = sender.getD();
				unsigned short port = sender.getPort();
				memcpy(&packet[startIndex+4], &port, sizeof(unsigned short));
				//packet[10] // used up by unsigned short type
			} 
			else if (m_mode == MODE_CLIENT) 
			{
				packet[startIndex] = sender.getA(); // server info
				packet[startIndex+1] = sender.getB();
				packet[startIndex+2] = sender.getC();
				packet[startIndex+3] = sender.getD();
				unsigned short port = sender.getPort();
				memcpy(&packet[startIndex+4], &port, sizeof(unsigned short));
				//packet[9] = sender.getPort();
				//packet[10] // used up by unsigned short type
			}

			memcpy( data, &packet[4], bytes_read + 6 - 4 );
			return bytes_read + 6 - 4;
		}
		vector<DiscoveryAddress>* Discovery::getServers() {
			return &m_servers;
		}
		
		Discovery::~Discovery() {
			stop();
		}

	}
}
