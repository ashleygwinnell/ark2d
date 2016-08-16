/*
 * Rectangle.h
 *
 *  Created on: 10-May-2010
 *      Author: Ashley
 */

#ifndef GIGARECTANGLE_H_
#define GIGARECTANGLE_H_

//#ifndef ARK2D_UBUNTU_LINUX

 	//#include "../Includes.h"
	#include "../../Namespaces.h"
	#include "../ARK2D.h"
	#include "Vector2.h"
	#include "../GameContainer.h"



	template <class T>
	class GigaRectangle {

		public:
			static const unsigned int SIDE_NONE  = 0;
			static const unsigned int SIDE_TOP   = 1;
			static const unsigned int SIDE_LEFT  = 2;
			static const unsigned int SIDE_BOTTOM  = 3;
			static const unsigned int SIDE_RIGHT = 4;

			static Vector2<float> adjustmentVector;

		private:
			T m_x;
			T m_y;
			int m_width;
			int m_height;
		public:
			GigaRectangle(): m_x(0), m_y(0), m_width(1), m_height(1) {

			}
			GigaRectangle(T x, T y, int width, int height): m_x(x), m_y(y), m_width(width), m_height(height) {

			}
			T getX() const {
				return m_x;
			}
			T getY() const {
				return m_y;
			}
			int getWidth() const {
				return m_width;
			}
			int getHeight() const {
				return m_height;
			}

			T getCenterX() const {
				return m_x + (m_width/2);
			}
			T getCenterY() const {
				return m_y + (m_height/2);
			}

			T getMaxX() const {
				return m_x + m_width;
			}
			T getMaxY() const {
				return m_y + m_height;
			}

			void setX(T x) {
				m_x = x;
			}
			void setY(T y) {
				m_y = y;
			}
			void setWidth(int w) {
				m_width = w;
			}
			void setHeight(int h) {
				m_height = h;
			}
			void setLocation(T x, T y) {
				m_x = x;
				m_y = y;
			}
			void setLocationByCenter(T x, T y) {
				m_x = x - (m_width/2);
				m_y = y - (m_height/2);
			}

			bool contains(T x, T y) {
				if (x >= m_x && x <= m_x+m_width
						&& y >= m_y && y <= m_y+m_height) {
					return true;
				}
				return false;
			}
			bool collides(GigaRectangle* r) {
				return s_collides(this, r);
			}

			bool isOffscreen() {
				return s_isOffscreen(this);
			}
			bool isPartlyOffscreen() {
				return s_isPartlyOffscreen(this);
			}
			static bool s_collides(GigaRectangle* rect1, GigaRectangle* rect2) {
				return s_collides(
					rect1->getX(), rect1->getY(), rect1->getWidth(), rect1->getHeight(),
					rect2->getX(), rect2->getY(), rect2->getWidth(), rect2->getHeight()
				);
			}
			static bool s_collides(GigaRectangle* rect, T x2, T y2, T w2, T h2) {
				return s_collides(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight(), x2, y2, w2, h2);
			}
			static bool s_collides(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2) {
				if ((	x1 + w1) >= x2
						&& x1 <= (x2 + w2)
						&& (y1 + h1) >= y2
						&& y1 <= (y2 + h2)
				)  {
					return true;
				}
				return false;
			}
			static bool s_contains(T x1, T y1, T w1, T h1, T x2, T y2) {
				return s_collides(
					x1, y1, w1, h1, x2, y2, 1, 1
				);
			}

			static unsigned int s_collidesSide(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2) {
				if ((	x1 + w1) >= x2
						&& x1 <= (x2 + w2)
						&& (y1 + h1) >= y2
						&& y1 <= (y2 + h2)
				)  {

					// Axis stores the value of X or Y.  X = 0, Y = 1.
					// Side stores the value of left (-1) or right (+1).
					//int axis = 0;
					//int side = 0;
					unsigned int returnSide = SIDE_NONE;

					 // This stores the absolute minimum distance we'll need to separate our colliding object.
					float minimumTranslationDistance;

					 // This is re-used to calculate the difference in distance between sides.
					float difference = 0.0f;

					// left
					difference = (x1 + w1) - x2; // left.getMaxX() - right.getMinX();
					if (difference < 0.0f) {
						return SIDE_NONE;
					} else {
						minimumTranslationDistance = difference;
						//axis = 0;
						//side = -1;
						returnSide = SIDE_LEFT;
					}

					//right
					difference = (x2 + w2) - x1; //right.getMaxX() - left.getMinX();
					if (difference < 0.0f) {
						return SIDE_NONE;
					}
					if (difference < minimumTranslationDistance) {
						minimumTranslationDistance = difference;
						//axis = 0;
						//side = 1;
						returnSide = SIDE_RIGHT;
					}

					// bottom
					difference = (y1 + h1) - y2;// left.getMaxY() - right.getMinY();
					if (difference < 0.0f) {
						return SIDE_NONE;
					}
					if (difference < minimumTranslationDistance) {
						minimumTranslationDistance = difference;
						//axis = 1;
						//side = -1;
						returnSide = SIDE_BOTTOM;
					}

					// top
					difference = (y2 + h2) - y1;// right.getMaxY() - left.getMinY();
					if (difference < 0.0f) {
						return SIDE_NONE;
					}
					if (difference < minimumTranslationDistance) {
						minimumTranslationDistance = difference;
						//axis = 1;
						//side = 1;
						returnSide = SIDE_TOP;
					}

					return returnSide;
				}
				return SIDE_NONE;
			}

			static Vector2<float>& s_collides_vec(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2) {
				adjustmentVector.set(0, 0);

				if ((	x1 + w1) >= x2
						&& x1 <= (x2 + w2)
						&& (y1 + h1) >= y2
						&& y1 <= (y2 + h2)
				)  {

					// Axis stores the value of X or Y.  X = 0, Y = 1.
					// Side stores the value of left (-1) or right (+1).
					int axis = 0;
					int side = 0;

					 // This stores the absolute minimum distance we'll need to separate our colliding object.
					float minimumTranslationDistance = 0.0f;

					 // This is re-used to calculate the difference in distance between sides.
					float difference = 0.0f;

					// left
					difference = (x1 + w1) - x2; // left.getMaxX() - right.getMinX();
					if (difference < 0.0f) {
						return adjustmentVector;
					} else {
						minimumTranslationDistance = difference;
						axis = 0;
						side = -1;
					}

					//right
					difference = (x2 + w2) - x1; //right.getMaxX() - left.getMinX();
					if (difference < 0.0f) {
						return adjustmentVector;
					}
					if (difference < minimumTranslationDistance) {
						minimumTranslationDistance = difference;
						axis = 0;
						side = 1;
					}

					// bottom
					difference = (y1 + h1) - y2;// left.getMaxY() - right.getMinY();
					if (difference < 0.0f) {
						return adjustmentVector;
					}
					if (difference < minimumTranslationDistance) {
						minimumTranslationDistance = difference;
						axis = 1;
						side = -1;
					}

					// top
					difference = (y2 + h2) - y1;// right.getMaxY() - left.getMinY();
					if (difference < 0.0f) {
						return adjustmentVector;
					}
					if (difference < minimumTranslationDistance) {
						minimumTranslationDistance = difference;
						axis = 1;
						side = 1;
					}

					// collided with
					if (axis == 1) { // Y Axis
						adjustmentVector.setY(side * minimumTranslationDistance);
					} else { // X Axis
						adjustmentVector.setX(side * minimumTranslationDistance);
					}
				}
				return adjustmentVector;
			}

			static bool s_isOffscreen(GigaRectangle* obj) {
				/*ARK::Core::GameContainer* container = ARK2D::getContainer();
				if (	(signed int)(obj->getY() + obj->getHeight()) <= 0
					|| 	(signed int)(obj->getX() + obj->getWidth()) <= 0
					|| 	(signed int)(obj->getX()) >= (signed int) container->getWidth()
					|| 	(signed int)(obj->getY()) >= (signed int) container->getHeight()) {
					return true;
				}
				return false;*/
                return false;
			}

			static bool s_isPartlyOffscreen(GigaRectangle* obj) {
				/*ARK::Core::GameContainer* container = ARK2D::getContainer();
				if (	(signed int)(obj->getX()) <= 0
					|| 	(signed int)(obj->getX()) <= 0
					|| 	(signed int)(obj->getX() + obj->getWidth()) >= (signed int) container->getWidth()
					|| 	(signed int)(obj->getY() + obj->getHeight()) >= (signed int) container->getHeight()) {
					return true;
				}
				return false;*/
                return false;
			}

			~GigaRectangle() {

			}
	};


	template<class T>
	Vector2<float> GigaRectangle<T>::adjustmentVector;

	//template<> Vector2<float> GigaRectangle<float>::adjustmentVector;

//#endif

#endif /* GIGARECTANGLE_H_ */
