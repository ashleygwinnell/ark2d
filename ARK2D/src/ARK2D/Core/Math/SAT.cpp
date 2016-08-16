
#include "SAT.h"
#include "MathUtil.h"
#include "../String.h"
#include "../Geometry/Polygon.h"
#include <algorithm>

SATIntersectionReturns::SATIntersectionReturns(bool b): 
	intersects(b),
	x(0),
	y(0) { 

}

SATIntersectionReturns::SATIntersectionReturns(float xx, float yy): 
	intersects(false), 
	x(xx), 
	y(yy) { 

}

SATIntersectionReturns::~SATIntersectionReturns() {

}





SATVector::SATVector() {
	
}

/*SATVector::SATVector(float x1, float y1, float angle_degrees, int length) {
	
	double angleRadians = MathUtil::toRadians(angle_degrees);

	this->x1 = x1;  
	this->y1 = y1;
	this->x2 = (float) (cos(angleRadians) * length);
	this->y2 = (float) (sin(angleRadians) * length);
	//this->x2 = x1 + (float) (cos(angleRadians) * length);
	//this->y2 = y1 + (float) (sin(angleRadians) * length);
	init();
} */

SATVector::SATVector(float x1, float y1, float x2, float y2) {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	init();
}

void SATVector::init() {
	this->vx = this->x2 - this->x1;
	this->vy = this->y2 - this->y1;
	this->length = this->calculateLength();
	this->angle = this->calculateAngleRadians(); 
	this->angle_degrees = this->calculateAngleDegrees();
}
 
float SATVector::calculateLength() {
	return sqrt(this->vx * this->vx + this->vy * this->vy);
}
float SATVector::calculateLengthSquared() {
	return this->vx * this->vx + this->vy * this->vy;
}
float SATVector::calculateAngleRadians() {

	return MathUtil::toRadians(MathUtil::anglef(0, 0, this->vx, this->vy));
	// return Math.atan2(this.vy, this.vx);
	//return 0;
}
float SATVector::calculateAngleDegrees() {
	return MathUtil::anglef(0, 0, this->vx, this->vy); //this->angle * 180 / MathUtil::PIE;
} 

SATVector SATVector::getNormal() {
	return SATVector(vy*-1, vx, vy, vx*-1);
}

void SATVector::divide(float f) {
	this->x1 = this->x1 / f;
	this->y1 = this->y1 / f;
	this->x2 = this->x2 / f;
	this->y2 = this->y2 / f;
	
	init();
}

string SATVector::toString() {
	String ss;
	ss += "Vector [x1: ";
	ss += x1;
	ss += ", y1: ";
	ss += y1;

	ss += ", x2: ";
	ss += x2;
	ss += ", y2: ";
	ss += y2;

	ss += ", vx: ";
	ss += vx;
	ss += ", vy: ";
	ss += vy;
/*
	ss += ", length: "; 
	ss += Cast::toString<double>(length);

	ss += ", angle_degrees: ";
	ss += Cast::toString<double>(angle_degrees);*/

	ss += "]";
	//return "Vector [x: " + x1 + ", y: " + y1 + "]";
	return ss.get();
}








CollisionInfo::CollisionInfo(bool b):
	intersects(b),
	one(NULL),
	two(NULL),
	distance(0.0f),
	vector(),
	translation(),
	shapeAContained(false),
	shapeBContained(false)
	{

}
string CollisionInfo::toString() {
	//return "CollisionInfo [vector: " + vector + ", dp: " + distance + ", translation: " + translation + "]";
	String ss("");
	ss += "distance: ";
	ss += Cast::toString<float>(distance);
	ss += ". vector: ";
	ss += vector.toString();
	ss += ". translation: ";
	ss += translation.toString();
	return ss.get();
}


