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
#include "OpenGL.h"
#include "VertexBuilder.h"

#include <d3d11_1.h>

#include <wrl/client.h>
#include <memory>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <memory>
#include <agile.h>

#include "AsyncHelper.h"

namespace gl2dx
{
   OpenGL* OpenGL::s_instance = NULL;

   OpenGL::OpenGL(ID3D11Device* device, ID3D11DeviceContext* context)
   {
      s_instance = this;
      _orthoLeft = 0;
      _orthoRight = 0;
      _orthoBottom = 0;
      _orthoTop = 0;

      _device = device;
      _context = context;
      _vertexBuffer = NULL;
      _currentTextureId = 0;
      _enableTexture2D = false;
      _batchCurrentTextureId = 0;
      _batchEnableTexture2D = false;
      _loadingComplete = false;
      _vertexBuilder = new VertexBuilder();
      _maxVertices = MaxBatchVertices;
      _firstDrawInFrame = false;

      _currentBatchVertex = 0;

      _enableScissoring = false;
      _scissorRect.left = 0;
      _scissorRect.right = 0;
      _scissorRect.top = 0;
      _scissorRect.bottom = 0;

      _vertexShader = NULL;
      _pixelShaderTexture = NULL;
      _pixelShaderColor = NULL;

      auto vsize = sizeof (this->_batchVertices);

      // Asynchronously load the vertex and pixel shaders
      auto loadVSTask = ReadDataAsync("OpenGLVS.cso");
      auto loadPSColorTask = ReadDataAsync("OpenGLColorPS.cso");
      auto loadPSTextureTask = ReadDataAsync("OpenGLTexturePS.cso");

      auto createVSTask = loadVSTask.then([this](ByteArray ba) {
         auto bytecodeVS = ba.data;
         ThrowIfFailed(
            _device->CreateVertexShader(
            bytecodeVS->Data,
            bytecodeVS->Length,
            nullptr,
            &_vertexShader
            )
            );

         const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
         {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
         }; 

         ThrowIfFailed(
            _device->CreateInputLayout(
            vertexDesc,
            ARRAYSIZE(vertexDesc),
            bytecodeVS->Data,
            bytecodeVS->Length,
            &_inputLayout
            )
            );
      });

      auto createPSColorTask = loadPSColorTask.then([this](ByteArray ba) {
         auto bytecodePS = ba.data;
         ThrowIfFailed(
            _device->CreatePixelShader(
            bytecodePS->Data,
            bytecodePS->Length,
            nullptr,
            &_pixelShaderColor
            )
            );
      });

      auto createPSTextureTask = loadPSTextureTask.then([this](ByteArray ba) {
         auto bytecodePS = ba.data;
         ThrowIfFailed(
            _device->CreatePixelShader(
            bytecodePS->Data,
            bytecodePS->Length,
            nullptr,
            &_pixelShaderTexture
            )
            );

         ThrowIfFailed(
            _device->CreateBuffer(
            &CD3D11_BUFFER_DESC(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER),
            nullptr,
            &_constantBuffer
            ));
      });

      createPSTextureTask.then([this] () {
         _loadingComplete = true;
      });

      ZeroMemory(&_rasterizerDesc, sizeof(_rasterizerDesc));
      _rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
      _rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
      _rasterizerDesc.FrontCounterClockwise = true;
      _rasterizerDesc.DepthBias = 0;
      _rasterizerDesc.SlopeScaledDepthBias = 0.f;
      _rasterizerDesc.DepthBiasClamp = 0.f;
      _rasterizerDesc.DepthClipEnable = true;
      _rasterizerDesc.ScissorEnable = false;
      _rasterizerDesc.MultisampleEnable = false;
      _rasterizerDesc.AntialiasedLineEnable = false;
      ID3D11RasterizerState* rasterizerState;
      _device->CreateRasterizerState(&_rasterizerDesc, &rasterizerState);
      _context->RSSetState(rasterizerState);

      // Push an identity matrix on the stack to start with
      DirectX::XMFLOAT4X4* matrix = new DirectX::XMFLOAT4X4();
      auto identity = DirectX::XMMatrixIdentity();
      DirectX::XMStoreFloat4x4(matrix, identity);
      _matrices.push(matrix);
   }

