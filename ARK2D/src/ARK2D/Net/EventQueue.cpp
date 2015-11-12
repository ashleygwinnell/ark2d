/*
 * EventQueue.cpp
 *
 *  Created on: 29 March 2015
 *      Author: Ashley
 */

#include "EventQueue.h"

namespace ARK {
	namespace Net { 
		EventQueue::EventQueue():
			m_queueData(),
			m_queueSz(),
			m_queueType(),
			m_total(0)
			{

		}
		void EventQueue::add(const char* data, unsigned int sz, unsigned int type, bool exclusive) {

			if (exclusive) {
				bool erased = false;
				unsigned int count = 0;
				std::list<unsigned int>::iterator it = m_queueType.begin();
				while(it != m_queueType.end()) {
					if (*it == type) {
						erased = true;
						m_queueType.erase(it);
						break;
					}
					it++;
					count++;
				}
				if (erased) {
					std::list<const char*>::iterator it1 = m_queueData.begin();
					std::list<unsigned int>::iterator it2 = m_queueSz.begin();

					advance(it1, count);
					advance(it2, count);

					m_queueData.erase(it1);
					m_queueSz.erase(it2);

					m_total--;
				}
			}

			const char* dataCpy = (const char*) malloc(sz * sizeof(char));
			memcpy((void*) dataCpy, data, sz);

			m_queueData.push_back(dataCpy);
			m_queueSz.push_back(sz);
			m_queueType.push_back(type);
			m_total++;
		}
		void EventQueue::get(unsigned char* packet) {
			
			memset( packet, 0, MAX_PACKET_SIZE );
			if (m_total == 0) {
				return;
			}
			unsigned int curptr = 1;
			unsigned char numEvents = 0;
			while (curptr + m_queueSz.front() + 2 < MAX_PACKET_SIZE) {
				const char* thisData = m_queueData.front();
				unsigned int thisSz = m_queueSz.front();
				unsigned int thisType = m_queueType.front();

				packet[curptr] = thisSz;
				packet[curptr+1] = thisType;
				memcpy((void*) (packet + curptr + 2), (void*) thisData, thisSz);

				free((void*) thisData);

				m_queueData.pop_front();
				m_queueSz.pop_front();
				m_queueType.pop_front();
				m_total--;

				curptr += thisSz + 2;
				numEvents++;
			}

			packet[0] = numEvents;

		}

	}
}