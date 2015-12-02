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
				unsigned int _ModelMatrix;
				unsigned int _ViewMatrix;
				unsigned int _ProjectionMatrix;
                unsigned int _NormalMatrix;
				unsigned int _VertexPositionIn;
                unsigned int _VertexNormalIn;
				unsigned int _VertexColorIn;
				
				unsigned int m_shaderBasicGeometry_ModelMatrix;
				unsigned int m_shaderBasicGeometry_ViewMatrix;
				unsigned int m_shaderBasicGeometry_ProjectionMatrix;
				unsigned int m_shaderBasicGeometry_NormalMatrix;
				unsigned int m_shaderBasicGeometry_VertexPositionIn;
				unsigned int m_shaderBasicGeometry_VertexNormalIn;
				unsigned int m_shaderBasicGeometry_VertexColorIn;

				BasicGeometryShader();  
				void load();
				virtual ~BasicGeometryShader();
		};
		class ARK2D_API BasicTextureShader : public Shader {
			public:
				unsigned int _ModelMatrix;
				unsigned int _ViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _NormalMatrix;

				unsigned int _TextureId;
				unsigned int _VertexPositionIn;
				unsigned int _VertexNormalIn;
				unsigned int _VertexTexCoordIn;
				unsigned int _VertexColorIn;
				
				unsigned int m_shaderBasicTexture_ModelMatrix;
				unsigned int m_shaderBasicTexture_ViewMatrix;
				unsigned int m_shaderBasicTexture_ProjectionMatrix;
				unsigned int m_shaderBasicTexture_NormalMatrix;
				unsigned int m_shaderBasicTexture_VertexPositionIn;
				unsigned int m_shaderBasicTexture_VertexNormalIn;
				unsigned int m_shaderBasicTexture_VertexTexCoordIn;
				unsigned int m_shaderBasicTexture_VertexColorIn;
				unsigned int m_shaderBasicTexture_TextureId;

				BasicTextureShader();  
				void load();
				void start();
				void stop();
				void startInternal();
				void stopInternal();
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
