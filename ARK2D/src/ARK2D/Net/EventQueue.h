/*
 * EventQueue.h
 *
 *  Created on: 29 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_EVENTQUEUE_H_
#define ARK_NET_EVENTQUEUE_H_

#include "../Includes.h"

namespace ARK { 
	namespace Net { 
		#define MAX_PACKET_SIZE 256
		class ARK2D_API EventQueue { 
			public:
				std::list<const char*> m_queueData;
				std::list<unsigned int> m_queueSz;
				std::list<unsigned int> m_queueType;
				unsigned int m_total;

				EventQueue();
				void add(const char* data, unsigned int sz, unsigned int type, bool exclusive);
				void get(unsigned char* packet);
		};
	}
}

#endif // ARK_NET_EVENTQUEUE_H_