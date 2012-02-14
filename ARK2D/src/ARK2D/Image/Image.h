/*
 * Image.h
 *
 *  Created on: 06-Nov-2009
 *      Author: user
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "../ARK2D_GL.h"
#include "../ARK2D_windres.h"


//#include <string>
#include "../ARK2D.h"
#include "TargaImage.h"
#include "BMPImage.h"
#include "PNGImage.h"
#include "../Color.h"
#include "../SpriteSheetDescriptionItem.h"

class Image {
	private:

		static unsigned int s_current_texture_id;

		void* m_data; // only used by one constructor.
		unsigned int m_resourceType; // ARK2D_RESOURCE_TYPE_PNG only supported.

		std::string filename;
		unsigned int m_Width;
		unsigned int m_Height;

		GLuint texture;
		float texture_width;
		float texture_height;
		float texture_offset_x;
		float texture_offset_y;

		float m_CenterX; // used for rotation
		float m_CenterY;

		Color m_tl_corner_color;
		Color m_tr_corner_color;

		float m_alpha;
		Color* m_color;

		double m_Rotation;

		GLuint load();
		GLuint load(const Color& mask);
		TargaImage* loadTGA();
		BMPImage* loadBMP();
		PNGImage* loadPNG();


	public:
		Image();
		Image(unsigned int resource, unsigned int resourceType);
		Image(void* data, unsigned int resourceType);
		Image(const std::string& fname);
		Image(const std::string& fname, const Color& mask);
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		double getRotation();

		void setAlpha(float f);
		float getAlpha() const;

		void setColor(Color* c); // color overrides alpha;
		Color* getColor() const;

		void setCenterOfRotation(int x, int y);
		void setRotation(double angle);
		void rotate(double angle);
		Image* getSubImage(const SpriteSheetDescriptionItem& desc);
		Image* getSubImage(int x, int y, int width, int height) const;
		Image* getScaledCopy(unsigned int x, unsigned int y);
		Image* getFlippedCopy(bool horizontal_flip, bool vertical_flip);
		void setSize(int x, int y);
		void setWidth(int w);
		void setHeight(int h);
		void bind() const;
		void unbind() const;
		void draw() const;
		void draw(int x, int y) const;
		void draw(float x, float y) const;
		void draw(float x, float y, unsigned int w, unsigned int h);
		void drawCentered(int x, int y) const;
		void drawCentered(float x, float y) const;
		void drawCenteredFlipped(int x, int y, bool flipx, bool flipy);
		void drawFlipped(int x, int y, bool flipx, bool flipy); // not const as modifies inner variables
		//void setCornerColor(unsigned int corner, Color c) const;
		//void copyAreaToImage(const Image& image, unsigned int dest_x, unsigned int dest_y,
		//						unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		virtual ~Image();
};

#endif /* IMAGE_H_ */
