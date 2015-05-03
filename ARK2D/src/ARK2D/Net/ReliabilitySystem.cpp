/*
 * ReliabilitySystem.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "ReliabilitySystem.h"

namespace ARK {
	namespace Net { 
		
		ReliabilitySystem::ReliabilitySystem( unsigned int max_sequence )
		{
			this->rtt_maximum = 1.0;
			this->max_sequence = max_sequence;
			reset();
		}
		
		void ReliabilitySystem::reset()
		{
			local_sequence = 0;
			remote_sequence = 0;
			sentQueue.clear();
			receivedQueue.clear();
			pendingAckQueue.clear();
			ackedQueue.clear();
			sent_packets = 0;
			recv_packets = 0;
			lost_packets = 0;
			acked_packets = 0;
			sent_bandwidth = 0.0f;
			acked_bandwidth = 0.0f;
			rtt = 0.0f;
			rtt_maximum = 1.0f;
		}
		
		void ReliabilitySystem::packetSent( int size )
		{
			if ( sentQueue.exists( local_sequence ) )
			{
				printf( "local sequence %d exists\n", local_sequence );				
				for ( PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor ) { 
					printf( " + %d\n", itor->sequence );
				}
			}
			assert( !sentQueue.exists( local_sequence ) );
			assert( !pendingAckQueue.exists( local_sequence ) );
			PacketData data;
			data.sequence = local_sequence;
			data.time = 0.0f;
			data.size = size;
			sentQueue.push_back( data );
			pendingAckQueue.push_back( data );
			sent_packets++;
			local_sequence++;
			if ( local_sequence > max_sequence ) { 
				local_sequence = 0;
			}
		}
		
		void ReliabilitySystem::packetReceived( unsigned int sequence, int size )
		{
			recv_packets++;
			if ( receivedQueue.exists( sequence ) ) { 
				return;
			}
			PacketData data;
			data.sequence = sequence;
			data.time = 0.0f;
			data.size = size;
			receivedQueue.push_back( data );
			if ( sequence_more_recent( sequence, remote_sequence, max_sequence ) ) { 
				remote_sequence = sequence;
			}
		}

		unsigned int ReliabilitySystem::generateAckBits()
		{
			return generate_ack_bits( getRemoteSequence(), receivedQueue, max_sequence );
		}
		
		void ReliabilitySystem::processAck( unsigned int ack, unsigned int ack_bits )
		{
			process_ack( ack, ack_bits, pendingAckQueue, ackedQueue, acks, acked_packets, rtt, max_sequence );
		}
				
		void ReliabilitySystem::update( float deltaTime )
		{
			acks.clear();
			advanceQueueTime( deltaTime );
			updateQueues();
			updateStats();
			#ifdef NET_UNIT_TEST
			Validate();
			#endif
		}
		
		void ReliabilitySystem::validate()
		{
			sentQueue.verify_sorted( max_sequence );
			receivedQueue.verify_sorted( max_sequence );
			pendingAckQueue.verify_sorted( max_sequence );
			ackedQueue.verify_sorted( max_sequence );
		}

		
		
		// data accessors
		unsigned int ReliabilitySystem::getLocalSequence() const {
			return local_sequence;
		}

		unsigned int ReliabilitySystem::getRemoteSequence() const {
			return remote_sequence;
		}
		
		unsigned int ReliabilitySystem::getMaxSequence() const {
			return max_sequence;
		}
				
 		void ReliabilitySystem::getAcks( unsigned int ** acks, int & count ) {
			*acks = &this->acks[0];
			count = (int) this->acks.size();
		}
		
		unsigned int ReliabilitySystem::getSentPackets() const {
			return sent_packets;
		}
		
		unsigned int ReliabilitySystem::getReceivedPackets() const {
			return recv_packets;
		}

		unsigned int ReliabilitySystem::getLostPackets() const {
			return lost_packets;
		}

		unsigned int ReliabilitySystem::getAckedPackets() const {
			return acked_packets;
		}

		float ReliabilitySystem::getSentBandwidth() const {
			return sent_bandwidth;
		}

		float ReliabilitySystem::getAckedBandwidth() const {
			return acked_bandwidth;
		}

		float ReliabilitySystem::getRoundTripTime() const {
			return rtt;
		}
		
		int ReliabilitySystem::getHeaderSize() const{
			return 12;
		}

		void ReliabilitySystem::advanceQueueTime( float deltaTime )
		{
			for ( PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); itor++ )
				itor->time += deltaTime;

			for ( PacketQueue::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++ )
				itor->time += deltaTime;

			for ( PacketQueue::iterator itor = pendingAckQueue.begin(); itor != pendingAckQueue.end(); itor++ )
				itor->time += deltaTime;

			for ( PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); itor++ )
				itor->time += deltaTime;
		}
		
		void ReliabilitySystem::updateQueues()
		{
			const float epsilon = 0.001f;

			while ( sentQueue.size() && sentQueue.front().time > rtt_maximum + epsilon ) {
				sentQueue.pop_front();
			}

			if ( receivedQueue.size() )
			{
				const unsigned int latest_sequence = receivedQueue.back().sequence;
				const unsigned int minimum_sequence = latest_sequence >= 34 ? ( latest_sequence - 34 ) : max_sequence - ( 34 - latest_sequence );
				while ( receivedQueue.size() && !sequence_more_recent( receivedQueue.front().sequence, minimum_sequence, max_sequence ) )
					receivedQueue.pop_front();
			}

			while ( ackedQueue.size() && ackedQueue.front().time > rtt_maximum * 2 - epsilon ) { 
				ackedQueue.pop_front();
			}

			while ( pendingAckQueue.size() && pendingAckQueue.front().time > rtt_maximum + epsilon )
			{
				pendingAckQueue.pop_front();
				lost_packets++;
			}
		}
		
		void ReliabilitySystem::updateStats()
		{
			int sent_bytes_per_second = 0;
			for ( PacketQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor ) { 
				sent_bytes_per_second += itor->size;
			}
			int acked_packets_per_second = 0;
			int acked_bytes_per_second = 0;
			for ( PacketQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); ++itor )
			{
				if ( itor->time >= rtt_maximum )
				{
					acked_packets_per_second++;
					acked_bytes_per_second += itor->size;
				}
			}
			sent_bytes_per_second /= rtt_maximum;
			acked_bytes_per_second /= rtt_maximum;
			sent_bandwidth = sent_bytes_per_second * ( 8 / 1000.0f );
			acked_bandwidth = acked_bytes_per_second * ( 8 / 1000.0f );
		}




		// utility functions
		bool ReliabilitySystem::sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence )
		{
			return (( s1 > s2 ) && ( s1 - s2 <= max_sequence/2 )) || (( s2 > s1 ) && ( s2 - s1 > max_sequence/2 ));
		}
		
		int ReliabilitySystem::bit_index_for_sequence( unsigned int sequence, unsigned int ack, unsigned int max_sequence )
		{
			assert( sequence != ack );
			assert( !sequence_more_recent( sequence, ack, max_sequence ) );
			if ( sequence > ack )
			{
				assert( ack < 33 );
				assert( max_sequence >= sequence );
 				return ack + ( max_sequence - sequence );
			}
			else
			{
				assert( ack >= 1 );
				assert( sequence <= ack - 1 );
 				return ack - 1 - sequence;
			}
		}
		
		unsigned int ReliabilitySystem::generate_ack_bits( unsigned int ack, const PacketQueue & received_queue, unsigned int max_sequence )
		{
			unsigned int ack_bits = 0;
			for ( PacketQueue::const_iterator itor = received_queue.begin(); itor != received_queue.end(); itor++ )
			{
				if ( itor->sequence == ack || sequence_more_recent( itor->sequence, ack, max_sequence ) ) { 
					break;
				}
				int bit_index = bit_index_for_sequence( itor->sequence, ack, max_sequence );
				if ( bit_index <= 31 ) { 
					ack_bits |= 1 << bit_index;
				}
			}
			return ack_bits;
		}
		
		void ReliabilitySystem::process_ack( unsigned int ack, unsigned int ack_bits, 
								 PacketQueue & pending_ack_queue, PacketQueue & acked_queue, 
								 std::vector<unsigned int> & acks, unsigned int & acked_packets, 
								 float & rtt, unsigned int max_sequence )
		{
			if ( pending_ack_queue.empty() ) {
				return;
			}
				
			PacketQueue::iterator itor = pending_ack_queue.begin();
			while ( itor != pending_ack_queue.end() )
			{
				bool acked = false;
				
				if ( itor->sequence == ack )
				{
					acked = true;
				}
				else if ( !sequence_more_recent( itor->sequence, ack, max_sequence ) )
				{
					int bit_index = bit_index_for_sequence( itor->sequence, ack, max_sequence );
					if ( bit_index <= 31 ) { 
						acked = ( ack_bits >> bit_index ) & 1;
					}
				}
				
				if ( acked )
				{
					rtt += ( itor->time - rtt ) * 0.1f;

					acked_queue.insert_sorted( *itor, max_sequence );
					acks.push_back( itor->sequence );
					acked_packets++;
					itor = pending_ack_queue.erase( itor );
				}
				else 
				{ 
					++itor;
				}
			}
		}

	}
}
