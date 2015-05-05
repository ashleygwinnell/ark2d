/*
 * HSVGeometryShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERS_HSVGEOMETRYSHADER_H_
#define ARK_GRAPHICS_SHADERS_HSVGEOMETRYSHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK { 
	namespace Graphics { 

		class ARK2D_API HSVGeometryShader : public ARK::Graphics::Shader 
		{ 
			public:
				unsigned int _ModelViewMatrix;
				unsigned int _ProjectionMatrix;
				unsigned int _HSVIn;

				unsigned int _VertexPositionIn;
				unsigned int _VertexColorIn;
				
				Shader* m_shaderBasicGeometry;
				QuadVBO* m_vbo; 
				unsigned int m_shaderBasicGeometry_ModelViewMatrix;
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
	}
}

#endif /* ARK_GRAPHICS_SHADERS_HSVGEOMETRYSHADER_H_ */
