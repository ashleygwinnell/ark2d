/*
 * ZXRenderer.h
 *
 *  Created on: 07 Jan 2017
 *      Author: Ashley
 */

#ifndef ARK2D_UTIL_ZXRENDERER_H_
#define ARK2D_UTIL_ZXRENDERER_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/Graphics/FBO.h"
#include "../Core/Graphics/Color.h"
#include "../Core/Graphics/Image.h"

#include <vector>
#include <string>
using namespace std;

namespace ARK {
	namespace Util {

		class ARK2D_API ZXRenderer {
			public:
				int widthInTiles;
				int heightInTiles;

				FBO* m_renderTargetFG;
				FBO* m_renderTarget;
				Image* m_image;

				unsigned int m_attributeBorder;
				vector<Color* > m_attributeColoursDim;
				vector<Color* > m_attributeColoursBright;
				vector<unsigned char> m_attributeBlocksBG;
				vector<unsigned char> m_attributeBlocksFG;
				vector<bool> m_attributeBlocksSet;

				bool m_stencilbounds;

				static const unsigned int COLOR_BLACK   = 0;
				static const unsigned int COLOR_BLUE    = 1;
				static const unsigned int COLOR_RED     = 2;
				static const unsigned int COLOR_MAGENTA = 3;
				static const unsigned int COLOR_GREEN   = 4;
				static const unsigned int COLOR_CYAN    = 5;
				static const unsigned int COLOR_YELLOW  = 6;
				static const unsigned int COLOR_WHITE   = 7;
				static const unsigned int COLOR_ROOM    = 8;
				static unsigned int colorToInt(string s);
				static string intToColorString(unsigned int s);
				static unsigned int colorNext(unsigned int c);

			public:
				void init(int widthTiles, int heightTiles);
				void reset();
				void start();
				void stop();
				void border(unsigned char color);
				void fg(unsigned char x, unsigned char y, unsigned char color);
				void bg(unsigned char x, unsigned char y, unsigned char color);
				void bright(unsigned char x, unsigned char y, bool bright);
				void resize();
				Image* getImage();

			private:
				void __renderBG();
				void __renderFG();
		};
	}
}

#endif /* ARK2D_UTIL_ZXRENDERER_H_ */
