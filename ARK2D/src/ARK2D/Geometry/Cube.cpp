/*
 * Cube.cpp
 *
 *  Created on: July 4, 2014
 *      Author: ashleygwinnell
 */

#include "Cube.h"  

#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"

namespace ARK {
    namespace Geometry {


        bool Cube::collides(Shape<float>* s){
            if (s == NULL) { ErrorDialog::createAndShow("A Shape was NULL"); }

            // cube
            CubeTemplate<float>* cube = NULL;
            cube = dynamic_cast<CubeTemplate<float>*>(s);
            if (cube != NULL) {
                return Shape<float>::collision_cubeCube(m_x, m_y, m_z, m_width, m_height, m_depth, cube->getMinX(), cube->getMinY(), cube->getMinZ(), cube->getWidth(), cube->getHeight(), cube->getDepth());
            }

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
            //Polygon<T>* poly = NULL;
            //poly = dynamic_cast<Polygon<T>* >(s);
            //if (poly != NULL) {
            //	return Shape<T>::collision_polygonRectangle(poly, this);
            //}

            return false;

        }
    
    }
}