   OpenGL::~OpenGL()
   {
      delete _vertexBuilder;
   }

   void OpenGL::StartFrame()
   {
      _firstDrawInFrame = true;
   }

   void OpenGL::EndFrame()
   {
      // Ensure that everything has been drawn before ending the frame.
      DrawBatch();
   }

   void OpenGL::BindTexture(GLenum target, GLuint texture)
   {
      // TODO: Ensure that the texture ID exists
      _currentTextureId = texture;
   }

   void OpenGL::SetColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
   {
      _vertexBuilder->SetColor(red, green, blue, alpha);
   }

   void OpenGL::SetColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
   {
      _vertexBuilder->SetColorPointer(size, type, stride, pointer);
   }

   void OpenGL::Disable(GLenum cap)
   {
      if (cap == GL_TEXTURE_2D)
      {
         _enableTexture2D = false;
      }
      else if (cap == GL_SCISSOR_TEST)
      {
         UpdateScissorState(false);
      }
      else
      {
         // TODO: 
         // GL_BLEND
         // GL_COLOR_MATERIAL
      }
   }

   void OpenGL::DisableClientState(GLenum array)
   {
      if (array == GL_COLOR_ARRAY)
      {
         _vertexBuilder->EnableColorPointer(false);
      }
      else if (array == GL_TEXTURE_COORD_ARRAY)
      {
         _vertexBuilder->EnableTexCoordPointer(false);
      }
   }

   void OpenGL::DrawArrays(GLenum mode, GLint first, GLsizei count)
   {
      if (_loadingComplete == false)
      {
         return;
      }

      // Create the vertex buffer
      auto vertices = _vertexBuilder->GetOpenGLVertices(mode, first, count);
      Draw(vertices);
   }

   void OpenGL::DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
   {
      if (_loadingComplete == false)
      {
         return;
      }

      std::vector<int> indicesVector;
      const GLubyte* indicesCharPtr = (const GLubyte*)indices;
      for (int i = 0; i < count; i++)
      {
         indicesVector.push_back((int)indicesCharPtr[i]);
      }

      auto vertices = _vertexBuilder->GetVerticesFromIndices(indicesVector);
      Draw(vertices);
   }

   void OpenGL::Draw(const std::vector<OpenGLVertex>& vertices)
   {
      if (_loadingComplete == false)
      {
         return;
      }

      if (_vertexBuffer == NULL)
      {
         D3D11_BUFFER_DESC bd;
         ZeroMemory(&bd, sizeof(bd));
         bd.Usage = D3D11_USAGE_DYNAMIC;
         bd.ByteWidth = _maxVertices * sizeof(OpenGLVertex);
         bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
         bd.MiscFlags = 0;
         bd.StructureByteStride = 0;
         D3D11_SUBRESOURCE_DATA initData;
         ZeroMemory(&initData, sizeof(initData));
         auto hr = _device->CreateBuffer(&bd, NULL, &_vertexBuffer);
      }

      if (_firstDrawInFrame)
      {
         _context->IASetInputLayout(_inputLayout);
         _context->VSSetShader(_vertexShader, nullptr, 0);
         _context->VSSetConstantBuffers(0, 1, &_constantBuffer);

         _firstDrawInFrame = false;
         _context->UpdateSubresource(_constantBuffer, 0, NULL, &_constantBufferData, 0, 0);
         UINT stride = sizeof(OpenGLVertex);
         UINT offset = 0;
         _context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
         _context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      }

      // Check if a texture state has changed
      if (_currentTextureId != _batchCurrentTextureId ||
         _enableTexture2D != _batchEnableTexture2D)
      {
         DrawBatch();
         _batchCurrentTextureId = _currentTextureId;
         _batchEnableTexture2D = _enableTexture2D;
      }

      auto matrix4x4 = _matrices.top();
      auto viewMatrix = DirectX::XMLoadFloat4x4(matrix4x4);
      int count = vertices.size();
      for (int i = 0; i < count; i++)
      {
         OpenGLVertex vertex = vertices[i];

         DirectX::XMVECTOR v = DirectX::XMVectorSet(vertex.x, vertex.y, vertex.z, 1.f);
         DirectX::XMVECTOR tv = DirectX::XMVector4Transform(v, viewMatrix);

         vertex.x = DirectX::XMVectorGetX( tv );
         vertex.y = DirectX::XMVectorGetY( tv );
         vertex.z = DirectX::XMVectorGetZ( tv );

         _batchVertices[_currentBatchVertex++] = vertex;
      }
   }

