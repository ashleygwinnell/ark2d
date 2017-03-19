/*
 * BasicShader.h
 *
 *  Created on: 1 Dec 2015
 *      Author: Ashley
 */

#include "BasicShader.h"
#include "../Image.h"
#include "../ShaderStore.h"

#include "../../Util/SystemUtil.h"

namespace ARK {
    namespace Core {
        namespace Graphics {


            BasicGeometryShader::BasicGeometryShader():
                ARK::Core::Graphics::Shader() {
                    setName("BasicGeometryShader");
            }
            void BasicGeometryShader::load() {
                ARK2D::getLog()->v("Geometry Shader Checking for errors before load...");
                showAnyGlErrorAndExitMacro();

                // Regular geometry shader
                ARK2D::getLog()->v("Geometry Shader: Init");

                #if defined(ARK2D_RENDERER_DIRECTX)

                    #if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS_STORE)

                        file_get_contents_binary_result geometryVertexShaderStruct = SystemUtil::file_get_contents_binary("ark2d/shaders/basic-geometry/geometry-dx11-vertex.cso");
                        file_get_contents_binary_result geometryFragmentShaderStruct = SystemUtil::file_get_contents_binary("ark2d/shaders/basic-geometry/geometry-dx11-pixel.cso");

                        //String* geometryVertexShaderStruct = Resource::get("ark2d/shaders/geometry-dx11-vertex.cso")->asString();
                        //String* geometryFragmentShaderStruct = Resource::get("ark2d/shaders/geometry-dx11-pixel.cso")->asString();

                        const char* geometryVertexShader = (const char*)geometryVertexShaderStruct.data;
                        const char* geometryFragmentShader = (const char*)geometryFragmentShaderStruct.data;

                        unsigned int geometryVertexShaderLen = geometryVertexShaderStruct.len;
                        unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct.len;

                    #else
                        // Compiled .cso files
                        GameContainerARK2DResource geometryVertexShaderStruct = GameContainerPlatform::getARK2DResourceWithLength(ARK2D_DX11_GEOM_VS, ARK2D_RESOURCE_TYPE_SHADER);
                        GameContainerARK2DResource geometryFragmentShaderStruct = GameContainerPlatform::getARK2DResourceWithLength(ARK2D_DX11_GEOM_PS, ARK2D_RESOURCE_TYPE_SHADER);

                        const char* geometryVertexShader = (const char*) geometryVertexShaderStruct.data;
                        const char* geometryFragmentShader = (const char*) geometryFragmentShaderStruct.data;

                        unsigned int geometryVertexShaderLen = geometryVertexShaderStruct.length;
                        unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct.length;
                    #endif

                    addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
                    addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen);

                    //ARK2D::getLog()->v(StringUtil::append("Vertex Resource Size (4): ", geometryVertexShaderLen));
                    //ARK2D::getLog()->v(StringUtil::append("Fragment Resource Size (4): ", geometryFragmentShaderLen));

                #elif defined(ARK2D_RENDERER_OPENGL)
                    #if defined(ARK2D_OPENGL_ES_2_0)

                        #ifdef ARK2D_ANDROID
                            // TODO:
                            // ... use Resource::get() as these are always within the APK.

                            RawDataReturns* geometryVertexShaderStruct = Resource::getRawData("assets/ark2d/shaders/basic-geometry/geometry-glsles100.vertex");
                            RawDataReturns* geometryFragmentShaderStruct = Resource::getRawData("assets/ark2d/shaders/basic-geometry/geometry-glsles100.fragment"); //->asString();

                            char* newtextbuffer = (char*) realloc(geometryVertexShaderStruct->data, geometryVertexShaderStruct->size+1);
                            newtextbuffer[geometryVertexShaderStruct->size] = '\0';
                            geometryVertexShaderStruct->data = newtextbuffer;

                            char* newtextbuffer2 = (char*) realloc(geometryFragmentShaderStruct->data, geometryFragmentShaderStruct->size+1);
                            newtextbuffer2[geometryFragmentShaderStruct->size] = '\0';
                            geometryFragmentShaderStruct->data = newtextbuffer2;

                            const char* geometryVertexShader = (const char*) geometryVertexShaderStruct->data;
                            const char* geometryFragmentShader = (const char*) geometryFragmentShaderStruct->data;

                            unsigned int geometryVertexShaderLen = geometryVertexShaderStruct->size; //strlen(textureVertexShader);
                            unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct->size; //strlen(textureFragmentShader);

