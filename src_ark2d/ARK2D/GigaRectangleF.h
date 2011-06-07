/*
 * Rectangle.h
 *
 *  Created on: 10-May-2010
 *      Author: Ashley
 */

#ifndef GIGARECTANGLEF_H_
#define GIGARECTANGLEF_H_

class GigaRectangleF {
	private:
		float m_x;
		float m_y;
		int m_width;
		int m_height;
	public:
		GigaRectangleF();
		GigaRectangleF(float x, float y, int width, int height);
		float getX() const;
		float getY() const;
		int getWidth() const;
		int getHeight() const;
		void setX(float x);
		void setY(float y);
		void setWidth(int w);
		void setHeight(int h);
		void setLocationByCenter(float x, float y);

		static bool s_collides(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

		~GigaRectangleF();
};

#endif /* GIGARECTANGLEF_H_ */