   void OpenGL::DrawBatch()
   {
      if (_currentBatchVertex > 0)
      {
         gl2dxAssert(_currentBatchVertex < _maxVertices, "Exceeded max vertices");

         if (_batchEnableTexture2D && _batchCurrentTextureId > 0)
         {
            ID3D11ShaderResourceView* texture = _textures[_batchCurrentTextureId];

            // Set shader texture resource in the pixel shader.
            _context->PSSetShader(_pixelShaderTexture, nullptr, 0);
            _context->PSSetShaderResources(0, 1, &texture);
         }
         else
         {
            _context->PSSetShader(_pixelShaderColor, nullptr, 0);
         }

         D3D11_MAPPED_SUBRESOURCE mappedResource;
         D3D11_MAP mapType = D3D11_MAP::D3D11_MAP_WRITE_DISCARD;

         auto hr = _context->Map(_vertexBuffer, 0, mapType, 0, &mappedResource);
         if (SUCCEEDED(hr))
         {
            OpenGLVertex *pData = reinterpret_cast<OpenGLVertex *>(mappedResource.pData);
            memcpy(pData, _batchVertices, sizeof(OpenGLVertex) * _currentBatchVertex);
            _context->Unmap(_vertexBuffer, 0);
         }

         _context->Draw(_currentBatchVertex, 0);
         _currentBatchVertex = 0;
      }
   }

   void OpenGL::Enable(GLenum cap)
   {
      if (cap == GL_TEXTURE_2D)
      {
         _enableTexture2D = true;
      }
      else if (cap == GL_SCISSOR_TEST)
      {  
         UpdateScissorState(true);
      }
      else
      {
         // TODO: 
         // GL_BLEND
         // GL_COLOR_MATERIAL
      }
   }

   void OpenGL::EnableClientState(GLenum array)
   {
      if (array == GL_COLOR_ARRAY)
      {
         _vertexBuilder->EnableColorPointer(true);
      }
      else if (array == GL_TEXTURE_COORD_ARRAY)
      {
         _vertexBuilder->EnableTexCoordPointer(true);
      }
   }

   void OpenGL::GenTextures(GLsizei n, GLuint *textures)
   {
      static GLuint textureId = 1;
      for (GLsizei i = 0; i < n; i++)
      {
         textures[i] = textureId++;
      }
   }

   void OpenGL::TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
   {
      D3D11_TEXTURE2D_DESC desc;
      ZeroMemory(&desc, sizeof(desc));
      desc.Width = width;
      desc.Height = height;
      desc.MipLevels = 1;
      desc.ArraySize = 1;
      desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      desc.SampleDesc.Count = 1;
      desc.SampleDesc.Quality = 0;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      desc.CPUAccessFlags = 0;
      desc.MiscFlags = 0;

      D3D11_SUBRESOURCE_DATA initialData;
      ZeroMemory(&initialData, sizeof(initialData));
      initialData.pSysMem = pixels;
      initialData.SysMemPitch = width * 4;

      ID3D11Texture2D* texture2D;
      HRESULT hr;
      hr = _device->CreateTexture2D(&desc, &initialData, &texture2D);
      if (FAILED(hr))
      {
         throw new GL2DXException("glTexImage2D failed at CreateTexture2D.");
      }

      D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
      memset( &SRVDesc, 0, sizeof( SRVDesc ) );
      SRVDesc.Format = desc.Format;
      SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      SRVDesc.Texture2D.MipLevels = 1;

      ID3D11ShaderResourceView* textureView;
      hr = _device->CreateShaderResourceView( texture2D, &SRVDesc, &textureView);
      if ( FAILED(hr) )
      {
         throw new GL2DXException("glTexImage2D failed at CreateShaderResourceView.");
      }

      _textures[_currentTextureId] = textureView;
   }

