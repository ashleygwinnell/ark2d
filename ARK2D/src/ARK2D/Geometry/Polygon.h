/*
 * Polygon.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Shape.h"
#include <vector>
using namespace std;

namespace ARK {
	template <class T>
	class Polygon: public Shape<T> {
		private:
		vector<Vector2<T>* > m_points;

		public:
			Polygon(): Shape<T>(), m_points() {

			}

			void addPoint(T x, T y) {
				m_points.push_back(new Vector2<T>(x, y));
			}

			virtual T getMinX() {
				T min = std::numeric_limits<T>::max();
				for(unsigned int i = 0; i < m_points.size(); i++) {
					if (m_points.at(i)->getX() < min) {
						min = m_points.at(i)->getX();
					}
				}
				return min;
			}
			virtual T getMaxX() {
				T max = std::numeric_limits<T>::min();
				for(unsigned int i = 0; i < m_points.size(); i++) {
					if (m_points.at(i)->getX() > max) {
						max = m_points.at(i)->getX();
					}
				}
				return max;
			}
			virtual T getCenterX() {
				return (getMinX() + getMaxX())/2;
			}

			virtual T getMinY() {
				T min = std::numeric_limits<T>::max();
				for(unsigned int i = 0; i < m_points.size(); i++) {
					if (m_points.at(i)->getY() < min) {
						min = m_points.at(i)->getY();
					}
				}
				return min;
			}
			virtual T getMaxY() {
				T max = std::numeric_limits<T>::min();
				for(unsigned int i = 0; i < m_points.size(); i++) {
					if (m_points.at(i)->getY() > max) {
						max = m_points.at(i)->getY();
					}
				}
				return max;
			}
			virtual T getCenterY() {
				return (getMinY() + getMaxY())/2;
			}

			virtual T getWidth() {
				return getMaxX() - getMinX();
			}
			virtual T getHeight() {
				return getMaxY() - getMinY();
			}

			virtual void setLocation(T x, T y) {

			}
			virtual void setLocationByCenter(T x, T y) {

			}

			virtual bool contains(T x, T y) {
				return false;
			}
			virtual bool collides(Shape<T>* s) {
				return false;
			}
			virtual void resolve(Shape<T>* s) {

			}

			virtual ~Polygon() {
				delete[] m_points;
			}
	};
}
#endif /* POLYGON_H_ */
