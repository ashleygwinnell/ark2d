// Common/OpenGL.h
// Date: 6 Oct 2014
#ifndef COMMON_OPENGL_INCLUDES_H_
#define COMMON_OPENGL_INCLUDES_H_

	#if defined(__AVM2__) || defined(ARK2D_FLASCC)
		#define ARK2D_RENDERER_OPENGL
		// OpenGL 1.* - OpenGL 2.*
		#include <GL/gl.h>
	 	#include <GL/glext.h>
	 	#include <GL/glu.h>
	#elif defined(ARK2D_EMSCRIPTEN_JS)
		#define ARK2D_RENDERER_OPENGL
	 	#define ARK2D_OPENGL_ES_2_0

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
		#include <GLES2/gl2platform.h>
		#include <EGL/egl.h>
	#elif defined(ARK2D_ANDROID)
		#define ARK2D_RENDERER_OPENGL
		#define ARK2D_OPENGL_ES_2_0

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
		#include <GLES2/gl2platform.h>
	#elif defined(ARK2D_IPHONE)
		#define ARK2D_RENDERER_OPENGL
		#define ARK2D_OPENGL_ES_2_0

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
		#include <OpenGLES/EAGLDrawable.h>
	#elif defined(ARK2D_WINDOWS_PHONE_8)
		#define ARK2D_RENDERER_DIRECTX

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <d3d11.h>
		#include <DirectXMath.h> 
	#elif defined(ARK2D_XBOXONE)
		#define ARK2D_RENDERER_DIRECTX

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <d3d11_x.h>
		#include <DirectXMath.h> 
	#elif defined(_WIN32) || defined(ARK2D_WINDOWS)
		#define ARK2D_OPENGL_3_2
		#define ARK2D_RENDERER_OPENGL
		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true
		//#define ARK2D_RENDERER_DIRECTX

		#if defined(ARK2D_RENDERER_OPENGL) && defined(ARK2D_OPENGL_3_2)
			
			#pragma comment(lib, "opengl32.lib")
			#include <GL/gl3w.h>

		#elif defined(ARK2D_RENDERER_DIRECTX)

			#pragma comment(lib, "d2d1.lib")
			#pragma comment(lib, "d3d11.lib")
			#pragma comment(lib, "dxgi.lib")
			#pragma comment(lib, "dwrite.lib")

			#include <dxgi.h>
			#include <d3dcommon.h>
			#include <d3d11_1.h>
			#include <d2d1.h>
			#include <d2d1_1.h>
			#include <d2d1effects_1.h>
			
			#include <dwrite.h>
			#include <dwrite_1.h>
			#include <wincodec.h>
			#include <DirectXColors.h>
			#include <DirectXMath.h>
			
			typedef unsigned char GLubyte;
			typedef unsigned int GLsizei;

		#endif

	#elif defined(ARK2D_MACINTOSH)
		#define ARK2D_RENDERER_OPENGL
		#define ARK2D_OPENGL_3_2

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <OpenGL/gl3.h>
		#include <OpenGL/gltypes.h>
		#include <OpenGL/gl3ext.h>
	#elif defined(__linux__) || defined(ARK2D_UBUNTU_LINUX)
		#define ARK2D_RENDERER_OPENGL
		#define ARK2D_OPENGL_3_2

		#define FBO_SUPPORT true
		#define SHADER_SUPPORT true
		#define NO_FIXED_FUNCTION_PIPELINE true

		#include <GL/gl3w.h>
		//#include <GL/glx.h>
		#include <GL/glu.h>
	#endif

	// Structs
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
	typedef void GLvoid;

	#if defined(ARK2D_RENDERER_OPENGL)

		typedef struct {
			float vertex[2];
			unsigned char color[4]; 
		} Renderer_InterleavingGeometryVertexData;

		typedef struct {
			float vertex[2];
			float texcoord[2];
			unsigned char color[4];
		} Renderer_InterleavingTextureVertexData;

		typedef struct {
			float vertex[2];
			float texcoord[2];
		} Renderer_InterleavingTextureVertexData_NoColor;

	#elif defined(ARK2D_RENDERER_DIRECTX)

		#include <string>
		#include <iostream>
		using namespace std;

		typedef struct {
			DirectX::XMFLOAT4 vertex;
			DirectX::XMFLOAT4 color; 
		} Renderer_DX_InterleavingGeometryVertexData;

		typedef struct {
			DirectX::XMFLOAT4 vertex;
			DirectX::XMFLOAT2 texcoord;
			DirectX::XMFLOAT4 color; 
		} Renderer_DX_InterleavingTextureVertexData;

		typedef struct {
			DirectX::XMFLOAT4X4 model;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
		} Renderer_DX_ModelViewProjectionMatrixBuffer; 

		#ifndef ARK2D_RENDERER_DIRECTX_BITS
			#define ARK2D_RENDERER_DIRECTX_BITS

			template<class Interface>
			inline void SafeRelease(
			    Interface **ppInterfaceToRelease
			    )
			{
			    if (*ppInterfaceToRelease != NULL)
			    {
			        (*ppInterfaceToRelease)->Release();

			        (*ppInterfaceToRelease) = NULL;
			    }
			}

			inline void DX_ThrowIfFailed(HRESULT hr)
		    {
		        if (FAILED(hr))
		        {
		            std::cerr << "DX THROW IF FAILED LOG..." << __FILE__ << " : " << __LINE__ << std::endl;
		            // Set a breakpoint on this line to catch Win32 API errors.
		            //throw Platform::Exception::CreateException(hr);
		        } 
		    }  
		    inline std::string DX_GetError(HRESULT hr)
		    {
		        if (hr == D3D11_ERROR_FILE_NOT_FOUND) {
		        	return "DX Error: D3D11_ERROR_FILE_NOT_FOUND.";
		        } else if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS) {
		        	return "DX Error: D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS.";
		        } else if (hr == D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS) {
		        	return "DX Error: D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS.";
		        } else if (hr == D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD) {
		        	return "DX Error: D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD.";
		        } else if (hr == DXGI_ERROR_INVALID_CALL) {
		        	return "DX Error: DXGI_ERROR_INVALID_CALL.";
		        //} else if (hr == D3DERR_INVALIDCALL) {
		        //	return "DX Error: D3DERR_INVALIDCALL.";
		        } else if (hr == DXGI_ERROR_WAS_STILL_DRAWING) {
		        	return "DX Error: DXGI_ERROR_WAS_STILL_DRAWING.";
		        //} else if (hr == D3DERR_WASSTILLDRAWING) {
		        //	return "DX Error: D3DERR_WASSTILLDRAWING.";
		        } else if (hr == E_FAIL) {
		        	return "DX Error: E_FAIL.";
		        } else if (hr == E_INVALIDARG) {
		        	return "DX Error: E_INVALIDARG.";
		        } else if (hr == E_OUTOFMEMORY) {
		        	return "DX Error: E_OUTOFMEMORY.";
		        } else if (hr == E_NOTIMPL) {
		        	return "DX Error: E_NOTIMPL.";
		        } else if (hr == S_FALSE) {
		        	return "DX Error: S_FALSE.";
		        }
		        return "DX Error: NO ERROR?";
		    }  

		   /* inline std::string DX_GetMatrixstring(XMMATRIX pm3) {
		  		string dxvalues = "{";
				dxvalues += Cast::toString<float>(XMVectorGetX(pm3.r[0])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetY(pm3.r[0])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetZ(pm3.r[0])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetW(pm3.r[0])); dxvalues += string(",");

				dxvalues += Cast::toString<float>(XMVectorGetX(pm3.r[1])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetY(pm3.r[1])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetZ(pm3.r[1])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetW(pm3.r[1])); dxvalues += string(",");

				dxvalues += Cast::toString<float>(XMVectorGetX(pm3.r[2])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetY(pm3.r[2])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetZ(pm3.r[2])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetW(pm3.r[2])); dxvalues += string(",");

				dxvalues += Cast::toString<float>(XMVectorGetX(pm3.r[3])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetY(pm3.r[3])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetZ(pm3.r[3])); dxvalues += string(",");
				dxvalues += Cast::toString<float>(XMVectorGetW(pm3.r[3]));
				dxvalues += string("}");
				return dxvalues;
			}*/ 

			

			#ifndef HINST_THISCOMPONENT
				EXTERN_C IMAGE_DOS_HEADER __ImageBase;
				#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
			#endif

		#endif

	#endif

	

#endif

