/*
 * Image.cpp
 *
 *  Created on: 06-Nov-2009
 *      Author: user
 */



#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "Image.h"
#include "TargaImage.h"
#include "BMPImage.h"
#include "PNGImage.h"
#include "StringUtil.h"
#include "Color.h"
#include "SpriteSheetDescriptionItem.h"

#include "ARK2D_GL.h"
#include "GameContainer.h"

// Currently bound Image texture;
unsigned int Image::s_current_texture_id = 0;

GLuint Image::load(const Color& mask) {

	unsigned int thisDataType = 0;
	if (m_data != NULL) {
		thisDataType = m_resourceType;
	} else {
		string ext = StringUtil::getExtension(this->filename);
		if (ext == "tga") {
			thisDataType = ARK2D_RESOURCE_TYPE_TGA;
		} else if (ext == "bmp") {
			thisDataType = ARK2D_RESOURCE_TYPE_BMP;
		} else if (ext == "png") {
			thisDataType = ARK2D_RESOURCE_TYPE_PNG;
		}
	}


	if (thisDataType == ARK2D_RESOURCE_TYPE_TGA) {
		//std::cout << "Loading TGA!" << std::endl;
		TargaImage* tga = loadTGA();
		this->setWidth(tga->getWidth());
		this->setHeight(tga->getHeight());

		// Generate one texture (we're creating only one).
		unsigned Object(0);
		glGenTextures(1, &Object);

		// bind?
		glBindTexture(GL_TEXTURE_2D, Object);
		// You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//for (int i = 0; i < tga->getImageData().size(); i++) {
		//	data = tga->getImageData().at(i);
		//}

		// Create the actual texture object.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->getWidth(), this->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, tga->getImageData());

		// don't forget to unbind the texture now.
		glBindTexture(GL_TEXTURE_2D, 0);

		// Free TGA memory.
		delete(tga);

		// and finally return
		return Object;
	} else if (thisDataType == ARK2D_RESOURCE_TYPE_BMP) {
		//std::cout << "Loading BMP!" << std::endl;
		BMPImage* bmp = loadBMP();
		m_Width = bmp->Width;
		m_Height = bmp->Height;

		// Generate one texture (we're creating only one).
		unsigned Object(0);
		glGenTextures(1, &Object);

		//for (unsigned int i = 0; i < sizeof(&bmp->Raster); i++) {
		//	byte pixel = byte(&bmp->Raster[i]);
		//	std::cout << pixel << std::endl;
		//}


		// generate a new Raster from the BMP information. this one has an alpha channel!
		unsigned int OldRasterSize = (bmp->BytesPerRow * bmp->Height);
		unsigned int NewRasterSize = ((bmp->BytesPerRow * bmp->Height) / 3) * 4;

		//std::cout << OldRasterSize << " : " << NewRasterSize << std::endl;
		//std::cout << bmp->BPP << std::endl;

		char NewRaster[NewRasterSize];	// 32 bits raster, yes!

		int NewRasterIndex = 0;
		for (unsigned int i = 0; i < OldRasterSize; i += 3) {
			unsigned int b = ((unsigned int) (unsigned char) bmp->Raster[i]); // blue because BMPs stores pixels in BGR not RGB
			unsigned int g = ((unsigned int) (unsigned char) bmp->Raster[i + 1]);
			unsigned int r = ((unsigned int) (unsigned char) bmp->Raster[i + 2]);

			NewRaster[NewRasterIndex] = (unsigned char) r;
			NewRaster[NewRasterIndex + 1] = (unsigned char) g;
			NewRaster[NewRasterIndex + 2] = (unsigned char) b;
			NewRaster[NewRasterIndex + 3] = (unsigned char) 255;
			// apply mask
			if (r == mask.getRed() && g == mask.getGreen() && b == mask.getBlue()) {
				NewRaster[NewRasterIndex + 3] = (unsigned char) 0;
			}
			NewRasterIndex += 4;
		}

		//std::cout << "Width: " << m_Width << ". Height: " << m_Width << std::endl;

		// bind?
		glBindTexture(GL_TEXTURE_2D, Object);

		// You can use these values to specify mipmaps if you want to, such as 'GL_LINEAR_MIPMAP_LINEAR'.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Create the actual texture object.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NewRaster);

		//for (unsigned int x = 0; x < bmp->Width; x++) {
		//	for (unsigned int y = 0; y < bmp->Height; y++) {
				/*int x = 3; int y = 3;
				unsigned char pixel[3];
				glReadPixels(x, y, 1, 1, GL_BGR, GL_UNSIGNED_BYTE, pixel);
				float fr, fg, fb;
				fr = pixel[0]/255.0;
				fg = pixel[1]/255.0;
				fb = pixel[2]/255.0;
				float r, g, b;
				r = pixel[0];
				g = pixel[1];
				b = pixel[2];
				std::cout << fr << " : " << fg << " : " << fb << std::endl;
				std::cout << r << " : " << g << " : " << b << std::endl; */
		//	}
		//}

		// don't forget to unbind the texture now.
		glBindTexture(GL_TEXTURE_2D, 0);


		// Free BMP memory.
		delete(bmp);

		// and return!
		return Object;
	} else if (thisDataType == ARK2D_RESOURCE_TYPE_PNG) {
		//std::cout << "Loading PNG!" << std::endl;
		PNGImage* png = loadPNG();

		unsigned char* pixelData = (unsigned char*) png->getImageData();
		m_Width = png->getWidth();
		m_Height = png->getHeight();

		std::cout << "w: " << m_Width << " h: " << m_Height << std::endl;

		// apply mask
		unsigned int offset = 0;
		for (unsigned int i = 0; i < m_Height; i++) {
			for (unsigned int j = 0; j < m_Width; j++) {
				unsigned int r = ((unsigned int) (unsigned char) pixelData[offset]);
				unsigned int g = ((unsigned int) (unsigned char) pixelData[offset+1]);
				unsigned int b = ((unsigned int) (unsigned char) pixelData[offset+2]);
				unsigned int a = ((unsigned int) (unsigned char) pixelData[offset+3]);

				/*unsigned int r = (unsigned int) pixelData[offset];
				unsigned int g = (unsigned int) pixelData[offset+1];
				unsigned int b = (unsigned int) pixelData[offset+2];
				unsigned int a = (unsigned int) pixelData[offset+3];
				*/

				if (i == 0 && j == 0) {
					std::cout << "tl pixel data- " << "r: " << r << " g: " << g << " b: " << b << " a: " << a << std::endl;
					std::cout << "mask data- " << "r: " << mask.getRed() << " g: " << mask.getGreen() << " b: " << mask.getBlue() << " a: " << mask.getAlpha() << std::endl;
				}

				if (r == mask.getRed() && g == mask.getGreen() && b == mask.getBlue() && a == mask.getAlpha()) {
					std::cout << "replacing " << j << ":" << i << std::endl;
					pixelData[offset] = (unsigned char) 0;
					pixelData[offset+1] = (unsigned char) 0;
					pixelData[offset+2] = (unsigned char) 0;
					pixelData[offset+3] = (unsigned char) 0;
				}

				/*if (i == 0 && j == 0) {
					std::cout << "after" << std::endl;
					std::cout << "tl pixel data- " << "r: " << r << " g: " << g << " b: " << b << " a: " << a << std::endl;
					std::cout << "mask data- " << "r: " << mask.getRed() << " g: " << mask.getGreen() << " b: " << mask.getBlue() << " a: " << mask.getAlpha() << std::endl;
				}*/

				offset += 4;
			}
		}

		//Now generate the OpenGL texture object
		unsigned Object(0);
		glGenTextures(1, &Object);
		glBindTexture(GL_TEXTURE_2D, Object);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		glBindTexture(GL_TEXTURE_2D, 0);
		texture = Object;

		delete png;

		return Object;
	}
	return 0;
}

