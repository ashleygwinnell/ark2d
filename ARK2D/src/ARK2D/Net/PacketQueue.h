/*
 * Socket.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_PACKETQUEUE_H_
#define ARK_NET_PACKETQUEUE_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <list>
using std::list;

namespace ARK {
	namespace Net {

	struct PacketData
	{
		unsigned int sequence;			// packet sequence number
		float time;					    // time offset since packet was sent or received (depending on context)
		int size;						// packet size in bytes
		bool operator==(const ARK::Net::PacketData pd) const { return (sequence == pd.sequence && time == pd.time && size == pd.size); }
		bool operator<(const ARK::Net::PacketData pd) const { return (time < pd.time); }
	};

	inline bool sequence_more_recent( unsigned int s1, unsigned int s2, unsigned int max_sequence );



		/*!
		 * \brief Packet queue to store information about sent and received packets sorted in sequence order.
		 * 		   + we define ordering using the "sequence_more_recent" function, this works provided there is a large gap when sequence wrap occurs
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API PacketQueue : public std::list<PacketData> {
			public:

				bool exists( unsigned int sequence );

				void insert_sorted( const PacketData & p, unsigned int max_sequence );

				void verify_sorted( unsigned int max_sequence );
		};
	}
}

#endif /* ARK_NET_PACKETQUEUE_H_ */
