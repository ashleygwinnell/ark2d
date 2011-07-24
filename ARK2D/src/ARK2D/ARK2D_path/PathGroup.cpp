/*
 * PathGroup.cpp
 *
 *  Created on: 5 Jul 2011
 *      Author: Ashley
 */

#include "PathGroup.h"
#include "SubPath.h"

PathGroup::PathGroup(): paths(), currentLocation(), calcVector(), current(0), timer(0.0f), relative(false) {
	calcVector.set(0, 0);
	currentLocation.set(0, 0);
}

void PathGroup::setRelative(bool b) {
	relative = b;
}
bool PathGroup::isRelative() {
	return relative;
}
void PathGroup::addPath(Path* p) {
	paths.push_back(p);
}
void PathGroup::addPath(Path* p, unsigned int index) {
	paths.insert(paths.begin() + index, p);
}
void PathGroup::removePath(unsigned int index) {
	paths.erase(paths.begin() + index);
}
void PathGroup::clear() {
	paths.clear();
}
vector<Path*> PathGroup::getPaths() {
	return paths;
}
unsigned int PathGroup::getNumPaths() {
	return paths.size();
}
Path* PathGroup::getPath(unsigned int index) {
	return paths.at(index);
}

void PathGroup::setName(string n) {
	name = n;
}
void PathGroup::setDescription(string d) {
	description = d;
}

string PathGroup::getName() {
	return name;
}
string PathGroup::getDescription() {
	return description;
}

void PathGroup::setTimer(float t) {
	timer = t;
}
float PathGroup::getTimer() {
	return timer;
}
void PathGroup::incrementTimer(float t) {
	timer += t;
}

