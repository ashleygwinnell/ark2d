/*
 * Vector3.h
 *
 *  Created on: 6th July 2014
 *      Author: Ashley
 */

#ifndef ARK_GEOMETRY_VECTOR3_H_
#define ARK_GEOMETRY_VECTOR3_H_

#include "../Cast.h"
#include <cmath>
#include <string>
using std::string;

namespace ARK {
    namespace Core {
        namespace Geometry {

		/*!
		 * \brief A 3d point in spaaaaace.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T=float>
        class Vector3;

        template <class T>
        class Vector3 {
			public:
                static const Vector3<T> FORWARD;
                static const Vector3<T> UP;

			public:
				T x;
				T y;
				T z;

				Vector3(): x(0), y(0), z(0) {

				}
				Vector3(T coords[]): x(coords[0]), y(coords[1]), z(coords[2]) {

				}
				Vector3(T lx, T ly, T lz): x(lx), y(ly), z(lz) {

				}

				T getX() const {
					return x;
				}
				T getY() const {
					return y;
				}
				T getZ() const {
					return z;
				}

				// Set
				void setX(T lx) {
					x = lx;
				}
				void setY(T ly) {
					y = ly;
				}
				void setZ(T lz) {
					z = lz;
				}
				void set(Vector3<T>* v) {
					set(v->getX(), v->getY(), v->getZ());
				}
                void set(const Vector3<T>& v) {
                    set(v.x, v.y, v.z);
                }
				void set(T lx, T ly, T lz) {
					x = lx;
					y = ly;
					z = lz;
				}
				void set(T lx, T ly) {
					x = lx;
					y = ly;
				}

				void adjustX(T lx) {
					x += lx;
				}
				void adjustY(T ly) {
					y += ly;
				}
				void adjustZ(T lz) {
					z += lz;
				}
				void adjust(T lx, T ly, T lz) {
					x += lx;
					y += ly;
					z += lz;
				}

				// Add
				Vector3* add(Vector3<T>* v) {
					return add(v->getX(), v->getY(), v->getZ());
				}
				Vector3* add(T lx, T ly, T lz) {
					x += lx;
					y += ly;
					z += lz;
					return this;
				}
				Vector3* add(T lx, T ly) {
					x += lx;
					y += ly;
					return this;
				}
				Vector3* addX(T lx) {
					x += lx;
					return this;
				}
				Vector3* addY(T ly) {
					y += y;
					return this;
				}
				Vector3* addZ(T lz) {
					z += z;
					return this;
				}

				// Multiply
				Vector3* multiply(Vector3<T>* v) {
					return multiply(v->getX(), v->getY(), v->getZ());
				}
				Vector3* multiply(T lx, T ly, T lz) {
					x *= lx;
					y *= ly;
					z *= lz;
					return this;
				}
				void multiplyX(T lx) {
					x *= lx;
				}
				void multiplyY(T ly) {
					y *= ly;
				}
				void multiplyZ(T lz) {
					z *= lz;
				}


				// Divide
				Vector3* divide(Vector3<T>* v) {
					return divide(v->getX(), v->getY(), v->getZ());
				}
				Vector3* divide(T lx, T ly, T lz) {
					x /= lx;
					y /= ly;
					z /= lz;
					return this;
				}
				void divideX(T lx) {
					x /= lx;
				}
				void divideY(T ly) {
					y /= ly;
				}
				void divideZ(T lz) {
					z /= lz;
				}

				// Subtract
				Vector3* subtract(Vector3<T>* v) {
					return subtract(v->getX(), v->getY(), v->getZ());
				}
				Vector3* subtract(T lx, T ly, T lz) {
					x -= lx;
					y -= ly;
					z -= lz;
					return this;
				}
				Vector3* subtractX(T lx) {
					x -= lx;
					return this;
				}
				Vector3* subtractY(T ly) {
					y -= ly;
					return this;
				}
				Vector3* subtractZ(T lz) {
					z -= lz;
					return this;
				}

				// Distance
				void distance(Vector3<T>* v, Vector3<T>* result) {
					distance(v->getX(), v->getY(), v->getZ(), result);
				}
				void distance(T lx, T ly, T lz, Vector3<T>* result) {
					result->set(abs(x - lx), abs(y - ly), abs(z - lz));
				}

				// Friction
				Vector3* friction(T lx, T ly, T lz) {
					frictionX(lx);
					frictionY(ly);
					frictionZ(lz);
					return this;
				}
				Vector3* frictionX(T lx) {
					if (x < 0) {
						x += lx;
						if (x > 0) { x = 0; }
					} else if (x > 0) {
						x -= lx;
						if (x < 0) { x = 0; }
					}
					return this;
				}
				Vector3* frictionY(T ly) {
					if (y < 0) {
						y += ly;
						if (y > 0) { y = 0; }
					} else if (y > 0) {
						y -= ly;
						if (y < 0) { y = 0; }
					}
					return this;
				}
				Vector3* frictionZ(T lz) {
					if (z < 0) {
						z += lz;
						if (z > 0) { z = 0; }
					} else if (z > 0) {
						z -= lz;
						if (z < 0) { z = 0; }
					}
					return this;
				}

				// Restrict
				void restrictMax(Vector3<T>* max) {
					restrictMax(max->getX(), max->getY(), max->getZ());
				}
				void restrictMax(T lx, T ly, T lz) {
					if (x > lx) {
						x = lx;
					}
					if (y > ly) {
						y = ly;
					}
					if (z > lz) {
						z = lz;
					}

					if (x < lx * -1) {
						x = lx * -1;
					}
					if (y < ly * -1) {
						y = ly * -1;
					}
					if (z < lz * -1) {
						z = lz * -1;
					}
				}

				T dot(Vector3<T>* v) {
					return dot(v->getX(), v->getY(), v->getZ());
				}
				T dot(T lx, T ly, T lz) {
					return (x * lx) + (y * ly) + (z * lz);
				}
				static T dot(const Vector3<T> lhs, const Vector3<T>& rhs) {
					return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
				}

				Vector3<T> cross(const Vector3<T>& rhs) {
					return cross(rhs.x, rhs.y, rhs.z);
				}
				Vector3<T> cross(Vector3<T>* rhs) {
					return cross(rhs->x, rhs->y, rhs->z);
				}
				Vector3<T> cross(T lx, T ly, T lz) {
					return Vector3<T>(
						y * lz - z * ly,
						z * lx - x * lz,
						x * ly - y * lx
					);
				}
                static Vector3<T> cross(const Vector3<T>& lhs, const Vector3<T>& rhs) {
                    return Vector3<T>(
                        lhs.y * rhs.z - lhs.z * rhs.y,
                        lhs.z * rhs.x - lhs.x * rhs.z,
                        lhs.x * rhs.y - lhs.y * rhs.x
                    );
                }

				T lengthSquared() const {
					return (x*x) + (y * y) + (z * z);
				}
				T length() const {
					return (T) sqrt((double) lengthSquared());
				}

				Vector3<T> normalise() {
					T len = length();
					x /= len;
					y /= len;
					z /= len;
					return Vector3<T>(x,y,z);
				}

				Vector3<T> operator*=(const Vector3<T>& other) {
					x *= other.x;
					y *= other.y;
					z *= other.z;
					return *this;
				}
				Vector3<T> operator*=(float other) {
					x *= other;
					y *= other;
					z *= other;
					return *this;
				}
				Vector3<T> operator*(float other) {
					return Vector3<T>(x*other, y*other, z*other);
				}
				Vector3<T> operator*(float other) const {
					return Vector3<T>(x*other, y*other, z*other);
				}

				Vector3<T> operator+=(const Vector3<T>& other) {
					add(other.x, other.y, other.z);
					return *this;
				}
				Vector3<T> operator+=(float other) {
					add(other, other, other);
					return *this;
				}

				Vector3<T> operator-=(const Vector3<T>& other) {
					subtract(other.x, other.y, other.z);
					return *this;
				}
				Vector3<T> operator-=(float other) {
					subtract(other, other, other);
					return *this;
				}
				Vector3<T> operator+(const Vector3<T>& rhs) const {
					return Vector3<T>(x+rhs.x, y+rhs.y, z+rhs.z);
				}
				Vector3<T> operator-(const Vector3<T>& rhs) const {
					return Vector3<T>(x-rhs.x, y-rhs.y, z-rhs.z);
				}




				string toString() {
					string s;
					s += "{";
					s += "\"x\":";
                    s += ARK::Core::Cast::toString<T>(x);
					s += ", \"y\":";
					s += ARK::Core::Cast::toString<T>(y);
					s += ", \"z\":";
					s += ARK::Core::Cast::toString<T>(z);
					s += "}";
					return s;
					// TODO: dop this without incl;uding ark2d.h
				}

				T& operator[](unsigned int i) {
					return *(&x + i);
				}

				const T& operator[](unsigned int i) const {
					return *(&x + i);
				}

				~Vector3() {

				}
            };

            template<typename T>
			const Vector3<T> Vector3<T>::UP = Vector3<T>(0,1,0);

			template<typename T>
			const Vector3<T> Vector3<T>::FORWARD = Vector3<T>(0,0,1);
        }
	}
}
#endif /* ARK_GEOMETRY_VECTOR3_H_ */

