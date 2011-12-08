/*
 * BMPImage.h
 *
 *  Created on: 13-Dec-2009
 *      Author: Ashley
 */

#ifndef BMPIMAGE_H_
#define BMPIMAGE_H_

#include <iostream>

#include "../ARK2D.h"

#if defined(ARK2D_WINDOWS)
	#include <windows.h>

	typedef BITMAPFILEHEADER ARK2D_BITMAPFILEHEADER;
	typedef BITMAPINFOHEADER ARK2D_BITMAPINFOHEADER;
	typedef RGBQUAD ARK2D_RGBQUAD;
	typedef BITMAPINFO ARK2D_BITMAPINFO;
#else
	typedef uint8_t 		ARK2D_BYTE;
	typedef uint16_t  		ARK2D_WORD;
	typedef uint32_t    	ARK2D_DWORD;
	typedef unsigned long  	ARK2D_LONG;

	struct ARK2D_BITMAPFILEHEADER {
		ARK2D_WORD bfType;
		ARK2D_DWORD bfSize;
		ARK2D_WORD bfReserved1;
		ARK2D_WORD bfReserved2;
		ARK2D_DWORD bfOffBits;
	};

	struct ARK2D_BITMAPINFOHEADER {
		ARK2D_DWORD biSize;
		ARK2D_LONG  biWidth;
		ARK2D_LONG  biHeight;
		ARK2D_WORD  biPlanes;
		ARK2D_WORD  biBitCount;
		ARK2D_DWORD biCompression;
		ARK2D_DWORD biSizeImage;
		ARK2D_LONG  biXPelsPerMeter;
		ARK2D_LONG  biYPelsPerMeter;
		ARK2D_DWORD biClrUsed;
		ARK2D_DWORD biClrImportant;
	};
	struct ARK2D_RGBQUAD {
		ARK2D_BYTE rgbBlue;
		ARK2D_BYTE rgbGreen;
		ARK2D_BYTE rgbRed;
		ARK2D_BYTE rgbReserved;
	};
	struct ARK2D_BITMAPINFO {
		ARK2D_BITMAPINFOHEADER bmiHeader;
		ARK2D_RGBQUAD          bmiColors[1];
	};
#endif


class BMPImage {
	public:
		int Width,Height;		// Dimensions
		int BPP;				// Bits Per Pixel.
		char* Raster;			// Bits of the Image.
		int BytesPerRow;		// Row Width (in bytes).

		ARK2D_RGBQUAD *Palette;		// RGB Palette for the image.
		ARK2D_BITMAPINFO *pbmi;		// BITMAPINFO structure

		// Member functions (defined later):
		int LoadBMP (const char* szFile);

};

#endif /* BMPIMAGE_H_ */
