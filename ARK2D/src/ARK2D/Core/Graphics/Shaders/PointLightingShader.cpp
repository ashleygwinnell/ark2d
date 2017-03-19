/*
 * PointLightingShader3d.cpp
 *
 *  Created on: 12 Oct 2014
 *      Author: Ashley
 */

#include "PointLightingShader.h"

 #include "../ShaderStore.h"
#include "../Image.h"

namespace ARK {
    namespace Core {
        namespace Graphics {


            //
            // GEOMETRY
            //
            PointLightingGeometryShader::PointLightingGeometryShader():
                ARK::Core::Graphics::Shader() {

            }
            void PointLightingGeometryShader::load() {
                setName("point-lighting-geometry");

                bool err1 = false;
                bool err2 = false;
                if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
                    addVertexShader("ark2d/shaders/point-lighting/geometry/glsles100-vertex.txt");
                    addFragmentShader("ark2d/shaders/point-lighting/geometry/glsles100-fragment.txt");
                } else {
                    //addVertexShader("ark2d/shaders/point-lighting/geometry/glsl330-vertex.txt");
                    //bool err1 = hasError();
                    //addFragmentShader("ark2d/shaders/point-lighting/geometry/glsl330-fragment.txt");
                    //bool err2 = hasError();

                    //if (err1 || err2) {
                        err1 = false;
                        err2 = false;

                        addVertexShader("ark2d/shaders/point-lighting/geometry/glsl150-vertex.txt");
                        err1 = hasError();
                        addFragmentShader("ark2d/shaders/point-lighting/geometry/glsl150-fragment.txt");
                        err2 = hasError();

                        if (err1 || err2) {
                            addVertexShader("ark2d/shaders/point-lighting/geometry/glsl130-vertex.txt");
                            addFragmentShader("ark2d/shaders/point-lighting/geometry/glsl130-fragment.txt");
                        }
                    //}
                }

                ShaderStore::getInstance()->addShader(m_programId, this);

                bindAttributeLocation(0, "ark_VertexPositionIn");
                bindAttributeLocation(1, "ark_VertexNormalIn");
                bindAttributeLocation(2, "ark_VertexColorIn");
                bindFragmentDataLocation(0, "ark_FragColor");
                link();
                linkDX();

                RendererState::start(RendererState::SHADER, getId());

                ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
                ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
                ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");

                _LightColor = getUniformVariable("u_sunlight.vColor");
                _LightDirection = getUniformVariable("u_sunlight.vDirection");
                _LightAmbientIntensity = getUniformVariable("u_sunlight.fAmbientIntensity");
                _LightStrength = getUniformVariable("u_sunlight.fStrength");

                _PointLightColor = getUniformVariable("u_pointlight.vColor");
                _PointLightPosition = getUniformVariable("u_pointlight.vPosition");
                _PointLightAmbient = getUniformVariable("u_pointlight.fAmbient");
                _PointLightConstantAtt = getUniformVariable("u_pointlight.fConstantAtt");
                _PointLightLinearAtt = getUniformVariable("u_pointlight.fLinearAtt");
                _PointLightExpAtt = getUniformVariable("u_pointlight.fExpAtt");

                ark_VertexPositionIn = 0;
                ark_VertexNormalIn = 1;
                ark_VertexColorIn = 2;

                RendererState::start(RendererState::NONE);
            }

            void PointLightingGeometryShader::bind() {
                Shader::bind();

                #ifdef ARK2D_RENDERER_OPENGL
                    glUniform3f(_LightDirection, container->direction.x, container->direction.y, container->direction.z);

                    glUniform3f(_LightColor, container->directionalColor.x, container->directionalColor.y, container->directionalColor.z);
                    glUniform1f(_LightAmbientIntensity, container->directionalAmbientIntensity);
                    glUniform1f(_LightStrength, container->directionalStrength);

                    glUniform3f(_PointLightColor, container->pointColor.x, container->pointColor.y, container->pointColor.z);
                    glUniform3f(_PointLightPosition, container->pointPosition.x, container->pointPosition.y, container->pointPosition.z);
                    glUniform1f(_PointLightAmbient, container->pointAmbience); //0.2f); //0.15f);
                    glUniform1f(_PointLightConstantAtt, container->pointConstantAtt); //0.0f); //1f);//3f);
                    glUniform1f(_PointLightLinearAtt, container->pointLinearAtt); //0.5f); //0.007f);
                    glUniform1f(_PointLightExpAtt, container->pointExpAtt); //0.00008f);
                #endif
            }
            void PointLightingGeometryShader::unbind() {
                Shader::unbind();
            }
            PointLightingGeometryShader::~PointLightingGeometryShader() {

            }




