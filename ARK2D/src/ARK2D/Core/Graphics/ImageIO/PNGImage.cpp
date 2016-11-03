/*
 * PNGImage.cpp
 *
 *  Created on: 15-Dec-2009
 *      Author: Ashley
 */

#include <string>
#include <iostream>
#include <cstdio>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
using namespace std;

#include "../../Controls/ErrorDialog.h"
#include "../../GameContainer.h"

#include "PNGImage.h"

namespace ARK {
    namespace Core {
        namespace Graphics {
            namespace ImageIO {

                #if defined(ARK2D_WINDOWS_PHONE_8)
                    //#if (PNG_LIBPNG_VER < 10400 || PNG_LIBPNG_VER >= 10500)
                    //	#define IF_JMPBUF if (setjmp(png_jmpbuf(png_ptr)))
                    //#else
                        // Warning is unavoidable if #define PNG_DEPSTRUCT is not present
                        //#define IF_JMPBUF(PTR) if (setjmp((PTR)->jmpbuf))

                        // no error checking?!
                        #define IF_JMPBUF(PTR) if (false)
                    //#endif
                #else
                    //#define IF_JMPBUF(PTR) if (setjmp((PTR)->jmpbuf))
                    #define IF_JMPBUF(PTR) if (setjmp(png_jmpbuf(png_ptr)))
                #endif

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
                bool PNGImage::isRGB() {
                    return (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB);
                }
                bool PNGImage::isRGBA() {
                    return (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA);
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

                void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
                    ARK2D::getLog()->e(error_msg);
                }
                void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg){
                    ARK2D::getLog()->w(warning_msg);
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
                    png_set_error_fn(png_ptr, NULL, &user_error_fn, &user_warning_fn);

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



                    IF_JMPBUF(png_ptr) {
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
                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
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
                        char opaque = 255;
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
                                memcpy((m_data + curOffset + 3), &opaque, 1);
                                curOffset += 4;
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
                    FILE* fp = NULL;
                    #if defined(ARK2D_WINDOWS_PHONE_8)
                        fopen_s(&fp, m_filename.c_str(), "rb");
                    #else
                        fp = fopen(m_filename.c_str(), "rb");
                    #endif

                    if (!fp) {
                        string s = string(m_filename.c_str()) + string(" could not be opened for reading.");
                        ErrorDialog::createAndShow(s);
                        return 1;
                    }

                    /*int tempres =*/ fread(header, 1, 8, fp);

                    if (png_sig_cmp((png_byte*) header, 0, 8)) {
                        string s = string(m_filename.c_str()) + string(" is not recognised as a PNG file.");
                        ErrorDialog::createAndShow(s);
                        return 1;
                    }


                    /* initialize stuff */
                    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

                    if (!png_ptr) {
                        string s = string(m_filename.c_str()) + string(": png_create_read_struct failed");
                        ErrorDialog::createAndShow(s);
                        return 1;
                    }


                    info_ptr = png_create_info_struct(png_ptr);
                    if (!info_ptr) {
                        string s = string(m_filename.c_str()) + string(" png_create_info_struct failed.");
                        ErrorDialog::createAndShow(s);
                        return 1;
                    }

                    end_info = png_create_info_struct(png_ptr);
                    if (!end_info) {
                        string s = string(m_filename.c_str()) + string(" png_create_info_struct failed (2).");
                        ErrorDialog::createAndShow(s);
                        return 1;
                    }

                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
                        string s = string(m_filename.c_str()) + string(" error during init_io.");
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
                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
                        string s = string(m_filename.c_str()) + string(" error during read_image.");
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
                        string s = string(m_filename.c_str()) + string(" [process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA or RGB (%d) (is %d)");
                        ErrorDialog::createAndShow(s);
                        return 1;
                    }

                    return 0;

                }

