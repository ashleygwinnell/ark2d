/*
 * AlphaMaskShader.h
 *
 *  Created on: 11 Nov 2015
 *      Author: Ashley
 */

#include "AlphaMaskShader.h"
#include "../../Includes.h"
#include "../Image.h"
#include "../ShaderStore.h"

namespace ARK { 
	namespace Graphics { 


		AlphaMaskGeometryShader::AlphaMaskGeometryShader():
			ARK::Graphics::Shader() {

		}
		void AlphaMaskGeometryShader::load() {
			addVertexShader("ark2d/shaders/alphamask/geometry-glsl150-vertex.txt");
			addFragmentShader("ark2d/shaders/alphamask/geometry-glsl150-fragment.txt");

			bindAttributeLocation(0, "ark_VertexPositionIn");
            bindAttributeLocation(1, "ark_VertexNormalIn");
			bindAttributeLocation(2, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");

			link(); 
			linkDX();

            showAnyGlErrorAndExitMacro();
            ShaderStore::getInstance()->addShader(m_programId, this);

			RendererState::start(RendererState::SHADER, getId());

			ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
			ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
            ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");
			ark_VertexPositionIn = 0;
            ark_VertexNormalIn = 1;
            ark_VertexColorIn = 2;

			RendererState::start(RendererState::NONE);
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
			ARK2D::getRenderer()->overrideBasicShaders(this, NULL);
        }
		void AlphaMaskGeometryShader::stopInternal() {
			ARK2D::getRenderer()->resetBasicShaders();
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
			bindAttributeLocation(1, "ark_VertexPositionIn");
            bindAttributeLocation(2, "ark_VertexNormalIn");
			bindAttributeLocation(3, "ark_VertexTexCoordIn");
			bindAttributeLocation(4, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			showAnyGlErrorAndExitMacro();
			ShaderStore::getInstance()->addShader(m_programId, this);

			RendererState::start(RendererState::SHADER, getId());

			ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
			ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
            ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");
			
			ark_TextureId = getUniformVariable("ark_TextureId");
			ark_VertexPositionIn = 1;
            ark_VertexNormalIn = 2;
			ark_VertexTexCoordIn = 3;
			ark_VertexColorIn = 4;

			RendererState::start(RendererState::NONE);
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
			ARK2D::getRenderer()->overrideBasicShaders(NULL, this);
		}
		void AlphaMaskTextureShader::stopInternal() {
			ARK2D::getRenderer()->resetBasicShaders();
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