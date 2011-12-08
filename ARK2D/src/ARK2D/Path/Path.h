/*
 * Path.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef PATH_H_
#define PATH_H_

class PathGroup;
class SubPath;
#include "../Easing.h"

#include <vector>
using namespace std;

class Path {
	friend class PathGroup;

	private:
		vector<SubPath*> subpaths;
		float timer;
		float duration;
		unsigned int easing;

	public:
		Path();

		void setDuration(float d);
		float getDuration();

		void setEasing(unsigned int e);
		unsigned int getEasing();

		void addPoint(float x, float y);
		void addPoint(float x, float y, int index);
		void removePoint(int index);
		Vector2<float>* getPoint(unsigned int i);
		vector<Vector2<float>*> getPoints();
		unsigned int getSize();

		virtual ~Path();
};

#endif /* PATH_H_ */
