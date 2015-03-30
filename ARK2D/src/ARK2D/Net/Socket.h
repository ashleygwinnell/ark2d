/*
 * Socket.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_SOCKET_H_
#define ARK_NET_SOCKET_H_

#include "Includes.h"

namespace ARK {
	namespace Net {

		class Address;

		/*!
		 * \brief BSD-style Network Socket.
		 *
		 * @author Glenn Fiedler
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Socket {
			private:
				signed int m_socket;
			public:
				Socket();
				
				bool open(unsigned short port);
				void close();

				bool isOpen() const;

				bool send(const Address & destination, const void * data, int size);
				int receive(Address & sender, void * data, int size);
		
				~Socket();

			public:
				static bool initializeSockets();
				static void shutdownSockets();
		};
	}
}

#endif /* ARK_NET_SOCKET_H_ */
