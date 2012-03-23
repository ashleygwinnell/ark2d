/*
 * Matrix44.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef MATRIX44_H_
#define MATRIX44_H_

#include "Vector4.h"

namespace ARK {
	namespace Util {

		/*!
		 * \brief Matrix manipulation for graphics rendering.
		 *
		 * This is not used internally.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=float>
		class Matrix44 {
			public:
				Vector4<T> m_x;
				Vector4<T> m_y;
				Vector4<T> m_z;
				Vector4<T> m_w;

			public:
				Matrix44(): m_x(), m_y(), m_z(), m_w() {
					m_x.toZero();
					m_y.toZero();
					m_z.toZero();
					m_w.toZero();

					m_x.m_x = 1;
					m_y.m_y = 1;
					m_z.m_z = 1;
					m_w.m_w = 1;
				}
				Vector4<T> getX() { return m_x; }
				Vector4<T> getY() { return m_y; }
				Vector4<T> getZ() { return m_z; }
				Vector4<T> getW() { return m_w; }

				void toValue(T v) {
					m_x.toValue(v);
					m_y.toValue(v);
					m_z.toValue(v);
					m_w.toValue(v);
				}
				void toZero() {
					toValue(0);
				}
				void toOne() {
					toValue(1);
				}

				virtual ~Matrix44() {

				}
		};
	}
}

#endif /* MATRIX44_H_ */
