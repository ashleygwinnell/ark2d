
#ifndef ARK_CORE_MATH_SAT_H_
#define ARK_CORE_MATH_SAT_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace ARK {
    namespace Core {
        namespace Geometry {

            template <class T=int>
            class Polygon;
        }
	}
}

namespace ARK {
	namespace Core {
		namespace Math {


		// http://www.sevenson.com.au/actionscript/sat/

			class ARK2D_API SATIntersectionReturns {
				public:
					bool intersects;
					float x;
					float y;
					SATIntersectionReturns(bool b);
					SATIntersectionReturns(float xx, float yy);
					virtual ~SATIntersectionReturns();
			};

			class ARK2D_API SATVector {
				public:
					float x1; // start x
					float y1; // start y

					float x2; // end x
					float y2; // end y

					float vx; // difference in x
					float vy; // difference in y

					float length; // length, uses pythagoras.

					float angle; // angle of vector in radians (measurements of pi)
					float angle_degrees; // angle of vector in degrees.

					float dx; // direction x of vector from 0.0 to 1.0 (1.0 if facing right, -1.0 if facing left.).
					float dy; // direction y of vector from 0.0 to 1.0 (1.0 if facing down, -1.0 if facing up).

					float dotproduct; // multiply x's and ADD multiply y's. if negative, vectors facing away, else same direction.

					SATVector();

					//SATVector(float x1, float y1, float angle_degrees, int length);

					SATVector(float x1, float y1, float x2, float y2);

					void init();

					float calculateLength();
					float calculateLengthSquared();
					float calculateAngleRadians();
					float calculateAngleDegrees();

					SATVector getNormal();

					void divide(float f);
					string toString();
			};

			class ARK2D_API CollisionInfo {

				public:
					bool intersects;
                    ARK::Core::Geometry::Polygon<int>* one;
					ARK::Core::Geometry::Polygon<int>* two;
					float distance;
					SATVector vector;
					SATVector translation;
					bool shapeAContained;
					bool shapeBContained;

					CollisionInfo(bool b);
					string toString();
			};

			class ARK2D_API SAT {
				public:
                    static CollisionInfo intersects(ARK::Core::Geometry::Polygon<int>* one, ARK::Core::Geometry::Polygon<int>* two);
					static float dotproduct(float x1, float y1, float x2, float y2);
					static float dotproduct_inverse(float dp, float x1, float y1, float x2, float y2);
					static vector<SATVector> getVectors(ARK::Core::Geometry::Polygon<int>* p);
					static void getVectors(ARK::Core::Geometry::Polygon<int>* p, vector<SATVector>* resultVectors);
					static SATIntersectionReturns intersectionUnlimited(const SATVector& one, const SATVector& two);
					static SATIntersectionReturns intersection(const SATVector& one, const SATVector& two);
			};
		}
	}
}

#endif /* ARK_CORE_MATH_SAT_H_ */


