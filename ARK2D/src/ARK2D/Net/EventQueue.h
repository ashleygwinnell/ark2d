/*
 * EventQueue.h
 *
 *  Created on: 29 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_EVENTQUEUE_H_
#define ARK_NET_EVENTQUEUE_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <list>
using std::list;

namespace ARK {
	namespace Net {
		#define MAX_PACKET_SIZE 256
		class ARK2D_API EventQueue {
			public:
				list<const char*> m_queueData;
				list<unsigned int> m_queueSz;
				list<unsigned int> m_queueType;
				unsigned int m_total;

				EventQueue();
				void add(const char* data, unsigned int sz, unsigned int type, bool exclusive);
				void get(unsigned char* packet);

				virtual void removeAtIndex(unsigned int index);
		};
	}
}

#endif // ARK_NET_EVENTQUEUE_H_
