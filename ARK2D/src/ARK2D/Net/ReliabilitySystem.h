/*
 * ReliabilitySystem.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_RELIABILITYSYSTEM_H_
#define ARK_NET_RELIABILITYSYSTEM_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "Includes.h"
#include "PacketQueue.h"

namespace ARK {
	namespace Net {

		/*!
		 * \brief Reliability system to support reliable connection.
		 *		+ Manages sent, received, pending ack and acked packet queues
		 *		+ separated out from reliable connection because it is quite complex (and i want to unit test it!)
		 *
		 * @author Glenn Fiedler
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ReliabilitySystem {
			private:

				unsigned int max_sequence;			// maximum sequence value before wrap around (used to test sequence wrap at low # values)
				unsigned int local_sequence;		// local sequence number for most recently sent packet
				unsigned int remote_sequence;		// remote sequence number for most recently received packet

				unsigned int sent_packets;			// total number of packets sent
				unsigned int recv_packets;			// total number of packets received
				unsigned int lost_packets;			// total number of packets lost
				unsigned int acked_packets;			// total number of packets acked

				float sent_bandwidth;				// approximate sent bandwidth over the last second
				float acked_bandwidth;				// approximate acked bandwidth over the last second
				float rtt;							// estimated round trip time
				float rtt_maximum;					// maximum expected round trip time (hard coded to one second for the moment)

				std::vector<unsigned int> acks;		// acked packets from last set of packet receives. cleared each update!

				PacketQueue sentQueue;				// sent packets used to calculate sent bandwidth (kept until rtt_maximum)
				PacketQueue pendingAckQueue;		// sent packets which have not been acked yet (kept until rtt_maximum * 2 )
				PacketQueue receivedQueue;			// received packets for determining acks to send (kept up to most recent recv sequence - 32)
				PacketQueue ackedQueue;				// acked packets (kept until rtt_maximum * 2)

			public:

				ReliabilitySystem( unsigned int max_sequence = 0xFFFFFFFF );

				void reset();

				void packetSent( int size );
				void packetReceived( unsigned int sequence, int size );

				unsigned int generateAckBits();
				void processAck( unsigned int ack, unsigned int ack_bits );

				void update( float deltaTime );

				void validate();

				// data accessors
				unsigned int getLocalSequence() const;
				unsigned int getRemoteSequence() const;
				unsigned int getMaxSequence() const;
		 		void getAcks( unsigned int ** acks, int & count );
				unsigned int getSentPackets() const;
				unsigned int getReceivedPackets() const;
				unsigned int getLostPackets() const;
				unsigned int getAckedPackets() const;
				float getSentBandwidth() const;
				float getAckedBandwidth() const;
				float getRoundTripTime() const;
				int getHeaderSize() const;



			protected:

				void advanceQueueTime( float deltaTime );
				void updateQueues();
				void updateStats();

			public:
				// utility functions
				static bool sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence );
				static int bit_index_for_sequence( unsigned int sequence, unsigned int ack, unsigned int max_sequence );
				static unsigned int generate_ack_bits( unsigned int ack, const PacketQueue & received_queue, unsigned int max_sequence );
				static void process_ack( unsigned int ack,
										 unsigned int ack_bits,
										 PacketQueue & pending_ack_queue,
										 PacketQueue & acked_queue,
										 std::vector<unsigned int> & acks,
										 unsigned int & acked_packets,
										 float & rtt,
										 unsigned int max_sequence );


		};
	}
}

#endif /* ARK_NET_SOCKET_H_ */
