/*
 * StencilShader.cpp
 *
 *  Created on: 5 Jun 2015
 *      Author: Ashley
 */

#include "../Shader.h"
#include "StencilShader.h"

namespace ARK { 
	namespace Graphics { 

		StencilShader::StencilShader(): 
			ARK::Graphics::Shader()
		{
			
		}
		void StencilShader::load() {

			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/stencil/texture-glsles100-vertex.txt");
				addFragmentShader("ark2d/shaders/stencil/texture-glsles100-fragment.txt");
			} else {
				addVertexShader("ark2d/shaders/stencil/texture-glsl150-vertex.txt");
				bool err1 = hasError();
				addFragmentShader("ark2d/shaders/stencil/texture-glsl150-fragment.txt");
				bool err2 = hasError();

				if (err1 || err2) { 
					addVertexShader("ark2d/shaders/stencil/texture-glsl130-vertex.txt");
					addFragmentShader("ark2d/shaders/stencil/texture-glsl130-fragment.txt");
				}
			}

			bindAttributeLocation(0, "ark_TextureId");
			bindAttributeLocation(1, "ark_VertexPosition");
			bindAttributeLocation(2, "ark_VertexTexCoordIn");
			bindAttributeLocation(3, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			RendererState::start(RendererState::SHADER, getId());

			_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			_ViewMatrix = getUniformVariable("ark_ViewMatrix");
			_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			
			_TextureId = getUniformVariable("ark_TextureId");
			_VertexPositionIn = 1;
			_VertexTexCoordIn = 2;
			_VertexColorIn = 3;
			
			RendererState::start(RendererState::GEOMETRY);
		}

		void StencilShader::startStatic(StencilShader* obj) { obj->startInternal(); }
		void StencilShader::stopStatic(StencilShader* obj) { obj->stopInternal(); }

		void StencilShader::start() {
			Renderer* r = ARK2D::getRenderer();
			if (r->isBatching()) {
				RendererBatchItem item;
				item.m_type = RendererBatchItem::TYPE_CUSTOM_OBJECT_FUNCTION;
				item.m_objectPointer = this;
				item.m_functionPointer = (void*) &startStatic;
				Renderer::s_batch->items.push_back(item);
				return;
			}
			startInternal();
		}
		void StencilShader::stop() {
			Renderer* r = ARK2D::getRenderer();
			if (r->isBatching()) {
				RendererBatchItem item;
				item.m_type = RendererBatchItem::TYPE_CUSTOM_OBJECT_FUNCTION;
				item.m_objectPointer = this;
				item.m_functionPointer = (void*) &stopStatic;
				Renderer::s_batch->items.push_back(item);
				return;
			}
			stopInternal();
		}

		void StencilShader::startInternal() {
		
			//RendererState::s_shaderId = getId();
			//Shader::bind();

			// Replace existing texture shader... how novel! 
			m_shaderBasicTexture 					= Renderer::s_shaderBasicTexture;
			m_shaderBasicTexture_ModelMatrix		= Renderer::s_shaderBasicTexture_ModelMatrix;
			m_shaderBasicTexture_ViewMatrix			= Renderer::s_shaderBasicTexture_ViewMatrix;
			m_shaderBasicTexture_ProjectionMatrix 	= Renderer::s_shaderBasicTexture_ProjectionMatrix;
			m_shaderBasicTexture_VertexPositionIn 	= Renderer::s_shaderBasicTexture_VertexPosition;
			m_shaderBasicTexture_VertexTexCoordIn 	= Renderer::s_shaderBasicTexture_VertexTexCoordIn;
			m_shaderBasicTexture_VertexColorIn 		= Renderer::s_shaderBasicTexture_VertexColorIn;
			m_shaderBasicTexture_TextureId 			= Renderer::s_shaderBasicTexture_TextureId;

			Renderer::s_shaderBasicTexture 					= this;
			Renderer::s_shaderBasicTexture_ModelMatrix 		= _ModelMatrix;
			Renderer::s_shaderBasicTexture_ViewMatrix 		= _ViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix = _ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition	= _VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn = _VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 	= _VertexColorIn;
			Renderer::s_shaderBasicTexture_TextureId 		= _TextureId;


		} 

		void StencilShader::stopInternal() {
			//Shader::unbind();

			Renderer::s_shaderBasicTexture 					= m_shaderBasicTexture;
			Renderer::s_shaderBasicTexture_ModelMatrix 		= m_shaderBasicTexture_ModelMatrix;
			Renderer::s_shaderBasicTexture_ViewMatrix 		= m_shaderBasicTexture_ViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix = m_shaderBasicTexture_ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition	= m_shaderBasicTexture_VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn = m_shaderBasicTexture_VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 	= m_shaderBasicTexture_VertexColorIn;
			Renderer::s_shaderBasicTexture_TextureId 		= m_shaderBasicTexture_TextureId;
		}
		  
		StencilShader::~StencilShader() { 
			 
		}
	}
}





