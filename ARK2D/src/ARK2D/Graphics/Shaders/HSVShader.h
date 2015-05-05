/*
 * HSVShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_HSVSHADER_H_
#define ARK_GRAPHICS_SHADERS_HSVSHADER_H_

#include "../../ARK2D.h"

namespace ARK { 
	namespace Graphics { 

		class HSVGeometryShader;
		class HSVTextureShader;

		class ARK2D_API HSVShader
		{ 
			HSVGeometryShader* geometry;
			HSVTextureShader* texture;
				
			public:
				HSVShader();
				void load();
				void set(float h, float s, float v);
				float getH();
				float getS();
				float getV();
				void start();
				void stop();
				virtual ~HSVShader();
		};
	}
}

#endif /* ARK_GRAPHICS_SHADERS_HSVSHADER_H_ */
