/*
 * Connection.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_CONNECTION_H_
#define ARK_NET_CONNECTION_H_

#include "Includes.h"
#include "Socket.h"
#include "Address.h"

namespace ARK {
	namespace Net {
		
		//class ARK2D_API Address;

		/*!
		 * \brief A connection on an Address and Socket
		 *
		 * @author Glenn Fiedler
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Connection {
			public:
				enum Mode
				{
					None,	
					Client,
					Server
				};

			private:
				enum State
				{
					Disconnected,
					Listening,
					Connecting,
					ConnectFail,
					Connected
				};

				unsigned int m_protocolId;
				float m_timeout;
				
				bool m_running;
				Mode m_mode;
				State m_state;
				Socket m_socket;
				float m_timeoutAccumulator;
				ARK::Net::Address m_address;
				vector<ARK::Net::Address> m_addresses;


			public:
				
				Connection();
				Connection( unsigned int protocolId, float timeout );
				
				bool start( int port );
				void stop();
				
				void connect( const Address & address );
				void listen();
				bool isRunning() const;
				bool isConnecting() const;
				bool isConnected() const;
				bool isListening() const;
				bool connectFailed() const;
				
				Mode getMode() const;
				
				virtual void update( float deltaTime );
				
				virtual bool sendPacket(const unsigned char data[], int size);
				virtual int receivePacket(unsigned char data[], int size);
				
				int getHeaderSize() const;

				virtual ~Connection();
				
			protected:
				
				virtual void onStart()		{ }
				virtual void onStop()		{ }
				virtual void onConnect()    { }
				virtual void onDisconnect() { }
					
			private:
				
				void clearData();
			
				
	
		};
	}
}

#endif /* ARK_NET_CONNECTION_H_ */
