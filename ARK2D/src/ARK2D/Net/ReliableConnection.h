/*
 * ReliableConnection.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_RELIABLECONNECTION_H_
#define ARK_NET_RELIABLECONNECTION_H_

#include "Connection.h"
#include "ReliabilitySystem.h"

namespace ARK {
	namespace Net {

		/*!
		 * \brief A Connection with reliability (seq/ack)
		 *
		 * @author Glenn Fiedler
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ReliableConnection : public Connection {
			public:
		
				ReliableConnection( unsigned int protocolId, float timeout, unsigned int max_sequence = 0xFFFFFFFF );
			
				// overriden functions from "Connection"
				bool sendPacket( const unsigned char data[], int size );
				int receivePacket( unsigned char data[], int size );

				void update( float deltaTime );
				
				int getHeaderSize() const;

				ReliabilitySystem & getReliabilitySystem();

				// unit test controls
				#ifdef NET_UNIT_TEST
					void SetPacketLossMask( unsigned int mask );
				#endif

				~ReliableConnection();
				
			protected:		
				
				void writeInteger( unsigned char * data, unsigned int value );

				void writeHeader( unsigned char * header, unsigned int sequence, unsigned int ack, unsigned int ack_bits );
				
				void readInteger( const unsigned char * data, unsigned int & value );
				
				void readHeader( const unsigned char * header, unsigned int & sequence, unsigned int & ack, unsigned int & ack_bits );

				virtual void onStop();
				virtual void onDisconnect();
				
			private:

				void clearData();

				#ifdef NET_UNIT_TEST
					unsigned int packet_loss_mask;			// mask sequence number, if non-zero, drop packet - for unit test only
				#endif
				
				ReliabilitySystem reliabilitySystem;	// reliability system: manages sequence numbers and acks, tracks network stats etc.
		};
	}
}

#endif /* ARK_NET_RELIABLECONNECTION_H_ */
