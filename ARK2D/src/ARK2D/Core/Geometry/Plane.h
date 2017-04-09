/*
 * Plane.h
 *
 *  Created on: 30 Nov, 2015
 *      Author: ashleygwinnell
 *
 * Original author:
 * https://github.com/libgdx/libgdx/blob/master/gdx/src/com/badlogic/gdx/math/Plane.java
 */

#ifndef ARK_CORE_GEOMETRY_PLANE_H_
#define ARK_CORE_GEOMETRY_PLANE_H_

namespace ARK {
	namespace Core {
		namespace Geometry {

			struct PlaneIntersection {
				bool intersects;
				Vector3<float> intersectsAtPoint;
				float intersectsDistance;
				bool inside; // its possible for a line to be entirely inside a plane. check for this!
			};

			/*!
			 * \brief A point and angle/rotation/direction that separate two 3d spaces.
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */
			template <class T>
			class PlaneTemplate {
				public:
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
	                    normal = p1;
	                    normal -= p2;
	                    normal.cross(p2.x - p3.x, p2.y-p3.y, p2.z-p3.z).normalise();
						d = p1.dot(&normal) * -1;
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

					PlaneIntersection intersects(const Vector3<float>& lineStart, const Vector3<float>& lineDirection) {
						return intersects(*this, lineStart, lineDirection);
					}
					static PlaneIntersection intersects(const PlaneTemplate<T>& plane, const Vector3<float>& lineStart, const Vector3<float>& lineDirection)
					{
					    PlaneIntersection result;
					    result.intersects = false;

					    // Find t.
					    float t = - (Vector3<float>::dot(plane.normal, lineStart) + plane.d) / (Vector3<float>::dot(plane.normal, lineDirection));
					    if (t < 0) { // the ray does not hit the surface, that is, the surface is "behind" the ray
					        return result;
						}

					    // Get a point on the plane.
					    Vector3<float> p = lineStart + (lineDirection * t);

					    // Does the ray intersect the plane inside or outside?
					    Vector3<float> planeToRayStart = lineStart - p;
					    double dot = Vector3<float>::dot(planeToRayStart, plane.normal);
					    if (dot > 0) {
					        result.inside = false;
					    } else {
					        result.inside = true;
					    }
					    result.intersects = true;
					    result.intersectsAtPoint = p;
					    result.intersectsDistance = t;
					    return result;
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
					Plane(Vector3<float> p1, Vector3<float> p2, Vector3<float> p3):
						PlaneTemplate(p1, p2, p3) {

					}

					//virtual ARK2D_API bool collides(Shape<float>* s);
					virtual ~Plane() {

					}
			};
		}
	}
}
#endif /* ARK_CORE_GEOMETRY_PLANE_H_ */