GLuint Image::load() {
	return load(Color::magenta);
}

BMPImage* Image::loadBMP() {
	BMPImage* bmp = new BMPImage();
	int suc = bmp->LoadBMP(this->filename.c_str());
	std::string errmsg = "";
	if (suc == 1) {
		errmsg = "Error opening bitmap file: " + this->filename + ".";
	} else if (suc == 2) {
		errmsg = this->filename + " is not a valid bitmap image.";
	}

	if (errmsg != "") {
		ErrorDialog::createAndShow(errmsg);
		return 0;
	}
	return bmp;
}

TargaImage* Image::loadTGA() {
	TargaImage* targa = new TargaImage();
	int suc = targa->load(this->filename);
	std::string errmsg = "";
	if (suc == -1) {
		errmsg = "unknown error with tga image: ";
	} else if (suc == 1) {
		errmsg = "could not open image for reading: ";
	} else if (suc == 2) {
		errmsg = "unsupported image tga type: ";
	} else if (suc == 3) {
		errmsg = "unsupported color depth, seem_bytesPerPixel: ";
	}

	if (errmsg != "") {
		ErrorDialog::createAndShow(errmsg);
		return 0;
	}
	return targa;
}

PNGImage* Image::loadPNG() {
	PNGImage* png = new PNGImage(this->filename);
	int suc = 0;
	if (m_data == NULL) {
		suc = png->load();
	} else {
		suc = png->load(m_data);
	}
	if (suc != 0) {
		//std::cout << suc << std::endl;
		string errmsg = StringUtil::append("Error loading png image: ", suc);
		ErrorDialog::createAndShow(errmsg);
		exit(0);
	}
	return png;
}

