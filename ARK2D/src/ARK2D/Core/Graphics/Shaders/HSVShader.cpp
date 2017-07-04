/*
 * HSVShader.h
 *
 *  Created on: 4 May 2015
 *      Author: Ashley
 */

#include "HSVShader.h"
#include "../Image.h"
#include "../ShaderStore.h"

namespace ARK {
    namespace Core {
        namespace Graphics {


            HSVGeometryShader::HSVGeometryShader(): 
                ARK::Core::Graphics::Shader(),
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

                bindAttributeLocation(0, "ark_VertexPositionIn");
                bindAttributeLocation(1, "ark_VertexNormalIn");
                bindAttributeLocation(2, "ark_VertexColorIn");
                bindFragmentDataLocation(0, "ark_FragColor");
                link(); 
                linkDX(); 

                showAnyGlErrorAndExitMacro();
                ShaderStore::getInstance()->addShader(m_programId, this);

                RendererState::start(RendererState::SHADER, getId());

                _ModelMatrix = getUniformVariable("ark_ModelMatrix");
                _ViewMatrix = getUniformVariable("ark_ViewMatrix");
                _ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                _NormalMatrix = getUniformVariable("ark_NormalMatrix");
                _HSVIn = getUniformVariable("ark_HSV");

                _VertexPositionIn = 0;
                _VertexNormalIn = 1;
                _VertexColorIn = 2;
                
                
                RendererState::start(RendererState::NONE);
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
                m_shaderBasicGeometry = Renderer::s_shaderBasicGeometry;
                Renderer::s_shaderBasicGeometry = this;
            }
            void HSVGeometryShader::stop() {
                Renderer::s_shaderBasicGeometry = m_shaderBasicGeometry;
            }
              
            HSVGeometryShader::~HSVGeometryShader() { 
                 
            }






            


            HSVTextureShader::HSVTextureShader(): 
                ARK::Core::Graphics::Shader(),
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

                _ModelMatrix = getUniformVariable("ark_ModelMatrix");
                _ViewMatrix = getUniformVariable("ark_ViewMatrix");
                _ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                _NormalMatrix = getUniformVariable("ark_NormalMatrix");
                _TextureId = getUniformVariable("ark_TextureId");
                _HSVIn = getUniformVariable("ark_HSV");

                _VertexPositionIn = 1;
                _VertexNormalIn = 2;
                _VertexTexCoordIn = 3;
                _VertexColorIn = 4;
                
                RendererState::start(RendererState::NONE);
            }

            void HSVTextureShader::bind() {
                Shader::bind();
                setUniformVariable3F(_HSVIn, h, s, v);	
            } 


            void HSVTextureShader::unbind() {
                Shader::unbind();
            }

            void HSVTextureShader::start() {
                m_shaderBasicTexture = Renderer::s_shaderBasicTexture;
                Renderer::s_shaderBasicTexture = this;
            }
            void HSVTextureShader::stop() {
                Renderer::s_shaderBasicTexture = m_shaderBasicTexture;
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
                //geometry->start();
                //texture->start();
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
            void HSVShader::stop() {
               // geometry->stop();
               // texture->stop();
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

            void HSVShader::startStatic(HSVShader* obj) { obj->startInternal(); }
            void HSVShader::stopStatic(HSVShader* obj) { obj->stopInternal(); }

            void HSVShader::startInternal() {
                Renderer* r = ARK2D::getRenderer();
                r->overrideBasicShaders(geometry, texture);
            } 

            void HSVShader::stopInternal() {
                Renderer* r = ARK2D::getRenderer();
                r->resetBasicShaders();
            }

            HSVShader::~HSVShader() {
                delete geometry;
                delete texture;
            }
        }
	}
}
