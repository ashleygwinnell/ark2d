/*
 * Rectangle.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARKRECTANGLE_H_
#define ARKRECTANGLE_H_

#include "Shape.h"
#include "Line.h"
#include "Vector2.h"
//#include "Polygon.h"
#include "../Graphics/Renderer.h"


  
namespace ARK { 
	namespace Geometry {

		/*!
		 * \brief Rectangle bounds class. Use this for rectangular collision/resolution.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
        
		template <class T>
		class RectangleTemplate: public Shape<T> {
			protected:
				T m_x;
				T m_y;
				int m_width;
				int m_height;

			public:
				RectangleTemplate():Shape<T>(), m_x(0), m_y(0), m_width(1), m_height(1) {

				}
				RectangleTemplate(T x, T y, int width, int height):Shape<T>(), m_x(x), m_y(y), m_width(width), m_height(height) {

				}
				virtual void set(T x, T y, T width, T height) {
					m_x = x;
					m_y = y; 
					m_width = (int) width;
					m_height = (int) height;
				}

				virtual T getMinX() {
					return m_x;
				}
				virtual T getMaxX() {
					return m_x + m_width;
				}
				virtual T getCenterX() {
					return m_x + (m_width/2);
				}

				virtual T getMinY() {
					return m_y;
				}
				virtual T getMaxY() {
					return m_y + m_height;
				}
				virtual T getCenterY() {
					return m_y + (m_height/2);
				}

				virtual T getWidth() {
					return m_width;
				}
				virtual T getHeight() {
					return m_height;
				}

				virtual void setX(T x) {
					m_x = x;
				}
				virtual void setY(T y) {
					m_y = y;
				}
				virtual void setLocation(T x, T y) {
					m_x = x;
					m_y = y;
				}
				virtual void setLocationByCenter(T x, T y) {
					m_x = x - (m_width/2);
					m_y = y - (m_height/2);
				}
				virtual void setLocationByAlignment(T x, T y, signed int alignX, signed int alignY) {
					
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
				virtual void setXByCenter(T x) {
					m_x = x - (m_width/2);
				}
				virtual void setYByCenter(T y) {
					m_y = y - (m_height/2);
				}

				virtual void adjustX(T x) {
					m_x += x; 
				}
				virtual void adjustY(T y) {
					m_y += y;
				}

				virtual void setWidth(T w) {
					m_width = (int) w;
				}
				virtual void setHeight(T h) {
					m_height = (int) h;
				}
				virtual void setSize(T w, T h) {
					m_width = (int) w;
					m_height = (int) h;
				}

				virtual bool contains(T x, T y) {
					if (x > m_x && x < getMaxX() && y > m_y && y < getMaxY()) {
						return true;
					}
					return false;
				}
				virtual bool collides(Shape<T>* s) {
					return false;
				}
				virtual void resolve(Shape<T>* s) {

				}
				virtual void render() {
					ARK2D::getRenderer()->drawRect(this);
				}

				// this polygon should have four points on it! 
				// OR ELSE FAILLLL!!
				Polygon<T>* asPolygon(Polygon<T>* polygon, float rotation) 
				{
					polygon->getPoint(0)->set(getMinX(), getMinY());
					polygon->getPoint(1)->set(getMinX(), getMaxY());
					polygon->getPoint(2)->set(getMaxX(), getMaxY());
					polygon->getPoint(3)->set(getMaxX(), getMinY()); 
					polygon->rotate(rotation);
					return polygon;
				}

				string toString() {
					ARK::Core::String s;
					s += "{";
					s += "\"x\":";
					s += m_x;
					s += ", \"y\":";
					s += m_y;
					s += ", \"w\":";
					s += m_width;
					s += ", \"h\":";
					s += m_height;
					s += "}";
					return s.get();
				}

				virtual ~RectangleTemplate() {

				}
		};

		class Rectangle : public RectangleTemplate<float> {
			public: 
				Rectangle():
					RectangleTemplate() {

				}
				Rectangle(float x, float y, float w, float h):
					RectangleTemplate(x, y, w, h) {

				}
				virtual ARK2D_API bool collides(Shape<float>* s);
				virtual ~Rectangle() {

				}
		};
	}
}
#endif /* ARKRECTANGLE_H_ */
