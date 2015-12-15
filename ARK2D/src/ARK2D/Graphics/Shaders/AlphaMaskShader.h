/*
 * AlphaMaskShader.h
 *
 *  Created on: 11 Nov 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_ALPHAMASKSHADER_H_
#define ARK_GRAPHICS_SHADERS_ALPHAMASKSHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API AlphaMaskGeometryShader : public Shader {
			public:
				AlphaMaskGeometryShader();  
				void load();
				void start();
				void stop();
				void startInternal();
				void stopInternal();
				static void startStatic(AlphaMaskGeometryShader* obj);
				static void stopStatic(AlphaMaskGeometryShader* obj);
				virtual ~AlphaMaskGeometryShader();
		};
		class ARK2D_API AlphaMaskTextureShader : public Shader {
			public:
				AlphaMaskTextureShader();  
				void load();
				void start();
				void stop();
				void startInternal();
				void stopInternal();
				static void startStatic(AlphaMaskTextureShader* obj);
				static void stopStatic(AlphaMaskTextureShader* obj);
				virtual ~AlphaMaskTextureShader();
		};
		class ARK2D_API AlphaMaskShader {
			public:
				AlphaMaskGeometryShader* geometry;
				AlphaMaskTextureShader* texture;
				
			public:
				AlphaMaskShader();
				void load();
				void start();
				void stop();
				virtual ~AlphaMaskShader();
		};
	}
}

#endif
