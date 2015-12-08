
#ifndef ARK_MATH_QUATERNION_H_
#define ARK_MATH_QUATERNION_H_

// http://blog.db-in.com/cameras-on-opengl-es-2-x/#quaternions
#define QUAT_PI 3.14159265359f
#include "../Util/Matrix44.h"
template <class T>
class Quaternion {
	public:
		T w;
		T x;
		T y;
		T z;
		
	public:
		Quaternion():
			w(1),
			x(0),
			y(0),
			z(0)
			{

		}
		Quaternion(float lw, float lx, float ly, float lz):
			w(lw),
			x(lx),
			y(ly),
			z(lz)
			{
		}
		Quaternion(float angle, float ax, float ay, float az, bool angleAxis):
			w(1),
			x(0),
			y(0),
			z(0)
			{
			fromVecAngle(ax, ay, az, angle);
		}
		
		void identity() {
			w = 1;
			x = 0;
			y = 0;
			z = 0;
		}
		
		void inverse() {
			x *= -1;
			y *= -1;
			z *= -1;
		}
		void normalise() {
			double n = sqrt(x*x + y*y + z*z + w*w);
		    x /= n;
		    y /= n;
		    z /= n;
		    w /= n;
		}

		void fromVecAngle(Vector3<T> vec, T degrees) {
            fromVecAngle(vec.getX(), vec.getY(), vec.getZ(), degrees);
		}
		void fromVecAngle(T vecX, T vecY, T vecZ, T degrees) {
            T radians = degrees * QUAT_PI/180.0f;
			
			T s = sin(radians * 0.5);
			T c = cos(radians * 0.5);

			w = c;
			x = vecX * s;
			y = vecY * s;
			z = vecZ * s;
		}
		
		void fromEuler(Vector3<T> vec) {
			fromEuler(vec.getX(), vec.getY(), vec.getZ());
		}

		void fromEuler(T degreesX, T degreesY, T degreesZ) {
            T radiansY = degreesY * QUAT_PI/180.0f; //MathUtil::toRadians(degreesY);
            T radiansZ = degreesZ * QUAT_PI/180.0f; //MathUtil::toRadians(degreesZ);
            T radiansX = degreesX * QUAT_PI/180.0f; //MathUtil::toRadians(degreesX);

			// Finds the Sin and Cosin for each half angles.
			T sY = sin(radiansY * 0.5);
			T cY = cos(radiansY * 0.5);
			T sZ = sin(radiansZ * 0.5);
			T cZ = cos(radiansZ * 0.5); 
			T sX = sin(radiansX * 0.5);
			T cX = cos(radiansX * 0.5);

			// Formula to construct a new Quaternion based on Euler Angles.
			w = cY * cZ * cX - sY * sZ * sX;
			x = sY * sZ * cX + cY * cZ * sX;
			y = sY * cZ * cX + cY * sZ * sX;
			z = cY * sZ * cX - sY * cZ * sX;
		}
		Vector3<T> toEuler() {
			Vector3<T> ret;
			ret.x = atan2(2*y*w - 2*x*z, 1 - 2*y*y - 2*z*z);
 			ret.y = atan2(2*x*w - 2*y*z, 1 - 2*x*x - 2*z*z);
 			ret.z = asin(2*x*y + 2*z*w);
 			return ret;
		}

        // angle in degrees
		T angle() {
            return angleRadians() * (180.0/QUAT_PI);
		}
        T angleRadians() {
            return 2.0f * acos(w);
        }
		Vector3<T> axis() {
			Vector3<T> ret;
			ret.x = x / sqrt(1-w*w);
			ret.y = y / sqrt(1-w*w);
			ret.z = z / sqrt(1-w*w);
			return ret;
		}

		Matrix44<float> toMatrix() {
            float vals[16] = {
                1.0f - 2.0f*y*y - 2.0f*z*z, 2.0f*x*y - 2.0f*z*w, 		2.0f*x*z + 2.0f*y*w, 		0.0f,
                2.0f*x*y + 2.0f*z*w, 		1.0f - 2.0f*x*x - 2.0f*z*z, 2.0f*y*z - 2.0f*x*w, 		0.0f,
                2.0f*x*z - 2.0f*y*w, 		2.0f*y*z + 2.0f*x*w, 		1.0f - 2.0f*x*x - 2.0f*y*y, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            return Matrix44<float>(vals);
		}

		Quaternion<T> cross(const Quaternion<T>& other) {
			glm::quat th(w, x, y, z);
			glm::quat th2(other.w, other.x, other.y, other.z);
			glm::quat ret = glm::cross(th, th2);
			return Quaternion<T>(ret.w, ret.x, ret.y, ret.z);
		}

		Quaternion<T> operator+=(const Quaternion<T>& other) {
			const Quaternion<T>& q1 = *this;
			const Quaternion<T>& q2 = other;
			T x = q1.x + q2.x;
		    T y = q1.y + q2.y;
		    T z = q1.z + q2.z;
		    T w = q1.w + q2.w;
    		this->x = x;
    		this->y = y;
    		this->z = z;
    		this->w = w;
    		return *this; 
		}
		Quaternion<T> operator*=(const Quaternion<T>& other) {
			const Quaternion<T>& q1 = *this;
			const Quaternion<T>& q2 = other;
			T x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    		T y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    		T z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    		T w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    		this->x = x;
    		this->y = y;
    		this->z = z;
    		this->w = w;
    		return *this; 
		}
		void set(glm::quat& q){
			w = q.w;
			x = q.x;
			y = q.y;
			z = q.z;
		}

		static Quaternion<T> angleAxis(float angleDegrees, float x, float y, float z) {
			return Quaternion<T>(angleDegrees, x, y, z, true);
		}
        static Quaternion<T> angleAxis(float angleDegrees, Vector3<float>& v) {
            return Quaternion<T>(angleDegrees, v.x, v.y, v.z, true);
        }

		~Quaternion() {

		}
};

#endif