 /*
 * GrainShader.h
 *
 *  Created on: 06 Oct 2017
 *      Author: Ashley
 */

#include "GrainShader.h"
#include "../Image.h"
#include "../ShaderStore.h"
#include "../../Util/SystemUtil.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            GrainTextureShader::GrainTextureShader(GrainShader* container):
                ARK::Core::Graphics::Shader(),
                _container(container) {
                    setName("GrainTextureShader");
            }
            void GrainTextureShader::load()
            {
                ARK2D::getLog()->v("Texture Shader Checking for errors before load...");
                showAnyGlErrorAndExitMacro();

                ARK2D::getLog()->v("Texture Shader Init");

                #if defined(ARK2D_RENDERER_DIRECTX)

                    // // TODO: replace with texture shader...
                    // #if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS_STORE)

                    //     file_get_contents_binary_result textureVertexShaderStruct = SystemUtil::file_get_contents_binary("ark2d/shaders/basic-texture/texture-dx11-vertex.cso");
                    //     file_get_contents_binary_result textureFragmentShaderStruct = SystemUtil::file_get_contents_binary("ark2d/shaders/basic-texture/texture-dx11-pixel.cso");

                    //     //String* textureVertexShaderStruct = Resource::get("ark2d/shaders/geometry-dx11-vertex.cso")->asString();
                    //     //String* textureFragmentShaderStruct = Resource::get("ark2d/shaders/geometry-dx11-pixel.cso")->asString();

                    //     const char* textureVertexShader = (const char*)textureVertexShaderStruct.data;
                    //     const char* textureFragmentShader = (const char*)textureFragmentShaderStruct.data;

                    //     unsigned int textureVertexShaderLen = textureVertexShaderStruct.len;
                    //     unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len;

                    // #else
                    //     // TODO switch out values here.
                    //     GameContainerARK2DResource textureVertexShaderStruct = GameContainerPlatform::getARK2DResourceWithLength(ARK2D_DX11_GEOM_VS, ARK2D_RESOURCE_TYPE_SHADER);
                    //     GameContainerARK2DResource textureFragmentShaderStruct = GameContainerPlatform::getARK2DResourceWithLength(ARK2D_DX11_GEOM_PS, ARK2D_RESOURCE_TYPE_SHADER);

                    //     const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
                    //     const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;

                    //     unsigned int textureVertexShaderLen = textureVertexShaderStruct.length;
                    //     unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.length;
                    // #endif

                    // addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                    // addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

                #elif defined(ARK2D_RENDERER_OPENGL)
                    // #if defined(ARK2D_OPENGL_ES_2_0)

                    //     #ifdef ARK2D_ANDROID
                    //         // TODO:
                    //         // ... use Resource::get() as these are always within the APK.

                    //         RawDataReturns* textureVertexShaderStruct = Resource::getRawData("assets/ark2d/shaders/basic-texture/texture-glsles100.vertex"); //->asString();
                    //         RawDataReturns* textureFragmentShaderStruct = Resource::getRawData("assets/ark2d/shaders/basic-texture/texture-glsles100.fragment"); //->asString();

                    //         char* newtextbuffer3 = (char*) realloc(textureVertexShaderStruct->data, textureVertexShaderStruct->size+1);
                    //         newtextbuffer3[textureVertexShaderStruct->size] = '\0';
                    //         textureVertexShaderStruct->data = newtextbuffer3;

                    //         char* newtextbuffer4 = (char*) realloc(textureFragmentShaderStruct->data, textureFragmentShaderStruct->size+1);
                    //         newtextbuffer4[textureFragmentShaderStruct->size] = '\0';
                    //         textureFragmentShaderStruct->data = newtextbuffer4;

                    //         const char* textureVertexShader = (const char*) textureVertexShaderStruct->data;
                    //         const char* textureFragmentShader = (const char*) textureFragmentShaderStruct->data;//->getc().c_str();

                    //         unsigned int textureVertexShaderLen = textureVertexShaderStruct->size; //strlen(textureVertexShader);
                    //         unsigned int textureFragmentShaderLen = textureFragmentShaderStruct->size; //strlen(textureFragmentShader);

                    //         addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                    //         addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

                    //         delete textureVertexShaderStruct;
                    //         delete textureFragmentShaderStruct;
                    //     #else

                    //         file_get_contents_text_result textureVertexShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsles100.vertex");
                    //         file_get_contents_text_result textureFragmentShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsles100.fragment");

                    //         const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
                    //         const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;

                    //         unsigned int textureVertexShaderLen = textureVertexShaderStruct.len; //strlen(textureVertexShader);
                    //         unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len; //strlen(textureFragmentShader);

                    //         addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                    //         addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

                    //     #endif



                    // #elif defined(ARK2D_OPENGL_3_2)
                        ARK2D::getLog()->v("Trying GLSL version 150.");
                        file_get_contents_text_result textureVertexShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/grain/texture-glsl150-vertex.txt");
                        file_get_contents_text_result textureFragmentShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/grain/texture-glsl150-fragment.txt");

                        const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
                        const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;

                        unsigned int textureVertexShaderLen = textureVertexShaderStruct.len; //strlen(textureVertexShader);
                        unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len; //strlen(textureFragmentShader);

                        bool glsl150_t_vertex_error = false;
                        bool glsl150_t_fragment_error = false;

                        addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                        glsl150_t_vertex_error = hasError();

                        addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);
                        glsl150_t_fragment_error = hasError();

                        if (glsl150_t_vertex_error || glsl150_t_fragment_error) {
                        	ErrorDialog::createAndShow("GLSL version 150 had a problem. ");
                        	ARK2D::getContainer()->close();
                            // ARK2D::getLog()->w("GLSL version 150 had a problem. Trying GLSL version 130.");
                            // file_get_contents_text_result textureVertexShaderStruct2 = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsl130-vertex.txt");
                            // file_get_contents_text_result textureFragmentShaderStruct2 = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsl130-fragment.txt");

                            // const char* textureVertexShader2 = (const char*) textureVertexShaderStruct2.data;
                            // const char* textureFragmentShader2 = (const char*) textureFragmentShaderStruct2.data;

                            // unsigned int textureVertexShaderLen2 = textureVertexShaderStruct2.len; //strlen(textureVertexShader);
                            // unsigned int textureFragmentShaderLen2 = textureFragmentShaderStruct2.len; //strlen(textureFragmentShader);

                            // addVertexShaderFromData((void*) textureVertexShader2, textureVertexShaderLen2);
                            // addFragmentShaderFromData((void*) textureFragmentShader2, textureFragmentShaderLen2);

                            // delete textureVertexShaderStruct2.data;
                            // delete textureFragmentShaderStruct2.data;
                        }

                   // #endif
                #endif

                showAnyGlErrorAndExitMacro();

                ARK2D::getLog()->v("Texture Shader: Bind Locations");
                bindAttributeLocation(0, "ark_TextureId");
                bindAttributeLocation(1, "ark_VertexPositionIn");
                bindAttributeLocation(2, "ark_VertexNormalIn");
                bindAttributeLocation(3, "ark_VertexTexCoordIn");
                bindAttributeLocation(4, "ark_VertexColorIn");
                bindFragmentDataLocation(0, "ark_FragColor");

                #ifdef ARK2D_RENDERER_DIRECTX
                    setName("texture-dx11");
                #endif

                link();
                linkDX();

                showAnyGlErrorAndExitMacro();

                ShaderStore::getInstance()->addShader(m_programId, this);

                showAnyGlErrorAndExitMacro();

                _timer = getUniformVariable("grain_Time");
                _resolution = getUniformVariable("grain_Resolution");
                _strength = getUniformVariable("grain_Strength");

                RendererState::start(RendererState::SHADER, getId());

                showAnyGlErrorAndExitMacro();

                ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
                ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
                ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");
                

                showAnyGlErrorAndExitMacro();

                ark_TextureId = getUniformVariable("ark_TextureId");

                showAnyGlErrorAndExitMacro();

                ark_VertexPositionIn = 1;//getAttributeVariable("ark_VertexPositionIn");
                ark_VertexNormalIn = 2;//getAttributeVariable("ark_VertexNormalIn");
                ark_VertexTexCoordIn = 3;//getAttributeVariable("ark_VertexTexCoordIn");
                ark_VertexColorIn = 4;//getAttributeVariable("ark_VertexColorIn");

                

                showAnyGlErrorAndExitMacro();
                
                ARK2D::getLog()->e( StringUtil::append( "ark_ModelMatrix ", 		ark_ModelMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_ViewMatrix ", 			ark_ViewMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_ProjectionMatrix ", 	ark_ProjectionMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_NormalMatrix ", 		ark_NormalMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_TextureId ", 			ark_TextureId ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexPositionIn ", 	ark_VertexPositionIn ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexNormalIn ", 		ark_VertexNormalIn ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexTexCoordIn ", 	ark_VertexTexCoordIn ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexColorIn ", 		ark_VertexColorIn ) );
                ARK2D::getLog()->e( StringUtil::append( "_timer ", 					_timer ) );
                ARK2D::getLog()->e( StringUtil::append( "_resolution ", 			_resolution ) );
                ARK2D::getLog()->e( StringUtil::append( "_strength ", 				_strength ) );

                showAnyGlErrorAndExitMacro();

                RendererState::start(RendererState::NONE);

                showAnyGlErrorAndExitMacro();

                // Free String* memory.
                #ifndef ARK2D_ANDROID
                    delete textureVertexShaderStruct.data;
                    delete textureFragmentShaderStruct.data;
                #endif
            }
            void GrainTextureShader::bind() {
                Shader::bind();

                setUniformVariableF( _timer, _container->timer );
                setUniformVariableF( _strength, _container->strength );
                setUniformVariable2F( _resolution, _container->resolution.getX(), _container->resolution.getY() );

                #if defined(ARK2D_OPENGL_ES_2_0)
                     enableVertexAttribArray(ark_VertexPositionIn);
                     enableVertexAttribArray(ark_VertexNormalIn);
                     enableVertexAttribArray(ark_VertexTexCoordIn);
                     enableVertexAttribArray(ark_VertexColorIn);
                     RendererStats::s_glCalls += 3;
                #endif
            }
            void GrainTextureShader::startStatic(GrainTextureShader* obj) { obj->startInternal(); }
            void GrainTextureShader::stopStatic(GrainTextureShader* obj) { obj->stopInternal(); }
            void GrainTextureShader::start() {
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
            void GrainTextureShader::stop() {
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
            void GrainTextureShader::startInternal() {
                ARK2D::getRenderer()->overrideBasicShaders(NULL, this);
            }
            void GrainTextureShader::stopInternal() {
                ARK2D::getRenderer()->resetBasicShaders();
            }

            GrainTextureShader::~GrainTextureShader() {

            }




            GrainShader::GrainShader() {
                texture = new GrainTextureShader(this);
                resolution.set(256.0f, 256.0f); // viewport resolution in pixels
                timer = 0.0f;
                strength = 0.1f;
            }
            void GrainShader::load() {
                texture->load();
            }
            void GrainShader::start() {
                texture->start();
            }
            void GrainShader::stop() {
                texture->stop();
            }
            GrainShader::~GrainShader() {
                delete texture;
            }

        }
    }
}