CollisionInfo SAT::intersects(ARK::Core::Geometry::Polygon<int>* one, ARK::Core::Geometry::Polygon<int>* two) {
	
	vector<SATVector> one_vectors; 
	SAT::getVectors(one, &one_vectors);
	
	vector<SATVector> two_vectors;
	SAT::getVectors(two, &two_vectors); 
			
	CollisionInfo result(true);// = new CollisionInfo();
	result.one = one;
	result.two = two; 
	
	float minimum_translation_distance = 0;
	float minimum_translation_distance_abs = 0;
	float shortest = std::numeric_limits<float>::max();//Float.MAX_VALUE;
	
	// These variables are used on for each axis to see if the two shapes overlap.
	float one_min_dp = 0;
	float one_max_dp = 0;
	float two_min_dp = 0;
	float two_max_dp = 0;
	
	//for each axis on the first shape.
	for (unsigned int i = 0; i < one_vectors.size(); ++i) {
		// get the axis. 
		const SATVector& normal = one_vectors[i].getNormal();
		
		// for each point on the first polygon, find the min and max
		for (unsigned int j = 0; j < one_vectors.size(); ++j) {
			float dp = dotproduct(normal.x1, normal.y1, one_vectors[j].x1, one_vectors[j].y1);
			if (j == 0) {
				one_min_dp = dp;
				one_max_dp = dp;
			}
			if (dp < one_min_dp) { one_min_dp = dp; }
			if (dp > one_max_dp) { one_max_dp = dp; }
		}
		
		// for each point on the second polygon, find the min and max.
		for (unsigned int k = 0; k < two_vectors.size(); ++k) {
			float dp = dotproduct(normal.x1, normal.y1, two_vectors[k].x1, two_vectors[k].y1);
			if (k == 0) {
				two_min_dp = dp;
				two_max_dp = dp;
			}
			if (dp < two_min_dp) { two_min_dp = dp; }
			if (dp > two_max_dp) { two_max_dp = dp; }
		}
		
		// test for (lack of) intersections.
		if ((one_max_dp < two_min_dp || two_max_dp < one_min_dp)
				|| (two_min_dp > one_max_dp || one_min_dp > two_max_dp) ) {
			return CollisionInfo(false); // null
		}
		//projections_lines.add(normal);
		
		
		
		// and translation distance...
		minimum_translation_distance = (two_max_dp - one_min_dp) * -1;  //Math.min(dist0, dist1);
		minimum_translation_distance_abs = (minimum_translation_distance < 0) ? minimum_translation_distance * -1 : minimum_translation_distance;
		// this distance is shorter so use it...
		if (minimum_translation_distance_abs < shortest) {
			result.distance = minimum_translation_distance;
			result.vector = normal;
			
			SATVector v; // = new Vector();
			v.x1 = ( minimum_translation_distance / (normal.x1*normal.x1 + normal.y1*normal.y1) ) * normal.x1;
			v.y1 = ( minimum_translation_distance / (normal.x1*normal.x1 + normal.y1*normal.y1) ) * normal.y1;
			result.translation = v;
			shortest = minimum_translation_distance_abs;
			
		}
		
	}
		
	//for each axis on the second shape.
	for (unsigned int i = 0; i < two_vectors.size(); ++i) {
		// get the axis.
		const SATVector& normal = two_vectors[i].getNormal();
		 
		// for each point on the first polygon, find the min and max
		for (unsigned int j = 0; j < one_vectors.size(); ++j) {
			float dp = dotproduct(normal.x1, normal.y1, one_vectors[j].x1, one_vectors[j].y1);
			if (j == 0) {
				one_min_dp = dp;
				one_max_dp = dp;
			}
			if (dp < one_min_dp) { one_min_dp = dp; }
			if (dp > one_max_dp) { one_max_dp = dp; }
		}
		
		// for each point on the second polygon, find the min and max.
		for (unsigned int k = 0; k < two_vectors.size(); ++k) {
			float dp = dotproduct(normal.x1, normal.y1, two_vectors[k].x1, two_vectors[k].y1);
			if (k == 0) {
				two_min_dp = dp;
				two_max_dp = dp;
			}
			if (dp < two_min_dp) { two_min_dp = dp; }
			if (dp > two_max_dp) { two_max_dp = dp; }
		}
		
		// test for (lack of) intersections.
		if ((one_max_dp < two_min_dp || two_max_dp < one_min_dp)
				|| (two_min_dp > one_max_dp || one_min_dp > two_max_dp) ) {
			return CollisionInfo(false); //null;
		}
		//projections_lines.add(normal);
		
		// and translation distance...
		minimum_translation_distance = (two_max_dp - one_min_dp) * -1;  //Math.min(dist0, dist1);
		minimum_translation_distance_abs = (minimum_translation_distance < 0) ? minimum_translation_distance * -1 : minimum_translation_distance;
		// this distance is shorter so use it...
		if (minimum_translation_distance_abs < shortest) {
			result.distance = minimum_translation_distance;
			result.vector = normal;
			
			SATVector v;// = new Vector();
			v.x1 = ( minimum_translation_distance / (normal.x1*normal.x1 + normal.y1*normal.y1) ) * normal.x1;
			v.y1 = ( minimum_translation_distance / (normal.x1*normal.x1 + normal.y1*normal.y1) ) * normal.y1;
			result.translation = v;
			shortest = minimum_translation_distance_abs;
			
		}
		
		
	}
	
		// shift polygonA's projected points
		//	float sOffset = dotproduct(normal.x1, normal.y1, vOffset[0], vOffset[1]);
		//	one_min_dp += sOffset;
		//	one_max_dp += sOffset;
		
		
	
	//System.out.println(index);
	return result;
}


