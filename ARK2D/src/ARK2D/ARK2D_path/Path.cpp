/*
 * Path.cpp
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#include "Path.h"
#include "SubPath.h"
#include "PathGroup.h"

Path::Path(): subpaths(), timer(0.0f), duration(1.0f), easing(Easing::LINEAR) {

}
void Path::setDuration(float d) {
	duration = d;
}
float Path::getDuration() {
	return duration;
}
void Path::setEasing(unsigned int e) {
	easing = e;
}
unsigned int Path::getEasing() {
	return easing;
}
void Path::addPoint(float x, float y) {
	subpaths.push_back(new SubPath());
	for(unsigned int i = 0; i < subpaths.size(); i++) {
		subpaths.at(i)->points.push_back(new Vector2<float>(x, y));
	}
}
void Path::addPoint(float x, float y, int index) {
	vector<Vector2<float>*> points = subpaths.at(0)->points;

	for(unsigned int i = 0; i < subpaths.size(); i++) {
		delete subpaths.at(i);
	}
	subpaths.clear();

	points.insert(points.begin() + index, new Vector2<float>(x, y));
	for(unsigned int i = 0; i < points.size(); i++) {
		addPoint(points.at(i)->getX(), points.at(i)->getY());
	}
}
/**
 * This DOES shift the indexes down.
 */
void Path::removePoint(int index) {
	vector<Vector2<float>*> points = subpaths.at(0)->points;
	for(unsigned int i = 0; i < subpaths.size(); i++) {
		delete subpaths.at(i);
	}
	subpaths.clear();

	points.erase(points.begin() + index);

	for(unsigned int i = 0; i < points.size(); i++) {
		addPoint(points.at(i)->getX(), points.at(i)->getY());
	}
}
Vector2<float>* Path::getPoint(unsigned int i) {
	return subpaths.at(0)->points.at(i);
}
vector<Vector2<float>*> Path::getPoints() {
	return subpaths.at(0)->points;
}
unsigned int Path::getSize() {
	return subpaths.at(0)->points.size();
}

Path::~Path() {
	//for(unsigned int i = 0; i < subpaths.size(); i++) {
	//	delete subpaths.at(i);
	//}
	//subpaths.clear();
}