                            addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
                            addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen);

                            delete geometryVertexShaderStruct;
                            delete geometryFragmentShaderStruct;

                        #else

                            file_get_contents_text_result geometryVertexShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-geometry/geometry-glsles100.vertex");
                            file_get_contents_text_result geometryFragmentShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-geometry/geometry-glsles100.fragment");

                            const char* geometryVertexShader = (const char*) geometryVertexShaderStruct.data;
                            const char* geometryFragmentShader = (const char*) geometryFragmentShaderStruct.data;

                            unsigned int geometryVertexShaderLen = geometryVertexShaderStruct.len; //strlen(geometryVertexShader);
                            unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct.len; //strlen(geometryFragmentShader);

                            addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
                            addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen);

                        #endif

                    #elif defined(ARK2D_OPENGL_3_2)
                        ARK2D::getLog()->v("Trying GLSL version 150.");
                        file_get_contents_text_result geometryVertexShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-geometry/geometry-glsl150-vertex.txt");
                        file_get_contents_text_result geometryFragmentShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-geometry/geometry-glsl150-fragment.txt");

                        const char* geometryVertexShader = (const char*) geometryVertexShaderStruct.data;
                        const char* geometryFragmentShader = (const char*) geometryFragmentShaderStruct.data;

                        unsigned int geometryVertexShaderLen = geometryVertexShaderStruct.len;
                        unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct.len;

                        bool glsl150_vertex_error = false;
                        bool glsl150_fragment_error = false;

                        addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
                        glsl150_vertex_error = hasError();

