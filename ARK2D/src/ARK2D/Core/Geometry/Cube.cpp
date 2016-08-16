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
    namespace Core {
        namespace Geometry {
            
            void Cube::setLocationByAlignment(float x, float y, signed int alignX, signed int alignY) {

                if (alignX == Renderer::ALIGN_CENTER) {
                    x -= m_width / 2.0f;
                } else if (alignX == Renderer::ALIGN_RIGHT || alignX == Renderer::ALIGN_END) {
                    x -= m_width;
                }

                if (alignY == Renderer::ALIGN_CENTER) {
                    y -= m_height / 2.0f;
                } else if (alignY == Renderer::ALIGN_END || alignY == Renderer::ALIGN_BOTTOM) {
                    y -= m_height;
                }

                m_x = x;
                m_y = y;
            }
            void Cube::setLocationByAlignment(float x, float  y, float z, signed int alignX, signed int alignY, signed alignZ) {

                if (alignX == Renderer::ALIGN_CENTER) {
                    x -= m_width / 2.0f;
                } else if (alignX == Renderer::ALIGN_RIGHT || alignX == Renderer::ALIGN_END) {
                    x -= m_width;
                }

                if (alignY == Renderer::ALIGN_CENTER) {
                    y -= m_height / 2.0f;
                } else if (alignY == Renderer::ALIGN_END || alignY == Renderer::ALIGN_BOTTOM) {
                    y -= m_height;
                }

                if (alignZ == Renderer::ALIGN_CENTER) {
                    z -= m_depth / 2.0f;
                } else if (alignZ == Renderer::ALIGN_END || alignZ == Renderer::ALIGN_BOTTOM) {
                    z -= m_depth;
                }

                m_x = x;
                m_y = y;
                m_z = z;
            }

            bool Cube::collides(Shape<float>* s){
                if (s == NULL) {
                    //ARK2D::getLog()->e("A Shape was NULL");
                    return false;
                }

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
}