void PathGroup::update(GameTimer* gt) {
	timer += gt->getDelta();

	float d = getDuration();
	if (timer >= d) {
		timer = d;
	}

	updateCurrent();
}
void PathGroup::updateCurrent() {

	// reset the timers
	for(unsigned int i = 0; i < paths.size(); i++) {
		paths.at(i)->timer = 0.0f;
	}

	// set the timers correctly.
	float acc = 0.0f;
	for(unsigned int i = 0; i < paths.size(); i++) {

		float part = timer - acc; // e.g. 5.2f - 5.0f = 0.2f
		if (timer >= acc) {
			current = i;
			paths.at(i)->timer = part;
			if (timer >= acc + paths.at(i)->duration) {
				paths.at(i)->timer = paths.at(i)->duration;
			}
		} else {
			paths.at(i)->timer = part;
			break;
		}
		acc += paths.at(i)->duration;
	}

	for(unsigned int h = 0; h < paths.size(); h++)
	{
		Path* path = paths.at(h);
		for(unsigned int i = 0; i < path->subpaths.size(); i++)
		{
			SubPath* thisPath = path->subpaths.at(i);
			for(unsigned int j = 0; j < thisPath->points.size(); j++)
			{
				if (j+1 < thisPath->points.size())
				{
					//std::cout << "path index: " << i << " - size: " << path->subpaths.at(i)->points.size() << std::endl;
					double angle = MathUtil::anglef(thisPath->points.at(j)->getX(), thisPath->points.at(j)->getY(),
													thisPath->points.at(j+1)->getX(), thisPath->points.at(j+1)->getY());
					double totalDistance = MathUtil::distance( 	thisPath->points.at(j)->getX(), thisPath->points.at(j)->getY(),
																thisPath->points.at(j+1)->getX(), thisPath->points.at(j+1)->getY());

					float e = (float) Easing::ease(
						path->easing,
						path->timer,
						0.0f,
						1.0f,
						path->duration
					);

					calcVector.set(thisPath->points.at(j)->getX(), thisPath->points.at(j)->getY());
					//MathUtil::moveAngle(&calcVector, angle, totalDistance * (path->timer / path->duration));
					MathUtil::moveAngle(&calcVector, angle, totalDistance * e);

					//pointImage->setSize(8, 8);
					//pointImage->drawCentered(v->getX(), v->getY());

					path->subpaths.at(i+1)->points.at(j)->set(calcVector.getX(), calcVector.getY());

					//delete v;
				}
			}
		}
	}
}
float PathGroup::getDuration() {
	float f = 0.0f;
	for(unsigned int i = 0; i < paths.size(); i++) {
		f += paths.at(i)->duration;
	}
	return f;
}
unsigned int PathGroup::getIndex() {
	return current;
}
/*float PathGroup::getStartTime(unsigned int index) {
	if (index >= paths.size()) {
		string s = StringUtil::append("PathGroup::getStartTime() called with index ", index);
		ErrorDialog::createAndShow(s);
		exit(0);
	}
	float f = 0.0f;
	for(unsigned int i = 0; i < paths.size(); i++) {
		if (index == i) { return f; }
		f += paths.at(i)->duration;
	}
	return f;
}*/
Vector2<float>* PathGroup::getLocation() {
	Path* path = paths.at(current);
	SubPath* lastPath = path->subpaths.at(path->subpaths.size()-1);
	Vector2<float>* lastPoint = lastPath->points.at(lastPath->points.size()-1);

	if (relative) {
		currentLocation.setX(lastPoint->getX() - paths.at(0)->getPoint(0)->getX());
		currentLocation.setY(lastPoint->getY() - paths.at(0)->getPoint(0)->getY());
		return &currentLocation;
	}
	return lastPoint;
}
void PathGroup::render() {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white);

	// render points.
	for(unsigned int i = 0; i < paths.size(); i++) {
		for(unsigned int j = 0; j < paths.at(i)->subpaths.at(0)->points.size(); j++) {
			Vector2<float>* p = paths.at(i)->subpaths.at(0)->points.at(j);
			renderPoint(p->getX(), p->getY(), (j == 0 || j == paths.at(i)->subpaths.at(0)->points.size()-1));
		}
	}

	// render curve.
	renderCurve();

	// render straight lines.
	for(unsigned int i = 0; i < paths.size(); i++) {
		SubPath* mainPath = paths.at(i)->subpaths.at(0);
		for(unsigned int j = 0; j < mainPath->points.size(); j++) {
			if (j+1 != mainPath->points.size()) {
				renderLine(
					mainPath->points.at(j)->getX(),
					mainPath->points.at(j)->getY(),
					mainPath->points.at(j+1)->getX(),
					mainPath->points.at(j+1)->getY()
				);
			}
		}
	}



}
void PathGroup::renderCurve() {
	Graphics* g = ARK2D::getGraphics();

	float ptimer = getTimer();
	g->setDrawColor(Color::red);
	setTimer(0.0f);
	updateCurrent();
	float px = getLocation()->getX(); //paths.at(0)->subpaths.at(0)->points.at(0)->getX();
	float py = getLocation()->getY();//paths.at(0)->subpaths.at(0)->points.at(0)->getY();
	float du = getDuration();
	for(float i = 0.05f; i <= du; i += 0.05f)
	{
		setTimer(i);
		updateCurrent();

		Vector2<float>* f = getLocation();
		g->drawLine(int(px), int(py), int(f->getX()), int(f->getY()));

		px = f->getX();
		py = f->getY();
	}
	setTimer(ptimer);
	updateCurrent();
}
void PathGroup::renderPoint(float x, float y, bool linkPoint) {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white);
	if (linkPoint) {
		g->fillCircle(int(x), int(y), 10, 10);
	} else {
		g->fillCircle(int(x), int(y), 5, 10);
	}
}
void PathGroup::renderLine(float x, float y, float x2, float y2) {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white);
	g->drawLine(int(x), int(y), int(x2), int(y2));
}

PathGroup::~PathGroup() {
	//for(unsigned int i = 0; i < paths.size(); i++) {
	//	delete paths.at(i);
	//}
	//paths.clear();
}

