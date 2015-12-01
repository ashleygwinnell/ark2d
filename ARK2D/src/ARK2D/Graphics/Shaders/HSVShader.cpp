/*
 * HSVShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#include "HSVShader.h"
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
				addVertexShader("ark2d/shaders/hsv/geometry-glsles100.vertex");
				addFragmentShader("ark2d/shaders/hsv/geometry-glsles100.fragment");
			} else {
				addVertexShader("ark2d/shaders/hsv/geometry-glsl150-vertex.txt");
				addFragmentShader("ark2d/shaders/hsv/geometry-glsl150-fragment.txt");
				bool err1 = hasError();
				bool err2 = hasError();

				if (err1 || err2) { 
					addVertexShader("ark2d/shaders/hsv/geometry-glsl130-vertex.txt");
					addFragmentShader("ark2d/shaders/hsv/geometry-glsl130-fragment.txt");
				}
			}

			bindAttributeLocation(0, "ark_VertexPosition");
			bindAttributeLocation(1, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			showAnyGlErrorAndExitMacro();

			RendererState::start(RendererState::SHADER, getId());

			_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			_ViewMatrix = getUniformVariable("ark_ViewMatrix");
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
			m_shaderBasicGeometry_ModelMatrix		= Renderer::s_shaderBasicGeometry_ModelMatrix;
			m_shaderBasicGeometry_ViewMatrix		= Renderer::s_shaderBasicGeometry_ViewMatrix;
			m_shaderBasicGeometry_ProjectionMatrix 	= Renderer::s_shaderBasicGeometry_ProjectionMatrix;
			m_shaderBasicGeometry_VertexPositionIn 	= Renderer::s_shaderBasicGeometry_VertexPosition;
			m_shaderBasicGeometry_VertexColorIn 	= Renderer::s_shaderBasicGeometry_VertexColorIn;
			
			Renderer::s_shaderBasicGeometry 					= this;
			Renderer::s_shaderBasicGeometry_ModelMatrix 		= _ModelMatrix;
			Renderer::s_shaderBasicGeometry_ViewMatrix 			= _ViewMatrix;
			Renderer::s_shaderBasicGeometry_ProjectionMatrix 	= _ProjectionMatrix;
			Renderer::s_shaderBasicGeometry_VertexPosition		= _VertexPositionIn;
			Renderer::s_shaderBasicGeometry_VertexColorIn 		= _VertexColorIn;
		}
		void HSVGeometryShader::stop() {
			Renderer::s_shaderBasicGeometry 					= m_shaderBasicGeometry;
			Renderer::s_shaderBasicGeometry_ModelMatrix 		= m_shaderBasicGeometry_ModelMatrix;
			Renderer::s_shaderBasicGeometry_ViewMatrix 			= m_shaderBasicGeometry_ViewMatrix;
			Renderer::s_shaderBasicGeometry_ProjectionMatrix 	= m_shaderBasicGeometry_ProjectionMatrix;
			Renderer::s_shaderBasicGeometry_VertexPosition		= m_shaderBasicGeometry_VertexPositionIn;
			Renderer::s_shaderBasicGeometry_VertexColorIn 		= m_shaderBasicGeometry_VertexColorIn;
			RendererState::start(RendererState::NONE);
		}
		  
		HSVGeometryShader::~HSVGeometryShader() { 
			 
		}






		


		HSVTextureShader::HSVTextureShader(): 
			ARK::Graphics::Shader(),
			h(0.0f),
			s(1.0f),
			v(1.0f)
		{
			
		}
		void HSVTextureShader::load() {

			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/hsv/texture-glsles100.vertex");
				addFragmentShader("ark2d/shaders/hsv/texture-glsles100.fragment");
			} else {
				addVertexShader("ark2d/shaders/hsv/texture-glsl150-vertex.txt");
				bool err1 = hasError();
				addFragmentShader("ark2d/shaders/hsv/texture-glsl150-fragment.txt");
				bool err2 = hasError();

				if (err1 || err2) { 
					addVertexShader("ark2d/shaders/hsv/texture-glsl130-vertex.txt");
					addFragmentShader("ark2d/shaders/hsv/texture-glsl130-fragment.txt");
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

			_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			_ViewMatrix = getUniformVariable("ark_ViewMatrix");
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
			m_shaderBasicTexture_TextureId			= Renderer::s_shaderBasicTexture_TextureId;
			m_shaderBasicTexture_ModelMatrix		= Renderer::s_shaderBasicTexture_ModelMatrix;
			m_shaderBasicTexture_ViewMatrix			= Renderer::s_shaderBasicTexture_ViewMatrix;
			m_shaderBasicTexture_ProjectionMatrix 	= Renderer::s_shaderBasicTexture_ProjectionMatrix;
			m_shaderBasicTexture_VertexPositionIn 	= Renderer::s_shaderBasicTexture_VertexPosition;
			m_shaderBasicTexture_VertexTexCoordIn 	= Renderer::s_shaderBasicTexture_VertexTexCoordIn;
			m_shaderBasicTexture_VertexColorIn 		= Renderer::s_shaderBasicTexture_VertexColorIn;
			
			Renderer::s_shaderBasicTexture 						= this;
			Renderer::s_shaderBasicTexture_TextureId 			= _TextureId;
			Renderer::s_shaderBasicTexture_ModelMatrix 			= _ModelMatrix;
			Renderer::s_shaderBasicTexture_ViewMatrix 			= _ViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix 	= _ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition		= _VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn		= _VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 		= _VertexColorIn;
		} 
		void HSVTextureShader::stop() {
			Renderer::s_shaderBasicTexture 						= m_shaderBasicTexture;
			Renderer::s_shaderBasicTexture_TextureId 			= m_shaderBasicTexture_TextureId;
			Renderer::s_shaderBasicTexture_ModelMatrix 			= m_shaderBasicTexture_ModelMatrix;
			Renderer::s_shaderBasicTexture_ViewMatrix 			= m_shaderBasicTexture_ViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix 	= m_shaderBasicTexture_ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition		= m_shaderBasicTexture_VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn		= m_shaderBasicTexture_VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 		= m_shaderBasicTexture_VertexColorIn;
			RendererState::start(RendererState::NONE);
		}
		  
		HSVTextureShader::~HSVTextureShader() { 
			 
		}









		HSVShader::HSVShader() {
			geometry = new HSVGeometryShader();
			texture = new HSVTextureShader();
		}
		void HSVShader::load() {
			geometry->load();
			texture->load();
		}
		void HSVShader::set(float h, float s, float v) {
			geometry->h = h;
			geometry->s = s;
			geometry->v = v;

			texture->h = h;
			texture->s = s;
			texture->v = v;
		}
		float HSVShader::getH() {
			return geometry->h;
		}
		float HSVShader::getS() {
			return geometry->s;
		}
		float HSVShader::getV() {
			return geometry->v;
		}
		void HSVShader::start() {
			geometry->start();
			texture->start();
		}
		void HSVShader::stop() {
			geometry->stop();
			texture->stop();
		}
		HSVShader::~HSVShader() {
			delete geometry;
			delete texture;
		}
	}
}
