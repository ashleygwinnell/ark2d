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
				static const unsigned int MODE_NONE 	= 0;
				static const unsigned int MODE_CLIENT 	= 1;
				static const unsigned int MODE_SERVER 	= 2;
				//enum Mode {
				//	None,	
				//	Client,
				//	Server
				//};

				static const unsigned int STATE_DISCONNECTED 	= 0;
				static const unsigned int STATE_LISTENING 		= 1;
				static const unsigned int STATE_CONNECTING 		= 2;
				static const unsigned int STATE_CONNECT_FAIL 	= 3;
				static const unsigned int STATE_CONNECTED 		= 4;
				// enum State
				// {
				// 	Disconnected,
				// 	Listening,
				// 	Connecting,
				// 	ConnectFail,
				// 	Connected
				// };

			protected:
			
				

				unsigned int m_protocolId;
				float m_timeout;
				
				bool m_running;
				unsigned int m_mode; // Mode enum
				//unsigned int m_state; // State enum
				Socket m_socket;
				//float m_timeoutAccumulator;
				//ARK::Net::Address m_address;
			
				vector<ARK::Net::Address> m_addresses; // for servers, this is a list of clients. for clients this is the server address.
				vector<float> m_timeoutAccumulators;
				vector<unsigned int> m_states;

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
				
				//Mode getMode() const;
				unsigned int getMode() const;

				virtual void addAddress(Address addr);
				bool hasAddress(Address& sender);
				signed int getAddressIndex(Address& sender);
				vector<ARK::Net::Address>* getAddresses();
				unsigned int countAddresses();
				
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
				virtual void onConnect(unsigned int num);
				virtual void onDisconnect(unsigned int num);
				virtual void onDisconnectAll();
			
			private:
				
				void clearData();
			
				
	
		};
	}
}

#endif /* ARK_NET_CONNECTION_H_ */
