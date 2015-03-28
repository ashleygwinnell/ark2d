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
#include "VertexBuilder.h"

namespace gl2dx
{
   VertexBuilder::VertexBuilder()
   {
      // Setup defaults
      _colorPointerEnabled = false;
      _texCoordPointerEnabled = false;
      _lineWidth = 0.f;
   }

   void VertexBuilder::SetVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
   {
      // Currently only 2 and 3 are valid sizes and GL_FLOAT is the only supported pointer type.
      gl2dxAssert(size == 2 || size == 3, "Only 2 and 3 are valid for size");
      gl2dxAssert(type == GL_FLOAT, "GL_FLOAT is the only supported vertex pointer type");

      _vectorPointerSize = size;
      _vectorPointerType = type;
      _vectorPointerStride = stride;
      _vectorPointer = (GLbyte*)pointer;
      _vectorPointerMemberSize = sizeof(GLfloat);
   }

   void VertexBuilder::EnableTexCoordPointer(bool enable)
   {
      _texCoordPointerEnabled = enable;
   }

   void VertexBuilder::SetTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
   {
      _texCoordPointerSize = size;
      _texCoordPointerType = type;
      _texCoordPointerStride = stride;
      _texCoordPointer = (const GLbyte*)pointer;
   }

   const std::vector<OpenGLVertex>& VertexBuilder::GetOpenGLVertices(GLenum mode, GLint first, GLsizei count)
   {
      _vertices.clear();
      if (mode == GL_TRIANGLE_STRIP)
      {
         GLsizei startOffset = first;
         for (GLsizei i = 2; i < count; i++)
         {
            auto v1 = GetVertexAtIndex(startOffset + i - 2);
            auto v2 = GetVertexAtIndex(startOffset + i - 1);
            auto v3 = GetVertexAtIndex(startOffset + i);
            _vertices.push_back(v1);
            _vertices.push_back(v2);
            _vertices.push_back(v3);
         }
      }
      else if (mode == GL_TRIANGLE_FAN)
      {
         // Convert a fan into a list of single triangles
         GLsizei startOffset = first;
         for (GLsizei i = 0; i < count - 1; i++)
         {
            _vertices.push_back(GetVertexAtIndex(startOffset));
            _vertices.push_back(GetVertexAtIndex(startOffset + i + 1));
            _vertices.push_back(GetVertexAtIndex(startOffset + i));
         }

         // Close the gap, by creating a triangle between the first and last vertices.
         _vertices.push_back(GetVertexAtIndex(startOffset));
         _vertices.push_back(GetVertexAtIndex(startOffset + 1));
         _vertices.push_back(GetVertexAtIndex(startOffset + count - 1));
      }
      else if (mode == GL_TRIANGLES)
      {
         GLsizei startOffset = first;
         for (GLsizei i = 0; i < count; i++)
         {
            _vertices.push_back(GetVertexAtIndex(startOffset + i));
         }
      }
      else if (mode == GL_LINE_STRIP)
      {
         GLsizei startOffset = first;
         OpenGLVertex v[4];
         OpenGLVertex lastVertices[2];
         for (GLsizei i = 0; i < count - 1; i++)
         {
            OpenGLVertex startPoint = GetVertexAtIndex(startOffset + i);
            OpenGLVertex endPoint = GetVertexAtIndex(startOffset + i + 1);

            // Create 6 vertices (2 triangles) to simulate the appropriate line width (thickness)
            float angle = atan2(startPoint.y - endPoint.y, startPoint.x - endPoint.x);

            // Calculate the angle perpendicular to the line's angle
            float perpAngle = angle + 3.1415926f / 2.f;

            // Calculate deltas to increase the width of the line to the specified width
            float dx = cos(perpAngle) * _lineWidth / 2.f;
            float dy = sin(perpAngle) * _lineWidth / 2.f;

            lastVertices[0] = v[2];
            lastVertices[1] = v[3];

            v[0] = startPoint;
            v[1] = startPoint;
            v[2] = endPoint;
            v[3] = endPoint;

            v[0].x += dx;
            v[0].y += dy;

            v[1].x -= dx;
            v[1].y -= dy;

            v[2].x += dx;
            v[2].y += dy;

            v[3].x -= dx;
            v[3].y -= dy;

            _vertices.push_back(v[0]);
            _vertices.push_back(v[1]);
            _vertices.push_back(v[2]);

            _vertices.push_back(v[1]);
            _vertices.push_back(v[2]);
            _vertices.push_back(v[3]);
         }
      }

      return _vertices;
   }

