/*
 * Connection.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_DISCOVERY_H_
#define ARK_NET_DISCOVERY_H_

#include "Includes.h"
#include "Socket.h"
#include "Address.h"

namespace ARK {
	namespace Net {
		
		struct DiscoveryAddress {
			string name;
			Address ipv4address;
			unsigned long lastresponsetime;
		};

		/*!
		 * \brief Discover servers running on the local area network. 
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Discovery {
			public:
				static const unsigned int MODE_NONE 	= 0;
				static const unsigned int MODE_CLIENT 	= 1;
				static const unsigned int MODE_SERVER 	= 2;

				static const unsigned char EVENT_SEARCH = 100;
				static const unsigned char EVENT_RESPONSE = 200;

			protected:
			
				unsigned int m_protocolId;
				unsigned int m_port;
				unsigned int m_mode;
				unsigned int m_state;
				Socket m_socket;

				Address m_broadcastAddress;
				vector<DiscoveryAddress> m_servers;

				float m_searchTimer;
				float m_searchDuration;

				bool m_running;

			public:
				
				Discovery();
				Discovery( unsigned int protocolId );

				bool isRunning();
				
				unsigned int getMode() const;

				bool start( int port );
				void stop();
				
				void search();
				void listen();
				
				bool call();
				bool respond(Address& a);
				int receive(unsigned char data[], int size);
				
				void update( float deltaTime );

				vector<DiscoveryAddress>* getServers();
				
				virtual ~Discovery();
				
			protected:
				
				//virtual void onStart()		{ }
				//virtual void onStop()		{ }
				//virtual void onConnect()    { }
				//virtual void onDisconnect() { }
					
			private:
				
				//void clearData();
			
				
	
		};
	}
}

#endif /* ARK_NET_DISCOVERY_H_ */
