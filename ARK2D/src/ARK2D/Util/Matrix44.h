/*
 * Matrix44.h
 *
 *  Created on: 14 Apr 2011
 *      Author: Ashley
 */

#ifndef MATRIX44_H_
#define MATRIX44_H_

#include "../Common/DLL.h"
#include "Vector4.h"
#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Geometry/Vector3.h"
#include "MathUtil.h"
//#include "../UI/ErrorDialog.h"

 #include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtx/transform.hpp"
#include "../vendor/glm/gtc/quaternion.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

namespace ARK {
	namespace Util {


		/*!
		 * \brief Matrix manipulation for graphics rendering.
		 *
		 * This is not used internally.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Matrix44 {
			public:
				Vector4 col[4];

				Vector4* m_x;
				Vector4* m_y;
				Vector4* m_z;
				Vector4* m_w;

			public:
				Matrix44();
				Matrix44(float vals[16]);
				const Vector4& getX();
				const Vector4& getY();
				const Vector4& getZ();
				const Vector4& getW();

				void toValue(float v);
				void toZero();
				void toOne();

				void identity();

				void ortho2d(float l, float r, float b, float t, float n, float f);
				void frustum(float l, float r, float b, float t, float n, float f);
				void perspective(float fovDegrees, float aspectRatio, float znear, float zfar);

				void lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
				void inverse();



				string toString();

				float* pointer();
				const float* pointer() const;

                void scale(float x, float y, float z);
				void translate(float x, float y, float z);
				void rotate(float angle, float x, float y, float z);

                static Matrix44 createTranslateMatrix(float x, float y, float z);
                static Matrix44 createScaleMatrix(float x, float y, float z);
				static Matrix44 createRotationMatrix(float angleDegrees, float x, float y, float z);

				virtual void set(const glm::mat4& m);
				virtual void set(const Matrix44& m);

				Matrix44 copy();

				Matrix44 operator*(const Matrix44& m) const;
				Matrix44& operator*=(const Matrix44& m);
				Vector4 operator*(const Vector4& other) const;

				Vector4& operator[](unsigned int i);
				const Vector4& operator[](unsigned int i) const;

				virtual ~Matrix44();
		};

	}
}

#endif /* MATRIX44_H_ */
