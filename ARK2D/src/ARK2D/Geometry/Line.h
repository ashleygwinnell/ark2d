/*
 * Line.h
 *
 *  Created on: Jan 23, 2012
 *      Author: ashleygwinnell
 *
 * Original Author: Kevin Glass
 * https://bob.newdawnsoftware.com/repos/slick/trunk/Slick/src/org/newdawn/slick/geom/Line.java
 */

#ifndef LINE_H_
#define LINE_H_

#include "Shape.h"

namespace ARK {
	template <class T>
	class Line: public Shape<T> {
		private:
			Vector2<T>* first;
			Vector2<T>* second;
			Vector2<T>* vec; // vector from first to second.
			bool dirty;

		public:
			Line(): first(new Vector2<T>(0,0)), second(new Vector2<T>(0,0)), vec(new Vector2<T>(0,0)) {

			}
			Line(T x1, T y1, T x2, T y2): first(new Vector2<T>(x1,y1)), second(new Vector2<T>(x2, y2)), vec(new Vector2<T>(0,0)) {
				vec->set(second);
				vec->subtract(first);
			}
			virtual Vector2<T>* getStart() {
				return first;
			}
			virtual Vector2<T>* getEnd() {
				return second;
			}
			virtual void setStart(T x, T y) {
				first->set(x, y);
				dirty = true;
				clean();
			}
			virtual void setEnd(T x, T y) {
				second->set(x, y);
				dirty = true;
				clean();
			}
			virtual T getMinX() {
				if (first->getX() < second->getX()) {
					return first->getX();
				}
				return second->getX();
			}
			virtual T getMaxX() {
				if (first->getX() > second->getX()) {
					return first->getX();
				}
				return second->getX();
			}
			virtual T getCenterX() {
				return (first->getX() + second->getX())/2;
			}

			virtual T getMinY() {
				if (first->getY() < second->getY()) {
					return first->getY();
				}
				return second->getY();
			}
			virtual T getMaxY() {
				if (first->getY() > second->getY()) {
					return first->getY();
				}
				return second->getY();
			}
			virtual T getCenterY() {
				return (first->getY() + second->getY())/2;
			}

			virtual T getWidth() {
				return getMaxX() - getMinX();
			}
			virtual T getHeight() {
				return getMaxY() - getMinY();
			}
			virtual T getLength() {
				return vec->length();
			}
			virtual T getLengthSquared() {
				return vec->lengthSquared();
			}
			virtual void clean() {
				if (!dirty) { return; }
				vec->set(second);
				vec->subtract(first);
			}

			/**
			 * Get the shortest distance squared from a point to this line
			 *
			 * @param point
			 *            The point from which we want the distance
			 * @return The distance squared from the line to the point
			 */
			float distanceSquared(Vector2<T>* point) {
				Vector2<T> closest;
				getClosestPoint(point, &closest);
				closest.subtract(point);

				return closest.lengthSquared();
			}

			/**
			 * Get the closest point on the line to a given point
			 *
			 * @param point
			 *            The point which we want to project
			 * @param result
			 *            The point on the line closest to the given point
			 */
			void getClosestPoint(Vector2<T>* point, Vector2<T>* result) {
				Vector2<T> loc;
				loc.set(point);
				loc.subtract(first);

				float projDistance = vec->dot(&loc);

				projDistance /= vec->lengthSquared();

				if (projDistance < 0) {
					result->set(first);
					return;
				}
				if (projDistance > 1) {
					result->set(second);
					return;
				}

				result->setX(first->getX() + projDistance * vec->getX());
				result->setY(first->getY() + projDistance * vec->getY());
			}


			virtual void setLocation(T x, T y) {
				// Sets by point 'first'.
			}
			virtual void setLocationByCenter(T x, T y) {
				//T cx = getCenterX();
				//T cy = getCenterY();

				T dx = x - getCenterX();
				T dy = y - getCenterY();

				first->add(dx, dy);
				second->add(dx, dy);

				dirty = true;
				clean();
			}

			virtual bool contains(T x, T y) {
				return false;
			}
			virtual bool collides(Shape<T>* s) {
				Line<T>* line = NULL;
				line = dynamic_cast<Line<T>* >(s);
				if (line != NULL) {
					return Shape<T>::collision_lineLine(first->getX(), first->getY(), second->getX(), second->getY(), line->getStart()->getX(), line->getStart()->getY(), line->getEnd()->getX(), line->getEnd()->getY());
				}

				Circle<T>* circle = NULL;
				circle = dynamic_cast<Circle<T>* >(s);
				if (circle != NULL) {
					return Shape<T>::collision_circleLine(circle->getCenterX(), circle->getCenterY(), circle->getRadius(), first->getX(), first->getY(), second->getX(), second->getY());
				}

				Rectangle<T>* rect = NULL;
				rect = dynamic_cast<Rectangle<T>* >(s);
				if (rect != NULL) {
					return Shape<T>::collision_rectangleLine(rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight(), first->getX(), first->getY(), second->getX(), second->getY());
				}

				return false;
			}
			virtual void resolve(Shape<T>* s) {

			}
			virtual void render() {
				Graphics* g = ARK2D::getGraphics();
				g->drawLine(first->getX(), first->getY(), second->getX(), second->getY());
			}

			virtual ~Line() {

			}
	};
}

#endif /* LINE_H_ */