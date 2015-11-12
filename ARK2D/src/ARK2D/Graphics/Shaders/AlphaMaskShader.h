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

		class AlphaMaskGeometryShader : public Shader {
			public:
				Shader* m_shaderBasicGeometry;
				unsigned int _ModelViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _VertexPositionIn;
				unsigned int _VertexColorIn;
				
				unsigned int m_shaderBasicGeometry_ModelViewMatrix;
				unsigned int m_shaderBasicGeometry_ProjectionMatrix;
				unsigned int m_shaderBasicGeometry_VertexPositionIn;
				unsigned int m_shaderBasicGeometry_VertexColorIn;

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
		class AlphaMaskTextureShader : public Shader {
			public:
				Shader* m_shaderBasicTexture;
				unsigned int _ModelViewMatrix;
				unsigned int _ProjectionMatrix;

				unsigned int _TextureId;
				unsigned int _VertexPositionIn;
				unsigned int _VertexTexCoordIn;
				unsigned int _VertexColorIn;
				
				unsigned int m_shaderBasicTexture_ModelViewMatrix;
				unsigned int m_shaderBasicTexture_ProjectionMatrix;
				unsigned int m_shaderBasicTexture_VertexPositionIn;
				unsigned int m_shaderBasicTexture_VertexTexCoordIn;
				unsigned int m_shaderBasicTexture_VertexColorIn;
				unsigned int m_shaderBasicTexture_TextureId;

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
		class AlphaMaskShader {
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
