/*
 * Vector4.cpp
 *
 *  Created on: Dec 8, 2011
 *      Author: ashleygwinnell
 */

#include "Vector4.h"
#include "../Math/Matrix44.h"

namespace ARK {
	namespace Core {
        namespace Geometry {

            Vector4::Vector4() {

            }
            Vector4::Vector4(const Vector4& r):
                x(r.x),
                y(r.y),
                z(r.z),
                w(r.w) {

            }
            Vector4::Vector4(float coords[]) {
                x = coords[0];
                y = coords[1];
                z = coords[2];
                w = coords[3];
            }
            Vector4::Vector4(float lx, float ly, float lz, float lw) {
                x = lx;
                y = ly;
                z = lz;
                w = lw;
            }

            float Vector4::getX() { return x; }
            float Vector4::getY() { return y; }
            float Vector4::getZ() { return z; }
            float Vector4::getW() { return w; }

            void Vector4::setX(float lx) { x = lx; }
            void Vector4::setY(float ly) { y = ly; }
            void Vector4::setZ(float lz) { z = lz; }
            void Vector4::setW(float lw) { w = lw; }

            Vector4* Vector4::add(Vector4 v) {
                return add(v.getX(), v.getY(), v.getZ(), v.getW());
            }
            Vector4* Vector4::add(float lx, float ly, float lz, float lw) {
                x += lx;
                y += ly;
                z += lz;
                w += lw;
                return this;
            }

            void Vector4::set(Vector4 v) {
                set(v.getX(), v.getY(), v.getZ(), v.getW());
            }
            void Vector4::set(float lx, float ly, float lz, float lw) {
                x = lx;
                y = ly;
                z = lz;
                w = lw;
            }

            Vector4* Vector4::subtract(Vector4 v) {
                return subtract(v.getX(), v.getY(), v.getZ(), v.getW());
            }
            Vector4* Vector4::subtract(float lx, float ly, float lz, float lw) {
                x -= lx;
                y -= ly;
                z -= lz;
                w -= lw;
                return this;
            }

            // Multiply
            Vector4* Vector4::multiply(Vector4& v) {
                return multiply(v.getX(), v.getY(), v.getZ(), v.getW());
            }
            Vector4* Vector4::multiply(Vector4* v) {
                return multiply(v->getX(), v->getY(), v->getZ(), v->getW());
            }
            Vector4* Vector4::multiply(float lx, float ly, float lz, float lw) {
                x *= lx;
                y *= ly;
                z *= lz;
                w *= lw;
                return this;
            }

            void Vector4::toValue(float v) {
                x = v;
                y = v;
                z = v;
                w = v;
            }
            void Vector4::toZero() {
                toValue(0);
            }
            void Vector4::toOne() {
                toValue(1);
            }

            Vector4 Vector4::copy() {
                return Vector4(x, y, z, w);
            }

            float& Vector4::operator[](unsigned int i) {
                return *(&x + i);
            }

            const float& Vector4::operator[](unsigned int i) const {
                return *(&x + i);
            }

            Vector4 Vector4::operator+=(const Vector4& other) {
                add(other[0], other[1], other[2], other[3]);
                return *this;
            }
            Vector4 Vector4::operator*=(const Vector4& other) {
                multiply(other[0], other[1], other[2], other[3]);
                return *this;
            }
            Vector4 Vector4::operator*=(float other) {
                multiply(other, other, other, other);
                return *this;
            }
            Vector4 Vector4::operator*(float other) {
                multiply(other, other, other, other);
                return *this;
            }

            Vector4 Vector4::operator*(const Matrix44& m) {
                Vector4 ret(*this);
                ret *= m;
                return ret;
            }
            Vector4 Vector4::operator*=(const Matrix44& m) {
                Vector4::multMatrix44(x, y, z, w, m);
                return *this;
            }

            void Vector4::multMatrix44(float& x, float& y, float& z, float& w, const Matrix44& m) {
                float newX = x*m[0][0] + y*m[1][0] + z*m[2][0] + w*m[3][0];
                float newY = x*m[0][1] + y*m[1][1] + z*m[2][1] + w*m[3][1];
                float newZ = x*m[0][2] + y*m[1][2] + z*m[2][2] + w*m[3][2];
                float newW = x*m[0][3] + y*m[1][3] + z*m[2][3] + w*m[3][3];
                x = newX;
                y = newY;
                z = newZ;
                w = newW;
            }

            Vector4::~Vector4() {

            }
        }
	}
}
