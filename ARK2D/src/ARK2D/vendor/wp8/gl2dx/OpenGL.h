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

#include "gl2dx.h"
#include <map>
#include <DirectXMath.h>
#include <stack>
#include <list>

#ifdef ARK2D_WINDOWS_PHONE_8
      #ifdef ARK2D_WINDOWS_DLL // abc.dll source code will define this macro before including this header
         #define ARK2D_API __declspec( dllexport )          
      #else
         #define ARK2D_API __declspec( dllimport )
      #endif 
   #endif 

namespace gl2dx
{ 
   class ARK2D_API VertexBuilder;

   // Constant buffer used by the OpenGLVS.hlsl Vertex shader.
   struct ARK2D_API ModelViewProjectionConstantBuffer
   {
      DirectX::XMMATRIX model;
      DirectX::XMMATRIX view;
      DirectX::XMMATRIX projection;
   };

   // The maximum number of vertices that can be rendered in a single batch
   const int MaxBatchVertices = 25000;

   // Manages the conversion of native OpenGL calls into their DirectX equivalents.
   class ARK2D_API OpenGL
   {
      public:
         static OpenGL* s_instance;
         static OpenGL* getInstance() { return s_instance; }
   public:
      // Ctor
      OpenGL(ID3D11Device* device, ID3D11DeviceContext* context);

      // Dtor
      ~OpenGL();

      // Must be called at the start of every frame
      void StartFrame();

      // Must be called at the end of every frame
      void EndFrame();

      // Handles glBindTexture
      void BindTexture(GLenum target, GLuint texture);

      // Handles glSetColor
      void SetColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);

      // Handles glSetColorPointer
      void SetColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

      // Handles glDisable
      void Disable(GLenum cap);

      // Handles glDisableClientState
      void DisableClientState(GLenum array);

      // Handles glDrawArrays
      void DrawArrays(GLenum mode, GLint first, GLsizei count);

      // Handles glDrawElements
      void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);

      // Handles glEnable
      void Enable(GLenum cap);

      // Handles glEnableClientState
      void EnableClientState(GLenum array);

      // Handles glGenTextures
      void GenTextures(GLsizei n, GLuint *textures);

      // Handles glTexImage2D
      void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

      // Handles glSetVertexPointer
      void SetVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

      // Handles glSetTexCoordPointer
      void SetTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

      // Handles glOrtho
      void Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

      // Handles glScissor
      void Scissor(GLint x, GLint y, GLsizei width, GLsizei height);

      // Handles glLoadMatrix
      void LoadMatrix(const GLfloat *m);

      // Handles glPushMatrix
      void PushMatrix();

      // Handles glPopMatrix
      void PopMatrix();

      // Handles glTranslate
      void Translate(GLfloat x, GLfloat y, GLfloat z);

      // Handles glScale
      void Scale(GLfloat x, GLfloat y, GLfloat z);

      // Handles glRotate
      void Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

      // Handles glSetLineWidth
      void SetLineWidth(GLfloat width);

   private:
      // Adds the list of vertices to the current batch of vertices to be rendered with the next call to DrawBatch
      void Draw(const std::vector<OpenGLVertex>& vertices);

      // Draws the current batch of vertices.
      void DrawBatch();

   private:
      // Updates the current scissor state.
      void UpdateScissorState(bool enabled);

   public:
      // The DX vertex buffer
      ID3D11Buffer* _vertexBuffer;

      // Indicates the first call to Draw in the frame.
      bool _firstDrawInFrame;

      // Orthogonal view projection coordinates
      float _orthoLeft;
      float _orthoRight;
      float _orthoBottom;
      float _orthoTop;

      // Texture state parameters
      GLuint _currentTextureId;
      bool _enableTexture2D;
      GLuint _batchCurrentTextureId;
      bool _batchEnableTexture2D;
      std::map<GLuint, ID3D11ShaderResourceView*> _textures;

      // Handles conversions between OpenGL color, vertex, and texture coordinate pointers into a single list
      // of OpenGLVertex objects.
      VertexBuilder* _vertexBuilder;

      // Indicates the async loading of the shaders is finished
      bool _loadingComplete;

      // The max number of vertices that can be rendered in a single DrawBatch call
      int _maxVertices;

      // References to DirectX objects
      ID3D11InputLayout* _inputLayout;
      ID3D11VertexShader* _vertexShader;
      ID3D11PixelShader* _pixelShaderTexture;
      ID3D11PixelShader* _pixelShaderColor;
      ID3D11Buffer* _constantBuffer;
      D3D11_RASTERIZER_DESC _rasterizerDesc;
      ID3D11Device* _device;
      ID3D11DeviceContext* _context;

      // Scissor state
      bool _enableScissoring;
      D3D11_RECT _scissorRect;

      // The current projection matrix
      DirectX::XMFLOAT4X4 _projectionMatrix;

      // The constant buffer, which is shared with the vertex shader
      ModelViewProjectionConstantBuffer _constantBufferData;

      // The current stack of model view matrices
      std::stack<DirectX::XMFLOAT4X4*> _matrices;

      // The current set of vertices to render with the next DrawBatch call
      OpenGLVertex _batchVertices[MaxBatchVertices];
      int _currentBatchVertex;
   };
}
