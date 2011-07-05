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

Path::~Path() {

}
