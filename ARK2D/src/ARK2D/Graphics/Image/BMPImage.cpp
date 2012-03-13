/*
 * BMPImage.cpp
 *
 *  Created on: 13-Dec-2009
 *      Author: Ashley
 */

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "BMPImage.h"
#include "../../UI/ErrorDialog.h"
#include "../../Util/StringUtil.h"

using std::ios;
using std::ifstream;

/**
 * BMPImage::LoadBMPFile (FileName);
 *  - loads a BMP file into a BMPImage object
 *  - supports non-RLE-compressed files of 1, 2, 4, 8 & 24 bits-per-pixel
 *  - requires power of two bitmap. yes really. otherwise wonky rendering!
 **/
int BMPImage::LoadBMP (const char* szFile)
{
	#if defined(ARK2D_MACINTOSH)
		ErrorDialog::createAndShow(StringUtil::append("Loading bitmaps is broken on Mac: ", string(szFile)));
		exit(0);
	#endif

	ARK2D_BITMAPFILEHEADER bmfh;
	ARK2D_BITMAPINFOHEADER bmih;

	// Open file.
	ifstream bmpfile (szFile , ios::in | ios::binary);
	if (! bmpfile.is_open()) return 1;		// Error opening file


	// Load bitmap fileheader & infoheader
	bmpfile.read ((char*)&bmfh, sizeof (ARK2D_BITMAPFILEHEADER));
	bmpfile.read ((char*)&bmih, sizeof (ARK2D_BITMAPINFOHEADER));

	// Check filetype signature
	if (bmfh.bfType != 'MB') return 2;		// File is not BMP

	// Assign some short variables:
	BPP = bmih.biBitCount;
	Width = bmih.biWidth;
	Height = (bmih.biHeight>0) ? bmih.biHeight : -bmih.biHeight; // absoulte value
	BytesPerRow = Width * BPP / 8;
	BytesPerRow += (4-BytesPerRow%4) % 4;	// int alignment

	// If BPP aren't 24, load Palette:
	if (BPP==24) pbmi=(ARK2D_BITMAPINFO*)new char [sizeof(ARK2D_BITMAPINFO)];
	else
	{
		pbmi=(ARK2D_BITMAPINFO*) new char[sizeof(ARK2D_BITMAPINFOHEADER)+(1<<BPP)*sizeof(ARK2D_RGBQUAD)];
		Palette=(ARK2D_RGBQUAD*)((char*)pbmi+sizeof(ARK2D_BITMAPINFOHEADER));
		bmpfile.read ((char*)Palette,sizeof (ARK2D_RGBQUAD) * (1<<BPP));
	}
	pbmi->bmiHeader=bmih;

	// Load Raster
	bmpfile.seekg (bmfh.bfOffBits, ios::beg);

	Raster = new char[BytesPerRow*Height];

	// (if height is positive the bmp is bottom-up, read it reversed)
	if (bmih.biHeight>0)
		for (int n=Height-1;n>=0;n--)
			bmpfile.read (Raster+BytesPerRow*n,BytesPerRow);
	else
		bmpfile.read (Raster,BytesPerRow*Height);

	// so, we always have a up-bottom raster (that is negative height for windows):
	pbmi->bmiHeader.biHeight=-Height;

	bmpfile.close();

	return 0;
}
