
#ifndef ARK_MATH_QUATERNION_H_
#define ARK_MATH_QUATERNION_H_

// http://blog.db-in.com/cameras-on-opengl-es-2-x/#quaternions
class Quaternion {
	public:
		float x;
		float y;
		float z;
		float w;
	public:
		Quaternion() {

		}
		
		void identity() {
			x = 0;
			y = 0;
			z = 0;
			w = 1;
		}
		
		void inverse() {
			x *= -1;
			y *= -1;
			z *= -1;
		}

		void fromVecAngle(Vector3<float> vec, float degrees) {
			float radians = MathUtil::toRadians(degrees);
			
			float s = sinf(radians * 0.5);
			float c = cosf(radians * 0.5);

			w = c;
			x = vec.m_x * s;
			y = vec.m_y * s;
			z = vec.m_z * s;
		}
		
		void fromEuler(Vector3<float> vec) {
			fromEuler(vec.getX(), vec.getY(), vec.getZ());
		}

		void fromEuler(float degreesX, float degreesY, float degreesZ) {
			float radiansY = MathUtil::toRadians(degreesY);
			float radiansZ = MathUtil::toRadians(degreesZ);
			float radiansX = MathUtil::toRadians(degreesX);

			// Finds the Sin and Cosin for each half angles.
			float sY = sinf(radiansY * 0.5);
			float cY = cosf(radiansY * 0.5);
			float sZ = sinf(radiansZ * 0.5);
			float cZ = cosf(radiansZ * 0.5); 
			float sX = sinf(radiansX * 0.5);
			float cX = cosf(radiansX * 0.5);

			// Formula to construct a new Quaternion based on Euler Angles.
			w = cY * cZ * cX - sY * sZ * sX;
			x = sY * sZ * cX + cY * cZ * sX;
			y = sY * cZ * cX + cY * sZ * sX;
			z = cY * sZ * cX - sY * cZ * sX;
		}

		~Quaternion() {

		}
};

#endif