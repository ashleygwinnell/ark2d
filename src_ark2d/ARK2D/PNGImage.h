/*
 * PNGImage.h
 *
 *  Created on: 15-Dec-2009
 *      Author: Ashley
 */

#ifndef PNGIMAGE_H_
#define PNGIMAGE_H_

#include "ARK2D_GL.h"

#include "lpng151/png.h"
//#include <png.h>
#include <string>
#include <iostream>

class PNGImage {
	public:
		PNGImage(const std::string filename);
		int load();
		int getWidth();
		int getHeight();
		void* getImageData();
		~PNGImage();
	private:
		std::string filename;
		//FILE *fp; // file handle.
		png_byte header[8]; // header for testing if it is a png
		png_structp png_ptr; // png read struct
		png_infop info_ptr; // png info struct.
		png_infop end_info; // another png info struct
		int bit_depth, color_type;
		png_uint_32 width, height; // width and height
		png_bytep* row_pointers; //row_pointers is for pointing to image_data for reading the png with libpng
		png_byte* image_data;


		unsigned char* m_data;

};

#endif /* PNGIMAGE_H_ */