Image::Image():
	m_data(NULL),
	m_resourceType(0),
	filename(""),
	texture_width(1),
	texture_height(1),
	texture_offset_x(0),
	texture_offset_y(0),
	m_CenterX(0),
	m_CenterY(0),
	m_tl_corner_color(),
	m_tr_corner_color(),
	m_alpha(1.0f),
	m_color(NULL),
	m_Rotation(0)
{

}

Image::Image(unsigned int resource, unsigned int resourceType):
	m_data(NULL),
	m_resourceType(resourceType),
	filename(""),
	texture_width(1),
	texture_height(1),
	texture_offset_x(0),
	texture_offset_y(0),
	m_CenterX(0),
	m_CenterY(0),
	m_tl_corner_color(),
	m_tr_corner_color(),
	m_alpha(1.0f),
	m_color(NULL),
	m_Rotation(0)
{
	std::cout << "Loading Image from resource. " << std::endl;
	m_data = GameContainerPlatform::getARK2DResource(resource, resourceType);
	this->texture = this->load();
}

Image::Image(void* data, unsigned int resourceType):
	m_data(data),
	m_resourceType(resourceType),
	filename(""),
	texture_width(1),
	texture_height(1),
	texture_offset_x(0),
	texture_offset_y(0),
	m_CenterX(0),
	m_CenterY(0),
	m_tl_corner_color(),
	m_tr_corner_color(),
	m_alpha(1.0f),
	m_color(NULL),
	m_Rotation(0)
{
	std::cout << "Loading Image from data. " << std::endl;
	this->texture = this->load();
}

Image::Image(const std::string& fname) :
	m_data(NULL),
	m_resourceType(0),
	filename(fname),
	texture_width(1),
	texture_height(1),
	texture_offset_x(0),
	texture_offset_y(0),
	m_CenterX(0),
	m_CenterY(0),
	m_tl_corner_color(),
	m_tr_corner_color(),
	m_alpha(1.0f),
	m_color(NULL),
	m_Rotation(0)
{
	this->texture = this->load(); // this sets the width and height too! :)
	std::cout << "Loaded Image: " << this->filename << std::endl; //  BMP must be 24bit unrestricted bmp!
}

Image::Image(const std::string& fname, const Color& mask) :
	m_data(NULL),
	m_resourceType(0),
	filename(fname),
	texture_width(1),
	texture_height(1),
	texture_offset_x(0),
	texture_offset_y(0),
	m_CenterX(0),
	m_CenterY(0),
	m_tl_corner_color(),
	m_tr_corner_color(),
	m_alpha(1.0f),
	m_color(NULL),
	m_Rotation(0)
{
	this->texture = this->load(mask); // this sets the width and height too! :)
	std::cout << "Loaded Image: " << this->filename << std::endl; //  BMP must be 24bit unrestricted bmp!
}

