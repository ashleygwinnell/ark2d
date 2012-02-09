/*
 * PathGroup.h
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#ifndef PATHGROUP_H_
#define PATHGROUP_H_

#include "../ARK2D.h"
#include "../Graphics.h"
#include "../Util/MathUtil.h"
#include "../GameTimer.h"
#include "../Geometry/Vector2.h"
#include <vector>

#include "Path.h"

using namespace std;

class PathGroup {
	private:
		vector<Path*> paths;
		Vector2<float> currentLocation;
		Vector2<float> centerLocation;
		Vector2<float> calcVector;
		unsigned int current;
		float timer;
		bool relative;

		string name;
		string description;

		bool m_isFlippedH;
		bool m_isFlippedV;
		float m_rotatedAngle;

	public:
		PathGroup();
		void setRelative(bool b);
		bool isRelative();
		void addPath(Path* p);
		void addPath(Path* p, unsigned int index);
		void removePath(unsigned int index);
		vector<Path*> getPaths();
		unsigned int getNumPaths();
		void clear();
		Path* getPath(unsigned int index);

		void setName(string n);
		void setDescription(string d);

		string getName();
		string getDescription();

		void setTimer(float t);
		float getTimer();
		void incrementTimer(float t);

		float getDuration();

		void update(GameTimer* gt);
		void updateCurrent();

		/*float getStartTime(unsigned int index);*/
		Vector2<float>* getLocation();
		unsigned int getIndex();

		Vector2<float>* getCenter();
		void updateCenter();

		void flip(bool hf, bool vf);
		void flip(bool hf, bool vf, int cx, int cy);
		void setFlip(bool hf, bool vf);
		bool isFlippedH();
		bool isFlippedV();

		void rotate(float degrees);
		void rotate(float degrees, int cx, int cy);
		void setRotation(float f);
		float getRotation();


		virtual void render();
		virtual void renderCurve();
		virtual void renderCurve(const Color& c);
		virtual void renderPoint(float x, float y, bool linkPoint);
		virtual void renderLine(float x, float y, float x2, float y2);
		virtual ~PathGroup();
};

#endif /* PATHGROUP_H_ */
