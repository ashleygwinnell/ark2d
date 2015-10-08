/*
 * StencilShader.h
 *
 *  Created on: 5 Jun 2014
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_STENCILSHADER_H_
#define ARK_GRAPHICS_STENCILSHADER_H_

#include "../../ARK2D.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API StencilShader : public ARK::Graphics::Shader 
		{ 
			public:
				unsigned int _ModelViewMatrix;
				unsigned int _ProjectionMatrix;

				unsigned int _TextureId;
				unsigned int _VertexPositionIn;
				unsigned int _VertexTexCoordIn;
				unsigned int _VertexColorIn;

				Shader* m_shaderBasicTexture;
				unsigned int m_shaderBasicTexture_ModelViewMatrix;
				unsigned int m_shaderBasicTexture_ProjectionMatrix;
				unsigned int m_shaderBasicTexture_VertexPositionIn;
				unsigned int m_shaderBasicTexture_VertexColorIn;
				unsigned int m_shaderBasicTexture_VertexTexCoordIn;
				unsigned int m_shaderBasicTexture_TextureId;

			public:
				StencilShader();  
				void load();
				void start();
				void stop();
				void startInternal();
				void stopInternal();
				
				static void startStatic(StencilShader* obj);
				static void stopStatic(StencilShader* obj);

				virtual ~StencilShader();
		};
	}
}

#endif /* ARK_GRAPHICS_STENCILSHADER_H_ */