float SAT::dotproduct(float x1, float y1, float x2, float y2) {
	return ((x1 * x2) + (y1 * y2));
}
float SAT::dotproduct_inverse(float dp, float x1, float y1, float x2, float y2) {
	return ((x1 * x2) + (y1 * y2));
}

vector<SATVector> SAT::getVectors(ARK::Core::Geometry::Polygon<int>* p) {

	vector<SATVector> vectors;
	getVectors(p, &vectors);
	return vectors;
}

void SAT::getVectors(ARK::Core::Geometry::Polygon<int>* p, vector<SATVector>* resultVectors) {
	//float[] points = p->getPoints();
	vector<Vector2<int> >* points = p->getPoints();

	resultVectors->reserve(points->size());
 
	//vector<SATVector> vectors;
	for (unsigned int i = 0; i < points->size(); ++i) 
	{
		unsigned int inext = i + 1;
		if (inext == points->size()) { inext = 0; }

		Vector2<int>* ths = &points->at(i);
		Vector2<int>* next = &points->at(inext);
		
		SATVector v(ths->getX(), ths->getY(), next->getX(), next->getY());
		//vectors.push_back(v);
		resultVectors->push_back(v);
	}

	//return vectors;

	//Vector[] vectors = new Vector[points.length/2];
	// int l = 0;
	// int next_x = 0; // these two are used for wrap around when out of array bounds. 
	// int next_y = 0; 
	// for (int i = 0; i < points.size(); i += 2) {
	// 	next_x = (i+2 >= points.size())?0:i+2;
	// 	next_y = (i+3 >= points.size())?1:i+3;
	// 	//vectors[l] = new Vector((int) points[i], (int)points[i+1], (int) points[next_x], (int)points[next_y]);

	// 	SATVector satvec( int(points.at(i)->getX()), int(points.at(i+1)), int(points.at(next_x)), int(points.at(next_y) ))
	// 	vectors.push_back(satvec);
	// 	l += 1;
	// }
	// return vectors;	
}

SATIntersectionReturns SAT::intersectionUnlimited(const SATVector& one, const SATVector& two) 
{
	// Store the values for fast access and easy
	// equations-to-code conversion
	float x1 = one.x1;
	float x2 = one.x2;
	float x3 = two.x1;
	float x4 = two.x2;

	float y1 = one.y1;
	float y2 = one.y2;
	float y3 = two.y1;
	float y4 = two.y2; 
 
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return SATIntersectionReturns(false);
 
	// Get the x and y
	float pre = (x1 * y2 - y1 * x2);
	float post = (x3 * y4 - y3 * x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
 
	return SATIntersectionReturns(x, y); //new float[] {x, y};
	// this isn't even used in the java version!!?!!
}
 
SATIntersectionReturns SAT::intersection(const SATVector& one, const SATVector& two) 
{
	// Store the values for fast access and easy
	// equations-to-code conversion
	float x1 = one.x1;
	float x2 = one.x2;
	float x3 = two.x1; 
	float x4 = two.x2;  
 
	float y1 = one.y1;
	float y2 = one.y2;
	float y3 = two.y1;
	float y4 = two.y2;
 
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return SATIntersectionReturns(false);
 
	// Get the x and y
	float pre = (x1*y2 - y1*x2);
	float post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
 
	// Check if the x and y coordinates are within both lines
	if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
		x < std::min(x3, x4) || x > std::max(x3, x4) ) return SATIntersectionReturns(false);
	if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
		y < std::min(y3, y4) || y > std::max(y3, y4) ) return SATIntersectionReturns(false);
 
	// Return the point of intersection
	//Vector2f ret = new Vector2f();
	//ret.x = x;
	//ret.y = y;
	return SATIntersectionReturns(x, y); //new float[] {x, y};
}
	