   const std::vector<OpenGLVertex>&  VertexBuilder::GetVerticesFromIndices(const std::vector<int> indices)
   {
      _vertices.clear();
      for (unsigned int i = 2; i < indices.size(); i++)
      {
         _vertices.push_back(GetVertexAtIndex(indices[i - 2]));
         _vertices.push_back(GetVertexAtIndex(indices[i - 1]));
         _vertices.push_back(GetVertexAtIndex(indices[i]));
      }

      return _vertices;
   }

   void VertexBuilder::EnableColorPointer(bool enable)
   {
      _colorPointerEnabled = enable;
   }

   void VertexBuilder::SetColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
   {
      gl2dxAssert(type == GL_UNSIGNED_BYTE, "GL_UNSIGNED_BYTE is the only supported glColorPointer mode");
      _colorPointerSize = size;
      _colorPointerType = type;
      _colorPointerStride = stride;
      _colorPointer = (GLubyte*)pointer;
   }

   void VertexBuilder::SetColor(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
   {
      _red = (float)red / 255.f;
      _green = (float)green / 255.f;
      _blue = (float)blue / 255.f;
      _alpha = (float)alpha / 255.f;
   }

   void VertexBuilder::SetLineWidth(GLfloat width)
   {
      _lineWidth = width;
   }

   OpenGLVertex VertexBuilder::GetVertexAtIndex(unsigned int index)
   {
      OpenGLVertex vertex;

      GLfloat* vectorPointer;
      if (_vectorPointerStride == 0)
      {
         vectorPointer = ((GLfloat*)_vectorPointer) + index * _vectorPointerSize;
      }
      else
      {
         vectorPointer = (GLfloat*)(_vectorPointer + index * _vectorPointerStride);
      }

      vertex.x = vectorPointer[0];
      vertex.y = vectorPointer[1];
      if (_vectorPointerSize == 2)
      {
         vertex.z = 0.f;
      }
      else if (_vectorPointerSize == 3)
      {
         vertex.z = vectorPointer[2];
      }

      if (_colorPointerEnabled)
      {
         unsigned int offset;
         if (_colorPointerStride == 0)
         {
            offset = index * _colorPointerSize;
         }
         else
         {
            offset = index * _colorPointerStride;
         }

         vertex.r = (float)(_colorPointer[offset + 0]) / 255.f;
         vertex.g = (float)(_colorPointer[offset + 1]) / 255.f;
         vertex.b = (float)(_colorPointer[offset + 2]) / 255.f;
         vertex.a = (float)(_colorPointer[offset + 3]) / 255.f;
      }
      else
      {
         vertex.r = _red;
         vertex.g = _green;
         vertex.b = _blue;
         vertex.a = _alpha;
      }

      if (_texCoordPointerEnabled)
      {
         GLfloat* texCoordPointer;
         if (_texCoordPointerStride == 0)
         {
            texCoordPointer = ((GLfloat*)_texCoordPointer) + index * _texCoordPointerSize;
         }
         else
         {
            texCoordPointer = (GLfloat*)(_texCoordPointer + index * _texCoordPointerStride);
         }

         vertex.u = texCoordPointer[0];
         vertex.v = texCoordPointer[1];
      }

      return vertex;
   }
}