unsigned int Image::getWidth() const {
	return m_Width;
}
unsigned int Image::getHeight() const {
	return m_Height;
}
double Image::getRotation() {
	return m_Rotation;
}
void Image::setRotation(double angle) {
	m_Rotation = angle;
}
void Image::rotate(double angle) {
	m_Rotation += angle;
}
void Image::setCenterOfRotation(int x, int y) {
	m_CenterX = x;
	m_CenterY = y;
}
void Image::setWidth(int w) {
	m_Width = w;
}
void Image::setHeight(int h) {
	m_Height = h;
}
Image* Image::getSubImage(const SpriteSheetDescriptionItem& desc) {
	return getSubImage(desc.getX(), desc.getY(), desc.getWidth(), desc.getHeight());
}
Image* Image::getSubImage(int x, int y, int width, int height) const {
	float newTextureOffsetX = ((x / (float) m_Width) * this->texture_width) + this->texture_offset_x;
	float newTextureOffsetY = ((y / (float) m_Height) * this->texture_height) + this->texture_offset_y;
	float newTextureWidth = ((width / (float) m_Width) * this->texture_width);
	float newTextureHeight = ((height / (float) m_Height) * this->texture_height);


	Image* sub = new Image();
	sub->texture = texture;
	sub->texture_width = newTextureWidth;
	sub->texture_height = newTextureHeight;
	sub->texture_offset_x = newTextureOffsetX;
	sub->texture_offset_y = newTextureOffsetY;
	sub->setWidth(width);
	sub->setHeight(height);
	return sub;

	/*Image sub;
	sub.texture = texture;
	sub.texture_width = newTextureWidth;
	sub.texture_height = newTextureHeight;
	sub.texture_offset_x = newTextureOffsetX;
	sub.texture_offset_y = newTextureOffsetY;
	sub.setWidth(width);
	sub.setHeight(height);
	sub.m_tl_corner_color = Color(255,255,255,255);
	sub.m_tr_corner_color = Color(255,255,255,255);
	return sub;*/
}

Image* Image::getScaledCopy(unsigned int x, unsigned int y) {

	Image* sub = new Image();
	sub->texture = texture;
	sub->texture_width = texture_width;
	sub->texture_height = texture_height;
	sub->texture_offset_x = texture_offset_x;
	sub->texture_offset_y = texture_offset_y;
	sub->setWidth(m_Width * x);
	sub->setHeight(m_Height * y);
	return sub;
}
Image* Image::getFlippedCopy(bool horizontal_flip, bool vertical_flip) {
	Image* sub = new Image();
	sub->texture = texture;
	sub->texture_width = texture_width;
	sub->texture_height = texture_height;
	sub->texture_offset_x = texture_offset_x;
	sub->texture_offset_y = texture_offset_y;
	sub->setWidth(m_Width);
	sub->setHeight(m_Height);

	if (horizontal_flip) {
		sub->texture_offset_x = texture_offset_x + texture_width;
		sub->texture_width = -texture_width;
	}
	if (vertical_flip) {
		sub->texture_offset_y = texture_offset_y + texture_height;
		sub->texture_height = -texture_height;
	}
	return sub;
}

void Image::setAlpha(float f) {
	m_alpha = f;
}
float Image::getAlpha() const {
	return m_alpha;
}

void Image::setColor(Color* c) {
	m_color = c;
}
Color* Image::getColor() const {
	return m_color;
}

void Image::drawFlipped(int x, int y, bool flipx, bool flipy) {

	float previous_texture_offset_x = texture_offset_x;
	float previous_texture_width = texture_width;
	if (flipx) {
		texture_offset_x = texture_offset_x + texture_width;
		texture_width = texture_width * -1;
	}

	float previous_texture_offset_y = texture_offset_y;
	float previous_texture_height = texture_height;
	if (flipy) {
		texture_offset_y = texture_offset_y + texture_height;
		texture_height = texture_height * -1;
	}

	draw(x, y);

	if (flipx) {
		texture_offset_x = previous_texture_offset_x;
		texture_width = previous_texture_width;
	}

	if (flipy) {
		texture_offset_y = previous_texture_offset_y;
		texture_height = previous_texture_height;
	}

}