                void PNGImage::saveFile(std::string filename, char* data, int w, int h) {

                    // copied from SystemUtil::file_put_contents OR SystemUtil::prependPlatform
                    {
                        #if defined(ARK2D_ANDROID)
                            filename = ARK2D::getContainer()->m_platformSpecific.m_externalDataStr + filename;
                        #elif defined(ARK2D_IPHONE)
                            // append Documents folder first.
                            NSArray* searchPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
                            NSString* documentsPath = [searchPaths objectAtIndex:0];
                            string respath( [ documentsPath UTF8String ] );

                            filename = respath + string("/") + filename;
                        #else
                            if (filename.substr(1,1).compare(":") == 0 || filename.substr(0,1).compare("/") == 0) {

                            } else {
                                filename = ARK2D::getContainer()->getResourcePath() + filename;
                            }
                        #endif

                        #if defined(ARK2D_FLASCC)
                            filename = string("/local") + filename;
                        #endif

                        ARK2D::getLog()->i(string("Making file: ") + string(filename));
                    }

                    /* create file */
                    FILE* fp = NULL;
                    #if defined(ARK2D_WINDOWS_PHONE_8)
                        fopen_s(&fp, filename.c_str(), "wb");
                    #else
                        fp = fopen(filename.c_str(), "wb");
                    #endif

                    if (!fp) {
                        ErrorDialog::createAndShow(string("[write_png_file] File %s could not be opened for writing ") + string(filename));
                        return;
                    }

                    /* initialize stuff */
                    png_structp png_ptr;
                    png_infop info_ptr;

                    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                    if (!png_ptr) {
                        ErrorDialog::createAndShow("[write_png_file] png_create_write_struct failed");
                        return;
                    }

                    info_ptr = png_create_info_struct(png_ptr);
                    if (!info_ptr) {
                        ErrorDialog::createAndShow("[write_png_file] png_create_info_struct failed");
                        return;
                    }

                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
                        ErrorDialog::createAndShow("[write_png_file] Error during init_io");
                        return;
                    }

                    png_init_io(png_ptr, fp);

                //	png_set_sig_bytes(png_ptr, 8);


                    /* write header */
                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
                        ErrorDialog::createAndShow("[write_png_file] Error during writing header");
                        return;
                    }

                    png_set_IHDR(png_ptr, info_ptr, w, h,
                                 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

                    png_write_info(png_ptr, info_ptr);


                    /* write bytes */
                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
                        ErrorDialog::createAndShow("[write_png_file] Error during writing bytes");
                        return;
                    }

                    png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * h);
                    for (int y=0; y<h; y++) {
                        row_pointers[y] = (png_byte*) malloc(w*4); //png_get_rowbytes(png_ptr,info_ptr));
                    }
                    int curOffset = 0;
                    for(int i = h-1; i >= 0; i--) {
                        png_byte* row = row_pointers[i];
                        for (int j = 0; j < w; j++) {
                            unsigned char opaque = 255;
                            png_byte* ptr = &row[j*4];
                            memcpy(&ptr[0], (data + curOffset), 1);
                            memcpy(&ptr[1], (data + curOffset + 1), 1);
                            memcpy(&ptr[2], (data + curOffset + 2), 1);
                            //memcpy(&ptr[3], (data + curOffset + 3), 1);
                            memcpy(&ptr[3], &opaque, 1);
                            curOffset += 4;
                        }
                    }
                    png_write_image(png_ptr, row_pointers);




                    /* end write */
                    //if (setjmp(png_jmpbuf(png_ptr))) {
                    IF_JMPBUF(png_ptr) {
                        ErrorDialog::createAndShow("[write_png_file] Error during end of write");
                        return;
                    }

                    png_write_end(png_ptr, NULL);

                    /* cleanup heap allocation */
                    for (int y = 0 ; y < h; y++) {
                        free(row_pointers[y]);
                    }
                    free(row_pointers);

                    fclose(fp);
                }

                PNGImage::~PNGImage() {
                    //clean up memory and close stuff
                    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                    //delete[] image_data;
                    delete[] row_pointers;
                    //delete m_data;

                    if (m_data != NULL) {
                        free(m_data);
                        m_data = NULL;
                    }

                    //fclose(fp);
                }
            }
		}
	}
}
