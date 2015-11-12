/*
 * AlphaMaskShader.h
 *
 *  Created on: 11 Nov 2015
 *      Author: Ashley
 */

#include "AlphaMaskShader.h"
#include "../Image.h"

namespace ARK { 
	namespace Graphics { 


		AlphaMaskGeometryShader::AlphaMaskGeometryShader():
			ARK::Graphics::Shader() {

		}
		void AlphaMaskGeometryShader::load() {
			addVertexShader("ark2d/shaders/alphamask/geometry-glsl150-vertex.txt");
			addFragmentShader("ark2d/shaders/alphamask/geometry-glsl150-fragment.txt");

			bindAttributeLocation(0, "ark_VertexPosition");
			bindAttributeLocation(1, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");

			link(); 
			linkDX();

            showAnyGlErrorAndExitMacro();

			RendererState::start(RendererState::SHADER, getId());

			_ModelViewMatrix = getUniformVariable("ark_ModelViewMatrix");
			_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			_VertexPositionIn = 0;
			_VertexColorIn = 1;

			RendererState::start(RendererState::GEOMETRY);
		}
		void AlphaMaskGeometryShader::startStatic(AlphaMaskGeometryShader* obj) { obj->startInternal(); }
		void AlphaMaskGeometryShader::stopStatic(AlphaMaskGeometryShader* obj) { obj->stopInternal(); }
		void AlphaMaskGeometryShader::start() {
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
		void AlphaMaskGeometryShader::stop() {
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
		void AlphaMaskGeometryShader::startInternal() {
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
		void AlphaMaskGeometryShader::stopInternal() {
			Renderer::s_shaderBasicGeometry 					= m_shaderBasicGeometry;
			Renderer::s_shaderBasicGeometry_ModelViewMatrix 	= m_shaderBasicGeometry_ModelViewMatrix;
			Renderer::s_shaderBasicGeometry_ProjectionMatrix 	= m_shaderBasicGeometry_ProjectionMatrix;
			Renderer::s_shaderBasicGeometry_VertexPosition		= m_shaderBasicGeometry_VertexPositionIn;
			Renderer::s_shaderBasicGeometry_VertexColorIn 		= m_shaderBasicGeometry_VertexColorIn;
			RendererState::start(RendererState::NONE);
		}
		AlphaMaskGeometryShader::~AlphaMaskGeometryShader() {

		}





		AlphaMaskTextureShader::AlphaMaskTextureShader():
			ARK::Graphics::Shader() {

		}
		void AlphaMaskTextureShader::load() {
			addVertexShader("ark2d/shaders/alphamask/texture-glsl150-vertex.txt");
			addFragmentShader("ark2d/shaders/alphamask/texture-glsl150-fragment.txt");

			bindAttributeLocation(0, "ark_TextureId");
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
			_VertexPositionIn = 1;
			_VertexTexCoordIn = 2;
			_VertexColorIn = 3;

			RendererState::start(RendererState::GEOMETRY);
		}
		void AlphaMaskTextureShader::startStatic(AlphaMaskTextureShader* obj) { obj->startInternal(); }
		void AlphaMaskTextureShader::stopStatic(AlphaMaskTextureShader* obj) { obj->stopInternal(); }
		void AlphaMaskTextureShader::start() {
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
		void AlphaMaskTextureShader::stop() {
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
		void AlphaMaskTextureShader::startInternal() {
			RendererState::start(RendererState::NONE);
			m_shaderBasicTexture 					= Renderer::s_shaderBasicTexture;
			m_shaderBasicTexture_ModelViewMatrix	= Renderer::s_shaderBasicTexture_ModelViewMatrix;
			m_shaderBasicTexture_ProjectionMatrix 	= Renderer::s_shaderBasicTexture_ProjectionMatrix;
			m_shaderBasicTexture_VertexPositionIn 	= Renderer::s_shaderBasicTexture_VertexPosition;
			m_shaderBasicTexture_VertexTexCoordIn 	= Renderer::s_shaderBasicTexture_VertexTexCoordIn;
			m_shaderBasicTexture_VertexColorIn 		= Renderer::s_shaderBasicTexture_VertexColorIn;
			m_shaderBasicTexture_TextureId 			= Renderer::s_shaderBasicTexture_TextureId;

			Renderer::s_shaderBasicTexture 					= this;
			Renderer::s_shaderBasicTexture_ModelViewMatrix 	= _ModelViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix = _ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition	= _VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn = _VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 	= _VertexColorIn;
			Renderer::s_shaderBasicTexture_TextureId 		= _TextureId;
		}
		void AlphaMaskTextureShader::stopInternal() {
			Renderer::s_shaderBasicTexture 					= m_shaderBasicTexture;
			Renderer::s_shaderBasicTexture_ModelViewMatrix 	= m_shaderBasicTexture_ModelViewMatrix;
			Renderer::s_shaderBasicTexture_ProjectionMatrix = m_shaderBasicTexture_ProjectionMatrix;
			Renderer::s_shaderBasicTexture_VertexPosition	= m_shaderBasicTexture_VertexPositionIn;
			Renderer::s_shaderBasicTexture_VertexTexCoordIn = m_shaderBasicTexture_VertexTexCoordIn;
			Renderer::s_shaderBasicTexture_VertexColorIn 	= m_shaderBasicTexture_VertexColorIn;
			Renderer::s_shaderBasicTexture_TextureId 		= m_shaderBasicTexture_TextureId;
			RendererState::start(RendererState::NONE);
		}
		AlphaMaskTextureShader::~AlphaMaskTextureShader() {

		}




		AlphaMaskShader::AlphaMaskShader() {
			geometry = new AlphaMaskGeometryShader();
			texture = new AlphaMaskTextureShader();
		}
		void AlphaMaskShader::load() {
			geometry->load();
			texture->load();
		}
		void AlphaMaskShader::start() {
			geometry->start();
			texture->start();
		}
		void AlphaMaskShader::stop() {
			geometry->stop();
			texture->stop();
		} 
		AlphaMaskShader::~AlphaMaskShader() {
			delete geometry;
			delete texture;
		}

	}
}