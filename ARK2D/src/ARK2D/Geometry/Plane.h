/*
 * Plane.h
 *
 *  Created on: 30 Nov, 2015
 *      Author: ashleygwinnell
 * 
 * Original author:
 * https://github.com/libgdx/libgdx/blob/master/gdx/src/com/badlogic/gdx/math/Plane.java
 */

#ifndef ARK_GEOMETRY_PLANE_H_
#define ARK_GEOMETRY_PLANE_H_

#include "../Util/Log.h"

namespace ARK {
	namespace Geometry {

		/*!
		 * \brief A point and angle/rotation/direction that separate two 3d spaces.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T>
		class PlaneTemplate {
			private:
				Vector3<T> normal;
				T d;

			public:
				
                PlaneTemplate():
                    normal(0,1,0),
                    d(0) {
                }
                PlaneTemplate(Vector3<T> norm, T dist):
					normal(norm),
					d(0) {
					normal.normalise();
					d = dist;
				}
				PlaneTemplate(Vector3<T> norm, Vector3<T> point):
					normal(norm),
					d(0) {
					normal.normalise();
					d = normal.dot(&point) * -1;
				}

				PlaneTemplate(Vector3<T> p1, Vector3<T> p2, Vector3<T> p3):
					normal(0,0,0),
					d(0) {
					set(p1, p2, p3);
				}

				void set(Vector3<T> p1, Vector3<T> p2, Vector3<T> p3) {
					normal.set(p1).subtract(p2).cross(p2.x - p3.x, p2.y-p3.y, p2.z-p3.z).normalise();
					d = p1.dot(normal) * -1;
				}
				void set(T nx, T ny, T nz, T dist) {
					normal.set(nx, ny, nz);
					d = dist;
				}

				T distance(Vector3<T> point) {
					return normal.dot(point) + d;
				}
				

				signed int testPoint (const Vector3<T>& point) {
					return testPoint(point.getX(), point.getY(), point.getY());
				}
				signed int testPoint (T x, T y, T z) {
					T dist = normal.dot(x, y, z) + d;

					if (dist == 0)
						return 0; //PlaneSide.OnPlane;
					else if (dist < 0)
						return -1; //PlaneSide.Back;
					else
						return 1; //PlaneSide.Front;
				}
				virtual string toString() {
					string nl = "\r\n";
					string str;
					str += "{";
						str += "	\"normal\": ";
						str += normal.toString();
					str += ",";
						str += "	\"distance\": ";
						str += Cast::toString<float>(d);
					str += "}";
					return str;
				}

				virtual ~PlaneTemplate() {

				}
		};

		class Plane : public PlaneTemplate<float> {
			public: 
				Plane():
					PlaneTemplate() {

				}
				Plane(Vector3<float> norm, float dist):
					PlaneTemplate(norm, dist) {
					
				}
				Plane(Vector3<float> norm, Vector3<float> point):
					PlaneTemplate(norm, point) {
					
				}
				
				//virtual ARK2D_API bool collides(Shape<float>* s);
				virtual ~Plane() {

				}
		};
	}
}
#endif /* ARK_GEOMETRY_PLANE_H_ */
