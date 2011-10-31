/*
 * PNGImage.cpp
 *
 *  Created on: 15-Dec-2009
 *      Author: Ashley
 */

#include <string>
//#include <png.h>
#include "vendor/lpng151/png.h"
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
	//#include "vendor/fmemopen.h"
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

void PNGImage::readFunction(png_structp pngPtr, png_bytep data, png_size_t length) {
    //Here we get our IO pointer back from the read struct.
    //This is the parameter we passed to the png_set_read_fn() function.
    //Our std::istream pointer.
    png_voidp a = png_get_io_ptr(pngPtr);
    PNGImagestream* piss = (PNGImagestream*) a;
    char* ps = (char*) piss->m_data;
    memcpy(data, ps + piss->m_pos, length);
    piss->m_pos += length;
    //Cast the pointer to std::istream* and read 'length' bytes into 'data'
    //((std::istream*)a)->read((char*)data, length);

}

// 0: SUCCESS! :D
// 1: fileopen err
// 2: not a png
// 3: cannot create read struct.
// 4: cannot create info struct.
// 5: misc png error.
// 6: could not create image_data;
// 7: could not create row_pointers;
int PNGImage::load(void* data) {
	char header[8];

	char* md = (char*) data;
	memcpy(header, md, sizeof(char) * 8);

	if (png_sig_cmp((png_byte*) header, 0, 8)) {
		string s = string("void* data is not recognised as a PNG file.");
		ErrorDialog::createAndShow(s);
		return 1;
	}

	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr) {
		string s = string("void* data: png_create_read_struct failed");
		ErrorDialog::createAndShow(s);
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
		ErrorDialog::createAndShow("[read_png_file] Error during init_io or reading png.");
		return 1;
	}


	//FILE* fp = fmemopen(data, sizeof(md), "r");

	//png_init_io(png_ptr, fp);
	//png_set_sig_bytes(png_ptr, 8);

	PNGImagestream piss;
	piss.m_data = data;
	piss.m_pos = 8;
	png_set_read_fn(png_ptr, &piss, &readFunction);

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

	//fclose(fp);




	/*
		if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
			ErrorDialog::createAndShow("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA (lacks the alpha channel)");
			return 1;
		}

		if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA) {
			ErrorDialog::createAndShow("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)");
			return 1;
		}
	*/

	//png_byte* row = row_pointers[0];
	//png_byte* ptr = &row[0];
	// printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
	//                               0, 0, ptr[0], ptr[1], ptr[2], ptr[3]);
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
		int curOffset = 0;
		m_data = (unsigned char*) malloc(width * height * 4);
		for(unsigned int i = 0; i < height; i++) {
			png_byte* row = row_pointers[i];
			for (unsigned int j = 0; j < width; j++) {
				png_byte* ptr = &row[j*3];
				//if (i < 1) {
				//	printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
				//			i, j, ptr[0], ptr[1], ptr[2], ptr[3]);
				//}
				memcpy((m_data + curOffset), &ptr[0], 1);
				memcpy((m_data + curOffset + 1), &ptr[1], 1);
				memcpy((m_data + curOffset + 2), &ptr[2], 1);
				//memcpy((m_data + curOffset + 3), &ptr[3], 1);
				curOffset += 3;
			}
		}
	} else if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
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
	} else {
		ErrorDialog::createAndShow("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)");
		return 1;
	}

	return 0;

}
int PNGImage::load() {


	char header[8];    // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE* fp = fopen(m_filename.c_str(), "rb");
	if (!fp) {
		string s = string(m_filename.c_str()) + " could not be opened for reading.";
		ErrorDialog::createAndShow(s);
		return 1;
	}

	fread(header, 1, 8, fp);

	if (png_sig_cmp((png_byte*) header, 0, 8)) {
		string s = string(m_filename.c_str()) + " is not recognised as a PNG file.";
		ErrorDialog::createAndShow(s);
		return 1;
	}


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr) {
		string s = string(m_filename.c_str()) + ": png_create_read_struct failed";
		ErrorDialog::createAndShow(s);
		return 1;
	}


	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		string s = string(m_filename.c_str()) + " png_create_info_struct failed.";
		ErrorDialog::createAndShow(s);
		return 1;
	}

	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		string s = string(m_filename.c_str()) + " png_create_info_struct failed (2).";
		ErrorDialog::createAndShow(s);
		return 1;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		string s = string(m_filename.c_str()) + " error during init_io.";
		ErrorDialog::createAndShow(s);
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
		string s = string(m_filename.c_str()) + " error during read_image.";
		ErrorDialog::createAndShow(s);
		return 1;
	}

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (unsigned int y=0; y<height; y++) {
		row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	}

	png_read_image(png_ptr, row_pointers);

	fclose(fp);




	/*
		if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
			string s = string(m_filename.c_str()) + " [process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA (lacks the alpha channel)";
			ErrorDialog::createAndShow(s);
			return 1;
		}

		if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA) {
			string s = string(m_filename.c_str()) + " [process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)";
			ErrorDialog::createAndShow(s);
			return 1;
		}
	*/

	//png_byte* row = row_pointers[0];
	//png_byte* ptr = &row[0];
	// printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
	//                               0, 0, ptr[0], ptr[1], ptr[2], ptr[3]);

	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
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
	} else if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
		int curOffset = 0;
		m_data = (unsigned char*) malloc(width * height * 4);
		for(unsigned int i = 0; i < height; i++) {
			png_byte* row = row_pointers[i];
			for (unsigned int j = 0; j < width; j++) {
				png_byte* ptr = &row[j*3];
				//if (i < 1) {
				//	printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
				//			i, j, ptr[0], ptr[1], ptr[2], ptr[3]);
				//}
				unsigned char a = 255;
				memcpy((m_data + curOffset), &ptr[0], 1);
				memcpy((m_data + curOffset + 1), &ptr[1], 1);
				memcpy((m_data + curOffset + 2), &ptr[2], 1);
				memcpy((m_data + curOffset + 3), &a, 1);
				//m_data[curOffset + 3] = 255;
				curOffset += 4;
			}
		}
	} else {
		string s = string(m_filename.c_str()) + " [process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA or RGB (%d) (is %d)";
		ErrorDialog::createAndShow(s);
		return 1;
	}

	return 0;

}

PNGImage::~PNGImage() {
	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	//delete[] image_data;
	delete[] row_pointers;
	delete m_data;
	//fclose(fp);
}
