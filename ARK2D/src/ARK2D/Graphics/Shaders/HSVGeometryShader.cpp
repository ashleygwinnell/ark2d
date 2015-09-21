/*
 * HSVGeometryShader.cpp
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#include "../Shader.h"
#include "HSVGeometryShader.h"
#include "../Image.h"

namespace ARK { 
	namespace Graphics { 

		HSVGeometryShader::HSVGeometryShader(): 
			ARK::Graphics::Shader(),
			h(0.0f),
			s(1.0f),
			v(1.0f)
		{
		
		}
		void HSVGeometryShader::load() {

			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/hsv-geometry-glsles100.vertex");
				addFragmentShader("ark2d/shaders/hsv-geometry-glsles100.fragment");
			} else {
				addVertexShader("ark2d/shaders/hsv-geometry-glsl150-vertex.txt");
				addFragmentShader("ark2d/shaders/hsv-geometry-glsl150-fragment.txt");
				bool err1 = hasError();
				bool err2 = hasError();

				if (err1 || err2) { 
					addVertexShader("ark2d/shaders/hsv-geometry-glsl130-vertex.txt");
					addFragmentShader("ark2d/shaders/hsv-geometry-glsl130-fragment.txt");
				}
			}

			bindAttributeLocation(0, "ark_VertexPosition");
			bindAttributeLocation(1, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			showAnyGlErrorAndExitMacro();

			RendererState::start(RendererState::SHADER, getId());

			_ModelViewMatrix = getUniformVariable("ark_ModelViewMatrix");
			_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			_HSVIn = getUniformVariable("ark_HSV");

			_VertexPositionIn = 0;
			_VertexColorIn = 1;
			
			
			RendererState::start(RendererState::GEOMETRY);
		}

		void HSVGeometryShader::bind() {
			Shader::bind();
			setUniformVariable3F(_HSVIn, h, s, v);	

		//	ARK2D::getLog()->w("bind HSL geom shader");
		} 


		void HSVGeometryShader::unbind() {
			Shader::unbind();
		}

		void HSVGeometryShader::start() {

			RendererState::start(RendererState::NONE);
			m_shaderBasicGeometry 					= Renderer::s_shaderBasicGeometry;
			m_shaderBasicGeometry_ModelViewMatrix	= Renderer::s_shaderBasicGeometry_ModelViewMatrix;
			m_shaderBasicGeometry_ProjectionMatrix 	= Renderer::s_shaderBasicGeometry_ProjectionMatrix;
			m_shaderBasicGeometry_VertexPositionIn 	= Renderer::s_shaderBasicGeometry_VertexPosition;
			m_shaderBasicGeometry_VertexColorIn 	= Renderer::s_shaderBasicGeometry_VertexColorIn;
			
			Renderer::s_shaderBasicGeometry 					= this;
			Renderer::s_shaderBasicGeometry_ModelViewMatrix 	= _ModelViewMatrix;
			Renderer::s_shaderBasicGeometry_ProjectionMatrix 	= _ProjectionMatrix;
			Renderer::s_shaderBasicGeometry_VertexPosition		= _VertexPositionIn;
			Renderer::s_shaderBasicGeometry_VertexColorIn 		= _VertexColorIn;
		}
		void HSVGeometryShader::stop() {
			Renderer::s_shaderBasicGeometry 					= m_shaderBasicGeometry;
			Renderer::s_shaderBasicGeometry_ModelViewMatrix 	= m_shaderBasicGeometry_ModelViewMatrix;
			Renderer::s_shaderBasicGeometry_ProjectionMatrix 	= m_shaderBasicGeometry_ProjectionMatrix;
			Renderer::s_shaderBasicGeometry_VertexPosition		= m_shaderBasicGeometry_VertexPositionIn;
			Renderer::s_shaderBasicGeometry_VertexColorIn 		= m_shaderBasicGeometry_VertexColorIn;
			RendererState::start(RendererState::NONE);
		}
		  
		HSVGeometryShader::~HSVGeometryShader() { 
			 
		}
	}
}






