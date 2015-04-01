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

			protected:
			
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
				//ARK::Net::Address m_address;
			
				vector<ARK::Net::Address> m_addresses; // for servers, this is a list of clients. for clients this is the server address.


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

				virtual void addAddress(Address addr);
				bool hasAddress(Address& sender);
				signed int getAddressIndex(Address& sender);
				
				virtual void update( float deltaTime );
				
				//virtual bool sendPacket(const unsigned char data[], int size);
				virtual bool sendPacket(unsigned int addressIndex, const unsigned char data[], int size);
				virtual bool sendPacketAll(const unsigned char data[], int size);
				virtual int receivePacket(Address& sender, unsigned char data[], int size);
				
				int getHeaderSize() const;

				virtual ~Connection();
				
			protected:
				
				virtual void onStart()		{ }
				virtual void onStop()		{ }
				virtual void onConnect(unsigned int num)    { }
				virtual void onDisconnect() { }
					
			private:
				
				void clearData();
			
				
	
		};
	}
}

#endif /* ARK_NET_CONNECTION_H_ */
