/*
 * Rectangle.cpp
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#include "Rectangle.h"  

#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Polygon.h"

namespace ARK {
    namespace Geometry {


        bool Rectangle::collides(Shape<float>* s){
			 
            if (s == NULL) { ARK2D::getLog()->e("A Shape was NULL"); return false; }

			// rectangle
			RectangleTemplate<float>* rect = NULL;
			rect = dynamic_cast<RectangleTemplate<float>*>(s);
			if (rect != NULL) {
				return Shape<float>::collision_rectangleRectangle(m_x, m_y, m_width, m_height, rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight());
			}

			// circle 
			Circle<float>* circle = NULL;
			circle = dynamic_cast<Circle<float>* >(s);
			if (circle != NULL) {
				return Shape<float>::collision_circleRectangle(circle->getCenterX(), circle->getCenterY(), circle->getRadius(), m_x, m_y, m_width, m_height);
			}

			// line
			Line<float>* line = NULL;
			line = dynamic_cast<Line<float>* >(s);
			if (line != NULL) {
				return Shape<float>::collision_rectangleLine(m_x, m_y, m_width, m_height, line->getStart()->getX(), line->getStart()->getY(), line->getEnd()->getX(), line->getEnd()->getY());
			}

			// Polygon
			Polygon<float>* poly = NULL;
			poly = dynamic_cast<Polygon<float>* >(s);
			if (poly != NULL) {
				return Shape<float>::collision_polygonRectangle(poly, this);
			}

			// Cube
			/*ARK::Geometry::Cube<T>* cube = NULL;
			cube = dynamic_cast<ARK::Geometry::Cube<T>* >(s);
			if (cube != NULL) {
				return Shape<T>::collision_cubeRectangle(cube, this);
			}*/

			return false;
		}
        
    }
}