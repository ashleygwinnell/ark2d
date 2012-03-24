/*
 * Range.h
 *
 *  Created on: 16 Apr 2011
 *      Author: Ashley
 */

#ifndef RANGE_H_
#define RANGE_H_

#include <math.h>

namespace ARK {
	namespace Util {

		/*!
		 * \brief Use to define and maintain a set to define random number generation
		 *
		 * Used by the Particle Engine.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=int>
		class Range {
			public:
				T m_low;
				T m_high;
				bool m_enabled;
				Range(T low, T high): m_low(low), m_high(high), m_enabled(true) {

				}
				T random() {
					// see MathUtil::randBetween
					T diff = m_high-m_low;
					if (diff == 0) {
						return m_low;
					}
					return m_low + (rand() % (m_high-m_low));
					//MathUtil::randBetween();
					//return T(int(m_low) + (rand() % int(int(m_high)-int(m_low))));
					//return 2;
				}
				bool isEnabled() {
					return m_enabled;
				}
				void setEnabled(bool b) {
					m_enabled = b;
				}
				T getMax() {
					return m_high;
				}
				T getMin() {
					return m_low;
				}

				void setMin(T i) {
					m_low = i;
				}
				void setMax(T i) {
					m_high = i;
				}
				void set(bool enabled, T low, T high) {
					m_enabled = enabled;
					m_low = low;
					m_high = high;
				}

				virtual ~Range() {

				}
		};
	}
}

#endif /* RANGE_H_ */
