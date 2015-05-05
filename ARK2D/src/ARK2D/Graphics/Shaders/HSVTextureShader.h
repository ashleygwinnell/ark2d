/*
 * HSVTextureShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_HSVTEXTURESHADER_H_
#define ARK_GRAPHICS_SHADERS_HSVTEXTURESHADER_H_

#include "../../ARK2D.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API HSVTextureShader : public ARK::Graphics::Shader 
		{ 
			public:
				unsigned int _ModelViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _TextureId;
				unsigned int _HSVIn;

				unsigned int _VertexPositionIn;
				unsigned int _VertexTexCoordIn;
				unsigned int _VertexColorIn;

				Shader* m_shaderBasicTexture;
				QuadVBO* m_vbo; 
				unsigned int m_shaderBasicTexture_ModelViewMatrix;
				unsigned int m_shaderBasicTexture_ProjectionMatrix;
				unsigned int m_shaderBasicTexture_VertexPositionIn;
				unsigned int m_shaderBasicTexture_VertexTexCoordIn;
				unsigned int m_shaderBasicTexture_VertexColorIn;

				float h;
				float s;
				float v;
				
			public:
				HSVTextureShader();  
				void load();
				virtual void bind();
				virtual void unbind();

				void start();
				void stop();
				virtual ~HSVTextureShader();
		};
	}
}

#endif /* ARK_GRAPHICS_SHADERS_HSVTEXTURESHADER_H_ */
