/*
 * HSVTextureShader.cpp
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#include "../Shader.h"
#include "HSVTextureShader.h"

namespace ARK { 
	namespace Graphics { 

		HSVTextureShader::HSVTextureShader(): 
			ARK::Graphics::Shader(),
			h(0.0f),
			s(1.0f),
			v(1.0f)
		{
			
		}
		void HSVTextureShader::load() {

			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/hsv-texture-glsles100.vertex");
				addFragmentShader("ark2d/shaders/hsv-texture-glsles100.fragment");
			} else {
				addVertexShader("ark2d/shaders/hsv-texture-glsl150-vertex.txt");
				bool err1 = hasError();
				addFragmentShader("ark2d/shaders/hsv-texture-glsl150-fragment.txt");
				bool err2 = hasError();

				if (err1 || err2) { 
					addVertexShader("ark2d/shaders/hsv-texture-glsl130-vertex.txt");
					addFragmentShader("ark2d/shaders/hsv-texture-glsl130-fragment.txt");
				}
			}

			bindAttributeLocation(1, "ark_VertexPosition");
			bindAttributeLocation(2, "ark_VertexTexCoordIn");
			bindAttributeLocation(3, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			showAnyGlErrorAndExitMacro();

			RendererState::start(RendererState::SHADER, getId());

			_ModelViewMatrix = getUniformVariable("ark_ModelViewMatrix");
			_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			_TextureId = getUniformVariable("ark_TextureId");
			_HSVIn = getUniformVariable("ark_HSV");

			_VertexPositionIn = 1;
			_VertexTexCoordIn = 2;
			_VertexColorIn = 3;
			
			RendererState::start(RendererState::GEOMETRY);
		}

		void HSVTextureShader::bind() {
			Shader::bind();
			setUniformVariable3F(_HSVIn, h, s, v);	
		} 


		void HSVTextureShader::unbind() {
			Shader::unbind();
		}

		void HSVTextureShader::start() {

			RendererState::start(RendererState::NONE);
			m_shaderBasicTexture 					= Renderer::s_shaderBasicTexture;
			m_shaderBasicTexture_ModelViewMatrix	= Renderer::s_shaderBasicTexture_ModelViewMatrix;
			m_shaderBasicTexture_ProjectionMatrix 	= Renderer::s_shaderBasicTexture_ProjectionMatrix;
			m_shaderBasicTexture_VertexPositionIn 	= Renderer::s_shaderBasicTexture_VertexPosition;
			m_shaderBasicTexture_VertexTexCoordIn 	= Renderer::s_shaderBasicTexture_VertexTexCoordIn;
			m_shaderBasicTexture_VertexColorIn 		= Renderer::s_shaderBasicTexture_VertexColorIn;
			
			Renderer::s_shaderBasicTexture 						= this;
			Renderer::s_shaderBasicTexture_ModelViewMatrix 		= _ModelViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix 	= _ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition		= _VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn		= _VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 		= _VertexColorIn;
		}
		void HSVTextureShader::stop() {
			Renderer::s_shaderBasicTexture 						= m_shaderBasicTexture;
			Renderer::s_shaderBasicTexture_ModelViewMatrix 		= m_shaderBasicTexture_ModelViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix 	= m_shaderBasicTexture_ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition		= m_shaderBasicTexture_VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn		= m_shaderBasicTexture_VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 		= m_shaderBasicTexture_VertexColorIn;
			RendererState::start(RendererState::NONE);
		}
		  
		HSVTextureShader::~HSVTextureShader() { 
			 
		}
	}
}






