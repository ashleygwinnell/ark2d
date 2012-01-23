/*
 * Shape.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "../ARK2D.h"
#include "../GameContainer.h"

namespace ARK {

	template <class T>
	class Rectangle;

	template <class T>
	class Circle;

	template <class T>
	class Polygon;

	template <class T>
	class Shape {
		public:
			Shape() {

			}

			virtual T getMinX() = 0;
			virtual T getMaxX() = 0;
			virtual T getCenterX() = 0;

			virtual T getMinY() = 0;
			virtual T getMaxY() = 0;
			virtual T getCenterY() = 0;

			virtual T getWidth() = 0;
			virtual T getHeight() = 0;

			virtual void setLocation(T x, T y) = 0;
			virtual void setLocationByCenter(T x, T y) = 0;

			virtual bool contains(T x, T y) = 0;
			virtual bool collides(Shape<T>* s) = 0;
			virtual void resolve(Shape<T>* s) = 0;

			virtual bool isOffscreen() {
				GameContainer* container = ARK2D::getContainer();
				if (	(signed int)(getMaxY()) <= 0
					|| 	(signed int)(getMaxX()) <= 0
					|| 	(signed int)(getMinX()) >= (signed int) container->getWidth()
					|| 	(signed int)(getMinY()) >= (signed int) container->getHeight()) {
					return true;
				}
				return false;
			}
			virtual bool isPartlyOffscreen() {
				GameContainer* container = ARK2D::getContainer();
				if (	(signed int)(getMinX()) <= 0
					|| 	(signed int)(getMinY()) <= 0
					|| 	(signed int)(getMaxX()) >= (signed int) container->getWidth()
					|| 	(signed int)(getMaxY()) >= (signed int) container->getHeight()) {
					return true;
				}
				return false;
			}

			Rectangle<T>* asRectangle() {
				return dynamic_cast<Rectangle<T>* >(this);
			}
			Circle<T>* asCircle() {
				return dynamic_cast<Circle<T>* >(this);
			}
			Polygon<T>* asPolygon() {
				return dynamic_cast<Polygon<T>* >(this);
			}

			virtual ~Shape() {

			}

			/**
			 * Static collision strategies.
			 */
			static bool collision_circleCircle(T x1, T y1, T r1, T x2, T y2, T r2) {
				float distance = (float) MathUtil::distance(x1, y1, x2, y2);
				if (distance <= r1 + r2) {
					return true;
				}
				return false;
			}
			static bool collision_rectangleRectangle(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2) {
				if ((x1 + w1) >= x2 && x1 <= (x2 + w2) && (y1 + h1) >= y2 && y1 <= (y2 + h2))  {
					return true;
				}
				return false;
			}
			static bool collision_circleRectangle(T x1, T y1, T r1, T x2, T y2, T w2, T h2) {
				ErrorDialog::createAndShow("Circle-rectangle collision is not implemented");
				return false;
			}
	};
}
#endif /* SHAPE_H_ */
