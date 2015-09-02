/*
 * AdvancedPolygon.cpp
 *
 *  Created on: 28 August 2015
 *      Author: ashleygwinnell
 */

#include "AdvancedPolygon.h"

void AdvancedPolygon::updateLines() {
	lines.clear();

	for (unsigned int i = 0; i < holes.size(); i++)
	{
		vector<Vector2<float> >* poly = holes[i].getPoints();
		for (unsigned int j = 0; j < poly->size(); j++) {
			Vector2<float>* start = &poly->at(j);
			Vector2<float>* end = &poly->at((j + 1) % poly->size());

			ARK::Geometry::Line<float> line(start->getX(), start->getY(), end->getX(), end->getY());
			lines.push_back(line);
		}
	}

	for (unsigned int i = 0; i < outers.size(); i++)
	{
		vector<Vector2<float> >* poly = outers[i].getPoints();
		for (unsigned int j = 0; j < poly->size(); j++) {
			Vector2<float>* start = &poly->at(j);
			Vector2<float>* end = &poly->at((j + 1) % poly->size());

			ARK::Geometry::Line<float> line(start->getX(), start->getY(), end->getX(), end->getY());
			lines.push_back(line);
		}
	}
	

}
void AdvancedPolygon::render() {

	for (unsigned int i = 0; i < holes.size(); i++)
	{
		ARK::Geometry::Polygon<float>* sp = &holes[i];
		renderInner(sp, true);
	}

	for (unsigned int i = 0; i < outers.size(); i++)
	{
		ARK::Geometry::Polygon<float>* sp = &outers[i];
		renderInner(sp, false);
	}

}
void AdvancedPolygon::renderInner(ARK::Geometry::Polygon<float>* sp, bool hole)
{
	GameContainer* container = ARK2D::getContainer();
	Renderer* r = ARK2D::getRenderer();

	Vector2<float> pos(ARK2D::getInput()->getMouseX(), ARK2D::getInput()->getMouseY());

	for (unsigned int j = 0; j < sp->getPoints()->size(); j++)
	{
		bool convex = MathUtil::isVertexConvex(sp->getPoints(), j);
		if (hole) { convex = !convex; }

		ARK::Geometry::Vector2<float>* point = sp->getPoint(j);
		r->setDrawColor(Color::white);
		if (hole) { r->setDrawColor(Color::red); }
		r->drawCircle(point->getX(), point->getY(), 5, 12);
		if (convex) {
			r->setDrawColor(Color::cyan);
			r->setLineWidth(2);
			r->drawCircle(point->getX(), point->getY(), 9, 12);
			r->setLineWidth(1);
		}

		/*bool los = InLineOfSight(sp->getPoints(), &pos, sp->getPoint(j));
		if (los) {
			r->setDrawColor(Color::yellow);
			r->drawCircle(point->getX(), point->getY(), 13, 12);
		}*/

		ARK::Geometry::Vector2<float>* spnext = NULL;
		if (j < sp->getPoints()->size() - 1) {
			spnext = sp->getPoint(j + 1);
		}
		else {
			spnext = sp->getPoint(0);
		}


		float spx = point->getX();
		float spy = point->getY();
		float spx2 = spnext->getX();
		float spy2 = spnext->getY();

		float spModifier = 1.0f; /// sp->shootTimer / sp->shootDuration;
		float spAngle = MathUtil::anglef(spx, spy, spx2, spy2);
		float spDistance = MathUtil::distance(spx, spy, spx2, spy2);
		float spHalfDistance = spDistance*0.5;
		MathUtil::moveAngle(spx, spy, spAngle, spHalfDistance);
		MathUtil::moveAngle(spx2, spy2, spAngle - 180, spHalfDistance);

		MathUtil::moveAngle(spx, spy, spAngle, (spHalfDistance*spModifier*-1.0f) + 4);
		MathUtil::moveAngle(spx2, spy2, spAngle - 180, (spHalfDistance*spModifier*-1.0f) + 4);

		r->setDrawColor(Color::white);
		if (hole) { r->setDrawColor(Color::red); }
		r->setLineWidth(1);
		r->drawLine(spx, spy, spx2, spy2);

	}
}
