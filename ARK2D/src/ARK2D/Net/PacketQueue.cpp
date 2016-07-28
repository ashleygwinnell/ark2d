/*
 * PacketQueue.cpp
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#include "PacketQueue.h"
#include <cassert>

namespace ARK {
	namespace Net {

		bool sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence )
		{
			return (( s1 > s2 ) && ( s1 - s2 <= max_sequence/2 )) || (( s2 > s1 ) && ( s2 - s1 > max_sequence/2 ));
		}

		bool PacketQueue::exists( unsigned int sequence )
		{
			for ( iterator itor = begin(); itor != end(); ++itor )
				if ( itor->sequence == sequence )
					return true;
			return false;
		}

		void PacketQueue::insert_sorted( const PacketData & p, unsigned int max_sequence )
		{
			if ( empty() )
			{
				push_back( p );
			}
			else
			{
				if ( !sequence_more_recent( p.sequence, front().sequence, max_sequence ) )
				{
					push_front( p );
				}
				else if ( sequence_more_recent( p.sequence, back().sequence, max_sequence ) )
				{
					push_back( p );
				}
				else
				{
					for ( PacketQueue::iterator itor = begin(); itor != end(); itor++ )
					{
						assert( itor->sequence != p.sequence );
						if ( sequence_more_recent( itor->sequence, p.sequence, max_sequence ) )
						{
							insert( itor, p );
							break;
						}
					}
				}
			}
		}

		void PacketQueue::verify_sorted( unsigned int max_sequence )
		{
			PacketQueue::iterator prev = end();
			for ( PacketQueue::iterator itor = begin(); itor != end(); itor++ )
			{
				assert( itor->sequence <= max_sequence );
				if ( prev != end() )
				{
					assert( sequence_more_recent( itor->sequence, prev->sequence, max_sequence ) );
					prev = itor;
				}
			}
		}
	}
}
