/*
 * HSVShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_HSVSHADER_H_
#define ARK_GRAPHICS_SHADERS_HSVSHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API HSVGeometryShader : public ARK::Graphics::Shader 
		{ 
			public:
				unsigned int _ModelMatrix;
				unsigned int _ViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _HSVIn;

				unsigned int _VertexPositionIn;
				unsigned int _VertexColorIn;
				
				Shader* m_shaderBasicGeometry;
				QuadVBO* m_vbo; 
				unsigned int m_shaderBasicGeometry_ModelMatrix;
				unsigned int m_shaderBasicGeometry_ViewMatrix;
				unsigned int m_shaderBasicGeometry_ProjectionMatrix;
				unsigned int m_shaderBasicGeometry_VertexPositionIn;
				unsigned int m_shaderBasicGeometry_VertexColorIn;

				float h;
				float s;
				float v;
				
			public:
				HSVGeometryShader();  
				void load();
				virtual void bind();
				virtual void unbind();

				void start();
				void stop();
				virtual ~HSVGeometryShader();
		};
		
		class ARK2D_API HSVTextureShader : public ARK::Graphics::Shader 
		{ 
			public:
				unsigned int _ModelMatrix;
				unsigned int _ViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _TextureId;
				unsigned int _HSVIn;

				unsigned int _VertexPositionIn;
				unsigned int _VertexTexCoordIn;
				unsigned int _VertexColorIn;

				Shader* m_shaderBasicTexture;
				QuadVBO* m_vbo; 
				unsigned int m_shaderBasicTexture_TextureId;
				unsigned int m_shaderBasicTexture_ModelMatrix;
				unsigned int m_shaderBasicTexture_ViewMatrix;
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
