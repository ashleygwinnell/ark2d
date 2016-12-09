/*
 * ShockwaveShader.h
 *
 *  Created on: 11 Nov 2015
 *      Author: Ashley
 */

#include "ShockwaveShader.h"
#include "../Image.h"
#include "../ShaderStore.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            ShockwaveTextureShader::ShockwaveTextureShader(ShockwaveShader* ss):
                ARK::Core::Graphics::Shader(),
                _container(ss) {

            }
            void ShockwaveTextureShader::load() {
                if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID || ARK2D::getPlatform() == ARK2D::PLATFORM_IOS ) {
                    addVertexShader("ark2d/shaders/shockwave/texture-glsles100-vertex.txt");
                    addFragmentShader("ark2d/shaders/shockwave/texture-glsles100-fragment.txt");
                } else {
                    addVertexShader("ark2d/shaders/shockwave/texture-glsl150-vertex.txt");
                    bool err1 = hasError();
                    addFragmentShader("ark2d/shaders/shockwave/texture-glsl150-fragment.txt");
                    bool err2 = hasError();

                    if (err1 || err2) {
                       // addVertexShader("ark2d/shaders/shockwave/texture-glsl130-vertex.txt");
                       // addFragmentShader("ark2d/shaders/shockwave/texture-glsl130-fragment.txt");
                        ErrorDialog::createAndShow("Could not create shockwave shader.");
                    }
                }

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

                _center = getUniformVariable("center");
                _time = getUniformVariable("time");
                _shockParams = getUniformVariable("shockParams");

                RendererState::start(RendererState::NONE);
            }
            void ShockwaveTextureShader::startStatic(ShockwaveTextureShader* obj) { obj->startInternal(); }
            void ShockwaveTextureShader::stopStatic(ShockwaveTextureShader* obj) { obj->stopInternal(); }
            void ShockwaveTextureShader::start() {
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
            void ShockwaveTextureShader::stop() {
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
            void ShockwaveTextureShader::startInternal() {
                ARK2D::getRenderer()->overrideBasicShaders(NULL, this);
            }
            void ShockwaveTextureShader::stopInternal() {
                ARK2D::getRenderer()->resetBasicShaders();
            }
            void ShockwaveTextureShader::bind() {
                Shader::bind();

                setUniformVariable2F( _center, _container->center.getX(), _container->center.getY() );
                setUniformVariable3F( _shockParams, _container->shockparams.getX(), _container->shockparams.getY(), _container->shockparams.getZ() );
                setUniformVariableF( _time, _container->timer );
            }

            ShockwaveTextureShader::~ShockwaveTextureShader() {

            }




            ShockwaveShader::ShockwaveShader():
                texture(NULL),
                center(0.0f, 0.0f),
                shockparams(10.0f, 0.25f, 0.1f),
                timer(0.0f)
                {
                texture = new ShockwaveTextureShader(this);
            }
            void ShockwaveShader::load() {
                texture->load();
            }
            void ShockwaveShader::start() {
                texture->start();
            }
            void ShockwaveShader::stop() {
                texture->stop();
            }
            ShockwaveShader::~ShockwaveShader() {
                delete texture;
            }

        }
    }
}