            //
            // TEXTURE
            //
            PointLightingTextureShader::PointLightingTextureShader():
                ARK::Core::Graphics::Shader() {

            }
            void PointLightingTextureShader::load() {
                setName("point-lighting-texture");

                bool err1 = false;
                bool err2 = false;
                if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
                    addVertexShader("ark2d/shaders/point-lighting/texture/glsles100-vertex.txt");
                    addFragmentShader("ark2d/shaders/point-lighting/texture/glsles100-fragment.txt");
                } else {
                    //addVertexShader("ark2d/shaders/point-lighting/texture/glsl330-vertex.txt");
                    //bool err1 = hasError();
                    //addFragmentShader("ark2d/shaders/point-lighting/texture/glsl330-fragment.txt");
                    //bool err2 = hasError();

                    //if (err1 || err2) {
                        err1 = false;
                        err2 = false;

                        addVertexShader("ark2d/shaders/point-lighting/texture/glsl150-vertex.txt");
                        err1 = hasError();
                        addFragmentShader("ark2d/shaders/point-lighting/texture/glsl150-fragment.txt");
                        err2 = hasError();

                        if (err1 || err2) {
                            addVertexShader("ark2d/shaders/point-lighting/texture/glsl130-vertex.txt");
                            addFragmentShader("ark2d/shaders/point-lighting/texture/glsl130-fragment.txt");
                        }
                    //}
                }

                ShaderStore::getInstance()->addShader(m_programId, this);

                bindAttributeLocation(1, "ark_VertexPositionIn");
                bindAttributeLocation(2, "ark_VertexNormalIn");
                bindAttributeLocation(3, "ark_VertexTexCoordIn");
                bindAttributeLocation(4, "ark_VertexColorIn");
                bindFragmentDataLocation(0, "ark_FragColor");
                link();
                linkDX();

                RendererState::start(RendererState::SHADER, getId());

                ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
                ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
                ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");

                ark_TextureId = getUniformVariable("ark_TextureId");

                _LightColor = getUniformVariable("u_sunlight.vColor");
                _LightDirection = getUniformVariable("u_sunlight.vDirection");
                _LightAmbientIntensity = getUniformVariable("u_sunlight.fAmbientIntensity");
                _LightStrength = getUniformVariable("u_sunlight.fStrength");

                _PointLightColor = getUniformVariable("u_pointlight.vColor");
                _PointLightPosition = getUniformVariable("u_pointlight.vPosition");
                _PointLightAmbient = getUniformVariable("u_pointlight.fAmbient");
                _PointLightConstantAtt = getUniformVariable("u_pointlight.fConstantAtt");
                _PointLightLinearAtt = getUniformVariable("u_pointlight.fLinearAtt");
                _PointLightExpAtt = getUniformVariable("u_pointlight.fExpAtt");

                ark_VertexPositionIn = 1;
                ark_VertexNormalIn = 2;
                ark_VertexTexCoordIn = 3;
                ark_VertexColorIn = 4;

                RendererState::start(RendererState::NONE);
            }

            void PointLightingTextureShader::bind() {
                Shader::bind();

                #ifdef ARK2D_RENDERER_OPENGL
                    glUniform3f(_LightDirection, container->direction.x, container->direction.y, container->direction.z);

                    glUniform3f(_LightColor, container->directionalColor.x, container->directionalColor.y, container->directionalColor.z);
                    glUniform1f(_LightAmbientIntensity, container->directionalAmbientIntensity);
                    glUniform1f(_LightStrength, container->directionalStrength);

                    glUniform3f(_PointLightColor, container->pointColor.x, container->pointColor.y,container->pointColor.z);
                    glUniform3f(_PointLightPosition, container->pointPosition.x, container->pointPosition.y, container->pointPosition.z);
                    glUniform1f(_PointLightAmbient, container->pointAmbience); //0.2f); //0.15f);
                    glUniform1f(_PointLightConstantAtt, container->pointConstantAtt); //0.0f); //1f);//3f);
                    glUniform1f(_PointLightLinearAtt, container->pointLinearAtt); //0.5f); //0.007f);
                    glUniform1f(_PointLightExpAtt, container->pointExpAtt); //0.00008f);
                #endif
            }
            void PointLightingTextureShader::unbind() {
                Shader::unbind();
            }
            PointLightingTextureShader::~PointLightingTextureShader() {

            }




            //
            // CONTAINER
            //
            PointLightingShader::PointLightingShader():
                geometry(NULL),
                texture(NULL),
                // Directional lighting vars
                direction(0,0,1),
                directionalColor(1.0f, 1.0f, 1.0f),
                directionalAmbientIntensity(0.4f),
                directionalStrength(1.0f),
                // Point lighting vars
                pointPosition(0,0,0),
                pointColor(1.0f, 1.0f, 1.0f),
                pointAmbience(0.2f),
                pointConstantAtt(0.0f),
                pointLinearAtt(0.5f),
                pointExpAtt(0.00008f)
                {
                geometry = new PointLightingGeometryShader();
                geometry->container = this;

                texture = new PointLightingTextureShader();
                texture->container = this;
            }
            void PointLightingShader::load() {
                geometry->load();
                texture->load();
            }
            PointLightingShader::~PointLightingShader() {
                delete geometry;
                delete texture;
            }

        }

	}
}