void Image::draw() const {
	this->draw(0, 0);
}
void Image::draw(int x, int y) const {
	this->draw((float)x, (float) y);
}
void Image::draw(float x, float y, unsigned int w, unsigned int h) {
	unsigned int w2 = m_Width;
	unsigned int h2 = m_Height;

	m_Width = w;
	m_Height = h;

	draw(x, y);

	m_Width = w2;
	m_Height = h2;
}
void Image::draw(float x, float y) const {
	if (this->texture == 0) { return; }
	glColor4f(1.0f, 1.0f, 1.0f, m_alpha);

	if (m_color != NULL) {
		glColor4f(m_color->getRed()/255.0f, m_color->getGreen()/255.0f, m_color->getBlue()/255.0f, m_color->getAlpha()/255.0f);
	}

	this->bind();

	// rotation
	if (m_Rotation != 0) {
		glTranslatef(x + m_CenterX, y + m_CenterY, 0);
		glRotatef(m_Rotation, 0, 0, 1);
		glTranslatef((x + m_CenterX) * -1, (y + m_CenterY) * -1, 0);
	}

	glPushMatrix();

	glTranslatef(x, y, 0);
	glBegin( GL_QUADS );
		//Top-left vertex (corner)
		//glTexCoord2f( 0, 0 );
		//glColor4f(m_tl_corner_color.getRed()/255.f, m_tl_corner_color.getGreen()/255.f,
		//		m_tl_corner_color.getBlue()/255.f, m_tl_corner_color.getAlpha()/255.f);
		glTexCoord2f( this->texture_offset_x, this->texture_offset_y );
		glVertex2f(0, 0);

		//Bottom-left vertex (corner)
		//glTexCoord2f( 0, 1 );
		glTexCoord2f( this->texture_offset_x, this->texture_offset_y + this->texture_height );
		glVertex2f(0, 0 + this->m_Height);

		//Bottom-right vertex (corner)
		//glTexCoord2f( 1, 1 );
		glTexCoord2f( this->texture_offset_x + this->texture_width, this->texture_offset_y + this->texture_height );
		glVertex2f(0 + this->m_Width, 0 + this->m_Height);

		//Top-right vertex (corner)
		//glTexCoord2f( 1, 0 );
	//	glColor4f(m_tr_corner_color.getRed()/255.f, m_tr_corner_color.getGreen()/255.f,
	//						m_tr_corner_color.getBlue()/255.f, m_tr_corner_color.getAlpha()/255.f);
		glTexCoord2f( this->texture_offset_x + this->texture_width, this->texture_offset_y );
		glVertex2f(0 + this->m_Width, 0);
	glEnd();
	glTranslatef(x * -1, y * -1, 0);

	glPopMatrix();


	if (m_Rotation != 0) {
		glTranslatef(x + m_CenterX, y + m_CenterY, 0);
		glRotatef(m_Rotation * -1, 0, 0, 1);
		glTranslatef((x + m_CenterX) * -1, (y + m_CenterY) * -1, 0);
	}


	this->unbind();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

void Image::drawCentered(int x, int y) const {
	this->drawCentered((float)x, (float) y);
}

void Image::drawCentered(float x, float y) const {
	this->draw(x - (this->m_Width/2), y - (this->m_Height/2));
}
void Image::drawCenteredFlipped(int x, int y, bool flipx, bool flipy) {
	this->drawFlipped(x - (this->m_Width/2), y - (this->m_Height/2), flipx, flipy);
}

void Image::bind() const {
	if (texture == s_current_texture_id) { return; }
	glBindTexture( GL_TEXTURE_2D, this->texture );
	s_current_texture_id = texture;
}
void Image::unbind() const {
	//if (texture == s_current_texture_id) { return; }
	glBindTexture(GL_TEXTURE_2D, 0);
	s_current_texture_id = 0;
}

/* void Image::copyAreaToImage(const Image& image, unsigned int dest_x, unsigned int dest_y, unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
	bind();
	glCopyTexSubImage2D(image.texture, 0, x, y, dest_x, dest_y, width, height );
	unbind();
} */
/*
void Image::setCornerColor(unsigned int corner, Color c) const {
	switch (corner) {
		case 0:
			m_tl_corner_color = Color(c); break;
		case 1:
			m_tr_corner_color = Color(c); break;
	}
}*/


void Image::setSize(int w, int h) {
	m_Width = w;
	m_Height = h;
}

Image::~Image() {
	//glDeleteTextures( 1, &texture );
}
