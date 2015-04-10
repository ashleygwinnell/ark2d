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
				unsigned int m_max_sequence;
				ReliableConnection( unsigned int protocolId, float timeout, unsigned int max_sequence = 0xFFFFFFFF );
			
				// overriden functions from "Connection"
				bool sendPacket( unsigned int addressIndex, const unsigned char data[], int size );
				bool sendPacketAll( const unsigned char data[], int size );
				int receivePacket( unsigned char data[], int size );

				void update( float deltaTime );
				
				int getHeaderSize() const;

				ReliabilitySystem & getReliabilitySystem(unsigned int index);

				// unit test controls
				#ifdef NET_UNIT_TEST
					void SetPacketLossMask( unsigned int mask );
				#endif

				void addAddress(Address addr);

				unsigned int getTotalSentPackets() const;
				unsigned int getTotalReceivedPackets() const;
				unsigned int getTotalLostPackets() const;
				unsigned int getTotalAckedPackets() const;
				float getAverageRoundTripTime() const ;
				float getTotalSentBandwidth() const;
				float getTotalAckedBandwidth() const;

				~ReliableConnection();
				
			protected:		
				
				static void writeInteger( unsigned char * data, unsigned int value );

				static void writeHeader( unsigned char * header, unsigned int sequence, unsigned int ack, unsigned int ack_bits );
				
				static void readInteger( const unsigned char * data, unsigned int & value );
				
				static void readHeader( const unsigned char * header, unsigned int & sequence, unsigned int & ack, unsigned int & ack_bits );

				virtual void onStop();
				virtual void onConnect(unsigned int num);
				virtual void onDisconnect(unsigned int num);
				
			private:

				void clearData();

				#ifdef NET_UNIT_TEST
					unsigned int packet_loss_mask;			// mask sequence number, if non-zero, drop packet - for unit test only
				#endif
				
				//ReliabilitySystem reliabilitySystem;	// reliability system: manages sequence numbers and acks, tracks network stats etc.
				vector<ReliabilitySystem> reliabilitySystems;
		};
	}
}

#endif /* ARK_NET_RELIABLECONNECTION_H_ */