                        addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen);
                        glsl150_fragment_error = hasError();

                        if (glsl150_vertex_error || glsl150_fragment_error) {
                            ARK2D::getLog()->w("GLSL version 150 had a problem. Trying GLSL version 130.");
                            file_get_contents_text_result geometryVertexShaderStruct2 = SystemUtil::file_get_contents_text("ark2d/shaders/basic-geometry/geometry-glsl130-vertex.txt");
                            file_get_contents_text_result geometryFragmentShaderStruct2 = SystemUtil::file_get_contents_text("ark2d/shaders/basic-geometry/geometry-glsl130-fragment.txt");

                            const char* geometryVertexShader2 = (const char*) geometryVertexShaderStruct2.data;
                            const char* geometryFragmentShader2 = (const char*) geometryFragmentShaderStruct2.data;

                            unsigned int geometryVertexShaderLen2 = geometryVertexShaderStruct2.len;
                            unsigned int geometryFragmentShaderLen2 = geometryFragmentShaderStruct2.len;

                            addVertexShaderFromData((void*) geometryVertexShader2, geometryVertexShaderLen2);
                            addFragmentShaderFromData((void*) geometryFragmentShader2, geometryFragmentShaderLen2);

                            delete geometryVertexShaderStruct2.data;
                            delete geometryFragmentShaderStruct2.data;
                        }
                    #endif
                #endif


                #ifdef ARK2D_RENDERER_DIRECTX
                    setName("geometry-dx11");
                #endif

                ShaderStore::getInstance()->addShader(m_programId, this);

                ARK2D::getLog()->v("Basic Geometry Shader: Bind Locations");
                //bindAttributeLocation(0, "ark_VertexPositionIn");
                //bindAttributeLocation(1, "ark_VertexNormalIn");
                //bindAttributeLocation(2, "ark_VertexColorIn");
                bindFragmentDataLocation(0, "ark_FragColor");

                link();
                linkDX();

                RendererState::start(RendererState::SHADER, getId());

                ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
                ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
                ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");

                ark_VertexPositionIn = getAttributeVariable("ark_VertexPositionIn");
                ark_VertexNormalIn = getAttributeVariable("ark_VertexNormalIn");
                ark_VertexColorIn = getAttributeVariable("ark_VertexColorIn");



                RendererState::start(RendererState::NONE);

                // Free String* memory.
    //            #ifndef ARK2D_ANDROID
    //                delete geometryVertexShaderStruct.data;
    //                delete geometryFragmentShaderStruct.data;
    //            #endif
            }
            void BasicGeometryShader::bind() {
                Shader::bind();
                #if defined(ARK2D_OPENGL_ES_2_0)
                    enableVertexAttribArray(ark_VertexPositionIn);
                    enableVertexAttribArray(ark_VertexNormalIn);
                    enableVertexAttribArray(ark_VertexColorIn);
                    RendererStats::s_glCalls += 2;
                #endif
            }

            BasicGeometryShader::~BasicGeometryShader() {

            }





            BasicTextureShader::BasicTextureShader():
                ARK::Core::Graphics::Shader() {
                    setName("BasicTextureShader");
            }
            void BasicTextureShader::load()
            {
                ARK2D::getLog()->v("Texture Shader Checking for errors before load...");
                showAnyGlErrorAndExitMacro();

                ARK2D::getLog()->v("Texture Shader Init");

                #if defined(ARK2D_RENDERER_DIRECTX)

                    // TODO: replace with texture shader...
                    #if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS_STORE)

                        file_get_contents_binary_result textureVertexShaderStruct = SystemUtil::file_get_contents_binary("ark2d/shaders/basic-texture/texture-dx11-vertex.cso");
                        file_get_contents_binary_result textureFragmentShaderStruct = SystemUtil::file_get_contents_binary("ark2d/shaders/basic-texture/texture-dx11-pixel.cso");

                        //String* textureVertexShaderStruct = Resource::get("ark2d/shaders/geometry-dx11-vertex.cso")->asString();
                        //String* textureFragmentShaderStruct = Resource::get("ark2d/shaders/geometry-dx11-pixel.cso")->asString();

                        const char* textureVertexShader = (const char*)textureVertexShaderStruct.data;
                        const char* textureFragmentShader = (const char*)textureFragmentShaderStruct.data;

                        unsigned int textureVertexShaderLen = textureVertexShaderStruct.len;
                        unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len;

                    #else
                        // TODO switch out values here.
                        GameContainerARK2DResource textureVertexShaderStruct = GameContainerPlatform::getARK2DResourceWithLength(ARK2D_DX11_GEOM_VS, ARK2D_RESOURCE_TYPE_SHADER);
                        GameContainerARK2DResource textureFragmentShaderStruct = GameContainerPlatform::getARK2DResourceWithLength(ARK2D_DX11_GEOM_PS, ARK2D_RESOURCE_TYPE_SHADER);

                        const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
                        const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;

                        unsigned int textureVertexShaderLen = textureVertexShaderStruct.length;
                        unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.length;
                    #endif

                    addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                    addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

                #elif defined(ARK2D_RENDERER_OPENGL)
                    #if defined(ARK2D_OPENGL_ES_2_0)

                        #ifdef ARK2D_ANDROID
                            // TODO:
                            // ... use Resource::get() as these are always within the APK.

                            RawDataReturns* textureVertexShaderStruct = Resource::getRawData("assets/ark2d/shaders/basic-texture/texture-glsles100.vertex"); //->asString();
                            RawDataReturns* textureFragmentShaderStruct = Resource::getRawData("assets/ark2d/shaders/basic-texture/texture-glsles100.fragment"); //->asString();

                            char* newtextbuffer3 = (char*) realloc(textureVertexShaderStruct->data, textureVertexShaderStruct->size+1);
                            newtextbuffer3[textureVertexShaderStruct->size] = '\0';
                            textureVertexShaderStruct->data = newtextbuffer3;

                            char* newtextbuffer4 = (char*) realloc(textureFragmentShaderStruct->data, textureFragmentShaderStruct->size+1);
                            newtextbuffer4[textureFragmentShaderStruct->size] = '\0';
                            textureFragmentShaderStruct->data = newtextbuffer4;

                            const char* textureVertexShader = (const char*) textureVertexShaderStruct->data;
                            const char* textureFragmentShader = (const char*) textureFragmentShaderStruct->data;//->getc().c_str();

                            unsigned int textureVertexShaderLen = textureVertexShaderStruct->size; //strlen(textureVertexShader);
                            unsigned int textureFragmentShaderLen = textureFragmentShaderStruct->size; //strlen(textureFragmentShader);

                            addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                            addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

                            delete textureVertexShaderStruct;
                            delete textureFragmentShaderStruct;
                        #else

                            file_get_contents_text_result textureVertexShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsles100.vertex");
                            file_get_contents_text_result textureFragmentShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsles100.fragment");

                            const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
                            const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;

                            unsigned int textureVertexShaderLen = textureVertexShaderStruct.len; //strlen(textureVertexShader);
                            unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len; //strlen(textureFragmentShader);

                            addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
                            addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

                        #endif



                    #elif defined(ARK2D_OPENGL_3_2)
                        ARK2D::getLog()->v("Trying GLSL version 150.");
                        file_get_contents_text_result textureVertexShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsl150-vertex.txt");
                        file_get_contents_text_result textureFragmentShaderStruct = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsl150-fragment.txt");

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
                            ARK2D::getLog()->w("GLSL version 150 had a problem. Trying GLSL version 130.");
                            file_get_contents_text_result textureVertexShaderStruct2 = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsl130-vertex.txt");
                            file_get_contents_text_result textureFragmentShaderStruct2 = SystemUtil::file_get_contents_text("ark2d/shaders/basic-texture/texture-glsl130-fragment.txt");

                            const char* textureVertexShader2 = (const char*) textureVertexShaderStruct2.data;
                            const char* textureFragmentShader2 = (const char*) textureFragmentShaderStruct2.data;

                            unsigned int textureVertexShaderLen2 = textureVertexShaderStruct2.len; //strlen(textureVertexShader);
                            unsigned int textureFragmentShaderLen2 = textureFragmentShaderStruct2.len; //strlen(textureFragmentShader);

                            addVertexShaderFromData((void*) textureVertexShader2, textureVertexShaderLen2);
                            addFragmentShaderFromData((void*) textureFragmentShader2, textureFragmentShaderLen2);

                            delete textureVertexShaderStruct2.data;
                            delete textureFragmentShaderStruct2.data;
                        }

                    #endif
                #endif

                showAnyGlErrorAndExitMacro();

                ShaderStore::getInstance()->addShader(m_programId, this);

                ARK2D::getLog()->v("Texture Shader: Bind Locations");
                //bindAttributeLocation(0, "ark_TextureId");
                //bindAttributeLocation(1, "ark_VertexPositionIn");
                //bindAttributeLocation(2, "ark_VertexNormalIn");
                //bindAttributeLocation(3, "ark_VertexTexCoordIn");
                //bindAttributeLocation(4, "ark_VertexColorIn");
                bindFragmentDataLocation(0, "ark_FragColor");

                #ifdef ARK2D_RENDERER_DIRECTX
                    setName("texture-dx11");
                #endif

                link();
                linkDX();

                showAnyGlErrorAndExitMacro();

                RendererState::start(RendererState::SHADER, getId());

                showAnyGlErrorAndExitMacro();

                ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
                ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
                ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
                ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");

                ark_TextureId = getUniformVariable("ark_TextureId");

                ark_VertexPositionIn = getAttributeVariable("ark_VertexPositionIn");
                ark_VertexNormalIn = getAttributeVariable("ark_VertexNormalIn");
                ark_VertexTexCoordIn = getAttributeVariable("ark_VertexTexCoordIn");
                ark_VertexColorIn = getAttributeVariable("ark_VertexColorIn");

                ARK2D::getLog()->e( StringUtil::append( "ark_ModelMatrix ", 		ark_ModelMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_ViewMatrix ", 			ark_ViewMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_ProjectionMatrix ", 	ark_ProjectionMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_NormalMatrix ", 		ark_NormalMatrix ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_TextureId ", 			ark_TextureId ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexPositionIn ", 	ark_VertexPositionIn ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexNormalIn ", 		ark_VertexNormalIn ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexTexCoordIn ", 	ark_VertexTexCoordIn ) );
                ARK2D::getLog()->e( StringUtil::append( "ark_VertexColorIn ", 		ark_VertexColorIn ) );

                showAnyGlErrorAndExitMacro();

                RendererState::start(RendererState::NONE);

                showAnyGlErrorAndExitMacro();

                // Free String* memory.
                #ifndef ARK2D_ANDROID
                    delete textureVertexShaderStruct.data;
                    delete textureFragmentShaderStruct.data;
                #endif
            }
            void BasicTextureShader::bind() {
                Shader::bind();
                #if defined(ARK2D_OPENGL_ES_2_0)
                    enableVertexAttribArray(ark_VertexPositionIn);
                    enableVertexAttribArray(ark_VertexNormalIn);
                    enableVertexAttribArray(ark_VertexTexCoordIn);
                    enableVertexAttribArray(ark_VertexColorIn);
                    RendererStats::s_glCalls += 3;
                #endif
            }

            BasicTextureShader::~BasicTextureShader() {

            }




            BasicShader::BasicShader() {
                geometry = new BasicGeometryShader();
                texture = new BasicTextureShader();
            }
            void BasicShader::load() {
                geometry->load();
                texture->load();
            }
            BasicShader::~BasicShader() {
                delete geometry;
                delete texture;
            }

        }
	}
}
