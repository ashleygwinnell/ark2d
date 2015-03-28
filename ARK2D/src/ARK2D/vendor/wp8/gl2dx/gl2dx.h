//-----------------------------------------------------------------------------------------------
// Copyright (c) 2012 Andrew Garrison
//-----------------------------------------------------------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the "Software"), to deal in the Software without 
// restriction, including without limitation the rights to use, copy, modify, merge, publish, 
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//-----------------------------------------------------------------------------------------------
#pragma once

#include "gl.h"
#include <d3d11_1.h>
#include <string>
#include <vector>
#include "GL2DXException.h"
#include "OpenGLVertex.h"

	#ifdef ARK2D_WINDOWS_PHONE_8
		#ifdef ARK2D_WINDOWS_DLL // abc.dll source code will define this macro before including this header
			#define ARK2D_API __declspec( dllexport )				
		#else
			#define ARK2D_API __declspec( dllimport )
		#endif 
	#endif 

 
// Initialized Direct3D with the current device and context
ARK2D_API void gl2dxInit(ID3D11Device* device, ID3D11DeviceContext* context);

// Must be called at the start of every frame
ARK2D_API void gl2dxStartFrame();

// Must be called at the end of every frame
ARK2D_API void gl2dxEndFrame();

// Special assert method
ARK2D_API void gl2dxAssert(bool condition, char* message);

// Special assert method when something unsupported is encountered
ARK2D_API void gl2dxNotSupported(char* message);

