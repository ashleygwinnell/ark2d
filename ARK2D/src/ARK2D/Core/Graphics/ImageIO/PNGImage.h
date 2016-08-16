/*
 * PNGImage.h
 *
 *  Created on: 15-Dec-2009
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_IMAGEIO_PNGIMAGE_H_
#define ARK_CORE_GRAPHICS_IMAGEIO_PNGIMAGE_H_

#include "../../ARK2D.h"
//#include "../../Includes.h"
#include "../../../Common/DLL.h"
#include "../../../Namespaces.h"
#include "../../../Common/Libraries/libpng.h"

namespace ARK {
    namespace Core {
        namespace Graphics {
            namespace ImageIO {

                class PNGImagestream {
                    public:
                        void* m_data;
                        unsigned int m_pos;
                };

                class ARK2D_API PNGImage {
                    public:
                        PNGImage(const std::string filename);
                        int load();
                        int load(void* data);
                        static void readFunction(png_structp pngPtr, png_bytep data, png_size_t length);
                        int getWidth();
                        int getHeight();
                        void* getImageData();
                        bool isRGB();
                        bool isRGBA();
                        static void saveFile(std::string filename, char* data, int w, int h);
                        ~PNGImage();
                    private:
                        std::string m_filename;
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
            }
        }
	}
}

#endif /* PNGIMAGE_H_ */
