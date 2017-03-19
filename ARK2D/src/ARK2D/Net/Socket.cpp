/*
 * Socket.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "Includes.h"
#include "Socket.h"
#include "Address.h"
#include "../Core/ARK2D.h"
#include "../Core/Log.h"

namespace ARK {
	namespace Net {

		bool Socket::initializeSockets()
		{
			#if defined(ARK2D_WINDOWS)
		    	WSADATA wsaData;
				int err = WSAStartup( MAKEWORD(2,2), &wsaData );
				if (err == NO_ERROR) {

					if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
				        /* Tell the user that we could not find a usable */
				        /* WinSock DLL.                                  */
				        printf("Could not find a usable version of Winsock.dll\n");
				       	shutdownSockets();
				        return false;
				    } else {
				        printf("The Winsock 2.2 dll was found okay\n");
				    }

					return true;
				}
				ARK2D::getLog()->e(StringUtil::append("WSAStartup failed with error: ", err));
				return false;
			#else
				return true;
			#endif
		}

		void Socket::shutdownSockets()
		{
			#if defined(ARK2D_WINDOWS)
				WSACleanup();
			#endif
		}

		Socket::Socket():
			m_socket(0)
		{

		}

		bool Socket::open( unsigned short port )
		{
			assert( !isOpen() );

			// create socket
			m_socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

			if ( m_socket <= 0 )
			{
				ARK2D::getLog()->e( "failed to create socket" );
				m_socket = 0;
				return false;
			}

			// bind to port
			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = INADDR_ANY;
			address.sin_port = htons( (unsigned short) port );

			if ( ::bind( (unsigned int) m_socket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
			{
				ARK2D::getLog()->e( "failed to bind socket" );
				close();
				return false;
			}

			// set non-blocking io
			#if defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX)

				int nonBlocking = 1;
				if ( fcntl( m_socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
				{
					ARK2D::getLog()->e( "failed to set non-blocking socket" );
					close();
					return false;
				}

			#elif defined(ARK2D_WINDOWS)

				DWORD nonBlocking = 1;
				if ( ioctlsocket( m_socket, FIONBIO, &nonBlocking ) != 0 )
				{
					ARK2D::getLog()->e( "failed to set non-blocking socket" );
					close();
					return false;
				}

			#endif

			return true;
		}

		void Socket::close()
		{
			if (m_socket != 0)
			{
				#if defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX)
					::close( m_socket );
				#elif defined(ARK2D_WINDOWS)
					closesocket( m_socket );
				#endif
				m_socket = 0;
			}
		}

		bool Socket::isOpen() const {
			return m_socket != 0;
		}

		bool Socket::send( const Address& destination, const void* data, int size )
		{
			assert( data );
			assert( size > 0 );

			if ( m_socket == 0 ) {
				return false;
			}

			assert( destination.getAddress() != 0 );
			assert( destination.getPort() != 0 );

			if (destination.getAddress() == INADDR_BROADCAST) {
				ARK2D::getLog()->i("sending to broadcast.");
				char broadcastPermission[] = "TRUE";
				if ((setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*) broadcastPermission, sizeof(broadcastPermission))) < 0)
				{
#ifdef ARK2D_WINDOWS
					printf("%d",WSAGetLastError());
#endif
					while(1);
					//exit(1);
				}

			}

			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = htonl( destination.getAddress() );
			address.sin_port = htons( (unsigned short) destination.getPort() );

			int sent_bytes = sendto( m_socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in) );

			return sent_bytes == size;
		}

		int Socket::receive( Address & sender, void * data, int size )
		{
			assert( data );
			assert( size > 0 );

			if ( m_socket == 0 ) {
				return false;
			}

			#if defined(ARK2D_WINDOWS) || defined(ARK2D_WINDOWS_VS)
                typedef int socklen_t;
			#endif

			sockaddr_in from;
			socklen_t fromLength = sizeof( from );

			#if defined(ARK2D_WINDOWS) || defined(ARK2D_WINDOWS_VS) || defined(ARK2D_ANDROID)
				int received_bytes = recvfrom( m_socket, (char*)data, size, 0, (sockaddr*)&from, (int*) &fromLength );
			#else
				int received_bytes = recvfrom( m_socket, (char*)data, size, 0, (sockaddr*)&from, (unsigned int*) &fromLength );
			#endif

			if ( received_bytes <= 0 )
				return 0;

			unsigned int address = ntohl( from.sin_addr.s_addr );
			unsigned short port = ntohs( from.sin_port );

			sender = Address( address, port );

			return received_bytes;
		}

		Socket::~Socket()
		{
			close();
		}
	}
}