   void OpenGL::SetVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
   {
      _vertexBuilder->SetVertexPointer(size, type, stride, pointer);
   }

   void OpenGL::SetTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
   {
      _vertexBuilder->SetTexCoordPointer(size, type, stride, pointer);
   }

   void OpenGL::Ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
   {
      _orthoLeft = (float)left;
      _orthoRight = (float)right;
      _orthoBottom = (float)bottom;
      _orthoTop = (float)top;

      auto projectionMatrix = DirectX::XMMatrixOrthographicOffCenterRH((float)left, (float)right, (float)bottom, (float)top, (float)zNear, (float)zFar);
      _constantBufferData.projection = DirectX::XMMatrixTranspose(projectionMatrix);
   }

   void OpenGL::Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
   {
      _scissorRect.left = x;
      _scissorRect.right = x + width;

      _scissorRect.top = (LONG)((_orthoTop - _orthoBottom) - (y + height));
      _scissorRect.bottom = _scissorRect.top + height;

      D3D11_RECT rects[1];
      rects[0] = _scissorRect;
      _context->RSSetScissorRects(1, rects);
   }

   void OpenGL::LoadMatrix(const GLfloat *m)
   {
      DirectX::XMFLOAT4X4 matrix(m);

      // Replace the top matrix with this one
      auto pMatrix = _matrices.top();
      (*pMatrix) = matrix;
   }

   void OpenGL::PushMatrix()
   {
      // TODO: Include different matrix modes
      DirectX::XMFLOAT4X4* matrix = new DirectX::XMFLOAT4X4(*_matrices.top());

      // Push the new matrix on the stack
      _matrices.push(matrix);
   }

   void OpenGL::PopMatrix()
   {
      gl2dxAssert(_matrices.size() > 0, "Matrix stack is empty in PopMatrix");
      auto matrix = _matrices.top();
      _matrices.pop();
      delete matrix;
   }

   void OpenGL::Translate(GLfloat x, GLfloat y, GLfloat z)
   {
      auto matrix4x4 = _matrices.top();
      DirectX::XMMATRIX matrix = DirectX::XMLoadFloat4x4(matrix4x4);
      DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation((float)x, (float)y, (float)z);
      auto newMatrix = DirectX::XMMatrixMultiply(translation, matrix);
      DirectX::XMStoreFloat4x4(matrix4x4, newMatrix);
   }

   void OpenGL::Scale(GLfloat x, GLfloat y, GLfloat z)
   {
      auto matrix4x4 = _matrices.top();
      DirectX::XMMATRIX matrix = DirectX::XMLoadFloat4x4(matrix4x4);
      DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling((float)x, (float)y, (float)z);
      auto newMatrix = DirectX::XMMatrixMultiply(scaling, matrix);
      DirectX::XMStoreFloat4x4(matrix4x4, newMatrix);
   }

   void OpenGL::Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
   {
      auto matrix4x4 = _matrices.top();
      DirectX::XMMATRIX matrix = DirectX::XMLoadFloat4x4(matrix4x4);
	  DirectX::XMVECTOR axis = DirectX::XMVectorSet(x, y, z, 0.0f);
      DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(axis, angle * 3.14159265359f / 180.f);
      auto newMatrix = DirectX::XMMatrixMultiply(rotation, matrix);
      DirectX::XMStoreFloat4x4(matrix4x4, newMatrix);
   }

   void OpenGL::SetLineWidth(GLfloat width)
   {
      _vertexBuilder->SetLineWidth(width);
   }

   void OpenGL::UpdateScissorState(bool enabled)
   {
      DrawBatch();
      _enableScissoring = enabled;
      _rasterizerDesc.ScissorEnable = _enableScissoring;
      ID3D11RasterizerState* rasterizerState;
      _device->CreateRasterizerState(&_rasterizerDesc, &rasterizerState);
      _context->RSSetState(rasterizerState);
   }
}
