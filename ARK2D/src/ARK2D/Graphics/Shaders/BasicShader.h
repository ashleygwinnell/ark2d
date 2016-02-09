/*
 * BasicShader.h
 *
 *  Created on: 1 Dec 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_BASICSHADER_H_
#define ARK_GRAPHICS_SHADERS_BASICSHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API BasicGeometryShader : public Shader {
			public:
			
				BasicGeometryShader();  
				void load();
				virtual void bind();
				virtual ~BasicGeometryShader();
		};
		class ARK2D_API BasicTextureShader : public Shader {
			public:
				

				BasicTextureShader();  
				void load();
				virtual void bind();
				virtual ~BasicTextureShader();
		};
		class ARK2D_API BasicShader {
			public:
				BasicGeometryShader* geometry;
				BasicTextureShader* texture;
				
			public:
				BasicShader();
				void load();
				virtual ~BasicShader();
		};
	}
}

#endif
