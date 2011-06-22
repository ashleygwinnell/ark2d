/*
 * PNGImage.cpp
 *
 *  Created on: 15-Dec-2009
 *      Author: Ashley
 */

#include <string>
//#include <png.h>
#include "lpng151/png.h"
#include <iostream>
#include <cstdio>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "ARK2D_GL.h"
#include "ErrorDialog.h"

#include "PNGImage.h"

#ifdef ARK2D_WINDOWS
	#include <windows.h>
#endif

using namespace std;

PNGImage::PNGImage(const string filename): m_filename(filename) {

}
int PNGImage::getWidth() {
	return this->width;
}
int PNGImage::getHeight() {
	return this->height;
}
void* PNGImage::getImageData() {
	//return (void*) this->image_data;
	//return (void*) row_pointers;
	return (void*) m_data;
}

// 0: SUCCESS! :D
// 1: fileopen err
// 2: not a png
// 3: cannot create read struct.
// 4: cannot create info struct.
// 5: misc png error.
// 6: could not create image_data;
// 7: could not create row_pointers;
int PNGImage::load() {


	char header[8];    // 8 is the maximum size that can be checked

	        /* open file and test for it being a png */
	        FILE* fp = fopen(m_filename.c_str(), "rb");
	        if (!fp) {
	        	ErrorDialog::createAndShow("[read_png_file] File %s could not be opened for reading");
	        	return 1;
	        }

	        fread(header, 1, 8, fp);

	        if (png_sig_cmp((png_byte*) header, 0, 8)) {
	        	ErrorDialog::createAndShow("[read_png_file] File %s is not recognized as a PNG file");
	        	return 1;
	        }


	        /* initialize stuff */
	        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	        if (!png_ptr) {
	        	ErrorDialog::createAndShow("[read_png_file] png_create_read_struct failed");
	        	return 1;
	        }


	        info_ptr = png_create_info_struct(png_ptr);
	        if (!info_ptr) {
	        	ErrorDialog::createAndShow("[read_png_file] png_create_info_struct failed");
	        	return 1;
	        }

	        end_info = png_create_info_struct(png_ptr);
	        if (!end_info) {
				ErrorDialog::createAndShow("[read_png_file] png_create_info_struct failed 2");
				return 1;
			}

	        if (setjmp(png_jmpbuf(png_ptr))) {
	        	ErrorDialog::createAndShow("[read_png_file] Error during init_io");
	        	return 1;
	        }

	        png_init_io(png_ptr, fp);
	        png_set_sig_bytes(png_ptr, 8);

	        png_read_info(png_ptr, info_ptr);

	        width = png_get_image_width(png_ptr, info_ptr);
	        height = png_get_image_height(png_ptr, info_ptr);
	        color_type = png_get_color_type(png_ptr, info_ptr);
	        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	       // int number_of_passes = png_set_interlace_handling(png_ptr);
	        png_read_update_info(png_ptr, info_ptr);


	        /* read file */
	        if (setjmp(png_jmpbuf(png_ptr))) {
	        	ErrorDialog::createAndShow("[read_png_file] Error during read_image");
	        	return 1;
	        }

	        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	        for (unsigned int y=0; y<height; y++) {
	        	row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	        }

	        png_read_image(png_ptr, row_pointers);

	        fclose(fp);




	        if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
	        	ErrorDialog::createAndShow("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA (lacks the alpha channel)");
	        	return 1;
	        }

			if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA) {
				ErrorDialog::createAndShow("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)");
				return 1;
			}

			//png_byte* row = row_pointers[0];
			//png_byte* ptr = &row[0];
			// printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
			//                               0, 0, ptr[0], ptr[1], ptr[2], ptr[3]);

			int curOffset = 0;
			m_data = (unsigned char*) malloc(width * height * 4);
			for(unsigned int i = 0; i < height; i++) {
				png_byte* row = row_pointers[i];
				for (unsigned int j = 0; j < width; j++) {
					png_byte* ptr = &row[j*4];
					//if (i < 1) {
					//	printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
					//			i, j, ptr[0], ptr[1], ptr[2], ptr[3]);
					//}
					memcpy((m_data + curOffset), &ptr[0], 1);
					memcpy((m_data + curOffset + 1), &ptr[1], 1);
					memcpy((m_data + curOffset + 2), &ptr[2], 1);
					memcpy((m_data + curOffset + 3), &ptr[3], 1);
					curOffset += 4;
				}
			}

	        return 0;
/*
	//open file as binary
	fp = fopen(m_filename.c_str(), "rb");
	if (!fp) {
		return 1;
	}

	//read the header
	fread(header, 1, 8, fp);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		fclose(fp);
		return 2;
	}

	//create png struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return 3;
	}

	//create png info struct
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return 4;
	}

	//create png info struct
	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fclose(fp);
		return 4;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 5;
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	//int bit_depth, color_type; - replaced with member variables.
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

	//update width and height based on png info
	width = twidth;
	height = theight;

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	this->image_data = new png_byte[rowbytes * height];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 6;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		fclose(fp);
		return 7;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (unsigned int i = 0; i < height; ++i) {
		row_pointers[height - 1 - i] = image_data + i * rowbytes;
	}

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	// 0 is sccess! :D
	return 0;*/
}

PNGImage::~PNGImage() {
	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	//delete[] image_data;
	delete[] row_pointers;
	delete m_data;
	//fclose(fp);
}
