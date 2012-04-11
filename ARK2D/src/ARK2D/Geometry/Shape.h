/*
 * Shape.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "../ARK2D.h"
#include "../Core/GameContainer.h"
#include "Vector2.h"
#include <limits>
namespace ARK {
	namespace Geometry {

		template <class T>
		class Polygon;

		template <class T>
		class Rectangle;

		template <class T>
		class Circle;

		template <class T>
		class Line;

		/*!
		 * \brief Abstract class for all Geometry.
		 *
		 * @todo Some collision-pairs are missing.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
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

				virtual void render() = 0;

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
				static bool collision_rectangleLine(T x1, T y1, T w1, T h1, T x2, T y2, T x3, T y3) {
					if (collision_rectangleRectangle(x1,y1,w1,h1,x2,y2,0,0) || collision_rectangleRectangle(x1,y1,w1,h1,x3,y3,0,0)) {
						return true;
					}
					T lines[] = {
						x1, 	y1,		x1+w1, 	y1,
						x1+w1, 	y1, 	x1+w1, 	y1+h1,
						x1+w1, 	y1+h1, 	x1, 	y1+h1,
						x1, 	y1+h1, 	x1, 	y1
					};
					for(unsigned int i = 0; i < 16; i += 4) {
						bool b = collision_lineLine(lines[i], lines[i+1], lines[i+2], lines[i+3], x2, y2, x3, y3);
						if (b) { return true; }
					}
					return false;
				}
				static bool collision_circleLine(T x1, T y1, T r1, T x2, T y2, T x3, T y3) { // x1, y1 center of circle.
					Vector2<T> pos(x1, y1);
					Line<T> line(x2,y2,x3,y3);
					T r2 = (r1 * r1);
					float dis = line.distanceSquared(&pos);
					if (dis < r2) {
						return true;
					}
					return false;
				}
				static bool collision_circleRectangle(T x1, T y1, T r1, T x2, T y2, T w2, T h2) {

					if (collision_rectangleRectangle(x1, y1, 0, 0, x2, y2, w2, h2)) {
						return true; // center of circle checked first.
					}

					// clockwise rect.
					Line<T> topside(	x2, 	y2,		x2+w2, 	y2);
					Line<T> rightside(	x2+w2, 	y2, 	x2+w2, 	y2+h2);
					Line<T> bottomside(	x2+w2, 	y2+h2, 	x2, 	y2+h2);
					Line<T> leftside(	x2, 	y2+h2, 	x2, 	y2);
					Line<T>* lines[4] = {&topside, &rightside, &bottomside, &leftside};

					T r2 = (r1 * r1);

					Vector2<T> pos(x1, y1);

					for (unsigned int i = 0; i < 4; i++) {
						float dis = lines[i]->distanceSquared(&pos);
						if (dis < r2) {
							return true;
						}
					}
					return false;

				}
				static bool collision_lineLine(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) {
					T d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
					if (d == 0) {
						return false;
					}
					T pre = (x1*y2 - y1*x2);
					T post = (x3*y4 - y3*x4);
					T x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
					T y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

					if (x < min<T>(x1, x2) || x > max<T>(x1, x2) || x < min<T>(x3, x4) || x > max<T>(x3, x4)) { return false; }
					if (y < min<T>(y1, y2) || y > max<T>(y1, y2) || y < min<T>(y3, y4) || y > max<T>(y3, y4)) { return false; }
					return true;
				}
				static bool collision_polygonPolygon(Polygon<T>* one, Polygon<T>* two) { // convex polys only.


					return false;
				}
				static bool collision_polygonCircle(Polygon<T>* one, T x1, T y1, T r1) { // any poly even with holes in - not very efficient.
					vector<Vector2<T>* >* points = one->getPoints();

					//! @todo: Contains check here.

					Line<T> lines[points->size()];
					for(unsigned int i = 0; i < points->size(); i++) {
						unsigned int nextIndex = i+1;
						if (nextIndex == points->size()) { nextIndex = 0; }
						lines[i].setStart(points->at(i)->getX(), points->at(i)->getY());
						lines[i].setEnd(points->at(nextIndex)->getX(), points->at(nextIndex)->getY());
					}

					T r2 = (r1 * r1);
					Vector2<T> pos(x1, y1);
					for (unsigned int i = 0; i < points->size(); i++) {
						float dis = lines[i].distanceSquared(&pos);
						if (dis < r2) {
							return true;
						}
					}
					return false;
				}
		};
	}
}
#endif /* SHAPE_H_ */
