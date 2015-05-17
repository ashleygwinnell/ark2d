 /*
 * Renderer.cpp
 *  Created on: 16-Oct-2009
 *      Author: Ashley Gwinnell
 */

#include "Renderer.h"
#include "Color.h"
#include "../Font/Font.h"
#include "../Font/BMFont.h" 
#include "../Font/FTFont.h"

#include "../Includes.h"
#include "../ARK2D.h"
#include "../Core/GameContainer.h"

#include "../Geometry/Rectangle.h"
#include "../Geometry/Circle.h"
#include "../Geometry/Polygon.h"
#include "../Geometry/Line.h" 

#include "../Util/Cast.h"

#include "Texture.h" 
#include "TextureStore.h"
#include "Shader.h"
#include "FBO.h" 

#include "../Common/OpenGL.h"

//#if defined(ARK2D_WINDOWS_PHONE_8)
	#include "../Util/FileUtil.h"
//#endif
 
namespace ARK {  
	namespace Graphics {

		#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH) || (defined(ARK2D_WINDOWS) && defined(ARK2D_RENDERER_OPENGL)) || defined(ARK2D_UBUNTU_LINUX) || defined(ARK2D_EMSCRIPTEN_JS) )
			
			bool Renderer::gluCheckExtensionARK(const GLubyte* extName, const GLubyte* extString) { 
				GLboolean flag=GL_FALSE;
				char* word;
				char* lookHere;
				char* deleteThis;

				if (extString==NULL)
				{
				 return GL_FALSE;
				}

				deleteThis=lookHere=(char*)malloc(strlen((const char*)extString)+1); 
				if (lookHere==NULL)
				{
				 return GL_FALSE;
				} 

				/* strtok() will modify string, so copy it somewhere */
				strcpy(lookHere,(const char*)extString);

				while ((word=strtok(lookHere, " "))!=NULL)
				{
				 if (strcmp(word,(const char*)extName)==0)
				 {
				    flag=GL_TRUE;
				    break;
				 }
				 lookHere=NULL; /* get next token */ 
				}
				free((void*)deleteThis);

				return flag;
			}

			/*void Renderer::gl3wGenVertexArrays(GLsizei n, GLuint* arrays) {

					}
					void Renderer::gl3wBindVertexArray(GLuint array) {  

					} */  

		#else 

				bool Renderer::gluCheckExtensionARK(const GLubyte* extName, const GLubyte* extString) { 
					#if (defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC) || defined(ARK2D_WINDOWS_PHONE_8))
						return false;
					#else
						RendererStats::s_glCalls++;
						return gluCheckExtension(extName, extString);  
					#endif
				}

				#ifdef ARK2D_UBUNTU_LINUX

					

				#else

					void Renderer::glGenVertexArrays(GLsizei n, GLuint* arrays) {

					}
					void Renderer::glBindVertexArray(GLuint array) {  

					}
				#endif 


		#endif 

		// renderer statistics
		unsigned int RendererStats::s_glCalls = 0;
		unsigned int RendererStats::s_lines = 0;
		unsigned int RendererStats::s_tris = 0;
		unsigned int RendererStats::s_textureSwaps = 0;	
		unsigned int RendererStats::s_shaderSwaps = 0;	
		unsigned int RendererStats::s_textureAllocatedBytes = 0;	

		RendererStats::RendererStats() {

		}
		void RendererStats::reset() {
			s_glCalls = 0;
			s_lines = 0;
			s_tris = 0;
			s_textureSwaps = 0;
			s_shaderSwaps = 0;
			// s_textureAllocatedBytes .. don't reset this.
		}
		RendererStats::~RendererStats() {
 
		}


		// renderer state
		int RendererState::s_renderMode = -1;
		int RendererState::s_textureId = 0;
		int RendererState::s_shaderId = 0;
		//int RendererState::s_multitextureId0 = 0;
		//int RendererState::s_multitextureId1 = 0;

		void RendererState::start(int renderMode) {
			start(renderMode, 0);//, 0);
		}
		void RendererState::start(int renderMode, int textureId) {
			//start(renderMode, textureId);//, 0);
			
			/*#if defined(ARK2D_FLASCC)
				if (renderMode == NONE) { startNone(); }
				else if (renderMode == GEOMETRY) { startGeometry(); }
				else if (renderMode == TEXTURE) { startTexture(textureId); }
				else if (renderMode == SHADER) { startShader(textureId); }
			 #else */
			if (renderMode == NONE) { startNone(); }
			else if (renderMode == GEOMETRY && (renderMode != s_renderMode || s_renderMode == 0)) { startGeometry(); }
			else if (renderMode == TEXTURE && (textureId != s_textureId || s_textureId == 0)) { startTexture(textureId); }
			else if (renderMode == SHADER && (textureId != s_shaderId || s_shaderId == 0)) { startShader(textureId); }
			//	else if (renderMode == MULTITEXTURE && (textureId != s_multitextureId0 || s_multitextureId0 == 0 || textureId2 != s_multitextureId1 || s_multitextureId1 == 0)) { startMultitexture(textureId, textureId2); }
			//#endif
			
			s_renderMode = renderMode;
			
		}  
		//void RendererState::start(int renderMode, int textureId, int textureId2)
		//{
		//
		//}

		//void RendererState::start(int renderMode, Shader* s) {
		//	start(renderMode, s->getId());
		//}

		void switchRendererState(unsigned int current, unsigned int to) {

			if (current == (unsigned int) RendererState::TEXTURE) {
				RendererState::s_textureId = 0;
				RendererStats::s_textureSwaps++;

				if (to == (unsigned int) RendererState::GEOMETRY || to == (unsigned int) RendererState::NONE) { 

					#if defined(ARK2D_RENDERER_OPENGL)

						glBindTexture(GL_TEXTURE_2D, 0);
						RendererStats::s_glCalls++;
					 
						#ifndef NO_FIXED_FUNCTION_PIPELINE 
							glDisable(GL_TEXTURE_2D); 
							glDisableClientState(GL_TEXTURE_COORD_ARRAY);
							RendererStats::s_glCalls += 2;

							//glDisableClientState(GL_COLOR_ARRAY);
						#elif defined(ARK2D_OPENGL_ES_2_0)
							glDisableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPosition);
							glDisableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexTexCoordIn);
							glDisableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);
							RendererStats::s_glCalls += 3;
						#endif

					#elif defined(ARK2D_RENDERER_DIRECTX)



					#endif

				} 

			} else if (current == (unsigned int) RendererState::SHADER) {
				RendererState::s_shaderId = 0;
				RendererStats::s_shaderSwaps++; 
 				//#ifndef NO_FIXED_FUNCTION_PIPELINE 
				//	glUseProgram(0);
				//#endif
			} else if (current == (unsigned int) RendererState::GEOMETRY) {
				#if defined(ARK2D_OPENGL_ES_2_0)
					glDisableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
					glDisableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
					RendererStats::s_glCalls += 2;
				#endif	
			}

			//#endif
		}

		void RendererState::startNone() {
			//ARK2D::getLog()->v("RendererState::startNone");
			switchRendererState(s_renderMode, NONE);
		}
		void RendererState::startGeometry() {
			//ARK2D::getLog()->v("RendererState::startGeometry");
			switchRendererState(s_renderMode, GEOMETRY);
			
			#if defined(ARK2D_RENDERER_OPENGL)

				#ifndef NO_FIXED_FUNCTION_PIPELINE 
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_COLOR_ARRAY);
					RendererStats::s_glCalls += 2;
				#endif


				#ifdef NO_FIXED_FUNCTION_PIPELINE
					RendererStats::s_shaderSwaps++;

					Renderer::s_shaderBasicGeometry->bind(); 
					RendererStats::s_glCalls++;
					s_shaderId = Renderer::s_shaderBasicGeometry->getId();

					#if defined(ARK2D_OPENGL_ES_2_0)
						glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
						glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
						RendererStats::s_glCalls += 2;
					#endif
				#endif

			#elif defined(ARK2D_RENDERER_DIRECTX)

				Renderer::s_shaderBasicGeometry->bind();
			
			#endif
		}
		void RendererState::startTexture(int textureId) {
			//ARK2D::getLog()->v("RendererState::startTexture");
			switchRendererState(s_renderMode, TEXTURE);

			#if defined(ARK2D_RENDERER_OPENGL)

				#ifndef NO_FIXED_FUNCTION_PIPELINE 
					glEnable(GL_TEXTURE_2D);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_COLOR_ARRAY);
					RendererStats::s_glCalls += 2;
				#endif 

				glBindTexture(GL_TEXTURE_2D, textureId);
				RendererStats::s_glCalls++;

				#ifdef NO_FIXED_FUNCTION_PIPELINE
					RendererStats::s_shaderSwaps++;

					Renderer::s_shaderBasicTexture->bind();
					RendererStats::s_glCalls++;
					s_shaderId = Renderer::s_shaderBasicTexture->getId();

					#if defined(ARK2D_OPENGL_ES_2_0)
						glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPosition);
						glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexTexCoordIn);
						glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);
						RendererStats::s_glCalls += 3;
					#endif
				#endif

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->getPlatformSpecific()->m_deviceContext;
				Texture* tx = TextureStore::getInstance()->getTexture(textureId); 
				deviceContext->PSSetSamplers(0, 1, &tx->m_dxSampler);
				deviceContext->PSSetShaderResources(0, 1, &tx->m_dxResourceView);
				Renderer::s_shaderBasicTexture->bind();

			#endif
			
			s_textureId = textureId;  
			RendererStats::s_textureSwaps++;
		}
		void RendererState::startShader(int shaderId) {
			//ARK2D::getLog()->v("RendererState::startShader");
			//ARK2D::getLog()->v("start shader");
			switchRendererState(s_renderMode, SHADER);

			#if defined(ARK2D_RENDERER_OPENGL)
				glUseProgram(shaderId);
			#endif
			
			s_shaderId = shaderId;
			RendererStats::s_shaderSwaps++;
			RendererStats::s_glCalls++;
		}

		// void RendererState::startMultitexture(int textureId0, int textureId1) {
		// 	startTexture(textureId0);

		// 	s_multitextureId0 = textureId0;
		// 	s_multitextureId1 = textureId1;
		// }

		void RendererState::internalBindTexture(int textureId) {
			#if defined(ARK2D_RENDERER_OPENGL)

				#ifndef NO_FIXED_FUNCTION_PIPELINE 
					glEnable(GL_TEXTURE_2D);
					RendererStats::s_glCalls++;
				#endif

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureId);
				RendererStats::s_glCalls += 2;
			#elif defined(ARK2D_RENDERER_DIRECTX)
				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, textureId);
			#endif
			 
			s_textureId = textureId;
		} 



		MatrixStack::MatrixStack(unsigned int type):
			m_type(type),
			m_stack(),  
			m_stackIndex(0),
			m_dirty(true)
		{
			Matrix44<float> defaultMatrix;
			m_stack.push_back(defaultMatrix);
		}
		void MatrixStack::translate(float x, float y, float z) {
			// glTranslatef(x,y,0);
			m_stack.at(m_stackIndex).translate(x, y, z); 
			m_dirty = true;
		}
		void MatrixStack::rotate(float angle, float x, float y, float z) {
			// glRotatef(angle, 0, 0, 1);
			m_stack.at(m_stackIndex).rotate(angle, x, y, z);
			m_dirty = true;
		}
		void MatrixStack::scale(float x, float y, float z) {
			// glScalef(x, y, 0.0f);
			m_stack.at(m_stackIndex).scale(x, y, z);
			m_dirty = true;
		}
		void MatrixStack::ortho2d(float l, float r, float b, float t, float n, float f) {
			m_stack.at(m_stackIndex).ortho2d(l, r, b, t, n, f);
			m_dirty = true; 
		}
		void MatrixStack::frustum(float l, float r, float b, float t, float n, float f) {
			m_stack.at(m_stackIndex).frustum(l, r, b, t, n, r);
			m_dirty = true;
		}
		void MatrixStack::perspective(float fov, float aspect, float n, float f) {
			m_stack.at(m_stackIndex).perspective(fov, aspect, n, f);
			m_dirty = true;
		}
		void MatrixStack::lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ) {
			m_stack.at(m_stackIndex).lookAt(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
			m_dirty = true;
		}
		void MatrixStack::identity() {
			m_stack.at(m_stackIndex).identity();
			m_dirty = true;
		}
		float* MatrixStack::data(float* mx) {
			const Matrix44<float>& thismx = m_stack.at(m_stackIndex); 
			mx[0] = thismx[0][0];
			mx[1] = thismx[0][1];
			mx[2] = thismx[0][2];
			mx[3] = thismx[0][3];
			mx[4] = thismx[1][0];
			mx[5] = thismx[1][1]; 
			mx[6] = thismx[1][2]; 
			mx[7] = thismx[1][3];
			mx[8] = thismx[2][0];
			mx[9] = thismx[2][1];
			mx[10] = thismx[2][2];
			mx[11] = thismx[2][3];
			mx[12] = thismx[3][0];
			mx[13] = thismx[3][1];
			mx[14] = thismx[3][2];
			mx[15] = thismx[3][3]; 
			return &mx[0];
		}
		void* MatrixStack::pointer() {
			return m_stack.at(m_stackIndex).pointer();
		}

		void MatrixStack::setDirty(bool b) {
			m_dirty = b;
		}
		bool MatrixStack::isDirty() {
			return m_dirty;
		}

		Matrix44<float>* MatrixStack::current() {
			return &m_stack[m_stackIndex];
		}
		
		#ifdef ARK2D_RENDERER_DIRECTX
			DirectX::XMMATRIX MatrixStack::d3dpointer() {
				//ARK2D::getLog()->v("Updating Matrix Buffer...");

				/*float* d = (float*) m_stack.at(m_stackIndex).pointer();

				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				HRESULT result = deviceContext->Map(m_d3d_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				if (FAILED(result)) {
					ARK2D::getLog()->e(StringUtil::append("Could not Update (Map) Matrix Buffer (d3dpointer): ", m_type));
					return m_d3d_matrix;
				}
				void* dataPtr = (void*) mappedResource.pData;
				memcpy(dataPtr, d, sizeof(float) * 16);
				deviceContext->Unmap(m_d3d_buffer, 0);*/

				const Matrix44<float>& thismx = m_stack.at(m_stackIndex); 
				//ARK2D::getLog()->v("1");
				
				using namespace DirectX;
				
				

				// switched row-major for column-major!
				/*
				XMMATRIX m_d3d_matrix;
				XMVectorSetX(m_d3d_matrix.r[0], thismx[0][0]);
				XMVectorSetY(m_d3d_matrix.r[0], thismx[1][0]);
				XMVectorSetW(m_d3d_matrix.r[0], thismx[2][0]);
				XMVectorSetZ(m_d3d_matrix.r[0], thismx[3][0]);

				XMVectorSetX(m_d3d_matrix.r[1], thismx[0][1]); 
				XMVectorSetY(m_d3d_matrix.r[1], thismx[1][1]);
				XMVectorSetW(m_d3d_matrix.r[1], thismx[2][1]);
				XMVectorSetZ(m_d3d_matrix.r[1], thismx[3][1]);
 
				XMVectorSetX(m_d3d_matrix.r[2], thismx[0][2]);
				XMVectorSetY(m_d3d_matrix.r[2], thismx[1][2]);
				XMVectorSetW(m_d3d_matrix.r[2], thismx[2][2]);
				XMVectorSetZ(m_d3d_matrix.r[2], thismx[3][2]);

				XMVectorSetX(m_d3d_matrix.r[3], thismx[0][3]);
				XMVectorSetY(m_d3d_matrix.r[3], thismx[1][3]);
				XMVectorSetW(m_d3d_matrix.r[3], thismx[2][3]);
				XMVectorSetZ(m_d3d_matrix.r[3], thismx[3][3]);*/

				/*XMMATRIX m_d3d_matrix;
				XMVectorSetX(m_d3d_matrix.r[0], thismx[0][0]);
				XMVectorSetY(m_d3d_matrix.r[0], thismx[0][1]);
				XMVectorSetW(m_d3d_matrix.r[0], thismx[0][2]);
				XMVectorSetZ(m_d3d_matrix.r[0], thismx[0][3]);

				XMVectorSetX(m_d3d_matrix.r[1], thismx[1][0]); 
				XMVectorSetY(m_d3d_matrix.r[1], thismx[1][1]);
				XMVectorSetW(m_d3d_matrix.r[1], thismx[1][2]);
				XMVectorSetZ(m_d3d_matrix.r[1], thismx[1][3]);
 
				XMVectorSetX(m_d3d_matrix.r[2], thismx[2][0]);
				XMVectorSetY(m_d3d_matrix.r[2], thismx[2][1]);
				XMVectorSetW(m_d3d_matrix.r[2], thismx[2][2]);
				XMVectorSetZ(m_d3d_matrix.r[2], thismx[2][3]);

				XMVectorSetX(m_d3d_matrix.r[3], thismx[3][0]);
				XMVectorSetY(m_d3d_matrix.r[3], thismx[3][1]);
				XMVectorSetW(m_d3d_matrix.r[3], thismx[3][2]);
				XMVectorSetZ(m_d3d_matrix.r[3], thismx[3][3]);*/

				XMMATRIX m_d3d_matrix = XMMATRIX(
					thismx[0][0], thismx[0][1], thismx[0][2], thismx[0][3],
					thismx[1][0], thismx[1][1], thismx[1][2], thismx[1][3],
					thismx[2][0], thismx[2][1], thismx[2][2], thismx[2][3],
					thismx[3][0], thismx[3][1], thismx[3][2], thismx[3][3]
				);
				 
				//ARK2D::getLog()->v("4");
				return m_d3d_matrix;
				//return NULL;

			}
		#endif

		void MatrixStack::toStringDialog() {
			float mx[16]; 
			data(&mx[0]);
 
			float* mx2;
			mx2 = (float*) pointer(); 

			string compareStr = "";
			for(unsigned int i = 0; i < 16; i++) {
				compareStr += "{";
				compareStr += Cast::toString<float>(mx[i]);
				compareStr += ",";
				compareStr += Cast::toString<float>(mx2[i]);
				compareStr += "}, ";
			}
			ErrorDialog::createAndShow(compareStr);
		}
		void MatrixStack::pushMatrix() {
			//Matrix44<float> copy = m_stack.at(m_stackIndex).copy();
			//m_stack.push_back(copy);
			m_stackIndex++; 

			if (m_stackIndex >= m_stack.size()) {
				Matrix44<float> copy = m_stack.at(m_stackIndex-1).copy();
				m_stack.push_back(copy);
			} else { 
				m_stack.at(m_stackIndex).set(m_stack.at(m_stackIndex-1));
			}

			m_dirty = true;
		}
		void MatrixStack::popMatrix() {

			if (m_stackIndex == 0) {
				ARK2D::getLog()->e("Cannot pop Matrix Stack below 0.");
				return;
			}
			//m_stack.pop_back();
			m_stackIndex--;
			 
			m_dirty = true;
		}
		MatrixStack::~MatrixStack() {
 
		}





		// BATCH STUFF
		RendererBatch* Renderer::s_batch = new RendererBatch();
		RendererBatchItem::RendererBatchItem():
			geomtris(),
			textris(),
			m_type(TYPE_GEOMETRY_TRIS),
			m_textureId(0)
 		{

		}
		void RendererBatchItem::clear() {
			geomtris.clear();
			textris.clear();
			m_type = TYPE_GEOMETRY_TRIS;
			m_textureId = 0;
		}

		void* dx_fillTriangles_data = NULL;
		unsigned int dx_fillTriangles_size = 0;
		
		void* dx_texturedTriangles_data = NULL;
		unsigned int dx_texturedTriangles_size = 0;
		
		void* dx_renderBatch_data = NULL;
		unsigned int dx_renderBatch_size = 0;

		void RendererBatchItem::renderDX() {
			#ifdef ARK2D_RENDERER_DIRECTX
				// Optimisations for DirectX. Less memory allocated.
				if (m_type == TYPE_GEOMETRY_TRIS) 
				{
					unsigned int countTriangles = geomtris.size();
					if (countTriangles == 0) { clear(); return; }
					
					RendererState::start(RendererState::GEOMETRY);

					ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
					ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

					Renderer::__internalsDXUpdateMatrices();

					unsigned int countVerts = countTriangles * 3;
					unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingGeometryVertexData);

					if (dx_renderBatch_data == NULL || dx_renderBatch_size < memorySize) {
						dx_renderBatch_data = realloc(dx_renderBatch_data, memorySize);
						dx_renderBatch_size = memorySize;
					}

					Renderer_DX_InterleavingGeometryVertexData* rawVertices = (Renderer_DX_InterleavingGeometryVertexData*) dx_renderBatch_data;
					unsigned int count = 0;
					for (unsigned int i = 0; i < countVerts; i +=3) {
						RendererBatchItem_GeomTri* tri = &geomtris[count];
						rawVertices[i].vertex = DirectX::XMFLOAT4(tri->vertexData[0], tri->vertexData[1], 0.0f, 0.0f);
						rawVertices[i].color = DirectX::XMFLOAT4(tri->colorData[0]/255.0f, tri->colorData[1]/255.0f, tri->colorData[2]/255.0f, tri->colorData[3]/255.0f);

						rawVertices[i+1].vertex = DirectX::XMFLOAT4(tri->vertexData[2], tri->vertexData[3], 0.0f, 0.0f);
						rawVertices[i+1].color = DirectX::XMFLOAT4(tri->colorData[4]/255.0f, tri->colorData[5]/255.0f, tri->colorData[6]/255.0f, tri->colorData[7]/255.0f);

						rawVertices[i+2].vertex = DirectX::XMFLOAT4(tri->vertexData[4], tri->vertexData[5], 0.0f, 0.0f);
						rawVertices[i+2].color = DirectX::XMFLOAT4(tri->colorData[8]/255.0f, tri->colorData[9]/255.0f, tri->colorData[10]/255.0f, tri->colorData[11]/255.0f);
						
						count++;
					}
					Renderer::s_vboQuadVerts->setData((void*)&rawVertices[0], memorySize);
					
					unsigned int stride = sizeof(Renderer_DX_InterleavingGeometryVertexData);
					unsigned int offset = 0;
					deviceContext->IASetVertexBuffers(0, 1, &Renderer::s_vboQuadVerts->m_buffer, &stride, &offset);
					deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					deviceContext->Draw(countVerts, 0);

				}
				else if (m_type == TYPE_TEXTURE_TRIS)
				{
					unsigned int countTriangles = textris.size();
					if (countTriangles == 0) { clear(); return; }
					
					RendererState::start(RendererState::TEXTURE, m_textureId);

					ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
					ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

					Renderer::__internalsDXUpdateMatrices();

					unsigned int countVerts = countTriangles * 3;
					unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingTextureVertexData);

					if (dx_renderBatch_data == NULL || dx_renderBatch_size < memorySize) {
						dx_renderBatch_data = realloc(dx_renderBatch_data, memorySize);
						dx_renderBatch_size = memorySize;
					}

					Renderer_DX_InterleavingTextureVertexData* rawVertices = (Renderer_DX_InterleavingTextureVertexData*) dx_renderBatch_data;
					unsigned int count = 0;
					for (unsigned int i = 0; i < countVerts; i +=3) {
						RendererBatchItem_TexTri* tri = &textris[count];
						rawVertices[i].vertex = DirectX::XMFLOAT4(tri->vertexData[0], tri->vertexData[1], 0.0f, 0.0f);
						rawVertices[i].texcoord = DirectX::XMFLOAT2(tri->textureData[0], tri->textureData[1]);
						rawVertices[i].color = DirectX::XMFLOAT4(tri->colorData[0]/255.0f, tri->colorData[1]/255.0f, tri->colorData[2]/255.0f, tri->colorData[3]/255.0f);

						rawVertices[i+1].vertex = DirectX::XMFLOAT4(tri->vertexData[2], tri->vertexData[3], 0.0f, 0.0f);
						rawVertices[i+1].texcoord = DirectX::XMFLOAT2(tri->textureData[2], tri->textureData[3]);
						rawVertices[i+1].color = DirectX::XMFLOAT4(tri->colorData[4]/255.0f, tri->colorData[5]/255.0f, tri->colorData[6]/255.0f, tri->colorData[7]/255.0f);

						rawVertices[i+2].vertex = DirectX::XMFLOAT4(tri->vertexData[4], tri->vertexData[5], 0.0f, 0.0f);
						rawVertices[i+2].texcoord = DirectX::XMFLOAT2(tri->textureData[4], tri->textureData[5]);
						rawVertices[i+2].color = DirectX::XMFLOAT4(tri->colorData[8]/255.0f, tri->colorData[9]/255.0f, tri->colorData[10]/255.0f, tri->colorData[11]/255.0f);
						
						count++;
					}
					Renderer::s_vboQuadVerts->setData((void*)&rawVertices[0], memorySize);
					
					unsigned int stride = sizeof(Renderer_DX_InterleavingTextureVertexData);
					unsigned int offset = 0;
					deviceContext->IASetVertexBuffers(0, 1, &Renderer::s_vboQuadVerts->m_buffer, &stride, &offset);
					deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
					deviceContext->Draw(countVerts, 0);
				}
			#endif
			clear();
		}
		void RendererBatchItem::render() {
			#if defined(ARK2D_RENDERER_DIRECTX)
				renderDX();
			#else 
				if (m_type == TYPE_GEOMETRY_TRIS) 
				{
					unsigned int tsz = geomtris.size();
					int sz = tsz * 6; 
					if (sz == 0) { clear(); return; }

					#ifdef ARK2D_WINDOWS_VS
						float* all_vertexData = (float*) alloca(sz * sizeof(float));
						unsigned char* all_colorData = (unsigned char*)alloca(sz * 2 * sizeof(unsigned char));
					
						Assert(all_vertexData);
						Assert(all_colorData);
					#else
						float all_vertexData[sz];
						unsigned char all_colorData[sz*2];
					#endif
					

					for(unsigned int i = 0; i < tsz; i++) { 
						memcpy(&all_vertexData[i*6], &geomtris[i].vertexData, sizeof(float) * 6);
						memcpy(&all_colorData[i*12], &geomtris[i].colorData, sizeof(unsigned char) * 12);
					}

					ARK2D::getRenderer()->fillTriangles(&all_vertexData[0], &all_colorData[0], tsz, true);

					//#ifdef ARK2D_WINDOWS_VS
					//	free(all_vertexData);
					//	free(all_colorData);
					//#endif
				}
				else if (m_type == TYPE_TEXTURE_TRIS) 
				{
					unsigned int tsz = textris.size();
					int sz = tsz * 6; 
					if (sz == 0) { clear(); return; }

					#ifdef ARK2D_WINDOWS_VS
						float* all_vertexData = (float*) alloca(sz * sizeof(float));
						float* all_textureData = (float*) alloca(sz * sizeof(float));
						unsigned char* all_colorData = (unsigned char*) alloca(sz * 2 * sizeof(unsigned char) );

						Assert(all_vertexData); 
						Assert(all_textureData);
						Assert(all_colorData);
					#else
						float all_vertexData[sz];
						float all_textureData[sz];
						unsigned char all_colorData[sz*2];
					#endif

					for(unsigned int i = 0; i < tsz; i++) { 
						memcpy(&all_vertexData[i*6], &textris[i].vertexData, sizeof(float) * 6);
						memcpy(&all_textureData[i*6], &textris[i].textureData, sizeof(float) * 6);
						memcpy(&all_colorData[i*12], &textris[i].colorData, sizeof(unsigned char) * 12);
					}

					ARK2D::getRenderer()->texturedTriangles(m_textureId, &all_vertexData[0], &all_textureData[0], &all_colorData[0], tsz, true);

					//#ifdef ARK2D_WINDOWS_VS
					//	free(all_vertexData);
					//	free(all_textureData);
					//	free(all_colorData);
					//#endif
				}
				clear();
			#endif
		}
		RendererBatchItem::~RendererBatchItem() { 
			clear();
		}

		RendererBatchItem_GeomTri::RendererBatchItem_GeomTri(): 
			vertexData(), 
			colorData() 
		{ 
			//vertexData[0] = 0;
			//vertexData[1] = 0;
			//vertexData[2] = 0;
			//vertexData[3] = 0;
			//vertexData[4] = 0;
			//vertexData[5] = 0;
		}
		RendererBatchItem_TexTri::RendererBatchItem_TexTri():
			vertexData(),
			textureData(),
			colorData()
		{

		}
		
		RendererBatch::RendererBatch():
			items(),
			enabled(false)
		{

		}
		void RendererBatch::addGeometryTri(float* verts, unsigned char* colors) 
		{
			/*if (items.size() == 0 || items.at(items.size()-1).m_type == RendererBatchItem::TYPE_TEXTURE_TRIS) {
				items.push_back(RendererBatchItem());
			}
			RendererBatchItem* item = &items.at(items.size()-1);
			item->m_type = RendererBatchItem::TYPE_GEOMETRY_TRIS;
			item->m_textureId = 0;

			RendererBatchItem_GeomTri one;
			memcpy(&one.vertexData[0], verts, sizeof(float) * 6);
			memcpy(&one.colorData[0], colors, sizeof(unsigned char) * 12);
			item->geomtris.push_back(one);*/

			addGeometryTri(
				verts[0], verts[1], 
				verts[2], verts[3], 
				verts[4], verts[5], 
				colors[0], colors[1], colors[2], colors[3],
				colors[4], colors[5], colors[6], colors[7],
				colors[8], colors[9], colors[10], colors[11]
			); 
		}
		void RendererBatch::addGeometryTri(
			float x1, float y1, 
			float x2, float y2, 
			float x3, float y3, 
			unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
			unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
			unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a) 
		{
			if (items.size() == 0 || items.at(items.size()-1).m_type == RendererBatchItem::TYPE_TEXTURE_TRIS) {
				items.push_back(RendererBatchItem());
			}

			RendererBatchItem* item = &items.at(items.size()-1);
			item->m_type = RendererBatchItem::TYPE_GEOMETRY_TRIS;
			item->m_textureId = 0;

			RendererBatchItem_GeomTri one;
			one.vertexData[0] = x1;
			one.vertexData[1] = y1;
			one.vertexData[2] = x2;
			one.vertexData[3] = y2;
			one.vertexData[4] = x3;
			one.vertexData[5] = y3;
			one.colorData[0] = c1r;
			one.colorData[1] = c1g;
			one.colorData[2] = c1b;
			one.colorData[3] = c1a;
			one.colorData[4] = c2r;
			one.colorData[5] = c2g;
			one.colorData[6] = c2b;
			one.colorData[7] = c2a;
			one.colorData[8] = c3r;
			one.colorData[9] = c3g;
			one.colorData[10] = c3b;
			one.colorData[11] = c3a;
			item->geomtris.push_back(one);

		}
		void RendererBatch::addGeometryQuad(
			float x1, float y1, 
			float x2, float y2, 
			float x3, float y3, 
			float x4, float y4, 
			unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
			unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
			unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a,
			unsigned char c4r, unsigned char c4g, unsigned char c4b, unsigned char c4a) 
		{
			if (items.size() == 0 || items.at(items.size()-1).m_type == RendererBatchItem::TYPE_TEXTURE_TRIS) {
				items.push_back(RendererBatchItem());
			}
			RendererBatchItem* item = &items.at(items.size()-1);
			item->m_type = RendererBatchItem::TYPE_GEOMETRY_TRIS;
			item->m_textureId = 0;

			RendererBatchItem_GeomTri one;
			one.vertexData[0] = x1;
			one.vertexData[1] = y1;
			one.vertexData[2] = x2;
			one.vertexData[3] = y2;
			one.vertexData[4] = x3;
			one.vertexData[5] = y3;
			one.colorData[0] = c1r;
			one.colorData[1] = c1g;
			one.colorData[2] = c1b;
			one.colorData[3] = c1a;
			one.colorData[4] = c2r;
			one.colorData[5] = c2g;
			one.colorData[6] = c2b;
			one.colorData[7] = c2a;
			one.colorData[8] = c3r;
			one.colorData[9] = c3g;
			one.colorData[10] = c3b;
			one.colorData[11] = c3a;
			item->geomtris.push_back(one);

			RendererBatchItem_GeomTri two;
			two.vertexData[0] = x3;
			two.vertexData[1] = y3;
			two.vertexData[2] = x2;
			two.vertexData[3] = y2;
			two.vertexData[4] = x4;
			two.vertexData[5] = y4;
			two.colorData[0] = c3r;
			two.colorData[1] = c3g;
			two.colorData[2] = c3b;
			two.colorData[3] = c3a;
			two.colorData[4] = c2r;
			two.colorData[5] = c2g;
			two.colorData[6] = c2b;
			two.colorData[7] = c2a;
			two.colorData[8] = c4r;
			two.colorData[9] = c4g;
			two.colorData[10] = c4b;
			two.colorData[11] = c4a;
			item->geomtris.push_back(two);
		}

		void RendererBatch::addTexturedTri(unsigned int texId, float* verts, float* texcoords, unsigned char* colors) 
		{
			addTexturedTri(
				texId, 
				verts[0], verts[1], 
				verts[2], verts[3], 
				verts[4], verts[5], 
				texcoords[0], texcoords[1], 
				texcoords[2], texcoords[3], 
				texcoords[4], texcoords[5], 
				colors[0], colors[1], colors[2], colors[3],
				colors[4], colors[5], colors[6], colors[7],
				colors[8], colors[9], colors[10], colors[11]
			); 
		}
		void RendererBatch::addTexturedTri(
			unsigned int texId, 
			float x1, float y1, float x2, float y2, float x3, float y3, 
			float tx1, float ty1, float tx2, float ty2, float tx3, float ty3,
			unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
			unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
			unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a) 
		{
			if (items.size() == 0 || items.at(items.size()-1).m_type == RendererBatchItem::TYPE_GEOMETRY_TRIS || items.at(items.size()-1).m_textureId != texId) {
				items.push_back(RendererBatchItem());
			}

			RendererBatchItem* item = &items.at(items.size()-1);
			item->m_type = RendererBatchItem::TYPE_TEXTURE_TRIS;
			item->m_textureId = texId;

			RendererBatchItem_TexTri one;
			one.vertexData[0] = x1;
			one.vertexData[1] = y1;
			one.vertexData[2] = x2;
			one.vertexData[3] = y2;
			one.vertexData[4] = x3;
			one.vertexData[5] = y3;
			one.textureData[0] = tx1;
			one.textureData[1] = ty1;
			one.textureData[2] = tx2;
			one.textureData[3] = ty2;
			one.textureData[4] = tx3;
			one.textureData[5] = ty3;
			one.colorData[0] = c1r;
			one.colorData[1] = c1g;
			one.colorData[2] = c1b;
			one.colorData[3] = c1a;
			one.colorData[4] = c2r;
			one.colorData[5] = c2g;
			one.colorData[6] = c2b;
			one.colorData[7] = c2a;
			one.colorData[8] = c3r;
			one.colorData[9] = c3g;
			one.colorData[10] = c3b;
			one.colorData[11] = c3a;
			item->textris.push_back(one);

		}
		void RendererBatch::addTexturedQuad(unsigned int texId, float* verts, float* texcoords, unsigned char* colors) {
			addTexturedQuad(
				texId, 
				
				verts[0], verts[1], 
				verts[2], verts[3], 
				verts[4], verts[5], 
				verts[10], verts[11],
				
				texcoords[0], texcoords[1], 
				texcoords[2], texcoords[3], 
				texcoords[4], texcoords[5], 
				texcoords[10], texcoords[11],

				colors[0], colors[1], colors[2], colors[3],
				colors[4], colors[5], colors[6], colors[7],
				colors[8], colors[9], colors[10], colors[11],
				colors[20], colors[21], colors[22], colors[23] 
			); 
		} 
		void RendererBatch::addTexturedQuad(
			unsigned int texId,  
			float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, 
			float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4,
			unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
			unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
			unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a,
			unsigned char c4r, unsigned char c4g, unsigned char c4b, unsigned char c4a) {

			if (items.size() == 0 || items.at(items.size()-1).m_type == RendererBatchItem::TYPE_GEOMETRY_TRIS || items.at(items.size()-1).m_textureId != texId) {
				items.push_back(RendererBatchItem());
			}
			RendererBatchItem* item = &items.at(items.size()-1);
			item->m_type = RendererBatchItem::TYPE_TEXTURE_TRIS;
			item->m_textureId = texId;

			RendererBatchItem_TexTri one;
			one.vertexData[0] = x1;
			one.vertexData[1] = y1;
			one.vertexData[2] = x2;
			one.vertexData[3] = y2;
			one.vertexData[4] = x3;
			one.vertexData[5] = y3;
			one.textureData[0] = tx1;
			one.textureData[1] = ty1;
			one.textureData[2] = tx2;
			one.textureData[3] = ty2;
			one.textureData[4] = tx3;
			one.textureData[5] = ty3;
			one.colorData[0] = c1r;
			one.colorData[1] = c1g;
			one.colorData[2] = c1b;
			one.colorData[3] = c1a;
			one.colorData[4] = c2r;
			one.colorData[5] = c2g;
			one.colorData[6] = c2b;
			one.colorData[7] = c2a;
			one.colorData[8] = c3r;
			one.colorData[9] = c3g;
			one.colorData[10] = c3b;
			one.colorData[11] = c3a;
			item->textris.push_back(one);

			RendererBatchItem_TexTri two;
			two.vertexData[0] = x3;
			two.vertexData[1] = y3;
			two.vertexData[2] = x2;
			two.vertexData[3] = y2;
			two.vertexData[4] = x4;
			two.vertexData[5] = y4;
			two.textureData[0] = tx3;
			two.textureData[1] = ty3;
			two.textureData[2] = tx2;
			two.textureData[3] = ty2;
			two.textureData[4] = tx4;
			two.textureData[5] = ty4;
			two.colorData[0] = c3r;
			two.colorData[1] = c3g;
			two.colorData[2] = c3b;
			two.colorData[3] = c3a;
			two.colorData[4] = c2r;
			two.colorData[5] = c2g;
			two.colorData[6] = c2b;
			two.colorData[7] = c2a;
			two.colorData[8] = c4r;
			two.colorData[9] = c4g;
			two.colorData[10] = c4b;
			two.colorData[11] = c4a;
			item->textris.push_back(two);
		}
		void RendererBatch::render() {
			for(unsigned int i = 0; i < items.size(); i++) {
				items[i].render();
			}
			items.clear();
		}
		RendererBatch::~RendererBatch() {

		}













		// renderer static
		signed int Renderer::s_interpolation = Renderer::INTERPOLATION_NEAREST;
		signed int Renderer::getInterpolation() {
			return s_interpolation;
		} 
		void Renderer::setInterpolation(signed int i) {
			s_interpolation = i;
		}
		unsigned int Renderer::getInterpolationGL() {
			#ifdef ARK2D_RENDERER_OPENGL
				switch(s_interpolation) {
					case INTERPOLATION_NEAREST:
						return GL_NEAREST;
					case INTERPOLATION_LINEAR:
						return GL_LINEAR;
				}
				return GL_NEAREST;
			#else
				return 0; 
			#endif
		}

		// -------------------------------
		// MATRIX STUFF
		// -------------------------------
		MatrixStack* Renderer::s_matrixProjection = NULL;
		MatrixStack* Renderer::s_matrixModelView = NULL;
		MatrixStack* Renderer::s_matrix = NULL;
		#if defined(ARK2D_RENDERER_DIRECTX)
			ID3D11Buffer* Renderer::s_d3d_matrixBuffer = NULL;
			CD3D11_BUFFER_DESC Renderer::s_d3d_matrixBufferDesc;
			Renderer_DX_ModelViewProjectionMatrixBuffer* Renderer::s_d3d_matrixBufferData = NULL;
		#endif

		MatrixStack* Renderer::getMatrix() { 
			return s_matrix;  
		}  
		MatrixStack* Renderer::getMatrix(unsigned int type) {
			if (type == MatrixStack::TYPE_PROJECTION) { return s_matrixProjection; }
			else if (type == MatrixStack::TYPE_MODELVIEW) { return s_matrixModelView; }
			ARK2D::getLog()->e(StringUtil::append("Trying to retrieve invalid matrix", type));
			return NULL;
		}
		void Renderer::multiplyMatrix(Matrix44<float>* by) {
			Matrix44<float>* subject = &s_matrix->m_stack[s_matrix->m_stackIndex];
			*subject *= *by;
		}
		void Renderer::matrixMode(unsigned int mode) {
			//ARK2D::getLog()->i("matrixMode");
			if (mode == MatrixStack::TYPE_PROJECTION) {
				s_matrix = s_matrixProjection;

				#ifndef NO_FIXED_FUNCTION_PIPELINE
					//ARK2D::getLog()->v("fixed-function-pipeline");
					glMatrixMode(GL_PROJECTION);
					RendererStats::s_glCalls++;
				#endif

			} else if (mode == MatrixStack::TYPE_MODELVIEW) {
				s_matrix = s_matrixModelView;

				#ifndef NO_FIXED_FUNCTION_PIPELINE 
					//ARK2D::getLog()->v("fixed-function-pipeline");
					glMatrixMode(GL_MODELVIEW);
					RendererStats::s_glCalls++;
				#endif

			} else { 
				ARK2D::getLog()->e(StringUtil::append("Invalid Matrix Mode", mode));
			}
			showAnyGlErrorAndExitMacro();
		}
		void Renderer::pushMatrix() const {
			//ARK2D::getLog()->v("pushMatrix");

			s_matrix->pushMatrix();  
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glPushMatrix();
				RendererStats::s_glCalls++;
				//glLoadMatrixf((float*)s_matrix->pointer());
			#endif

			showAnyGlErrorAndExitMacro();
		} 
		void Renderer::popMatrix() const {
			//ARK2D::getLog()->v("popMatrix");

			s_matrix->popMatrix();
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glPopMatrix();
				RendererStats::s_glCalls++;
				//	glLoadMatrixf((float*)s_matrix->pointer());
			#endif

			showAnyGlErrorAndExitMacro();
		}
		void Renderer::loadIdentity() const {
			//ARK2D::getLog()->i("loadIdentity");
 
			s_matrix->identity(); // only changes the topmost matrix.
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//ARK2D::getLog()->v("fixed-function-pipeline");
				glLoadIdentity();
				RendererStats::s_glCalls++;
			#endif

			showAnyGlErrorAndExitMacro();
		}

		void Renderer::loadMatrix(Matrix44<float>* m) const {
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glLoadMatrixf((const GLfloat*) m->pointer());//->m_x->m_x);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::loadMatrix(const Matrix44<float>& m) const {
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glLoadMatrixf((const GLfloat*) m.pointer());//&m[0][0]);
				showAnyGlErrorAndExitMacro(); 
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::resetMatrix() const {
			loadIdentity();
		}

		// -------------------
		// SHADER STUFF
		// -------------------
		Shader* Renderer::s_shaderCurrent = NULL;
		Shader* Renderer::getShader() {
			return s_shaderCurrent;
		}

		Shader* Renderer::s_shaderBasicGeometry = NULL;
		unsigned int Renderer::s_shaderBasicGeometry_ModelViewMatrix = 0;
		unsigned int Renderer::s_shaderBasicGeometry_ProjectionMatrix = 0;
		unsigned int Renderer::s_shaderBasicGeometry_VertexPosition = 0;
		unsigned int Renderer::s_shaderBasicGeometry_VertexColorIn = 0;
		Shader* Renderer::getBasicGeometryShader() {
			return s_shaderBasicGeometry;
		}

		Shader* Renderer::s_shaderBasicTexture = NULL;
		unsigned int Renderer::s_shaderBasicTexture_ModelViewMatrix = 0;
		unsigned int Renderer::s_shaderBasicTexture_ProjectionMatrix = 0;
		unsigned int Renderer::s_shaderBasicTexture_VertexPosition = 0;
		unsigned int Renderer::s_shaderBasicTexture_VertexColorIn = 0;
		unsigned int Renderer::s_shaderBasicTexture_VertexTexCoordIn = 0;
		unsigned int Renderer::s_shaderBasicTexture_TextureId = 0;
		Shader* Renderer::getBasicTextureShader() {
			return s_shaderBasicTexture;  
		}


		// VAO and VBO bits
		Mat44VBO* Renderer::s_vboMatrixProjection = new Mat44VBO(); 
		Mat44VBO* Renderer::s_vboMatrixModelView = new Mat44VBO();
		QuadVBO* Renderer::s_vboQuadVerts = new QuadVBO();
		QuadVBO* Renderer::s_vboQuadTexCoords = new QuadVBO();
		QuadVBO* Renderer::s_vboQuadColors = new QuadVBO();
		VBO* Renderer::s_vboIndices = new VBO();
		VAO* Renderer::s_vaoQuad = new VAO(); 
 
		VBO::VBO(): 
			m_id(0), 
			m_msize(0)
			#if defined( ARK2D_RENDERER_DIRECTX )
				, m_buffer(NULL)
				, m_bufferDesc()
				, m_bufferData()
			#endif
		{
			
		} 
		void VBO::setSize(unsigned int szbytes) {
			ARK2D::getLog()->v(StringUtil::append("Setting buffer sz: ", szbytes));
			m_msize = szbytes;
		}
		unsigned int VBO::getId() { 
			return m_id; 
		}
		void VBO::init() { 
			#if defined(ARK2D_RENDERER_OPENGL)
				glGenBuffers(1, &m_id); 
			#elif defined(ARK2D_RENDERER_DIRECTX)

				ARK2D::getLog()->v(StringUtil::append("Buffer sz: ", m_msize));
				
				void* data = malloc( m_msize ); 

				D3D11_BUFFER_DESC vertexBufferDesc = {0};
				ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

				vertexBufferDesc.ByteWidth = m_msize;
				vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				vertexBufferDesc.BindFlags = m_indexType; // D3D11_BIND_VERTEX_BUFFER or D3D11_BIND_INDEX_BUFFER.
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				vertexBufferDesc.MiscFlags = 0;
				vertexBufferDesc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
				ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
				vertexBufferData.pSysMem = data;
				vertexBufferData.SysMemPitch = 0;
				vertexBufferData.SysMemSlicePitch = 0;

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				m_result = device->CreateBuffer(&vertexBufferDesc, NULL, &m_buffer);
				if (FAILED(m_result)) {
					string s = StringUtil::append("Could not set data of VBO.", HRESULT_CODE(m_result));
					ErrorDialog::createAndShow(s);
					exit(0);
				}

				m_bufferData = vertexBufferData;
				m_bufferDesc = vertexBufferDesc;

				/*m_bufferDesc.Usage = D3D11_USAGE_DYNAMIC;//D3D11_USAGE_DEFAULT;
				m_bufferDesc.ByteWidth = 0;//size; //sizeof(VertexType) * m_vertexCount;
				m_bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				m_bufferDesc.CPUAccessFlags = 0;
				m_bufferDesc.MiscFlags = 0;
				m_bufferDesc.StructureByteStride = 0;

				m_bufferData.pSysMem = NULL;
				m_bufferData.SysMemPitch = 0;
				m_bufferData.SysMemSlicePitch = 0;

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				m_result = device->CreateBuffer(&m_bufferDesc, &m_bufferData, &m_buffer);
				if (FAILED(m_result)) {
					ErrorDialog::createAndShow("Could not set data of VBO.");
					exit(0);
				}*/
			#endif
			RendererStats::s_glCalls++;
		}
		void VBO::setData(void* data, unsigned int size) {
			#if defined(ARK2D_RENDERER_OPENGL)

				glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

			#elif defined(ARK2D_RENDERER_DIRECTX)
				
				//ARK2D::getLog()->v("Updating VBO...");
				ID3D11DeviceContext* context = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
				
				if (size > m_msize) {

					// dynamically resize this buffer. eesh.
					m_buffer->Release();
					//delete m_buffer;
					
					m_msize = size;
					init();

					ARK2D::getLog()->v("Resizing DX buffer...");
					//ARK2D::getLog()->w("Will not draw something. D3D buffer is the wrong size");
					//return;
					//ARK2D::getLog()->v("realloc");
					//realloc(r.pData, size);
				}

				D3D11_MAPPED_SUBRESOURCE r;
				//ZeroMemory(&r, sizeof(D3D11_MAPPED_SUBRESOURCE));

				HRESULT hr = context->Map(m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &r);
				if (FAILED(hr)) {
					ErrorDialog::createAndShow(StringUtil::append("Failed to update DirectX 11 VBO.", DX_GetError(hr)));
					exit(0);
				}

				
				memcpy(r.pData, data, size);
				context->Unmap(m_buffer, 0);
				
				//m_msize = size;        		

			#endif
			RendererStats::s_glCalls++; 
		}
		void VBO::bind() { 
			#if defined(ARK2D_RENDERER_OPENGL)
				glBindBuffer(GL_ARRAY_BUFFER, m_id); 
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
			RendererStats::s_glCalls++;
		}
		void VBO::unbind() { 
			#if defined(ARK2D_RENDERER_OPENGL)
				glBindBuffer(GL_ARRAY_BUFFER, 0); 
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
			RendererStats::s_glCalls++;
		}
		
		QuadVBO::QuadVBO(): VBO(), m_width(2), m_height(4) { 
			#if defined(ARK2D_RENDERER_DIRECTX)
				//setSize( sizeof(Renderer_DX_InterleavingGeometryVertexData)*4 );
				setSize(sizeof(Renderer_DX_InterleavingTextureVertexData) * 4);
			#endif
		}
		void QuadVBO::setWidth(int w) { 
			m_width = w;  
		}
		void QuadVBO::setHeight(int h) { 
			m_height = h;  
		}
		void QuadVBO::setData(void* data, unsigned int size) {
			VBO::setData(data, size);
		}
		void QuadVBO::setData(float* data) {
			#if defined(ARK2D_RENDERER_OPENGL) 
				
				if (m_width == 0 || m_height == 0) { return; }
				/*#ifdef ARK2D_WINDOWS_VS 
					if (sizeof(float) * m_width * m_height != sizeof(data)) {
						ARK2D::getLog()->w("Invalid buffer. Size mis-match.");
						ARK2D::getLog()->w(StringUtil::append("buffer: ", sizeof(float) * m_width * m_height));
						ARK2D::getLog()->w(StringUtil::append("data: ", sizeof(data)));
					}
				#endif*/

				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_width * m_height, data, GL_DYNAMIC_DRAW); 
			#elif defined(ARK2D_RENDERER_DIRECTX)
				VBO::setData((void*) data, sizeof(float) * m_width * m_height);
			#endif
			RendererStats::s_glCalls++;
		}
		void QuadVBO::setData(unsigned char* data) { 
			#if defined(ARK2D_RENDERER_OPENGL)
				
				if (m_width == 0 || m_height == 0) { return; }
				
				/*#ifdef ARK2D_WINDOWS_VS 
					if (sizeof(unsigned char) * m_width * m_height != sizeof(data)) {
						ARK2D::getLog()->w("Invalid buffer. Size mis-match.");
						ARK2D::getLog()->w(StringUtil::append("buffer: ", sizeof(unsigned char) * m_width * m_height));
						ARK2D::getLog()->w(StringUtil::append("data: ", sizeof(data)));
					}
				#endif*/
				
				glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * m_width * m_height, data, GL_DYNAMIC_DRAW); 
			#elif defined(ARK2D_RENDERER_DIRECTX)
				VBO::setData((void*) data, sizeof(unsigned char) * m_width * m_height);
			#endif
			RendererStats::s_glCalls++;
		}

		Mat44VBO::Mat44VBO(): VBO() {
			#if defined(ARK2D_RENDERER_DIRECTX)
				setSize( sizeof(float)*16 );
			#endif
		} 
		void Mat44VBO::setData(float* data) { 
			#if defined(ARK2D_RENDERER_OPENGL)
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, data, GL_DYNAMIC_DRAW); 
			#elif defined(ARK2D_RENDERER_DIRECTX)
				VBO::setData((void*) data, sizeof(float) * 16);
			#endif
			RendererStats::s_glCalls++;
		}
		void Mat44VBO::bind() {
			#if defined(ARK2D_RENDERER_DIRECTX)
				//setData()
			#endif
		}

		signed int VAO::s_currentVAO = -1; 
		VAO::VAO(): m_id(0) { } 
		signed int VAO::getId() { 
			return m_id; 
		}
		void VAO::init() { 
			if (FBO::isVAOSupported()) { 
				glGenVertexArraysARK( 1, &m_id );
				RendererStats::s_glCalls++;
			}
		} 
		bool VAO::bind() {
			if (s_currentVAO == (signed int) m_id) { return false; }

			if (FBO::isVAOSupported()) { 
				glBindVertexArrayARK(m_id);
				RendererStats::s_glCalls++;
			}
			s_currentVAO = (signed int) m_id;  
			return true;
		}



		// renderer 
		Renderer::Renderer():
			m_DefaultFont(NULL),
			m_Font(NULL),  
			m_DrawColor(255, 255, 255),
			m_MaskColor(255, 0, 255),
			m_LineWidth(1),
			m_pointSize(1), 
			m_blendMode(BLEND_NORMAL), 
			m_ScissorBoxColors()
		{
			//m_DefaultFont = new BMFont("data/calibri.fnt", "data/calibri.bmp", Color::magenta);
			//m_Font = m_DefaultFont;
		} 
		void Renderer::preinit() {
			s_matrixProjection = new MatrixStack(MatrixStack::TYPE_PROJECTION);
			s_matrixModelView = new MatrixStack(MatrixStack::TYPE_MODELVIEW);
			s_matrix = s_matrixModelView;
		}
		void Renderer::init() {
			// this should be called when opengl is set up and ready to start loading shaders and crap
			ARK2D::getLog()->v("Renderer::init");
			#ifdef SHADER_SUPPORT 

				bool fbosupport = FBO::isSupported();

				#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
					glGenVertexArraysOES = NULL;
					glBindVertexArrayOES = NULL;
			
					#if defined(ARK2D_ANDROID)
				
						if (FBO::isVAOSupported()) {
							glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC) eglGetProcAddress("glGenVertexArraysOES");
							glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC) eglGetProcAddress("glBindVertexArrayOES");
						}
			
					#endif
			
				#endif

				

				#if defined (ARK2D_RENDERER_DIRECTX)
					ARK2D::getLog()->v("Creating Matrix Buffer...");
					ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
					
					CD3D11_BUFFER_DESC bufferDesc(sizeof(Renderer_DX_ModelViewProjectionMatrixBuffer), D3D11_BIND_CONSTANT_BUFFER);
					HRESULT result = device->CreateBuffer(&bufferDesc, NULL, &s_d3d_matrixBuffer);
					if (FAILED(result)) {
						ErrorDialog::createAndShow("Could not create Matrix Buffer.");
						exit(0);
					} 
					s_d3d_matrixBufferDesc = bufferDesc;

					s_d3d_matrixBufferData = new Renderer_DX_ModelViewProjectionMatrixBuffer();

					s_vboMatrixProjection->m_indexType = D3D11_BIND_CONSTANT_BUFFER;
					s_vboMatrixModelView->m_indexType = D3D11_BIND_CONSTANT_BUFFER;
					s_vboQuadVerts->m_indexType = D3D11_BIND_VERTEX_BUFFER;
					s_vboQuadTexCoords->m_indexType = D3D11_BIND_VERTEX_BUFFER; // not used? 
					s_vboQuadColors->m_indexType = D3D11_BIND_VERTEX_BUFFER; // not used?
					s_vboIndices->m_indexType = D3D11_BIND_INDEX_BUFFER;

					ARK2D::getLog()->v("Created Matrix Buffer.");
				#endif

				s_vboMatrixProjection->init();
				s_vboMatrixModelView->init();

				s_vaoQuad->init();

				s_vboQuadVerts->init();
				s_vboQuadVerts->setWidth(2);
				s_vboQuadVerts->setHeight(4);
			
				s_vboQuadTexCoords->init();
				s_vboQuadTexCoords->setWidth(2);
				s_vboQuadTexCoords->setHeight(4);
				
				s_vboQuadColors->init();
				s_vboQuadColors->setWidth(4); 
				s_vboQuadColors->setHeight(4);

				s_vboIndices->setSize(sizeof(unsigned int) * 6); 
				s_vboIndices->init();

				showAnyGlErrorAndExitMacro();


				// Regular geometry shader
				ARK2D::getLog()->v("Geometry Shader: Init"); 
				Shader* s = new Shader();
				#if defined(ARK2D_RENDERER_DIRECTX) 
					
					#ifdef ARK2D_WINDOWS_PHONE_8
					
						file_get_contents_binary_result geometryVertexShaderStruct = FileUtil::file_get_contents_binary("ark2d/shaders/geometry-dx11-vertex.cso");
						file_get_contents_binary_result geometryFragmentShaderStruct = FileUtil::file_get_contents_binary("ark2d/shaders/geometry-dx11-pixel.cso");

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

					s->addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
					s->addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen); 
					
					//ARK2D::getLog()->v(StringUtil::append("Vertex Resource Size (4): ", geometryVertexShaderLen));
					//ARK2D::getLog()->v(StringUtil::append("Fragment Resource Size (4): ", geometryFragmentShaderLen));
					
				#elif defined(ARK2D_RENDERER_OPENGL)
					#if defined(ARK2D_OPENGL_ES_2_0)

						#ifdef ARK2D_ANDROID
							// TODO:
							// ... use Resource::get() as these are always within the APK.

							RawDataReturns* geometryVertexShaderStruct = Resource::getRawData("assets/ark2d/shaders/geometry-glsles100.vertex");
							RawDataReturns* geometryFragmentShaderStruct = Resource::getRawData("assets/ark2d/shaders/geometry-glsles100.fragment"); //->asString();

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

							s->addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
							s->addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen);

							delete geometryVertexShaderStruct;
							delete geometryFragmentShaderStruct;

						#else

							file_get_contents_text_result geometryVertexShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/geometry-glsles100.vertex");
							file_get_contents_text_result geometryFragmentShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/geometry-glsles100.fragment");
							
							const char* geometryVertexShader = (const char*) geometryVertexShaderStruct.data;
							const char* geometryFragmentShader = (const char*) geometryFragmentShaderStruct.data;
								
							unsigned int geometryVertexShaderLen = geometryVertexShaderStruct.len; //strlen(geometryVertexShader);
							unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct.len; //strlen(geometryFragmentShader);
	 
							s->addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
							s->addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen); 

						#endif

					#elif defined(ARK2D_OPENGL_3_2)
						ARK2D::getLog()->v("Trying GLSL version 150.");
						file_get_contents_text_result geometryVertexShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/geometry-glsl150-vertex.txt");
						file_get_contents_text_result geometryFragmentShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/geometry-glsl150-fragment.txt");

						const char* geometryVertexShader = (const char*) geometryVertexShaderStruct.data;
						const char* geometryFragmentShader = (const char*) geometryFragmentShaderStruct.data;
							
						unsigned int geometryVertexShaderLen = geometryVertexShaderStruct.len; 
						unsigned int geometryFragmentShaderLen = geometryFragmentShaderStruct.len; 

						bool glsl150_vertex_error = false;
						bool glsl150_fragment_error = false;
						
						s->addVertexShaderFromData((void*) geometryVertexShader, geometryVertexShaderLen);
						glsl150_vertex_error = s->hasError();

						s->addFragmentShaderFromData((void*) geometryFragmentShader, geometryFragmentShaderLen); 
						glsl150_fragment_error = s->hasError();

						if (glsl150_vertex_error || glsl150_fragment_error) {
							ARK2D::getLog()->w("GLSL version 150 had a problem. Trying GLSL version 130.");
							file_get_contents_text_result geometryVertexShaderStruct2 = FileUtil::file_get_contents_text("ark2d/shaders/geometry-glsl130-vertex.txt");
							file_get_contents_text_result geometryFragmentShaderStruct2 = FileUtil::file_get_contents_text("ark2d/shaders/geometry-glsl130-fragment.txt");

							const char* geometryVertexShader2 = (const char*) geometryVertexShaderStruct2.data;
							const char* geometryFragmentShader2 = (const char*) geometryFragmentShaderStruct2.data;
								
							unsigned int geometryVertexShaderLen2 = geometryVertexShaderStruct2.len; 
							unsigned int geometryFragmentShaderLen2 = geometryFragmentShaderStruct2.len; 

							s->addVertexShaderFromData((void*) geometryVertexShader2, geometryVertexShaderLen2);
							s->addFragmentShaderFromData((void*) geometryFragmentShader2, geometryFragmentShaderLen2); 

							delete geometryVertexShaderStruct2.data;
							delete geometryFragmentShaderStruct2.data;
						}
					#endif
				#endif

				
				//ARK2D::getLog()->v(StringUtil::append("Vertex Shader len:", geometryVertexShaderLen));
				//ARK2D::getLog()->v(StringUtil::append("Fragment Shader len:", geometryFragmentShaderLen));
				
				
				
				//#ifdef ARK2D_MACINTOSH
				//	glBindAttribLocation();
				//#endif 


				#ifdef ARK2D_RENDERER_DIRECTX
					s->setName("geometry-dx11");
				#endif
				//glBindAttribLocation(s->getId(), 0, "ark_ModelViewMatrix");
				//glBindAttribLocation(s->getId(), 1, "ark_ProjectionMatrix");
				ARK2D::getLog()->v("Geometry Shader: Bind Locations");
				s->bindAttributeLocation(0, "ark_VertexPosition");
				s->bindAttributeLocation(1, "ark_VertexColorIn");
				s->bindFragmentDataLocation(0, "ark_FragColor");

				//glEnableVertexAttribArray(s_shaderBasicGeometry_VertexPosition);
				//glEnableVertexAttribArray(s_shaderBasicGeometry_VertexColorIn);

				s->link(); 
				s->linkDX();
 
 				
				

				RendererState::start(RendererState::SHADER, s->getId());
				 
				showAnyGlErrorAndExitMacro();
				/*s->initVariables();
				s_shaderBasicGeometry_ProjectionMatrix = s->getInittedVariable("ark_ProjectionMatrix");
				s_shaderBasicGeometry_ModelViewMatrix = s->getInittedVariable("ark_ModelViewMatrix");
				s_shaderBasicGeometry_VertexPosition = s->getInittedVariable("ark_VertexPosition");
				s_shaderBasicGeometry_VertexColorIn = s->getInittedVariable("ark_VertexColorIn");*/

				s_shaderBasicGeometry_ModelViewMatrix = s->getUniformVariable("ark_ModelViewMatrix");
				s_shaderBasicGeometry_ProjectionMatrix = s->getUniformVariable("ark_ProjectionMatrix");
				s_shaderBasicGeometry_VertexPosition = 0; //s->getAttributeVariableVertexArray("ark_VertexPosition");
				s_shaderBasicGeometry_VertexColorIn = 1; //s->getAttributeVariableVertexArray("ark_VertexColorIn");
			 
				//s->enableVertexAttribArray(0);
				//s->enableVertexAttribArray(1);

				s_shaderBasicGeometry = s; 
				RendererState::start(RendererState::GEOMETRY);
 
				showAnyGlErrorAndExitMacro();

				// Texture Shader
				ARK2D::getLog()->v("Texture Shader Init");
				s = new Shader();
				#if defined(ARK2D_RENDERER_DIRECTX)

					// TODO: replace with texture shader...
					#ifdef ARK2D_WINDOWS_PHONE_8
						
						file_get_contents_binary_result textureVertexShaderStruct = FileUtil::file_get_contents_binary("ark2d/shaders/texture-dx11-vertex.cso");
						file_get_contents_binary_result textureFragmentShaderStruct = FileUtil::file_get_contents_binary("ark2d/shaders/texture-dx11-pixel.cso");

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

					s->addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
					s->addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

				#elif defined(ARK2D_RENDERER_OPENGL)
					#if defined(ARK2D_OPENGL_ES_2_0)

						#ifdef ARK2D_ANDROID
							// TODO:
							// ... use Resource::get() as these are always within the APK.

							RawDataReturns* textureVertexShaderStruct = Resource::getRawData("assets/ark2d/shaders/texture-glsles100.vertex"); //->asString();
							RawDataReturns* textureFragmentShaderStruct = Resource::getRawData("assets/ark2d/shaders/texture-glsles100.fragment"); //->asString();

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

							s->addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
							s->addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

							delete textureVertexShaderStruct;
							delete textureFragmentShaderStruct;
						#else

							file_get_contents_text_result textureVertexShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/texture-glsles100.vertex");
							file_get_contents_text_result textureFragmentShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/texture-glsles100.fragment");

							const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
							const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;
								
							unsigned int textureVertexShaderLen = textureVertexShaderStruct.len; //strlen(textureVertexShader);
							unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len; //strlen(textureFragmentShader);

							s->addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
							s->addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);

						#endif

						

					#elif defined(ARK2D_OPENGL_3_2)  
						ARK2D::getLog()->v("Trying GLSL version 150.");
						file_get_contents_text_result textureVertexShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/texture-glsl150-vertex.txt");
						file_get_contents_text_result textureFragmentShaderStruct = FileUtil::file_get_contents_text("ark2d/shaders/texture-glsl150-fragment.txt");
	 
						const char* textureVertexShader = (const char*) textureVertexShaderStruct.data;
						const char* textureFragmentShader = (const char*) textureFragmentShaderStruct.data;
							
						unsigned int textureVertexShaderLen = textureVertexShaderStruct.len; //strlen(textureVertexShader);
						unsigned int textureFragmentShaderLen = textureFragmentShaderStruct.len; //strlen(textureFragmentShader);

						bool glsl150_t_vertex_error = false;
						bool glsl150_t_fragment_error = false;

						s->addVertexShaderFromData((void*) textureVertexShader, textureVertexShaderLen);
						glsl150_t_vertex_error = s->hasError();

						s->addFragmentShaderFromData((void*) textureFragmentShader, textureFragmentShaderLen);
						glsl150_t_fragment_error = s->hasError();

						if (glsl150_t_vertex_error || glsl150_t_fragment_error) {
							ARK2D::getLog()->w("GLSL version 150 had a problem. Trying GLSL version 130.");
							file_get_contents_text_result textureVertexShaderStruct2 = FileUtil::file_get_contents_text("ark2d/shaders/texture-glsl130-vertex.txt");
							file_get_contents_text_result textureFragmentShaderStruct2 = FileUtil::file_get_contents_text("ark2d/shaders/texture-glsl130-fragment.txt");
		 
							const char* textureVertexShader2 = (const char*) textureVertexShaderStruct2.data;
							const char* textureFragmentShader2 = (const char*) textureFragmentShaderStruct2.data;
								
							unsigned int textureVertexShaderLen2 = textureVertexShaderStruct2.len; //strlen(textureVertexShader);
							unsigned int textureFragmentShaderLen2 = textureFragmentShaderStruct2.len; //strlen(textureFragmentShader);

							s->addVertexShaderFromData((void*) textureVertexShader2, textureVertexShaderLen2);
							s->addFragmentShaderFromData((void*) textureFragmentShader2, textureFragmentShaderLen2);

							delete textureVertexShaderStruct2.data;
							delete textureFragmentShaderStruct2.data;
						}

					#endif
				#endif
				  

				
				//ARK2D::getLog()->v(StringUtil::append("Vertex Shader len:", textureVertexShaderLen));
				//ARK2D::getLog()->v(StringUtil::append("Fragment Shader len:", textureFragmentShaderLen));
				
				
				showAnyGlErrorAndExitMacro();
				
				//#ifdef ARK2D_MACINTOSH 
				//glBindAttribLocation(s->getId(), 0, "ark_ModelViewMatrix");
				//glBindAttribLocation(s->getId(), 1, "ark_ProjectionMatrix");
				ARK2D::getLog()->v("Texture Shader: Bind Locations");
				s->bindAttributeLocation(0, "ark_TextureId");
				s->bindAttributeLocation(1, "ark_VertexPosition");
				s->bindAttributeLocation(2, "ark_VertexTexCoordIn");
				s->bindAttributeLocation(3, "ark_VertexColorIn");
				s->bindFragmentDataLocation(0, "ark_FragColor");
				//#endif

				#ifdef ARK2D_RENDERER_DIRECTX
					s->setName("texture-dx11");
				#endif

				
				s->link();
				s->linkDX();

				RendererState::start(RendererState::SHADER, s->getId());

				
				  
				showAnyGlErrorAndExitMacro(); 

				s_shaderBasicTexture_ModelViewMatrix = s->getUniformVariable("ark_ModelViewMatrix");
				s_shaderBasicTexture_ProjectionMatrix = s->getUniformVariable("ark_ProjectionMatrix");
				
				s_shaderBasicTexture_TextureId = s->getUniformVariable("ark_TextureId");
				s_shaderBasicTexture_VertexPosition = 1; //s->getAttributeVariableVertexArray("ark_VertexPosition");
				s_shaderBasicTexture_VertexTexCoordIn = 2; //s->getAttributeVariableVertexArray("ark_VertexTexCoordIn");
				s_shaderBasicTexture_VertexColorIn = 3; //s->getAttributeVariableVertexArray("ark_VertexColorIn");
				
				//s->enableVertexAttribArray(1);
				//s->enableVertexAttribArray(2);
				//s->enableVertexAttribArray(3);

 				RendererState::start(RendererState::GEOMETRY);
				s_shaderBasicTexture = s; 

				// Free String* memory.
				//#ifdef ARK2D_RENDERER_DIRECTX
					//#ifdef ARK2D_WINDOWS_PHONE_8
					#ifdef ARK2D_ANDROID
					//	delete geometryVertexShaderStruct;
					//	delete geometryFragmentShaderStruct;

					//	delete textureVertexShaderStruct;
					//	delete textureFragmentShaderStruct;
					#else
						delete geometryVertexShaderStruct.data;
						delete geometryFragmentShaderStruct.data;

						delete textureVertexShaderStruct.data;
						delete textureFragmentShaderStruct.data;
					#endif
				//	#endif
				//#endif


				showAnyGlErrorAndExitMacro();

			#endif
			ARK2D::getLog()->v("Renderer::init complete");
			showAnyGlErrorAndExitMacro();
		} 

		void* Renderer::readPixels(int x, int y, int w, int h) {
			//void* data[w * h];// = malloc(w * h * 8);
			//glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	
			GLubyte* pixels = NULL;
			/*{
				int size = w * h * 4;
				GLubyte* pixels = new GLubyte[size];
				if (pixels == NULL) return NULL; 
				glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			}*/

			#if defined(ARK2D_RENDERER_OPENGL)
				{
					int size = (w * h) * 4; 
					pixels = new GLubyte[size];
					if (pixels == NULL) return NULL;  
					glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
					RendererStats::s_glCalls++;

					showAnyGlErrorAndExitMacro();
				}
			#endif

			//delete[] pixels; 
			return pixels;
		} 
		void Renderer::flush() {
			#if defined(ARK2D_RENDERER_OPENGL)
				glFlush();
			#elif defined(ARK2D_RENDERER_DIRECTX)
				ARK2D::getContainer()->m_platformSpecific.m_deviceContext->Flush();
			#endif
			RendererStats::s_glCalls++;
		}
		void Renderer::finish() {
			#if defined(ARK2D_RENDERER_OPENGL)
				glFinish();
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
			RendererStats::s_glCalls++;
		}

		void Renderer::setFont(ARK::Font::Font* f) {
			m_Font = f;
		} 
		void Renderer::setDefaultFont(ARK::Font::Font* f) {
			m_DefaultFont = f;
		}

		ARK::Font::Font* Renderer::getFont() const {
			return m_Font;
		}
		ARK::Font::Font* Renderer::getDefaultFont() const {
			return m_DefaultFont;
		} 

		void Renderer::translate(int x, int y) const {
			// glTranslatef(x,y,0);
			s_matrix->translate(x, y, 0);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLoadMatrixf((float*)s_matrix->pointer());
				glTranslatef(x, y, 0);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		} 
		void Renderer::translate(float x, float y) const {
			//glTranslatef(x,y,0);
			s_matrix->translate(x, y, 0.0f);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLoadMatrixf((float*)s_matrix->pointer());
				glTranslatef(x, y, 0);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		} 
		void Renderer::translate(float x, float y, float z) const {
			s_matrix->translate(x, y, z);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glTranslatef(x, y, 0);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}

		void Renderer::rotate(int angle) const {  
			//glRotatef(angle, 0, 0, 1); 
			s_matrix->rotate(angle, 0, 0, 1);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLoadMatrixf((float*)s_matrix->pointer());
				glRotatef(angle, 0, 0, 1);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::rotate(float angle) const { 
			//glRotatef(angle, 0, 0, 1);
			s_matrix->rotate(angle, 0, 0, 1);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLoadMatrixf((float*)s_matrix->pointer());
				glRotatef(angle, 0, 0, 1);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::rotate(float angle, float x, float y, float z) const {
			s_matrix->rotate(angle, x, y, z);
		
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLoadMatrixf((float*)s_matrix->pointer());	
				glRotatef(angle, x, y, z);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::scale(float x, float y) const {
			//glScalef(x, y, 0.0f);
			s_matrix->scale(x, y, 1.0f);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLoadMatrixf((float*)s_matrix->pointer());
				glScalef(x, y, 1.0f);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::scale(float x, float y, float z) const {
			s_matrix->scale(x, y, z);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glScalef(x, y, z);
				showAnyGlErrorAndExitMacro();
				RendererStats::s_glCalls++;
			#endif
		}

		// STENCIL THINGS
		void Renderer::enableStencil() const {
			#ifdef ARK2D_RENDERER_OPENGL
				// Clear stencil buffer (but not color)
				glClear( GL_STENCIL_BUFFER_BIT );
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::startStencil() const {
			#ifdef ARK2D_RENDERER_OPENGL
				// Disable rendering to the color buffer
			    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

			    // Start using the stencil
			    glEnable( GL_STENCIL_TEST );

			    // Place a 1 where rendered
			    glStencilFunc( GL_ALWAYS, 1, 1 );
			   
			    // Replace where rendered
			    glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );

			    RendererStats::s_glCalls += 4;
		    #endif
		}
		/*void Renderer::inverseStencil() const {
			#ifdef ARK2D_RENDERER_OPENGL
				// Place a 1 where rendered
			    glStencilFunc( GL_ALWAYS, 1, 0 );

			    // Replace where rendered
			    glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );

			    RendererStats::s_glCalls += 4;
		    #endif
		}*/
		void Renderer::stopStencil() const {
			#ifdef ARK2D_RENDERER_OPENGL
				// Reenable color
			    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

			    // Where a 1 was rendered
			    glStencilFunc( GL_EQUAL, 1, 1 ); // GL_EQUAL...?

			    // Keep the pixel
			    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

			    RendererStats::s_glCalls += 3;
		    #endif
		}
		void Renderer::disableStencil() const {
			#ifdef ARK2D_RENDERER_OPENGL
				// Finished using stencil
				glDisable( GL_STENCIL_TEST );
				RendererStats::s_glCalls++;
			#endif
		}

		// MULTISAMPLING
		void Renderer::enableMultisampling() const {
			#ifdef ARK2D_RENDERER_OPENGL
				#ifdef ARK2D_OPENGL_3_2
					glEnable(GL_MULTISAMPLE);
				#endif
			#endif
		}
		void Renderer::disableMultisampling() const {
			#ifdef ARK2D_RENDERER_OPENGL
				#ifdef ARK2D_OPENGL_3_2
					glDisable(GL_MULTISAMPLE);
				#endif
			#endif
		}
		
		// SCISSORING
		void Renderer::setScissorTestEnabled(bool b) const {
			#ifdef ARK2D_RENDERER_OPENGL
				if (b) {
					glEnable(GL_SCISSOR_TEST);
				} else {
					glDisable(GL_SCISSOR_TEST);
				}
			#elif defined(ARK2D_RENDERER_DIRECTX)

				if (!b) {
					GameContainer* container = ARK2D::getContainer();
					GameContainerPlatform* platform = container->getPlatformSpecific();
					ID3D11DeviceContext* deviceContext = platform->m_deviceContext;
					D3D11_RECT rects[1];
					rects[0].left = 0;
					rects[0].right = platform->convertDipsToPixels(container->getDynamicWidth());
					rects[0].top = 0;
					rects[0].bottom = platform->convertDipsToPixels(container->getDynamicHeight());
					deviceContext->RSSetScissorRects( 1, rects );
				}

			#endif

			RendererStats::s_glCalls++;
		}
		void Renderer::scissor(int x, int y, int w, int h) const {
			#ifdef ARK2D_RENDERER_OPENGL
				//glScissor(x, y - (signed int) ARK2D::getContainer()->getHeight(), w, h * -1);
				//glScissor(x, ARK2D::getContainer()->getHeight() - (y + h), w, h);
				glScissor(x, ARK2D::getContainer()->getDynamicHeight() - (y + h), w, h);
				RendererStats::s_glCalls++;
			#elif defined(ARK2D_RENDERER_DIRECTX)

				GameContainerPlatform* platform = ARK2D::getContainer()->getPlatformSpecific();
				ID3D11DeviceContext* deviceContext = platform->m_deviceContext;
				D3D11_RECT rects[1];
				rects[0].left = platform->convertDipsToPixels(max(x, 0));
				rects[0].right = platform->convertDipsToPixels(max(x + w, 0));
				rects[0].top = platform->convertDipsToPixels(max(y, 0));
				rects[0].bottom = platform->convertDipsToPixels(max(y + h, 0));
				deviceContext->RSSetScissorRects( 1, rects );

			#endif
		}
		void Renderer::viewport(int x, int y, int w, int h) const {
			#if defined(ARK2D_RENDERER_OPENGL)
				glViewport(x, y, w, h);
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
			RendererStats::s_glCalls++;
		}
		void Renderer::ortho2d(int x, int y, int w, int h) {
			ortho2d(x, y, w, h, -1.0f, 1.0f);
		}
		void Renderer::ortho2d(int x, int y, int w, int h, float n, float f) {
			//
			//glOrtho(0, w, h, 0, -1, 1);
		
			//ARK2D::getContainer()->disable2D();
			//ARK2D::getContainer()->enable2D();
			
			//#if (defined(ARK2D_WINDOWS) || defined(ARK2D_FLASCC))
			#if (defined(ARK2D_FLASCC))		
				ARK2D::getLog()->v("ortho2d");
				string ortho2dStr = "";
				ortho2dStr += "l: ";
				ortho2dStr += Cast::toString<double>(x);
				ortho2dStr += " r: ";  
				ortho2dStr += Cast::toString<double>(x + w);
				ortho2dStr += " b: ";
				ortho2dStr += Cast::toString<double>(y + h);
				ortho2dStr += " t: "; 
				ortho2dStr += Cast::toString<double>(y);
				ortho2dStr += " n: ";
				ortho2dStr += Cast::toString<float>(n);
				ortho2dStr += " f: "; 
				ortho2dStr += Cast::toString<float>(f);
				ARK2D::getLog()->v(ortho2dStr);

				s_matrix->ortho2d(float(x), float(x + w), float(y + h), float(y), n, f);
				glOrtho(
					double(x), 
					double(x + w), 
					double(y + h),  
					double(y),
					double(n),
					double(f)
				);
				//glLoadMatrixf((float*)s_matrix->pointer());	
		
			#else 
				//ARK2D::getLog()->i("ortho2d");  
				s_matrix->ortho2d(x, (x + w), (y + h), y, n, f);
				
				#if !defined(NO_FIXED_FUNCTION_PIPELINE)
					//glOrtho(x, x + w, y + h, y, n, f); 

					glOrtho(x, (x + w), (y + h), y, n, f);

				#endif

			#endif

			showAnyGlErrorAndExitMacro();
			RendererStats::s_glCalls++;

			//s_matrix->pointer()->ortho(x, x + w, y + h, y, near, far);
		}

		void Renderer::drawString(const std::string str, int x, int y) const {
			if (m_Font == NULL) { ARK2D::getLog()->e("Renderer::m_Font is NULL..."); return; }
			//ARK2D::getLog()->i("draw string"); 
			//m_Font->asBMFont()->getImage()->draw(x, y);
			m_Font->drawString(str, x, y); 
		}
		void Renderer::drawString(const std::string str, float x, float y, signed int alignX, signed int alignY, float rotation, float sc) 
		{
			if (m_Font == NULL && m_DefaultFont == NULL) { return; }
			
			ARK::Font::Font* thisFont = m_Font;
			if (m_Font == NULL) {
				thisFont = m_DefaultFont;
			}
			float strWidth = float(thisFont->getStringWidth(str)) * sc;
			float strHeight = float(thisFont->getLineHeight()) * sc;

			//if (rotation != 0 && alignY == ALIGN_CENTER) // there's gotta be a better way of calculating height of rotated rectangle.
			//{ 
				/*float cx = x + (strWidth/2);
				float cy = y + (strHeight/2);
				Vector2<float> tl(x, y);
				Vector2<float> bl(x, y + strHeight);
				Vector2<float> br(x + strWidth, y + strHeight);
				Vector2<float> tr(x + strWidth, y);
				Vector2<float> c(cx, cy);

				MathUtil::rotatePointAroundPoint<float>(&tl, &c, rotation);
				MathUtil::rotatePointAroundPoint<float>(&bl, &c, rotation);
				MathUtil::rotatePointAroundPoint<float>(&br, &c, rotation);
				MathUtil::rotatePointAroundPoint<float>(&tr, &c, rotation);
				
				vector<Vector2<float>* > coords();
				coords.push_back(&tl);  
				coords.push_back(&bl); 
				coords.push_back(&br);  
				coords.push_back(&tr);

				Vector2<float> mincoord = *(MathUtil::minY<float>(coords));
				Vector2<float> maxcoord = *(MathUtil::maxY<float>(coords));

				strHeight = maxcoord.getY() - mincoord.getY();*/
				//strHeight = strHeight * (abs(rotation)*0.1f);
			//}

			// Alignment
			/*if (alignX == ALIGN_CENTER) {
				x -= strWidth/2;
			} else if (alignX == ALIGN_RIGHT || alignX == ALIGN_END) {
				x -= strWidth; 
			}

			if (alignY == ALIGN_CENTER) {
				y -= strHeight/2;
			} else if (alignY == ALIGN_END || alignY == ALIGN_BOTTOM) {
				y -= strHeight;
			}*/ 

			float inv_scale = 1.0f / sc;

			float offsetX = 0.0f;
			if (alignX == ALIGN_CENTER) { 
				offsetX = (strWidth/2.0f) * -1.0f;	
			} else if (alignX == ALIGN_LEFT || alignX == ALIGN_START) {
				
			} else if (alignX == ALIGN_RIGHT || alignX == ALIGN_END) {
				offsetX = strWidth * -1.0f;
			} 

			float offsetY = 0.0f;
			if (alignY == ALIGN_CENTER) { 
				offsetY = (strHeight/2.0f) * -1.0f;	
			} else if (alignY == ALIGN_START || alignY == ALIGN_TOP) {
				
			} else if (alignY == ALIGN_END || alignY == ALIGN_BOTTOM) {
				offsetY = strHeight * -1.0f;
			} 


			pushMatrix();
			translate(x, y); 

			bool doRot = false;
			if (rotation != 0.0f) { 
				doRot = true;
				pushMatrix();
				rotate(rotation);
			}
			
			pushMatrix();

			if (sc != 1.0f) 
				scale(sc, sc);



			thisFont->drawString(str, int(0 + (offsetX*inv_scale)), int(0 + (offsetY*inv_scale)));

			//if (sc != 1.0f) 
			//	scale(1.0f/sc, 1.0f/sc);
			
			popMatrix();

			if (doRot)
				popMatrix();

			//if (rotation != 0.0f)
			//	rotate(rotation * -1);

			//translate(x * -1, y * -1, 0);
			

			popMatrix();

		}
		void Renderer::drawStringCenteredAt(const std::string str, int x, int y) const {
			m_Font->drawStringCenteredAt(str, x, y);
		}
		void Renderer::drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing) const {
			
			drawStringWordWrap(str, x, y, maxWidth, ySpacing, Renderer::ALIGN_LEFT);
		}

		void Renderer::drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing, int alignX) const {
			
			Renderer* r = const_cast<Renderer*>(this);

			int drawX = x; 

			if (alignX == Renderer::ALIGN_CENTER) {
				drawX += maxWidth/2;
			} else if (alignX == Renderer::ALIGN_RIGHT) { 
				drawX += maxWidth;
			}

			vector<string> lines = StringUtil::getLinesForWordWrap(str, maxWidth);
			for (unsigned int i = 0; i < lines.size(); i++) {
				r->drawString(lines.at(i), drawX, y, alignX, Renderer::ALIGN_CENTER, 0.0f, 1.0f);
				y += m_Font->getStringHeight(lines.at(i)) + ySpacing;
			}
		}

		
		void Renderer::readPixels(void* data, int x, int y, int w, int h) {
			#if defined(ARK2D_RENDERER_OPENGL)
				glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
			#endif
		}

		void Renderer::drawLine(float x1, float y1, float x2, float y2) const {
			drawLine((int)x1,(int)y1,(int)x2,(int)y2);
		}
		void Renderer::drawLine(int x1, int y1, int x2, int y2) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_FLASCC) || defined(NO_FIXED_FUNCTION_PIPELINE) || defined(ARK2D_WINDOWS))
				float angle = MathUtil::anglef(x1, y1, x2, y2, true);

				float topLeftX = x1; 
				float topLeftY = y1;
				float topRightX = x1;
				float topRightY = y1; 

				float bottomLeftX = x2;
				float bottomLeftY = y2;
				float bottomRightX = x2;
				float bottomRightY = y2;
				
				MathUtil::moveAngle(topLeftX, topLeftY, angle - 90, m_LineWidth/2.0f);
				MathUtil::moveAngle(topRightX, topRightY, angle + 90, m_LineWidth/2.0f);

				MathUtil::moveAngle(bottomLeftX, bottomLeftY, angle - 90, m_LineWidth/2.0f);
				MathUtil::moveAngle(bottomRightX, bottomRightY, angle + 90, m_LineWidth/2.0f);
				
				fillQuad(topLeftX, topLeftY, bottomLeftX, bottomLeftY, topRightX, topRightY, bottomRightX, bottomRightY);
			#else
				RendererState::start(RendererState::GEOMETRY);
				glBegin(GL_LINES);
					glVertex2i(x1, y1);
					glVertex2i(x2, y2);
				glEnd();

				RendererStats::s_lines++;
				RendererStats::s_glCalls += 4;
			#endif

			showAnyGlErrorAndExitMacro();
		}
		// image, scale... line coordinates. image line subcoordinates.
		void Renderer::texturedLineOverlay_data (
			float* data, 
			Image* img, 
			float scale, 
			float x1, float y1, 
			float x2, float y2,
			float startX, float startY, 
			float endX, float endY) 
		{
			Vector2<float> tempVector(0, 0);
			Vector2<float> tempVector2(0, 0);

			float lineAngle = MathUtil::anglef(x1, y1, x2, y2);

			// top left. 
			tempVector.set( (startX * -1) * scale, (startY * -1) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float tl_x = x1 + tempVector.getX();
			float tl_y = y1 + tempVector.getY();

			// bottom left. 
			tempVector.set( (startX * -1) * scale, (img->getHeight() - startY) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float bl_x = x1 + tempVector.getX();
			float bl_y = y1 + tempVector.getY();

			// top right. 
			tempVector.set( (img->getWidth() - endX) * scale, (endY * -1) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float tr_x = x2 + tempVector.getX();
			float tr_y = y2 + tempVector.getY();

			// bottom right. 
			tempVector.set( (img->getWidth() - endX) * scale, (img->getHeight() - endY) * scale);
			MathUtil::rotatePointAroundPoint(&tempVector, &tempVector2, lineAngle);
			float br_x = x2 + tempVector.getX();
			float br_y = y2 + tempVector.getY();

			data[0] = tl_x;
			data[1] = tl_y;
			data[2] = bl_x;
			data[3] = bl_y;
			data[4] = tr_x;
			data[5] = tr_y;
			data[6] = br_x;
			data[7] = br_y;
		}

		// image, scale... line coordinates. image line subcoordinates.
		void Renderer::texturedLineOverlay(
			Image* img, 
			float scale, 
			float x1, float y1, 
			float x2, float y2,
			float startX, float startY, 
			float endX, float endY) 
		{
			float data[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
			texturedLineOverlay_data(&data[0], img, scale, x1, y1, x2, y2, startX, startY, endX, endY);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glColor4f(1.0f, 1.0f, 1.0f, img->getAlpha());
				RendererStats::s_glCalls++;
			#endif
			texturedQuad(img->getTexture()->getId(), 
				data[0], data[1], 
				data[2], data[3], 
				data[4], data[5], 
				data[6], data[7], 
				img->getTextureX(), img->getTextureY(), 
				img->getTextureX(), img->getTextureY() + img->getTextureH(),
				img->getTextureX() + img->getTextureW(), img->getTextureY(),
				img->getTextureX() + img->getTextureW(), img->getTextureY() + img->getTextureH()
				
			);
			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				RendererStats::s_glCalls++;
			#endif
		}

		void Renderer::fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle) const {
			fillArc(cx, cy, width, height, startAngle, endAngle, DEFAULT_SEGMENTS);
		}
		void Renderer::fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle, int segs) const {
			if (segs == 0) { return; }

			#if defined(ARK2D_RENDERER_OPENGL) && defined(ARK2D_OPENGL_3_2)

				
				float halfwidth = width / 2.0f; 
				float halfheight = height / 2.0f;

				float diffAngle = endAngle - startAngle;
				float eachAngle = diffAngle / float(segs);

				#ifdef ARK2D_WINDOWS_VS
					float* rawVertices = (float*) alloca((segs * 3) * 2 * sizeof(float));
					unsigned char* rawColors = (unsigned char*) alloca((segs * 3) * 4 * sizeof(unsigned char));
				#else
					float rawVertices[(segs*3)*2];
					unsigned char rawColors[(segs*3)*4];
				#endif

				int j = 0;
				for (float i = 0; i < segs; i++) {
					float localStartAngle = startAngle + (i*eachAngle);
					float nextAngle = localStartAngle + eachAngle;
					if (nextAngle > endAngle) {
						nextAngle = endAngle;
					}

					double angle = 2 * MY_PI * localStartAngle / 360; 
					double nangle = 2 * MY_PI * (nextAngle) / 360; 

					rawVertices[j] = cx;
					rawVertices[j+1] = cy;

					rawVertices[j+2] = cx + (cos(angle) * halfwidth);
					rawVertices[j+3] = cy + (sin(angle) * halfheight);

					rawVertices[j+4] = cx + (cos(nangle) * halfwidth);
					rawVertices[j+5] = cy + (sin(nangle) * halfheight);
 
					j+=6;
				}


				
				for(unsigned int i = 0; i < ((segs*3)*4); i += 4 ) {
					rawColors[i+0] = m_DrawColor.getRedc();
					rawColors[i+1] = m_DrawColor.getGreenc();
					rawColors[i+2] = m_DrawColor.getBluec(); 
					rawColors[i+3] = m_DrawColor.getAlphac();
				}

				RendererState::start(RendererState::GEOMETRY);

				s_vboQuadVerts->setWidth(2);
				s_vboQuadVerts->setHeight(segs*3);
				s_vboQuadColors->setWidth(4); 
				s_vboQuadColors->setHeight(segs*3); 
			
				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
		
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, segs*3); 

				s_vboQuadVerts->setHeight(4);
				s_vboQuadColors->setHeight(4);

				RendererStats::s_glCalls += 7;
				RendererStats::s_tris += segs;

			#elif (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(NO_FIXED_FUNCTION_PIPELINE))
				// TODO...
			#else

				int halfwidth = width/2;
				int halfheight = height/2;

				glBegin(GL_TRIANGLE_FAN);

					glVertex2f(cx, cy); 

					int step = 360 / segs;

					for (int i = (int) startAngle; i < (endAngle + step); i += step) {
						float a = i;
						if (a > endAngle) {
							a = endAngle;
						}
						float x = cx + cos(MathUtil::toRadians(a)) * halfwidth;
						float y = cy + (sin(MathUtil::toRadians(a)) * halfheight * -1); // we want to stick to the angles scheme we have going.

						glVertex2f(x, y);
						RendererStats::s_glCalls++;
					}

				glEnd();

				RendererStats::s_tris += segs;
				RendererStats::s_glCalls += 3;

			#endif
		}

		//void Renderer::fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle, int segs) const {
		//	if (segs == 0) { return; }

		//	#if defined(ARK2D_RENDERER_OPENGL) && defined(ARK2D_OPENGL_3_2)

		//		float halfwidth = width / 2.0f; 
		//		float halfheight = height / 2.0f;

		//		float diffAngle = endAngle - startAngle;
		//		float eachAngle = diffAngle / float(segs);

		//		float rawVertices[(segs*3)*2];
		//		unsigned char rawColors[(segs*3)*4];

		//		int j = 0;
		//		for (float i = 0; i < segs; i++) {
		//			float localStartAngle = startAngle + (i*eachAngle);
		//			float nextAngle = localStartAngle + eachAngle;
		//			if (nextAngle > endAngle) {
		//				nextAngle = endAngle;
		//			}

		//			double angle = 2 * MY_PI * localStartAngle / 360; 
		//			double nangle = 2 * MY_PI * (nextAngle) / 360; 

		//			rawVertices[j] = cx;
		//			rawVertices[j+1] = cy;

		//			rawVertices[j+2] = cx + (cos(angle) * halfwidth);
		//			rawVertices[j+3] = cy + (sin(angle) * halfheight);

		//			rawVertices[j+4] = cx + (cos(nangle) * halfwidth);
		//			rawVertices[j+5] = cy + (sin(nangle) * halfheight);
 
		//			j+=6;
		//		}


		//		
		//		for(unsigned int i = 0; i < ((segs*3)*4); i += 4 ) {
		//			rawColors[i+0] = m_DrawColor.getRedc();
		//			rawColors[i+1] = m_DrawColor.getGreenc();
		//			rawColors[i+2] = m_DrawColor.getBluec(); 
		//			rawColors[i+3] = m_DrawColor.getAlphac();
		//		}

		//		RendererState::start(RendererState::GEOMETRY);

		//		s_vboQuadVerts->setWidth(2);
		//		s_vboQuadVerts->setHeight(segs*3);
		//		s_vboQuadColors->setWidth(4); 
		//		s_vboQuadColors->setHeight(segs*3); 
		//	
		//		s_vaoQuad->bind();
		//		glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
		//		glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
		//
		//		s_vboQuadVerts->bind();
		//		s_vboQuadVerts->setData(&rawVertices[0]);
		//		glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
		//		
		//		s_vboQuadColors->bind();
		//		s_vboQuadColors->setData(&rawColors[0]);
		//		glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
		//		
		//		glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
		//		glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

		//		glDrawArrays(GL_TRIANGLES, 0, segs*3); 

		//		s_vboQuadVerts->setHeight(4);
		//		s_vboQuadColors->setHeight(4);

		//		RendererStats::s_glCalls += 7;
		//		RendererStats::s_tris += segs;

		//	#elif defined(ARK2D_RENDERER_OPENGL) && defined(ARK2D_OPENGL_ES_2_0)
		//		// TODO...
		//	#elif defined(ARK2D_RENDERER_DIRECTX)
		//		// TODO...
		//	#else

		//		
		//	#endif
		//}

		void Renderer::fillRoundedRect(float x, float y, int width, int height, int radius) const {
			fillRoundedRect(x, y, width, height, radius, DEFAULT_SEGMENTS);
		}
		void Renderer::fillRoundedRect(float x, float y, int width, int height, int radius, int segs) const {
			if (radius <= 0) {
				fillRect(x, y, width, height);
				return;
			}

			fillRect(x + radius, y, width - (radius*2), height);
			fillRect(x, y + radius, radius, height - (radius*2));
			fillRect(x+width-radius, y + radius, radius, height - (radius*2));

			fillArc(x + radius, y + radius, 2*radius, 2*radius, 90, 180, segs);
			fillArc(x + width - radius, y + radius, 2*radius, 2*radius, 0, 90, segs);
			fillArc(x + radius, y + height - radius, 2*radius, 2*radius, 180, 270, segs);
			fillArc(x + width - radius, y + height - radius, 2*radius, 2*radius, 270, 360, segs);
		}

		
		/*
		 * Immediate Mode rendering! :(
		 * ...what ever happened to vertex arrays?
		 * ...or maybe even OpenGL 3.0: hello deprecation model!
		 */
		void Renderer::drawRect(ARK::Geometry::Rectangle<int>* rect) const { 
		 	drawRect(rect->getMinX(), rect->getMinY(), rect->getWidth(), rect->getHeight());
		}
		void Renderer::drawRect(ARK::Geometry::Rectangle<float>* rect) const { 
		 	drawRect(rect->getMinX(), rect->getMinY(), (int) rect->getWidth(), (int) rect->getHeight());
		}
		void Renderer::drawRect(float x, float y, int width, int height) const {
			//#if defined(ARK2D_ANDROID)

			if (Renderer::isBatching()) { 
				float hlw = (m_LineWidth/2.0f);
				float fw = (float) width;
				float fh = (float) height;

				// left side
				s_batch->addGeometryQuad(
					x - hlw,		y,	// tl
					x + hlw,		y,	// tr
					x - hlw,		y + fh,	  	// bl
					x + hlw,		y + fh,		// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);

				// right side
				s_batch->addGeometryQuad(
					x + fw - hlw,		y,	// tl
					x + fw + hlw,		y,	// tr
					x + fw - hlw,		y + fh,	  	// bl
					x + fw + hlw,		y + fh,		// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);

				// top side
				s_batch->addGeometryQuad(
					x,		y - hlw,	// tl
					x+fw,	y - hlw,	// tr
					x,		y + hlw,	// bl
					x+fw,	y + hlw,	// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);

				// bottom side
				s_batch->addGeometryQuad(
					x,		y + fh - hlw,	// tl 
					x+fw,	y + fh - hlw,	// tr
					x,		y + fh + hlw,	// bl
					x+fw,	y + fh + hlw,	// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);
				return; 
			}

			//#if ( defined(ARK2D_FLASCC) || defined(ARK2D_WINDOWS) )
			#if ( defined(ARK2D_FLASCC) )
				RendererState::start(RendererState::GEOMETRY);

				float hlw = (m_LineWidth/2.0f);
				float fw = (float) width;
				float fh = (float) height;
				float rawVertices[] = {
					// left side
					x - hlw,		y,	// tl
					x + hlw,		y,	// tr
					x - hlw,		y + fh,	  	// bl
					x - hlw,		y + fh,	  	// bl
					x + hlw,		y,	// tr
					x + hlw,		y + fh,		// br

					// right side side
					x + fw - hlw,		y,	// tl
					x + fw + hlw,		y,	// tr
					x + fw - hlw,		y + fh,	  	// bl
					x + fw - hlw,		y + fh,	  	// bl
					x + fw + hlw,		y,	// tr
					x + fw + hlw,		y + fh,		// br

					// top side
					x,		y - hlw,	// tl
					x+fw,	y - hlw,	// tr
					x,		y + hlw,	// bl
					x,		y + hlw,	// bl
					x+fw,	y - hlw,	// tr
					x+fw,	y + hlw,	// br

					// bottom side
					x,		y + fh - hlw,	// tl
					x+fw,	y + fh - hlw,	// tr
					x,		y + fh + hlw,	// bl
					x,		y + fh + hlw,	// bl
					x+fw,	y + fh - hlw,	// tr
					x+fw,	y + fh + hlw	// br
				};

				unsigned char rawColors[] = {
					// left
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// right
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// top
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// bottom
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};


				//pushMatrix();
				//translate(x, y);  
			 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 24); //6); // 6 is number of points in verts.
 				RendererStats::s_glCalls += 3;

				//popMatrix(); 

				RendererStats::s_tris += 8;

			#elif defined(ARK2D_OPENGL_3_2)

				s_vboQuadVerts->setWidth(2);// 12); // 8 verts -- needs to be 48
				s_vboQuadColors->setWidth(4);// 24);  // 16 verts -- needs to be 96
				
				s_vboQuadVerts->setHeight(24);
				s_vboQuadColors->setHeight(24);

				float hlw = (m_LineWidth/2.0f);
				float fw = (float) width;
				float fh = (float) height;
				
				float rawVertices[] = {
					// left side
					x - hlw,		y,			// tl
					x + hlw,		y,			// tr
					x - hlw,		y + fh,	  	// bl
					x - hlw,		y + fh,	  	// bl
					x + hlw,		y,			// tr
					x + hlw,		y + fh,		// br

					// right side side
					x + fw - hlw,		y,	// tl
					x + fw + hlw,		y,	// tr
					x + fw - hlw,		y + fh,	  	// bl
					x + fw - hlw,		y + fh,	  	// bl
					x + fw + hlw,		y,	// tr
					x + fw + hlw,		y + fh,		// br

					// top side
					x,		y - hlw,	// tl
					x + fw,	y - hlw,	// tr
					x,		y + hlw,	// bl
					x,		y + hlw,	// bl
					x + fw,	y - hlw,	// tr
					x + fw,	y + hlw,	// br

					// bottom side
					x,		y + fh - hlw,	// tl
					x + fw,	y + fh - hlw,	// tr
					x,		y + fh + hlw,	// bl
					x,		y + fh + hlw,	// bl
					x + fw,	y + fh - hlw,	// tr
					x + fw,	y + fh + hlw	// br
				};

				unsigned char rawColors[] = {
					// left
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// right
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// top
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// bottom
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};


				RendererState::start(RendererState::GEOMETRY);
				//pushMatrix();
				//translate(x, y);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 24); 
				

				//popMatrix();
				//RendererStats::s_lines += 4;
				RendererStats::s_tris += 8;
				RendererStats::s_glCalls += 7;

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadColors->setWidth(4); 

			#elif defined(ARK2D_OPENGL_ES_2_0)

				/*Renderer_InterleavingGeometryVertexData vertices[] = {
					// left side
					{ {x - hlw,			y}, 				{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tl
					{ {x + hlw,			y}, 				{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x - hlw,			y + fh}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x - hlw,			y + fh}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x + hlw,			y}, 				{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x + hlw,			y + fh}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // br
					// right side side
					{ {x + fw - hlw,	y}, 				{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tl
					{ {x + fw + hlw,	y}, 				{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x + fw - hlw,	y + fh}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x + fw - hlw,	y + fh}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x + fw + hlw,	y}, 				{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x + fw + hlw,	y + fh}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // br
					// top side
					{ {x,				y - hlw}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tl
					{ {x + fw,			y - hlw}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x,				y + hlw}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x,				y + hlw}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x + fw,			y - hlw}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x + fw,			y + hlw}, 			{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // br
					// bottom side 
					{ {x,				y + fh - hlw}, 		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tl
					{ {x + fw,			y + fh - hlw}, 		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x,				y + fh + hlw}, 		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x,				y + fh + hlw}, 		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // bl
					{ {x + fw,			y + fh - hlw}, 		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }, // tr
					{ {x + fw,			y + fh + hlw}, 		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} } // br
				};

				//char* rawVertices = (char*) &vertices;
				//unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*2);
				*/

				float hlw = (m_LineWidth/2.0f);
				float fw = (float) width;
				float fh = (float) height;
				
				float rawVertices[] = {
					// left side
					x - hlw,		y,			// tl
					x + hlw,		y,			// tr
					x - hlw,		y + fh,	  	// bl
					x - hlw,		y + fh,	  	// bl
					x + hlw,		y,			// tr
					x + hlw,		y + fh,		// br

					// right side side
					x + fw - hlw,		y,	// tl
					x + fw + hlw,		y,	// tr
					x + fw - hlw,		y + fh,	  	// bl
					x + fw - hlw,		y + fh,	  	// bl
					x + fw + hlw,		y,	// tr
					x + fw + hlw,		y + fh,		// br

					// top side
					x,		y - hlw,	// tl
					x + fw,	y - hlw,	// tr
					x,		y + hlw,	// bl
					x,		y + hlw,	// bl
					x + fw,	y - hlw,	// tr
					x + fw,	y + hlw,	// br

					// bottom side
					x,		y + fh - hlw,	// tl
					x + fw,	y + fh - hlw,	// tr
					x,		y + fh + hlw,	// bl
					x,		y + fh + hlw,	// bl
					x + fw,	y + fh - hlw,	// tr
					x + fw,	y + fh + hlw	// br
				};

				unsigned char rawColors[] = {
					// left
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// right
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// top
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),

					// bottom
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::GEOMETRY);

				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, rawVertices);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 24); 

				RendererStats::s_tris += 8;
				RendererStats::s_glCalls += 5;

			#elif defined(ARK2D_RENDERER_DIRECTX)



				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				__internalsDXUpdateMatrices();

				float hlw = (m_LineWidth/2.0f);
				float fw = (float) width;
				float fh = (float) height;

				fillRect(x, y - hlw, fw, hlw*2); // top
				fillRect(x, y + fh - hlw, fw, hlw*2); // bottom
				fillRect(x - hlw, y, hlw*2, fh); // left
				fillRect(x - hlw + fw, y, hlw*2, fh); // right
				return;
				
				unsigned int countVerts = 16;
				unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingGeometryVertexData);
				if (dx_texturedTriangles_data == NULL || dx_texturedTriangles_size < memorySize) {
					dx_texturedTriangles_data = realloc(dx_texturedTriangles_data, memorySize);
					dx_texturedTriangles_size = memorySize;
				}
				
				Renderer_DX_InterleavingGeometryVertexData* rawVertices = (Renderer_DX_InterleavingGeometryVertexData*) dx_texturedTriangles_data;

				// Left side
				rawVertices[0].vertex = DirectX::XMFLOAT4(x - hlw, y, 	   0.0f, 0.0f); // tl
				rawVertices[1].vertex = DirectX::XMFLOAT4(x + hlw, y, 	   0.0f, 0.0f); // tr
				rawVertices[2].vertex = DirectX::XMFLOAT4(x - hlw, y + fh, 0.0f, 0.0f); // bl
				rawVertices[3].vertex = DirectX::XMFLOAT4(x + hlw, y + fh, 0.0f, 0.0f); // br

				// Right side
				rawVertices[4].vertex = DirectX::XMFLOAT4(x + fw - hlw, y, 		0.0f, 0.0f); // tl
				rawVertices[5].vertex = DirectX::XMFLOAT4(x + fw + hlw, y, 		0.0f, 0.0f); // tr
				rawVertices[6].vertex = DirectX::XMFLOAT4(x + fw - hlw, y + fh, 0.0f, 0.0f); // bl
				rawVertices[7].vertex = DirectX::XMFLOAT4(x + fw + hlw, y + fh, 0.0f, 0.0f); // br

				// Top side
				rawVertices[8].vertex  = DirectX::XMFLOAT4(x, 	   y - hlw, 0.0f, 0.0f); // tl
				rawVertices[9].vertex  = DirectX::XMFLOAT4(x + fw, y - hlw, 0.0f, 0.0f); // tr
				rawVertices[10].vertex = DirectX::XMFLOAT4(x, 	   y + hlw, 0.0f, 0.0f); // bl
				rawVertices[11].vertex = DirectX::XMFLOAT4(x + fw, y + hlw, 0.0f, 0.0f); // br

				// Bottom side
				rawVertices[12].vertex = DirectX::XMFLOAT4(x, 	   y + fh - hlw, 0.0f, 0.0f); // tl
				rawVertices[13].vertex = DirectX::XMFLOAT4(x + fw, y + fh - hlw, 0.0f, 0.0f); // tr
				rawVertices[14].vertex = DirectX::XMFLOAT4(x, 	   y + fh + hlw, 0.0f, 0.0f); // bl
				rawVertices[15].vertex = DirectX::XMFLOAT4(x + fw, y + fh + hlw, 0.0f, 0.0f); // br
				
				rawVertices[0].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[1].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[2].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[3].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[4].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[5].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[6].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[7].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[8].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[9].color  = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[10].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[11].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[12].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[13].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[14].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[15].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				
				unsigned int rawIndices[] = {
					0, 1, 2, 2, 1, 3, // left side
					4, 5, 6, 6, 5, 7, // right side
					8, 9, 10, 10, 9, 11, // top side
					12, 13, 14, 14, 13, 15 // bottom side
				};
				s_vboQuadVerts->setData((void*) rawVertices, memorySize);
				s_vboIndices->setData((void*) rawIndices, sizeof(unsigned int) * 24);
				
				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingGeometryVertexData);
				unsigned int offset = 0; 
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);
				deviceContext->IASetIndexBuffer(s_vboIndices->m_buffer, DXGI_FORMAT_R32_UINT, 0);

				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				deviceContext->DrawIndexed(countVerts, 0, 0);

				RendererStats::s_lines += 4;
				RendererStats::s_glCalls += 4;

			#else 

				float rawVertices[] = {
					x, y,
					x + width, y,
					x + width, y + height,
					x, y + height
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::GEOMETRY);
				//pushMatrix();
				//translate(x, y);
 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_LINE_LOOP, 0, 4);

				//popMatrix();

				RendererStats::s_lines += 4;
				RendererStats::s_glCalls += 3;

			#endif
		
			showAnyGlErrorAndExitMacro();
		}
		//! @todo: drawRects is broken.
		// rects must be [[x1,y1,x2,y2,x3,y3,x4,y4], etc].
		// colors must be [[r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a], etc], or NULL
		void Renderer::drawRects(float rects[], int colors[]) const {
			return;

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				int total = (sizeof(&rects) / sizeof(int))/8;
				
				//glDisable(GL_TEXTURE_2D);
				//glEnableClientState(GL_VERTEX_ARRAY);
				RendererState::start(RendererState::GEOMETRY);
				
				if (colors != NULL) {
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(4, GL_UNSIGNED_INT, 0, &colors);
					RendererStats::s_glCalls += 2;
				}

				#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE))
					glVertexPointer(2, GL_FIXED, 0, rects);
				#else
					glVertexPointer(2, GL_INT, 0, rects);
				#endif

				for(int i = 0; i < total; i++) {
					glDrawArrays(GL_LINE_LOOP, i*4, 4);
				}
				RendererStats::s_glCalls += 1 + total;
			#endif

			//glDisableClientState(GL_VERTEX_ARRAY);
			//glEnable(GL_TEXTURE_2D);
		}
		void Renderer::fillGradientRect(float x, float y, float width, float height, Color* top, Color* bottom) const {
			
			if (Renderer::isBatching()) { 
				s_batch->addGeometryQuad(
					x, 			y, 
					x + width, 	y, 
					x, 			y + height, 
					x + width, 	y + height, 
					top->getRedc(), 	top->getGreenc(), 	 top->getBluec(), 		top->getAlphac(), 		// tl 
					top->getRedc(), 	top->getGreenc(), 	 top->getBluec(), 		top->getAlphac(), 		// tr
					bottom->getRedc(), 	bottom->getGreenc(), bottom->getBluec(), 	bottom->getAlphac(), 	// bl
					bottom->getRedc(), 	bottom->getGreenc(), bottom->getBluec(), 	bottom->getAlphac() 	// br
				);
				return; 
			}
			//pushMatrix();
			//translate(x, y);
			

			#if defined(ARK2D_FLASCC) 

				RendererState::start(RendererState::GEOMETRY);
				//glEnableClientState(GL_COLOR_ARRAY);

				float rawVertices[] = {
					x,					y,						// tl
					x + (float) width,	y,						// tr
					x,					y + (float)height,	  	// bl
					x,					y + (float)height,	  	// bl
					x + (float) width,	y,						// tr
					x + (float) width,	y + (float)height		// br
				};

				unsigned char rawColors[] = {
					top->getRedc(), 	top->getGreenc(), 	 top->getBluec(), 		top->getAlphac(), 		// tl 
					top->getRedc(), 	top->getGreenc(), 	 top->getBluec(), 		top->getAlphac(), 		// tr
					bottom->getRedc(), 	bottom->getGreenc(), bottom->getBluec(), 	bottom->getAlphac(), 	// bl
					bottom->getRedc(), 	bottom->getGreenc(), bottom->getBluec(),  	bottom->getAlphac(), 	// bl
					top->getRedc(), 	top->getGreenc(),  	 top->getBluec(),  		top->getAlphac(), 		// tr
					bottom->getRedc(), 	bottom->getGreenc(), bottom->getBluec(), 	bottom->getAlphac() 	// br
				};
 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				RendererStats::s_glCalls += 3;

			#else

				

				#ifdef NO_FIXED_FUNCTION_PIPELINE
					
					RendererState::start(RendererState::GEOMETRY);

					#if defined(ARK2D_OPENGL_3_2)

						float rawVertices[] = {
							x,					y,		  				// tl
							x + (float) width,	y,						// tr
							x,					y + (float) height,	  	// bl
							x + (float) width,	y + (float) height 		// br
						};

						unsigned char rawColors[] = {
							top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac(),	// tl
							top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac(),	// tr
							bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac(),	// bl
							bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac()		// br
						};

						s_vboQuadVerts->setWidth(2);
						s_vboQuadColors->setWidth(4);

						s_vboQuadVerts->setHeight(4);
						s_vboQuadColors->setHeight(4);
					
						s_vaoQuad->bind();
						glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
						glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
				
						s_vboQuadVerts->bind();
						s_vboQuadVerts->setData(&rawVertices[0]);
						glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
						
						s_vboQuadColors->bind();
						s_vboQuadColors->setData(&rawColors[0]);
						glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
						
						glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
						glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
						RendererStats::s_glCalls += 7;

					#elif defined(ARK2D_OPENGL_ES_2_0)
 
						/*Renderer_InterleavingGeometryVertexData vertices[] = {
							{ {x, 					y}, 					{top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac()} },
							{ {x + float(width), 	y}, 					{top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac()} },
							{ {x, 					y + float(height)}, 	{bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac()} },
							{ {x + float(width), 	y + float(height)}, 	{bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac()} }
						};
 
 						char* rawVertices = (char*) &vertices;
						unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*2);*/

						float rawVertices[] = {
							x,					y,		  				// tl
							x + (float) width,	y,						// tr
							x,					y + (float) height,	  	// bl
							x + (float) width,	y + (float) height 		// br
						};

						unsigned char rawColors[] = {
							top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac(),	// tl
							top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac(),	// tr
							bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac(),	// bl
							bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac()		// br
						};
						 
						glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingGeometryVertexData)
 						glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
 						
 						glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
						glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
						RendererStats::s_glCalls += 5;

					#endif

				#else 

					float rawVertices[] = {
						x,					y,		  			// tl
						x + (float) width,	y,					// tr
						x,					y + (float)height,	// bl
						x + (float) width,	y + (float)height 	// br
					};

					unsigned char rawColors[] = {
						top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac(),	// tl
						top->getRedc(), top->getGreenc(), top->getBluec(), top->getAlphac(),	// tr
						bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac(),	// bl
						bottom->getRedc(), bottom->getGreenc(), bottom->getBluec(), bottom->getAlphac()		// br
					};

					RendererState::start(RendererState::GEOMETRY);

					glEnableClientState(GL_COLOR_ARRAY);

					glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
					glVertexPointer(2, GL_FLOAT, 0, rawVertices);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					glDisableClientState(GL_COLOR_ARRAY);

					RendererStats::s_glCalls += 5;

				#endif

			#endif

			
			//translate(x * -1, y * -1, 0);
			//popMatrix();	

			RendererStats::s_tris += 2;
			showAnyGlErrorAndExitMacro();
		}
		void Renderer::fillRect(float x, float y, int width, int height) const {
			
			if (Renderer::isBatching()) { 
				s_batch->addGeometryQuad(
					x, 			y, 
					x + width, 	y, 
					x, 			y + height, 
					x + width, 	y + height, 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);
				return; 
			}

			#if defined(ARK2D_OPENGL_3_2)// || defined(ARK2D_EMSCRIPTEN_JS)

				s_vboQuadVerts->setWidth(2);
				s_vboQuadColors->setWidth(4);

				s_vboQuadVerts->setHeight(4);
				s_vboQuadColors->setHeight(4);

				RendererState::start(RendererState::GEOMETRY);
				//pushMatrix();
				//translate(x, y);

				float rawVertices[] = {
					x,					y,		  			// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x + (float) width,	y + (float) height	// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				}; 

				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
		
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				//popMatrix();
				RendererStats::s_tris += 2;
				RendererStats::s_glCalls += 7;

			#elif defined(ARK2D_OPENGL_ES_2_0)

				RendererState::start(RendererState::GEOMETRY);
				//pushMatrix();
				//translate(x, y);

				/*Renderer_InterleavingGeometryVertexData vertices[] = {
					{ {x, 					y}, 					{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x + float(width), 	y}, 					{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x, 					y + float(height)}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x + float(width), 	y + float(height)}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }
				};

				char* rawVertices = (char*) &vertices;
				unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*2); */

				float rawVertices[] = {
					x,					y,		  			// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x + (float) width,	y + (float) height	// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				}; 

				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingGeometryVertexData)
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
					
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				//popMatrix();
				RendererStats::s_tris += 2;
				RendererStats::s_glCalls += 5;

			#elif defined(ARK2D_FLASCC)

				RendererState::start(RendererState::GEOMETRY);
				//pushMatrix();
				//translate(x, y, 0);

				float rawVertices[] = {
					x,					y,						// tl
					x + (float) width,	y,						// tr
					x,					y + (float)height,	  	// bl
					x,					y + (float)height,	  	// bl
					x + (float) width,	y,						// tr
					x + (float) width,	y + (float)height		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};
 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				//glDisableClientState(GL_COLOR_ARRAY);
				//translate(x * -1, y * -1, 0);
				//popMatrix();
				RendererStats::s_tris += 2;
				RendererStats::s_glCalls += 3;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				RendererState::start(RendererState::GEOMETRY); 

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
				
				__internalsDXUpdateMatrices();
								
				Renderer_DX_InterleavingGeometryVertexData rawVertices[] = {
					{ DirectX::XMFLOAT4(x, y, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x + width, y, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x, y + height, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x + width, y + height, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) }
				};

				unsigned int rawIndices[] = {0, 1, 2, 2, 1, 3};

				
				// Lock the vertex buffer so it can be written to. Get a pointer to the data in the vertex buffer.
				// Copy the data into the vertex buffer. Unlock the vertex buffer.
				s_vboQuadVerts->setData((void*)&rawVertices[0], (unsigned int) sizeof(Renderer_DX_InterleavingGeometryVertexData) * 4);
				s_vboIndices->setData((void*)&rawIndices[0], sizeof(unsigned int) * 6);

				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingGeometryVertexData);
				unsigned int offset = 0; 
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);
 
				// Set the index buffer to active in the input assembler so it can be rendered.
				deviceContext->IASetIndexBuffer(s_vboIndices->m_buffer, DXGI_FORMAT_R32_UINT, 0);

				// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// DRAW! 
				deviceContext->DrawIndexed(6, 0, 0);				 
				//deviceContext->Draw(6, 0);//, 0, 0);		



			#else

				float rawVertices[] = {
					x,					y,					// tl
					x + (float) width,	y,					// tr
					x,					y + (float)height,	// bl
					x + (float) width,	y + (float)height	// br
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::GEOMETRY);
				//pushMatrix();
				//translate(x, y);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);			

				//popMatrix(); 
				RendererStats::s_tris += 2;
				RendererStats::s_glCalls += 3;

			#endif

			showAnyGlErrorAndExitMacro();

		}

		void Renderer::texturedRect(unsigned int texId, float x, float y, int width, int height) const {
			
			if (Renderer::isBatching()) { 
				s_batch->addTexturedQuad(
					texId, 
					x, y, 
					x + width, y, 
					x, y + height, 
					x + width, y + height, 
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f,	// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);

				return; 
			}


			#if defined(ARK2D_OPENGL_3_2)

				float rawVertices[] = {
					x,					y,					// tl
					x + (float) width, 	y,					// tr
					x,					y + (float)height,	// bl
					x + (float) width,	y + (float)height	// br
				};
				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::TEXTURE, texId);

				s_vboQuadVerts->setWidth(2);
				s_vboQuadTexCoords->setWidth(2);
				s_vboQuadColors->setWidth(4);
				
				s_vboQuadVerts->setHeight(4);
				s_vboQuadTexCoords->setHeight(4);
				s_vboQuadColors->setHeight(4);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexTexCoordIn);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);

 				s_vboQuadTexCoords->bind(); 
				s_vboQuadTexCoords->setData(&rawTextureCoords[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				//pushMatrix();
				//translate(x, y);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				//popMatrix();

				RendererStats::s_tris += 2; 
				RendererStats::s_glCalls += 9;

			#elif defined(ARK2D_OPENGL_ES_2_0)
 
				/*Renderer_InterleavingTextureVertexData vertices[] = {
					{ 
						{x, 	y},
						{0.0f,	0.0f},		
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} 
					},
					{ 
						{x + float(width), 	y},
						{1.0f,				0.0f},			
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} 
					},
					{ 
						{x, 	y + float(height)}, 	
						{0.0f,	1.0f}, 	
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} 
					},
					{ 
						{x + float(width), 	y + float(height)}, 	
						{1.0f,				1.0f}, 	
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} 
					}
				};

				char* rawVertices = (char*) &vertices;
				char* rawTextureCoords = ((char*) &vertices) + (sizeof(float)*2);
				unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*4); */


				float rawVertices[] = {
					x,					y,					// tl
					x + (float) width, 	y,					// tr
					x,					y + (float)height,	// bl
					x + (float) width,	y + (float)height	// br
				};
				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::TEXTURE, texId);
				  
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingTextureVertexData)
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, &rawTextureCoords);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
					
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				//pushMatrix();
				//translate(x, y);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				//popMatrix();

				RendererStats::s_tris += 2; 
				RendererStats::s_glCalls += 6;

			#elif defined(ARK2D_FLASCC)

				float rawVertices[] = {
					x,					y,		  			// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x,					y + (float) height,	// bl
					x + (float) width,	y,					// tr
					x + (float) width,	y + (float) height	// br
				};
				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					0.0f,	1.0f,	// bl
					1.0f,	0.0f,	// tr
					1.0f,	1.0f	// br
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::TEXTURE, texId);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_tris += 2; 
				RendererStats::s_glCalls += 4;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ARK2D::getLog()->w("Renderer::texturedRect not implemented in Direct X.");

			#else 

				float rawVertices[] = {
					x,					y,		  			// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x + (float) width,	y + (float) height	// br
				};
				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				RendererState::start(RendererState::TEXTURE, texId);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);

				//pushMatrix();
				//translate(x, y);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				//popMatrix();

				RendererStats::s_tris += 2; 
				RendererStats::s_glCalls += 4;

			#endif

			showAnyGlErrorAndExitMacro();
		}

		/*void Renderer::texturedRectShader(Shader* useShader, const map<string, float>&, unsigned int texId, float x, float y, int width, int height) const 
		{
			#ifdef SHADER_SUPPORT
	 			float rawVertices[] = {
					0.0f,			0.0f,		  // tl
					(float) width,	0.0f,			// tr
					0.0f,			(float)height,	  // bl
					(float) width,	(float)height	// br
				};
				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};
				float rawColors[] = {
					m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf(),
					m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf(),
					m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf(),
					m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()
				};

				pushMatrix();
				translate(x, y);

				useShader->bind();  
				useShader->setUniformVariableMat4f(useShader->getInittedVariable("ark_ModelViewMatrix"), (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				useShader->setUniformVariableMat4f(useShader->getInittedVariable("ark_ProjectionMatrix"), (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());
				
				useShader->setAttributeVariableVertexPointerFloat(useShader->getInittedVariable("ark_VertexPosition"), 2, false, &rawVertices); 
				useShader->setAttributeVariableVertexPointerFloat(useShader->getInittedVariable("ark_VertexTexCoordIn"), 2, false, &rawTextureCoords); 
				useShader->setAttributeVariableVertexPointerFloat(useShader->getInittedVariable("ark_VertexColorIn"), 4, false, &rawColors); 

				//glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, );  
				//glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, &);
				//glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_FLOAT, GL_FALSE, 0, &); 
				
				glUniform1i(Renderer::s_shaderBasicTexture_TextureId, 0);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				
				useShader->unbind();

				popMatrix();
				RendererStats::s_tris += 2; 

			#else
				ARK2D::getLog()->w("Not shader support -- cannot use 'texturedRectShader'.");
			#endif
		}*/

		void Renderer::texturedRectAlpha(unsigned int texId, float x, float y, int width, int height, float alpha) const {

			//pushMatrix();
			//translate(x, y);


			if (Renderer::isBatching()) { 
				s_batch->addTexturedQuad(
					texId, 
					x, y, 
					x + width, y, 
					x, y + height, 
					x + width, y + height, 
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f,	// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)
				);

				return; 
			}

			RendererState::start(RendererState::TEXTURE, texId);
			
			#if defined(ARK2D_OPENGL_3_2)

				float rawVertices[] = {
					x,					y,					// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x + (float) width,	y + (float) height	// br
				};

				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)
				};

				s_vboQuadVerts->setWidth(2);
				s_vboQuadTexCoords->setWidth(2);
				s_vboQuadColors->setWidth(4);

				s_vboQuadVerts->setHeight(4);
				s_vboQuadTexCoords->setHeight(4);
				s_vboQuadColors->setHeight(4);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexTexCoordIn);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);

 				s_vboQuadTexCoords->bind();
				s_vboQuadTexCoords->setData(&rawTextureCoords[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				RendererStats::s_glCalls += 9;
			
			#elif defined(ARK2D_OPENGL_ES_2_0)

				/*Renderer_InterleavingTextureVertexData vertices[] = {
					{ 
						{x, 	y},
						{0.0f,	0.0f},		
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)} 
					},
					{ 
						{x + float(width), 	y},
						{1.0f,			0.0f},			
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)} 
					},
					{ 
						{x, 	y + float(height)}, 	
						{0.0f,	1.0f}, 	
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)} 
					},
					{ 
						{x + float(width), 	y + float(height)}, 	
						{1.0f,			1.0f}, 	
						{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)} 
					}
				};

				char* rawVertices = (char*) &vertices;
				char* rawTextureCoords = ((char*) &vertices) + (sizeof(float)*2);
				unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*4); */

				float rawVertices[] = {
					x,					y,					// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x + (float) width,	y + (float) height	// br
				};

				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)
				};

				RendererState::start(RendererState::TEXTURE, texId);
				 
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingTextureVertexData)
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, &rawTextureCoords);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				RendererStats::s_glCalls += 6;

			#elif defined(ARK2D_FLASCC)

				float rawVertices[] = {
					x,					y,		  			// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x,					y + (float) height,	// bl
					x + (float) width,	y,					// tr
					x + (float) width,	y + (float) height	// br
				};
				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					0.0f,	1.0f,	// bl
					1.0f,	0.0f,	// tr
					1.0f,	1.0f	// br
				}; 
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char) (m_DrawColor.getAlphac() * alpha)
				};

				RendererState::start(RendererState::TEXTURE, texId);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_tris += 2; 
				RendererStats::s_glCalls += 4;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ARK2D::getLog()->w("Renderer::texturedRectAlpha not implemented in Direct X.");

			#else 

				float rawVertices[] = {
					x,					y,					// tl
					x + (float) width,	y,					// tr
					x,					y + (float) height,	// bl
					x + (float) width,	y + (float) height	// br
				};

				float rawTextureCoords[] = {
					0.0f,	0.0f,	// tl
					1.0f,	0.0f,	// tr
					0.0f,	1.0f,	// bl
					1.0f,	1.0f	// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char)(m_DrawColor.getAlphac() * alpha), // tl 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char)(m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char)(m_DrawColor.getAlphac() * alpha),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), (unsigned char)(m_DrawColor.getAlphac() * alpha)
				};
			
				glEnableClientState(GL_COLOR_ARRAY);
 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				RendererStats::s_glCalls += 4;

			#endif

			//popMatrix();
 
			RendererStats::s_tris += 2;

			showAnyGlErrorAndExitMacro();
		}

		// order: anti-clockwise. 
		void Renderer::fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const 
		{
			showAnyGlErrorAndExitMacro();

			if (Renderer::isBatching()) { 
				s_batch->addGeometryQuad(
					x1, y1, 
					x2, y2, 
					x3, y3, 
					x4, y4, 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);
				return;
			}

			RendererState::start(RendererState::GEOMETRY);

			#if defined(ARK2D_OPENGL_3_2)

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				s_vboQuadVerts->setWidth(2);
				s_vboQuadColors->setWidth(4);

				s_vboQuadVerts->setHeight(6);
				s_vboQuadColors->setHeight(6);
				
				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 6);

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadColors->setWidth(4);

				RendererStats::s_glCalls += 7;

			#elif defined(ARK2D_OPENGL_ES_2_0)

				/*Renderer_InterleavingGeometryVertexData vertices[] = {
					{ {x1, 	y1}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x2, 	y2}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x3, 	y3}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x3, 	y3}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x2, 	y2}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} },
					{ {x4, 	y4}, 	{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()} }
				};

				char* rawVertices = (char*) &vertices; 
				unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*2); */

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};
				 
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingGeometryVertexData)
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
					
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 5;

			#elif defined(ARK2D_FLASCC)

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 3;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				RendererState::start(RendererState::GEOMETRY); 

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
				 
				__internalsDXUpdateMatrices();

				Renderer_DX_InterleavingGeometryVertexData rawVertices[] = {
					{ DirectX::XMFLOAT4(x1, y1, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x2, y2, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x3, y3, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x4, y4, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) }
				};

				unsigned int rawIndices[] = { 0, 1, 2, 2, 1, 3 };


				// Lock the vertex buffer so it can be written to. Get a pointer to the data in the vertex buffer.
				// Copy the data into the vertex buffer. Unlock the vertex buffer.
				s_vboQuadVerts->setData((void*)&rawVertices[0], (unsigned int) sizeof(Renderer_DX_InterleavingGeometryVertexData) * 4);
				s_vboIndices->setData((void*)&rawIndices[0], sizeof(unsigned int) * 6);

				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingGeometryVertexData);
				unsigned int offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);

				// Set the index buffer to active in the input assembler so it can be rendered.
				deviceContext->IASetIndexBuffer(s_vboIndices->m_buffer, DXGI_FORMAT_R32_UINT, 0);

				// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// DRAW! 
				deviceContext->DrawIndexed(6, 0, 0);	

			#else 

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};


				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 3;
			#endif

			RendererStats::s_tris += 2;
			showAnyGlErrorAndExitMacro();
		}

		void Renderer::texturedQuads(unsigned int texId, float* verts, float* texcoords, unsigned char* colors, unsigned int count) 
		{
			if (Renderer::isBatching()) { 
				for(unsigned int i = 0; i < count; i++) { 
					s_batch->addTexturedQuad(texId, &verts[i*6], &texcoords[i*6], &colors[i*12]);
					/*const int ii = i * 12; 
					const int iii = i * 24;
					s_batch->addTexturedQuad(
						texId, 
						verts[ii], verts[ii+1], 
						verts[ii+2], verts[ii+3],
						verts[ii+4], verts[ii+5],  
						verts[ii+10], verts[ii+11], 
						texcoords[ii], texcoords[ii+1], 
						texcoords[ii+2], texcoords[ii+3], 
						texcoords[ii+4], texcoords[ii+5], 
						texcoords[ii+10], texcoords[ii+11], 
						colors[iii+0], colors[iii+1], colors[iii+2], colors[iii+3],
						colors[iii+4], colors[iii+5], colors[iii+6], colors[iii+7],
						colors[iii+8], colors[iii+9], colors[iii+10], colors[iii+11],
						colors[iii+20], colors[iii+21], colors[iii+22], colors[iii+23]
					);*/
				}
				return;
			}

			

			#if defined(ARK2D_OPENGL_3_2)

				//int numVerts = count * 6 * 2;
				//int numCVerts = count * 6 * 4;

				//s_vboQuadVerts->setWidth(numVerts / 4); // 8 verts -- needs to be 48
				//s_vboQuadTexCoords->setWidth(numVerts / 4); // 8 verts -- needs to be 48
				//s_vboQuadColors->setWidth(numCVerts / 4);  // 16 verts -- needs to be 96

				RendererState::start(RendererState::TEXTURE, texId);

				s_vboQuadVerts->setWidth(2);
				s_vboQuadTexCoords->setWidth(2);
				s_vboQuadColors->setWidth(4);

				s_vboQuadVerts->setHeight(count * 6);
				s_vboQuadTexCoords->setHeight(count * 6);
				s_vboQuadColors->setHeight(count * 6);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(s_shaderBasicTexture_VertexPosition);
				glEnableVertexAttribArray(s_shaderBasicTexture_VertexTexCoordIn);
				glEnableVertexAttribArray(s_shaderBasicTexture_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(verts);
				glVertexAttribPointer(s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
				
 				s_vboQuadTexCoords->bind(); 
				s_vboQuadTexCoords->setData(texcoords);
				glVertexAttribPointer(s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, 0);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(colors);
				glVertexAttribPointer(s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
				
				glUniformMatrix4fv(s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());
				
				glUniform1i(s_shaderBasicTexture_TextureId, 0); 

				glDrawArrays(GL_TRIANGLES, 0, count * 6); 

				RendererStats::s_glCalls += 10;

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadTexCoords->setWidth(2); 
				//s_vboQuadColors->setWidth(4);

			#elif defined(ARK2D_OPENGL_ES_2_0)

				RendererState::start(RendererState::TEXTURE, texId);

				glVertexAttribPointer(s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, verts);
				glVertexAttribPointer(s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
				glVertexAttribPointer(s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors); 
			
				glUniformMatrix4fv(s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glUniform1i(s_shaderBasicTexture_TextureId, 0);  

				glDrawArrays(GL_TRIANGLES, 0, count * 6); 

				RendererStats::s_glCalls += 7;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				RendererState::start(RendererState::TEXTURE, texId);

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				__internalsDXUpdateMatrices();

				unsigned int countVerts = count * 6;
				unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingTextureVertexData);
				if (dx_texturedTriangles_data == NULL || dx_texturedTriangles_size < memorySize) {
					dx_texturedTriangles_data = realloc(dx_texturedTriangles_data, memorySize);
					dx_texturedTriangles_size = memorySize;
				}

				unsigned int vcount = 0; //i * 12;
				unsigned int ccount = 0; //i * 24;
				Renderer_DX_InterleavingTextureVertexData* rawVertices = (Renderer_DX_InterleavingTextureVertexData*) dx_texturedTriangles_data;
				for (unsigned int i = 0; i < countVerts; i += 6) // for each quad
				{
					rawVertices[i].vertex = DirectX::XMFLOAT4(verts[vcount], verts[vcount + 1], 0.0f, 0.0f);
					rawVertices[i].texcoord = DirectX::XMFLOAT2(texcoords[vcount], texcoords[vcount+1]);
					rawVertices[i].color = DirectX::XMFLOAT4(colors[ccount]/255.0f, colors[ccount+1]/255.0f, colors[ccount+2]/255.0f, colors[ccount+3]/255.0f);

					rawVertices[i+1].vertex = DirectX::XMFLOAT4(verts[vcount+2], verts[vcount + 3], 0.0f, 0.0f);
					rawVertices[i+1].texcoord = DirectX::XMFLOAT2(texcoords[vcount+2], texcoords[vcount + 3]);
					rawVertices[i+1].color = DirectX::XMFLOAT4(colors[ccount+4] / 255.0f, colors[ccount + 5] / 255.0f, colors[ccount + 6] / 255.0f, colors[ccount + 7] / 255.0f);

					rawVertices[i+2].vertex = DirectX::XMFLOAT4(verts[vcount+4], verts[vcount + 5], 0.0f, 0.0f);
					rawVertices[i+2].texcoord = DirectX::XMFLOAT2(texcoords[vcount+4], texcoords[vcount + 5]);
					rawVertices[i+2].color = DirectX::XMFLOAT4(colors[ccount+8] / 255.0f, colors[ccount + 9] / 255.0f, colors[ccount + 10] / 255.0f, colors[ccount + 11] / 255.0f);

					rawVertices[i+3].vertex = DirectX::XMFLOAT4(verts[vcount+6], verts[vcount + 7], 0.0f, 0.0f);
					rawVertices[i+3].texcoord = DirectX::XMFLOAT2(texcoords[vcount+6], texcoords[vcount + 7]);
					rawVertices[i+3].color = DirectX::XMFLOAT4(colors[ccount+12] / 255.0f, colors[ccount + 13] / 255.0f, colors[ccount + 14] / 255.0f, colors[ccount + 15] / 255.0f);

					rawVertices[i+4].vertex = DirectX::XMFLOAT4(verts[vcount+8], verts[vcount + 9], 0.0f, 0.0f);
					rawVertices[i+4].texcoord = DirectX::XMFLOAT2(texcoords[vcount+8], texcoords[vcount + 9]);
					rawVertices[i+4].color = DirectX::XMFLOAT4(colors[ccount+16] / 255.0f, colors[ccount + 17] / 255.0f, colors[ccount + 18] / 255.0f, colors[ccount + 19] / 255.0f);

					rawVertices[i+5].vertex = DirectX::XMFLOAT4(verts[vcount+10], verts[vcount + 11], 0.0f, 0.0f);
					rawVertices[i+5].texcoord = DirectX::XMFLOAT2(texcoords[vcount+10], texcoords[vcount + 11]);
					rawVertices[i+5].color = DirectX::XMFLOAT4(colors[ccount+20] / 255.0f, colors[ccount + 21] / 255.0f, colors[ccount + 22] / 255.0f, colors[ccount + 23] / 255.0f);

					vcount += 12;
					ccount += 24;
				}
				
				s_vboQuadVerts->setData((void*) rawVertices, memorySize);
				
				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingTextureVertexData);
				unsigned int offset = 0; 
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);

				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				deviceContext->Draw(countVerts, 0);


			#else
				glVertexPointer(2, GL_FLOAT, 0, verts);
				glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
				glDrawArrays(GL_TRIANGLES, 0, count * 6);

				RendererStats::s_glCalls += 4;
			#endif
 
			RendererStats::s_tris += count * 2; 

			showAnyGlErrorAndExitMacro();
		}

		void Renderer::__internalsDXUpdateMatrices() {
			#ifdef ARK2D_RENDERER_DIRECTX
				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				MatrixStack* modelViewMatrix = Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW);
				MatrixStack* projectionMatrix = Renderer::getMatrix(MatrixStack::TYPE_PROJECTION);

				if (modelViewMatrix->isDirty() || projectionMatrix->isDirty()) {
					XMStoreFloat4x4(&Renderer::s_d3d_matrixBufferData->modelView, XMMatrixTranspose(modelViewMatrix->d3dpointer()));
					XMStoreFloat4x4(&Renderer::s_d3d_matrixBufferData->projection, XMMatrixTranspose(projectionMatrix->d3dpointer()));
					deviceContext->UpdateSubresource(s_d3d_matrixBuffer, 0, NULL, s_d3d_matrixBufferData, 0, 0);

					ID3D11Buffer *const constantBuffers[1] = { Renderer::s_d3d_matrixBuffer };
					deviceContext->VSSetConstantBuffers(0, 1, constantBuffers);

					modelViewMatrix->setDirty(false);
					projectionMatrix->setDirty(false);
				}
			#endif
		}


		void Renderer::fillTriangles(float* verts, unsigned char* colors, unsigned int count, bool fromBatch) 
		{
			if (isBatching() && !fromBatch) {
				for(unsigned int i = 0; i < count; i++) {
					s_batch->addGeometryTri(&verts[i*6], &colors[i*12]);
				}
				return;
			}

			RendererState::start(RendererState::GEOMETRY);

			#if defined(ARK2D_OPENGL_3_2)

				//int numVerts = count * 3 * 2;
				//int numCVerts = count * 3 * 4;
 
				//s_vboQuadVerts->setWidth(numVerts / 3); // 8 verts -- needs to be 48
				//s_vboQuadColors->setWidth(numCVerts / 3);  // 16 verts -- needs to be 96

				s_vboQuadVerts->setWidth(2);
				s_vboQuadColors->setWidth(4);
				s_vboQuadVerts->setHeight(count * 3);
				s_vboQuadColors->setHeight(count * 3);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(s_shaderBasicGeometry_VertexPosition);
				glEnableVertexAttribArray(s_shaderBasicGeometry_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(verts);
				glVertexAttribPointer(s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
				
 				s_vboQuadColors->bind();
				s_vboQuadColors->setData(colors);
				glVertexAttribPointer(s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
				
				glUniformMatrix4fv(s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, count * 3); 

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadColors->setWidth(4);

				RendererStats::s_glCalls += 7;

			#elif defined(ARK2D_OPENGL_ES_2_0)

				int numVerts = count * 3 * 2;
				int numCVerts = count * 3 * 4;

				glVertexAttribPointer(s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, verts);
				glVertexAttribPointer(s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors); 
			
				glUniformMatrix4fv(s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, count * 3); 

				RendererStats::s_glCalls += 5;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				//ARK2D::getLog()->w("Renderer::fillTriangles");

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				__internalsDXUpdateMatrices();

				unsigned int countVerts = count * 3;
				unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingGeometryVertexData);
				
				// check memory is the right size. reduce allocations. 
				if (dx_fillTriangles_data == NULL || dx_fillTriangles_size < memorySize) {
					dx_fillTriangles_data = realloc(dx_fillTriangles_data, memorySize);
					dx_fillTriangles_size = memorySize;
				}

				Renderer_DX_InterleavingGeometryVertexData* rawVertices = (Renderer_DX_InterleavingGeometryVertexData*) dx_fillTriangles_data;
				unsigned int iv = 0;
				unsigned int ic = 0;
				for (unsigned int i = 0; i < count*3; i +=3) {
					rawVertices[i].vertex = DirectX::XMFLOAT4(verts[iv], verts[iv+1], 0.0f, 0.0f);
					rawVertices[i].color = DirectX::XMFLOAT4(colors[ic]/255.0f, colors[ic+1]/255.0f, colors[ic+2]/255.0f, colors[ic+3]/255.0f);

					rawVertices[i+1].vertex = DirectX::XMFLOAT4(verts[iv+2], verts[iv+3], 0.0f, 0.0f);
					rawVertices[i+1].color = DirectX::XMFLOAT4(colors[ic+4]/255.0f, colors[ic+5]/255.0f, colors[ic+6]/255.0f, colors[ic+7]/255.0f);

					rawVertices[i+2].vertex = DirectX::XMFLOAT4(verts[iv+4], verts[iv+5], 0.0f, 0.0f);
					rawVertices[i+2].color = DirectX::XMFLOAT4(colors[ic+8]/255.0f, colors[ic+9]/255.0f, colors[ic+10]/255.0f, colors[ic+11]/255.0f);
					iv += 6;
					ic += 12;
				}
				
				// Lock the vertex buffer so it can be written to. Get a pointer to the data in the vertex buffer.
				// Copy the data into the vertex buffer. Unlock the vertex buffer.
				s_vboQuadVerts->setData((void*)&rawVertices[0], memorySize);
				
				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingGeometryVertexData);
				unsigned int offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);

				// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// DRAW! 
				deviceContext->Draw(countVerts, 0);

			#else
				glVertexPointer(2, GL_FLOAT, 0, verts);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
				glDrawArrays(GL_TRIANGLES, 0, count * 3);

				RendererStats::s_glCalls += 3;
			#endif
 
			RendererStats::s_tris += count; 

			showAnyGlErrorAndExitMacro();
		}

		
		void Renderer::texturedTriangles(unsigned int texId, float* verts, float* texcoords, unsigned char* colors, unsigned int count, bool fromBatch) {
		
			if (isBatching() && !fromBatch) {
				for(unsigned int i = 0; i < count; i++) {
					s_batch->addTexturedTri(texId, &verts[i*6], &texcoords[i*6], &colors[i*12]);
				}
				return;
			}

			RendererState::start(RendererState::TEXTURE, texId);

			#if defined(ARK2D_OPENGL_3_2)

				//int numVerts = count * 3 * 2;
				//int numCVerts = count * 3 * 4;

				//s_vboQuadVerts->setWidth(numVerts / 3); // 8 verts -- needs to be 48
				//s_vboQuadTexCoords->setWidth(numVerts / 3); // 8 verts -- needs to be 48
				//s_vboQuadColors->setWidth(numCVerts / 3);  // 16 verts -- needs to be 96

				s_vboQuadVerts->setWidth(2);
				s_vboQuadTexCoords->setWidth(2);
				s_vboQuadColors->setWidth(4);
				s_vboQuadVerts->setHeight(count * 3);
				s_vboQuadTexCoords->setHeight(count * 3);
				s_vboQuadColors->setHeight(count * 3);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(s_shaderBasicTexture_VertexPosition);
				glEnableVertexAttribArray(s_shaderBasicTexture_VertexTexCoordIn);
				glEnableVertexAttribArray(s_shaderBasicTexture_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(verts);
				glVertexAttribPointer(s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
				
 				s_vboQuadTexCoords->bind(); 
				s_vboQuadTexCoords->setData(texcoords);
				glVertexAttribPointer(s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, 0);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(colors);
				glVertexAttribPointer(s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
				
				glUniformMatrix4fv(s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());
				
				glUniform1i(s_shaderBasicTexture_TextureId, 0); 

				glDrawArrays(GL_TRIANGLES, 0, count * 3); 

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadTexCoords->setWidth(2); 
				//s_vboQuadColors->setWidth(4);

				RendererStats::s_glCalls += 10;

			#elif defined(ARK2D_OPENGL_ES_2_0)

				int numVerts = count * 3 * 2;
				int numCVerts = count * 3 * 4;

				glVertexAttribPointer(s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, verts);
				glVertexAttribPointer(s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
				glVertexAttribPointer(s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors); 
			
				glUniformMatrix4fv(s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glUniform1i(s_shaderBasicTexture_TextureId, 0);  

				glDrawArrays(GL_TRIANGLES, 0, count * 3); 

				RendererStats::s_glCalls += 7;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				//ARK2D::getLog()->w("Renderer::texturedTriangles.");

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				__internalsDXUpdateMatrices();

				unsigned int countVerts = count * 3;
				unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingTextureVertexData);
				
				// check memory is the right size. reduce allocations. 
				if (dx_texturedTriangles_data == NULL || dx_texturedTriangles_size < memorySize) {
					dx_texturedTriangles_data = realloc(dx_texturedTriangles_data, memorySize);
					dx_texturedTriangles_size = memorySize;
				}

				Renderer_DX_InterleavingTextureVertexData* rawVertices = (Renderer_DX_InterleavingTextureVertexData*) dx_texturedTriangles_data;
				unsigned int iv = 0;
				unsigned int ic = 0;
				for (unsigned int i = 0; i < countVerts; i += 3) { 
					rawVertices[i].vertex = DirectX::XMFLOAT4(verts[iv], verts[iv + 1], 0.0f, 0.0f);
					rawVertices[i].texcoord = DirectX::XMFLOAT2(texcoords[iv], texcoords[iv + 1]);
					rawVertices[i].color = DirectX::XMFLOAT4(colors[ic] / 255.0f, colors[ic + 1] / 255.0f, colors[ic + 2] / 255.0f, colors[ic + 3] / 255.0f);

					rawVertices[i+1].vertex = DirectX::XMFLOAT4(verts[iv + 2], verts[iv + 3], 0.0f, 0.0f);
					rawVertices[i+1].texcoord = DirectX::XMFLOAT2(texcoords[iv+2], texcoords[iv + 3]);
					rawVertices[i+1].color = DirectX::XMFLOAT4(colors[ic + 4] / 255.0f, colors[ic + 5] / 255.0f, colors[ic + 6] / 255.0f, colors[ic + 7] / 255.0f);

					rawVertices[i+2].vertex = DirectX::XMFLOAT4(verts[iv + 4], verts[iv + 5], 0.0f, 0.0f);
					rawVertices[i+2].texcoord = DirectX::XMFLOAT2(texcoords[iv+4], texcoords[iv + 5]);
					rawVertices[i+2].color = DirectX::XMFLOAT4(colors[ic + 8] / 255.0f, colors[ic + 9] / 255.0f, colors[ic + 10] / 255.0f, colors[ic + 11] / 255.0f);
					iv += 6;
					ic += 12;
				}
				
				// Lock the vertex buffer so it can be written to. Get a pointer to the data in the vertex buffer.
				// Copy the data into the vertex buffer. Unlock the vertex buffer.
				s_vboQuadVerts->setData((void*)&rawVertices[0], memorySize);
				
				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingTextureVertexData);
				unsigned int offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);

				// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// DRAW! 
				deviceContext->Draw(countVerts, 0);

				//free(rawVertices);

			#else
				glVertexPointer(2, GL_FLOAT, 0, verts);
				glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
				glDrawArrays(GL_TRIANGLES, 0, count * 3);

				RendererStats::s_glCalls += 4;
			#endif
 
			RendererStats::s_tris += count; 

			showAnyGlErrorAndExitMacro();
		}

		void Renderer::texturedQuad(unsigned int texId, 
			float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, 
			float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4) 
		{
			
			if (Renderer::isBatching()) { 
				s_batch->addTexturedQuad(
					texId, 
					x1, y1, 
					x2, y2,
					x3, y3,
					x4, y4, 
					tx1, ty1,	// tl
					tx2, ty2,	// tr 
					tx3, ty3,	// bl
					tx4, ty4,	// br
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(),
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);

				return; 
			}

			RendererState::start(RendererState::TEXTURE, texId);

			#if defined(ARK2D_OPENGL_3_2)

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};
				
				float rawTextureCoords[] = {
					tx1,	ty1,	// tl
					tx2,	ty2,	// tr
					tx3,	ty3,	// bl
					tx3,	ty3,	// bl
					tx2,	ty2,	// tr
					tx4, 	ty4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				//s_vboQuadVerts->setWidth(3);
				//s_vboQuadTexCoords->setWidth(3);
				//s_vboQuadColors->setWidth(6);
				s_vboQuadVerts->setWidth(2);
				s_vboQuadTexCoords->setWidth(2);
				s_vboQuadColors->setWidth(4);
				s_vboQuadVerts->setHeight(6);
				s_vboQuadTexCoords->setHeight(6);
				s_vboQuadColors->setHeight(6);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexTexCoordIn);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);

 				s_vboQuadTexCoords->bind();
				s_vboQuadTexCoords->setData(&rawTextureCoords[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawTextureCoords);
				
				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 6);

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadTexCoords->setWidth(2);
				//s_vboQuadColors->setWidth(4);

				RendererStats::s_glCalls += 9;

			#elif defined(ARK2D_OPENGL_ES_2_0)

				/*Renderer_InterleavingTextureVertexData vertices[] = {
					{ {x1, y1},		{tx1, ty1},		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac() } },
					{ {x2, y2},		{tx2, ty2},		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac() } },
					{ {x3, y3},		{tx3, ty3},		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac() } },
					{ {x3, y3},		{tx3, ty3},		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac() } },
					{ {x2, y2},		{tx2, ty2},		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac() } },
					{ {x4, y4},		{tx4, ty4},		{m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac() } }
				};

				char* rawVertices = (char*) &vertices;
				char* rawTextureCoords = ((char*) &vertices) + (sizeof(float)*2);
				unsigned char* rawColors = ((unsigned char*) &vertices) + (sizeof(float)*4); */


				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};
				
				float rawTextureCoords[] = {
					tx1,	ty1,	// tl
					tx2,	ty2,	// tr
					tx3,	ty3,	// bl
					tx3,	ty3,	// bl
					tx2,	ty2,	// tr
					tx4, 	ty4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingTextureVertexData)
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, &rawTextureCoords);
				glVertexAttribPointer(Renderer::s_shaderBasicTexture_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicTexture_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 6;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				__internalsDXUpdateMatrices();

				unsigned int countVerts = 6;
				unsigned int memorySize = countVerts * sizeof(Renderer_DX_InterleavingTextureVertexData);
				if (dx_texturedTriangles_data == NULL || dx_texturedTriangles_size < memorySize) {
					dx_texturedTriangles_data = realloc(dx_texturedTriangles_data, memorySize);
					dx_texturedTriangles_size = memorySize;
				}
				
				Renderer_DX_InterleavingTextureVertexData* rawVertices = (Renderer_DX_InterleavingTextureVertexData*) dx_texturedTriangles_data;
				rawVertices[0].vertex = DirectX::XMFLOAT4(x1, y1, 0.0f, 0.0f);
				rawVertices[0].texcoord = DirectX::XMFLOAT2(tx1, ty1);
				rawVertices[0].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[1].vertex = DirectX::XMFLOAT4(x2, y2, 0.0f, 0.0f);
				rawVertices[1].texcoord = DirectX::XMFLOAT2(tx2, ty2);
				rawVertices[1].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[2].vertex = DirectX::XMFLOAT4(x3, y3, 0.0f, 0.0f);
				rawVertices[2].texcoord = DirectX::XMFLOAT2(tx3, ty3);
				rawVertices[2].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				rawVertices[3].vertex = DirectX::XMFLOAT4(x4, y4, 0.0f, 0.0f);
				rawVertices[3].texcoord = DirectX::XMFLOAT2(tx4, ty4);
				rawVertices[3].color = DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf());
				
				unsigned int rawIndices[] = {0, 1, 2, 2, 1, 3};
				s_vboQuadVerts->setData((void*) rawVertices, memorySize);
				s_vboIndices->setData((void*) rawIndices, sizeof(unsigned int) * 6);
				
				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingTextureVertexData);
				unsigned int offset = 0; 
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);

				deviceContext->IASetIndexBuffer(s_vboIndices->m_buffer, DXGI_FORMAT_R32_UINT, 0);

				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				deviceContext->DrawIndexed(countVerts, 0, 0);

			#else 

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};
				
				float rawTextureCoords[] = {
					tx1,	ty1,	// tl
					tx2,	ty2,	// tr
					tx3,	ty3,	// bl
					tx3,	ty3,	// bl
					tx2,	ty2,	// tr
					tx4, 	ty4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};
 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 4;

			#endif

			RendererStats::s_tris += 2;
			showAnyGlErrorAndExitMacro();

		}

		void Renderer::fillTriangle(int x, int y, int width, int height) const {
			RendererState::start(RendererState::GEOMETRY);

			#if (defined(ARK2D_FLASCC) || defined(NO_FIXED_FUNCTION_PIPELINE))

			#else
				glBegin(GL_TRIANGLES);
					glVertex2i(x + (width/2), y);
					glVertex2i(x, y + height);
					glVertex2i(x + width, y + height);
				glEnd();

				RendererStats::s_tris += 1;
				RendererStats::s_glCalls += 5;
			#endif 
			showAnyGlErrorAndExitMacro();
		}
		
		void Renderer::fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) const {
			
			if (Renderer::isBatching()) { 
				/*float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};*/

				//  
				// DON'T DO THIS IT CRASHES ON MAC!
				// See: http://pastebin.com/PfQ3P9Ji 
				//  
				/*s_batch->addGeometryTri(
					&rawVertices[0], 
					&rawColors[0]
				);*/

				s_batch->addGeometryTri(
					x1, y1, 
					x2, y2, 
					x3, y3, 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				);
				return;
			}

			RendererState::start(RendererState::GEOMETRY);

			#if defined(ARK2D_OPENGL_3_2)

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3
				};
				
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				s_vboQuadVerts->setWidth(2);
				s_vboQuadColors->setWidth(4);
				s_vboQuadVerts->setHeight(3);
				s_vboQuadColors->setHeight(3);

				s_vaoQuad->bind();
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexPosition);
				glEnableVertexAttribArray(Renderer::s_shaderBasicTexture_VertexColorIn);
				
				s_vboQuadVerts->bind();
				s_vboQuadVerts->setData(&rawVertices[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

 				s_vboQuadColors->bind();
				s_vboQuadColors->setData(&rawColors[0]);
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 3);

				//s_vboQuadVerts->setWidth(2);
				//s_vboQuadColors->setWidth(4);
				//s_vboQuadVerts->setHeight(4);
				//s_vboQuadColors->setHeight(4);

				RendererStats::s_tris += 1;
				RendererStats::s_glCalls += 7;

			#elif defined(ARK2D_OPENGL_ES_2_0)

				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3	  	// bl
				};
				
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingTextureVertexData)
				glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
				
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
				glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

				glDrawArrays(GL_TRIANGLES, 0, 3);

				RendererStats::s_tris += 1;
				RendererStats::s_glCalls += 5;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ID3D11Device* device = ARK2D::getContainer()->m_platformSpecific.m_device;
				ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;

				__internalsDXUpdateMatrices();

				Renderer_DX_InterleavingGeometryVertexData rawVertices[] = { 
					{ DirectX::XMFLOAT4(x1, y1, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x2, y2, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) },
					{ DirectX::XMFLOAT4(x3, y3, 0.0f, 0.0f), DirectX::XMFLOAT4(m_DrawColor.getRedf(), m_DrawColor.getGreenf(), m_DrawColor.getBluef(), m_DrawColor.getAlphaf()) }
				};

				
				// Lock the vertex buffer so it can be written to. Get a pointer to the data in the vertex buffer.
				// Copy the data into the vertex buffer. Unlock the vertex buffer.
				s_vboQuadVerts->setData((void*)&rawVertices[0], (unsigned int) sizeof(Renderer_DX_InterleavingGeometryVertexData) * 3);
				
				// Set the vertex buffer to active in the input assembler so it can be rendered.
				unsigned int stride = sizeof(Renderer_DX_InterleavingGeometryVertexData);
				unsigned int offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, &s_vboQuadVerts->m_buffer, &stride, &offset);

				// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// DRAW! 
				deviceContext->Draw(3, 0);

			#else 

				float rawVertices[] = {
					x1,		y1,		// tl 
					x2,		y2,		// tr
					x3,		y3	  	// bl
				};
				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 3);

				RendererStats::s_tris += 1;
				RendererStats::s_glCalls += 3;

			#endif

			
			
			showAnyGlErrorAndExitMacro();
		}

		void Renderer::drawPoint(int x, int y) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_FLASCC) || defined(NO_FIXED_FUNCTION_PIPELINE))
			#else
				glEnable(GL_POINT_SMOOTH);
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
				glDisable(GL_POINT_SMOOTH);
				RendererStats::s_glCalls += 5;
			#endif

			showAnyGlErrorAndExitMacro();
		}
		void Renderer::drawPoint(float x, float y) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_FLASCC) || defined(NO_FIXED_FUNCTION_PIPELINE))
			#else
				glEnable(GL_POINT_SMOOTH);
				glBegin(GL_POINTS);
				glVertex2f(x, y);
				glEnd(); 
				glDisable(GL_POINT_SMOOTH);
				RendererStats::s_glCalls += 5;
			#endif

			showAnyGlErrorAndExitMacro();
		}

		void Renderer::drawCircle(ARK::Geometry::Circle<int>* circle) {
			drawCircle(circle->getCenterX(), circle->getCenterY(), (int) circle->getRadius(), DEFAULT_SEGMENTS);
		}
		void Renderer::drawCircle(ARK::Geometry::Circle<float>* circle) {
			drawCircle(circle->getCenterX(), circle->getCenterY(), (int) circle->getRadius(), DEFAULT_SEGMENTS);
		}
		void Renderer::drawCircle(float x, float y, int radius) {
			drawCircle(x, y, radius, DEFAULT_SEGMENTS);
		}
		void Renderer::drawCircle(float x, float y, int radius, int points) {
			//#if defined(NO_FIXED_FUNCTION_PIPELINE)
				// lots of quads!

				/*float each = 360.0f / float(points);
				float verts[(points+1)*2];
				int j = 0;
				for(float i = 0; i <= 360; i += each) { 
					double angle = 2 * MY_PI * i / 360;
					verts[j] = float(0 + cos(angle) * radius);
					verts[j+1] = float(0 + sin(angle) * radius);
					j+=2;
				}

				drawLine(int x1, int y1, int x2, int y2)*/

				float innerRadius = float(radius) - (m_LineWidth/2.0f);
				float outerRadius = float(radius) + (m_LineWidth/2.0f);

				float currentAngle = 0.0f;
				float eachAngle = 360.0f / float(points);
				float nextAngle = eachAngle;

				#ifdef ARK2D_WINDOWS_VS
					float* verts = (float*) alloca(points * 6 * 2 * sizeof(float));
					unsigned char* colors = (unsigned char*) alloca(points * 6 * 4 * sizeof(unsigned char));
				#else
					float verts[points * 6 * 2];
					unsigned char colors[points * 6 * 4];
				#endif

				int vertsIndex = 0; 
				int colorsIndex = 0;
				for(signed int i = 0; i < points; i++)
				{
					float tl_x = x;
					float tl_y = y;

					float tr_x = x;
					float tr_y = y;

					float bl_x = x;
					float bl_y = y;

					float br_x = x;
					float br_y = y;

					MathUtil::moveAngle(tl_x, tl_y, currentAngle, outerRadius);
					MathUtil::moveAngle(tr_x, tr_y, currentAngle, innerRadius);
					MathUtil::moveAngle(bl_x, bl_y, nextAngle, outerRadius);
					MathUtil::moveAngle(br_x, br_y, nextAngle, innerRadius);

					verts[vertsIndex + 0] = tl_x;
					verts[vertsIndex + 1] = tl_y;
					verts[vertsIndex + 2] = tr_x;
					verts[vertsIndex + 3] = tr_y;
					verts[vertsIndex + 4] = bl_x;
					verts[vertsIndex + 5] = bl_y;

					verts[vertsIndex + 6] = bl_x;
					verts[vertsIndex + 7] = bl_y;
					verts[vertsIndex + 8] = tr_x;
					verts[vertsIndex + 9] = tr_y;
					verts[vertsIndex + 10] = br_x;
					verts[vertsIndex + 11] = br_y;

					colors[colorsIndex + 0] = m_DrawColor.getRedc();
					colors[colorsIndex + 1] = m_DrawColor.getGreenc();
					colors[colorsIndex + 2] = m_DrawColor.getBluec();
					colors[colorsIndex + 3] = m_DrawColor.getAlphac();

					colors[colorsIndex + 4] = m_DrawColor.getRedc();
					colors[colorsIndex + 5] = m_DrawColor.getGreenc();
					colors[colorsIndex + 6] = m_DrawColor.getBluec();
					colors[colorsIndex + 7] = m_DrawColor.getAlphac();

					colors[colorsIndex + 8] = m_DrawColor.getRedc();
					colors[colorsIndex + 9] = m_DrawColor.getGreenc();
					colors[colorsIndex + 10] = m_DrawColor.getBluec();
					colors[colorsIndex + 11] = m_DrawColor.getAlphac();

					colors[colorsIndex + 12] = m_DrawColor.getRedc();
					colors[colorsIndex + 13] = m_DrawColor.getGreenc();
					colors[colorsIndex + 14] = m_DrawColor.getBluec();
					colors[colorsIndex + 15] = m_DrawColor.getAlphac();

					colors[colorsIndex + 16] = m_DrawColor.getRedc();
					colors[colorsIndex + 17] = m_DrawColor.getGreenc();
					colors[colorsIndex + 18] = m_DrawColor.getBluec();
					colors[colorsIndex + 19] = m_DrawColor.getAlphac();

					colors[colorsIndex + 20] = m_DrawColor.getRedc();
					colors[colorsIndex + 21] = m_DrawColor.getGreenc();
					colors[colorsIndex + 22] = m_DrawColor.getBluec();
					colors[colorsIndex + 23] = m_DrawColor.getAlphac();

					currentAngle += eachAngle;
					nextAngle += eachAngle;
					vertsIndex += 12;
					colorsIndex += 24;
				}

				fillTriangles(&verts[0], &colors[0], points*2, false);
				//ARK2D:

				//#ifdef ARK2D_WINDOWS_VS
				//	free(verts);
				//	free(colors);
				//#endif

			//	return;
			/*#else
			//#if defined(ARK2D_ANDROID)
				//ARK2D::getLog()->i("drawCircle - preparation");
 
				float each = 360.0f / float(points);
				float verts[(points+1)*2];
				int j = 0;
				for(float i = 0; i <= 360; i += each) { 
					double angle = 2 * MY_PI * i / 360;
					verts[j] = float(0 + cos(angle) * radius);
					verts[j+1] = float(0 + sin(angle) * radius);
					j+=2;
				}

				//ARK2D::getLog()->i("drawCircle - render");
				//glDisable(GL_TEXTURE_2D);
				//glEnableClientState(GL_VERTEX_ARRAY);
				RendererState::start(RendererState::GEOMETRY);
				pushMatrix();
				translate(x, y);
 
				glVertexPointer(2, GL_FLOAT, 0, verts);
				glDrawArrays(GL_LINE_LOOP, 0, points+1);

				//translate(x * -1, y * -1, 0); 
				popMatrix();

				RendererStats::s_lines += points;
				RendererStats::s_glCalls += 2;

				//glDisableClientState(GL_VERTEX_ARRAY);
				//glEnable(GL_TEXTURE_2D);*/

			/*#else
				float each = 360.0f / float(points);
				glBegin(GL_LINE_LOOP);
				for(float i = 0; i <= 360; i += each){
					double angle = 2 * MY_PI * i / 360;
					glVertex2d(x + cos(angle) * radius, y + sin(angle) * radius);
				}
				glEnd();
			#endif*/

			//#endif
			showAnyGlErrorAndExitMacro();
		}

		void Renderer::fillCircle(float x, float y, int radius, int points) const {

			if (isBatching()) {

				float each = 360.0f / float(points);
				for(signed int i = 0; i < points; i++) {
					float j = float(i) * each;
					double angle = 2 * MY_PI * j / 360; 
					double nangle = 2 * MY_PI * (j+each) / 360; 

					float rawVertices[6];
					unsigned char rawColors[12];

					rawVertices[0] = x;
					rawVertices[1] = y; 
					rawVertices[2] = x + float(cos(angle) * radius);
					rawVertices[3] = y + float(sin(angle) * radius);
					rawVertices[4] = x + float(cos(nangle) * radius);
					rawVertices[5] = y + float(sin(nangle) * radius);

					rawColors[0] = m_DrawColor.getRedc();
					rawColors[1] = m_DrawColor.getGreenc();
					rawColors[2] = m_DrawColor.getBluec(); 
					rawColors[3] = m_DrawColor.getAlphac();
					rawColors[4] = m_DrawColor.getRedc();
					rawColors[5] = m_DrawColor.getGreenc();
					rawColors[6] = m_DrawColor.getBluec(); 
					rawColors[7] = m_DrawColor.getAlphac();
					rawColors[8] = m_DrawColor.getRedc();
					rawColors[9] = m_DrawColor.getGreenc();
					rawColors[10] = m_DrawColor.getBluec(); 
					rawColors[11] = m_DrawColor.getAlphac();

					s_batch->addGeometryTri(&rawVertices[0], &rawColors[0]);
				}

				return;
				
			}

			#if (defined(ARK2D_OPENGL_3_2) || defined(ARK2D_OPENGL_ES_2_0))

				float each = 360.0f / float(points);
				#ifdef ARK2D_WINDOWS_VS
					float* rawVertices = (float*)alloca((points * 3) * 2 * sizeof(float));
					unsigned char* rawColors = (unsigned char*)alloca((points * 3) * 4 * sizeof(unsigned char));
				#else
					float rawVertices[(points * 3) * 2];
					unsigned char rawColors[(points * 3) * 4];
				#endif

				rawVertices[0] = x;
				rawVertices[1] = y; 
				int j = 2;
				for(float i = 0; i <= 360; i += each) {
					double angle = 2 * MY_PI * i / 360; 
					rawVertices[j] = x + float(cos(angle) * radius);
					rawVertices[j+1] = y + float(sin(angle) * radius);
					j+=2;
				}
				rawVertices[j] = x;
				rawVertices[j+1] = y;
				j+=2;

				for(unsigned int i = 0; i < ((points+3)*4); i += 4 ) {
					rawColors[i+0] = m_DrawColor.getRedc();
					rawColors[i+1] = m_DrawColor.getGreenc();
					rawColors[i+2] = m_DrawColor.getBluec(); 
					rawColors[i+3] = m_DrawColor.getAlphac();
				}

				RendererState::start(RendererState::GEOMETRY);

				#if defined(ARK2D_OPENGL_3_2)

					//s_vboQuadVerts->setHeight(j/2);
					//s_vboQuadColors->setHeight(j); 
					s_vboQuadVerts->setWidth(2);
					s_vboQuadColors->setWidth(4);
					s_vboQuadVerts->setHeight(points*3);
					s_vboQuadColors->setHeight(points*3);
				
					s_vaoQuad->bind();
					glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
					glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
			
					s_vboQuadVerts->bind();
					s_vboQuadVerts->setData(&rawVertices[0]);
					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
					
					s_vboQuadColors->bind();
					s_vboQuadColors->setData(&rawColors[0]);
					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 

 					//s_shaderCurrent->predraw();
					
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

					glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

					//s_vboQuadVerts->setHeight(4);
					//s_vboQuadColors->setHeight(4);

					RendererStats::s_glCalls += 7;

				#elif defined(ARK2D_OPENGL_ES_2_0)

					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingGeometryVertexData)
					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
						
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

					glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

					RendererStats::s_glCalls += 5;

				#endif

				//#ifdef ARK2D_WINDOWS_VS
				//	free(rawVertices);
				//	free(rawColors);
				//#endif

				RendererStats::s_tris += points;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ARK2D::getLog()->w("Renderer::fillCircle not implemented in Direct X.");
				
			#else

				// convert fan to TRIANGLES.

				float each = 360.0f / float(points);

				#ifdef ARK2D_WINDOWS_VS
					float* rawVertices = (float*) alloca((points*3)*2 * sizeof(float));
					unsigned char* rawColors = (unsigned char*) alloca((points*3)*4 * sizeof(unsigned char));
				#else
					float rawVertices[(points*3)*2];
					unsigned char rawColors[(points*3)*4];
				#endif

			
				
				int j = 0;
				for(float i = 0; i <= 360; i += each) {
					double angle = 2 * MY_PI * i / 360; 
					double nangle = 2 * MY_PI * (i+each) / 360; 
					rawVertices[j+0] = x;
					rawVertices[j+1] = y; 
					rawVertices[j+2] = x + float(cos(angle) * radius);
					rawVertices[j+3] = y + float(sin(angle) * radius);
					rawVertices[j+4] = x + float(cos(nangle) * radius);
					rawVertices[j+5] = y + float(sin(nangle) * radius);
					j+=6;
				}
			
				for(signed int i = 0; i < ((points*3)*4); i += 12) {
					rawColors[i+0] = m_DrawColor.getRedc();
					rawColors[i+1] = m_DrawColor.getGreenc();
					rawColors[i+2] = m_DrawColor.getBluec(); 
					rawColors[i+3] = m_DrawColor.getAlphac();

					rawColors[i+4] = m_DrawColor.getRedc();
					rawColors[i+5] = m_DrawColor.getGreenc();
					rawColors[i+6] = m_DrawColor.getBluec(); 
					rawColors[i+7] = m_DrawColor.getAlphac();

					rawColors[i+8] = m_DrawColor.getRedc();
					rawColors[i+9] = m_DrawColor.getGreenc();
					rawColors[i+10] = m_DrawColor.getBluec(); 
					rawColors[i+11] = m_DrawColor.getAlphac();
				}

				RendererState::start(RendererState::GEOMETRY);

				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, points*3);

				RendererStats::s_glCalls += 4; 
				RendererStats::s_tris += points;

				//#ifdef ARK2D_WINDOWS_VS
				//	free(rawVertices);
				//	free(rawColors);
				//#endif

				/*
					
				float rawVertices[] = {
					x1,		y1,		// tl
					x2,		y2,		// tr
					x3,		y3,	  	// bl
					x3,		y3,	  	// bl
					x2,		y2,		// tr
					x4, 	y4		// br
				};
				
				float rawTextureCoords[] = {
					tx1,	ty1,	// tl
					tx2,	ty2,	// tr
					tx3,	ty3,	// bl
					tx3,	ty3,	// bl
					tx2,	ty2,	// tr
					tx4, 	ty4		// br
				};

				unsigned char rawColors[] = {
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac(), 
					m_DrawColor.getRedc(), m_DrawColor.getGreenc(), m_DrawColor.getBluec(), m_DrawColor.getAlphac()
				};
 
				glVertexPointer(2, GL_FLOAT, 0, rawVertices);
				glTexCoordPointer(2, GL_FLOAT, 0, rawTextureCoords);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				RendererStats::s_glCalls += 4;

				*/

			// #else 
			 
			// 	// windows 
			// 	ARK2D::getLog()->e("fillCircle not implemented on Windows.");

			#endif  

			

			showAnyGlErrorAndExitMacro();
		}

		void Renderer::fillEllipse(float x, float y, float rx, float ry) const 
		{
			const int points = 30; 

			#if (defined(ARK2D_OPENGL_3_2) || defined(ARK2D_OPENGL_ES_2_0))

				float each = 360.0f / float(points);
				float rawVertices[(points+3)*2];
				unsigned char rawColors[(points+3)*4];
				rawVertices[0] = x;
				rawVertices[1] = y; 
				int j = 2; 
				for(float i = 0; i <= 360; i += each) {
					double angle = 2 * MY_PI * i / 360; 
					rawVertices[j] = x + float(cos(angle) * rx);
					rawVertices[j+1] = y + float(sin(angle) * ry);
					j+=2;
				}
				rawVertices[j] = x;
				rawVertices[j+1] = y;
				j+=2;

				for(unsigned int i = 0; i < ((points+3)*4); i += 4 ) {
					rawColors[i+0] = m_DrawColor.getRedc();
					rawColors[i+1] = m_DrawColor.getGreenc();
					rawColors[i+2] = m_DrawColor.getBluec(); 
					rawColors[i+3] = m_DrawColor.getAlphac();
				}

				RendererState::start(RendererState::GEOMETRY);

				#if defined(ARK2D_OPENGL_3_2)

					//s_vboQuadVerts->setHeight(j/2);
					//s_vboQuadColors->setHeight(j); 
					s_vboQuadVerts->setWidth(2);
					s_vboQuadColors->setWidth(4);
					s_vboQuadVerts->setHeight((points+3) * 2);
					s_vboQuadColors->setHeight((points+3) * 2);
				
					s_vaoQuad->bind();
					glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexPosition);
					glEnableVertexAttribArray(Renderer::s_shaderBasicGeometry_VertexColorIn);
			
					s_vboQuadVerts->bind();
					s_vboQuadVerts->setData(&rawVertices[0]);
					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, 0); //&rawVertices);
					
					s_vboQuadColors->bind();
					s_vboQuadColors->setData(&rawColors[0]);
					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); //&rawColors); 
					
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

					glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

				//	s_vboQuadVerts->setHeight(4);
				//	s_vboQuadColors->setHeight(4); 

					RendererStats::s_glCalls += 7;

				#elif defined(ARK2D_OPENGL_ES_2_0)

					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexPosition, 2, GL_FLOAT, GL_FALSE, 0, &rawVertices); // sizeof(Renderer_InterleavingGeometryVertexData)
					glVertexAttribPointer(Renderer::s_shaderBasicGeometry_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &rawColors); 
						
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ModelViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODELVIEW)->pointer());
					glUniformMatrix4fv(Renderer::s_shaderBasicGeometry_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());

					glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

					RendererStats::s_glCalls += 5;

				#endif

				RendererStats::s_tris += points;

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ARK2D::getLog()->w("Renderer::fillEllipse not implemented in Direct X.");
				
			#else

				

				float each = 360.0f / float(points);
				

				#ifdef ARK2D_WINDOWS_VS
					float* verts = (float*) alloca((points+3)*2 * sizeof(float));
				#else
					float verts[(points+3)*2];
				#endif 

				verts[0] = 0;
				verts[1] = 0; 
				int j = 2;
				for(float i = 0; i <= 360; i += each) {
					double angle = 2 * MY_PI * i / 360;
					verts[j] = float(0 + cos(angle) * rx);
					verts[j+1] = float(0 + sin(angle) * ry);
					j+=2;
				}
				verts[j] = 0;
				verts[j+1] = 0;
				
				RendererState::start(RendererState::GEOMETRY);
				pushMatrix();
				translate(x, y);

				glVertexPointer(2, GL_FLOAT, 0, verts);
				glDrawArrays(GL_TRIANGLE_FAN, 0, points+2);

				//translate(x * -1, y * -1, 0);
				popMatrix();

				RendererStats::s_tris += points;
				RendererStats::s_glCalls += 2;
 
				//#ifdef ARK2D_WINDOWS_VS
				//	free(verts);
				//#endif
			#endif

			showAnyGlErrorAndExitMacro();
		}

		void Renderer::drawCircleSpikey(float x, float y, int radius, int points) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(NO_FIXED_FUNCTION_PIPELINE))

			#else
				float each = 360.0f / float(points);
				glBegin(GL_LINE_LOOP);
				for(float angle = 0; angle < 360; angle += each) {
					glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
				}
				glEnd();

				RendererStats::s_lines += points;
				RendererStats::s_glCalls += 2 + points;
			#endif
		}
		void Renderer::fillCircleSpikey(float x, float y, int radius, int points) const {
			#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(NO_FIXED_FUNCTION_PIPELINE))
			#else
				float each = 360.0f / float(points);
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, y);
				for (float angle = 0; angle < 360; angle += each) {
					glVertex2f(x + sin(angle) * radius, y + cos(angle) * radius);
				}
				glEnd();

				RendererStats::s_tris += points;
				RendererStats::s_glCalls += 2 + points;
			#endif
		}

		void Renderer::setDrawColor(string color) {
			m_DrawColor.set(color);
		}
		void Renderer::setDrawColor(string color, float alpha) {
			m_DrawColor.set(color);
			m_DrawColor.setAlpha(alpha);
		}
		void Renderer::setDrawColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
			m_DrawColor.setRed(r);
			m_DrawColor.setGreen(g);
			m_DrawColor.setBlue(b);
			m_DrawColor.setAlpha(a); 

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glColor4f(m_DrawColor.getRed()/255.f, m_DrawColor.getGreen()/255.f, m_DrawColor.getBlue()/255.f, m_DrawColor.getAlpha()/255.f);
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::setDrawColorf(float r, float g, float b, float a) {
			m_DrawColor.setRed((float) r);
			m_DrawColor.setGreen((float) g); 
			m_DrawColor.setBlue((float) b);
			m_DrawColor.setAlpha((float) a);

			#ifndef NO_FIXED_FUNCTION_PIPELINE
				glColor4f(r, g, b, a);
				RendererStats::s_glCalls++;
			#endif
		}
		void Renderer::setDrawColor(const Color& c) {
			m_DrawColor = c;
			c.bind();
			//glColor4b(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
			//glColor4f(c.getRed()/255.f, c.getGreen()/255.f, c.getBlue()/255.f, c.getAlpha()/255.f);
		}
		void Renderer::setDrawColor(Color* c) {
			setDrawColor(c->getRed(), c->getGreen(), c->getBlue(), c->getAlpha());
		}
		const Color& Renderer::getDrawColor() const {
			return m_DrawColor;
		}

		void Renderer::setMaskColor(const Color& c) {
			m_MaskColor = c;
		}
		const Color& Renderer::getMaskColor() const {
			return m_MaskColor;
		}

		void Renderer::setPointSize(float f) {
			#ifndef NO_FIXED_FUNCTION_PIPELINE 
				glPointSize(f);
			#endif
		}
		float Renderer::getPointSize() {
			return m_pointSize;
		}

		unsigned int Renderer::getMaxTextureSize() {
			#ifdef ARK2D_RENDERER_OPENGL 
				#if defined(ARK2D_IPHONE) || defined(ARK2D_ANDROID) || defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS)
					//ARK2D::getLog()->w("Renderer::getMaxTextureSize not supported. ");
					return 0;  
				#else
					GLint max; 
					glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
					return (unsigned int) max;
				#endif
			#endif
			return 0;
		}

		void Renderer::setLineWidth(unsigned int i) {
			//#ifndef NO_FIXED_FUNCTION_PIPELINE
				//glLineWidth(i);
			//#endif
			m_LineWidth = i;
		}
		unsigned int Renderer::getLineWidth() {
			return m_LineWidth;
		}

		void Renderer::setBlendMode(unsigned int blendMode) {
			if (m_blendMode == blendMode) { return; }

			#ifdef ARK2D_RENDERER_OPENGL

				if (blendMode == BLEND_NONE) {
					glDisable(GL_BLEND);
					RendererStats::s_glCalls++;
				} else {
					if (m_blendMode == BLEND_NONE) { // it was disabled, enable it.
						glEnable(GL_BLEND);	
						RendererStats::s_glCalls++;
					}
					
					if (blendMode == BLEND_NORMAL) {
						glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					} else if (blendMode == BLEND_ADDITIVE) {
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					} else if (blendMode == BLEND_SPINE) {
						glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
					} else if (blendMode == BLEND_TEST) {
						//glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
						glBlendFunc(GL_DST_COLOR, GL_ZERO);
						//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
						//glDepthFunc( GL_LEQUAL);
						//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
					} else if (blendMode == BLEND_MASK_SET) {
						glBlendFunc(GL_ONE, GL_ZERO);
						glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);

						RendererStats::s_glCalls++;
					} else if (blendMode == BLEND_MASK_USE) {
						glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
					}
					RendererStats::s_glCalls++;
				}


			#elif defined(ARK2D_RENDERER_DIRECTX) 
				
				//ARK2D::getLog()->w("Renderer::setBlendMode not implemented in Direct X.");
				GameContainerPlatform* platform = ARK2D::getContainer()->getPlatformSpecific();
				ID3D11Device* device = platform->m_device;
				ID3D11DeviceContext* deviceContext = platform->m_deviceContext;

				if (blendMode == BLEND_NORMAL) {
					float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
					deviceContext->OMSetBlendState(platform->m_blendState, blendFactor, 0xFFFFFFFF);
				} else if (blendMode == BLEND_ADDITIVE) {
					float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
					deviceContext->OMSetBlendState(platform->m_blendStateAdditive, blendFactor, 0xFFFFFFFF);
				}
				RendererStats::s_glCalls++;

			#endif

			m_blendMode = blendMode;
		}
		unsigned int Renderer::getBlendMode() {
			return m_blendMode;
		}

		void Renderer::setScissorBoxColors(const Color& top, const Color& left, const Color& bottom, const Color& right) {
			m_ScissorBoxColors[0] = top;
			m_ScissorBoxColors[1] = left;
			m_ScissorBoxColors[2] = bottom;
			m_ScissorBoxColors[3] = right;
		}
		void Renderer::drawScissorBoxes() { 
			GameContainer* container = ARK2D::getContainer();
			// draw scissor boxes because glScissor does not work on some HTC phones.
			//Color cc = container->getClearColor();
			//setDrawColor(cc.getRed(), cc.getGreen(), cc.getBlue(), cc.getAlpha());

			// if (container->isOrientationInverted()) {

			// 	int xAxisBoxLength = (int) container->getTranslateY();
			// 	// top edge (from the game's point of view... left side on the screen)
			// 	setDrawColor(m_ScissorBoxColors[0].getRed(), m_ScissorBoxColors[0].getGreen(), m_ScissorBoxColors[0].getBlue(), m_ScissorBoxColors[0].getAlpha());
			// 	fillRect(0, 0, xAxisBoxLength, container->getDynamicHeight());

			// 	// bottom edge (from the game's point of view... right side on the screen)
			// 	setDrawColor(m_ScissorBoxColors[2].getRed(), m_ScissorBoxColors[2].getGreen(), m_ScissorBoxColors[2].getBlue(), m_ScissorBoxColors[2].getAlpha());
			// 	fillRect(0, xAxisBoxLength + (container->getHeight()*container->getScaleY()),
			// 				xAxisBoxLength, container->getDynamicHeight());
			// } else {
				float width = container->getWidth();
				float height = container->getHeight();
				float dynamicWidth = container->getDynamicWidth();
				float dynamicHeight = container->getDynamicHeight();
				if (container->isOrientationInverted()) {
					width = container->getHeight();
					height = container->getWidth();
					dynamicWidth = container->getDynamicHeight();
					dynamicHeight = container->getDynamicWidth();
				}

				// left edge  
				setDrawColor(m_ScissorBoxColors[1].getRed(), m_ScissorBoxColors[1].getGreen(), m_ScissorBoxColors[1].getBlue(), m_ScissorBoxColors[1].getAlpha());
				fillRect(0,0, (int) container->getTranslateX(), dynamicHeight);// * container->getScaleY());

				// right edge
				setDrawColor(m_ScissorBoxColors[3].getRed(), m_ScissorBoxColors[3].getGreen(), m_ScissorBoxColors[3].getBlue(), m_ScissorBoxColors[3].getAlpha());
				fillRect(container->getTranslateX() + (width * container->getScaleX()), 0,
							(int) container->getTranslateX()+3, dynamicHeight);// * container->getScaleY());

				// top edge
				setDrawColor(m_ScissorBoxColors[0].getRed(), m_ScissorBoxColors[0].getGreen(), m_ScissorBoxColors[0].getBlue(), m_ScissorBoxColors[0].getAlpha());
				fillRect(0,0-1, dynamicWidth, (int) container->getTranslateY()+1);

				// bottom edge
				setDrawColor(m_ScissorBoxColors[2].getRed(), m_ScissorBoxColors[2].getGreen(), m_ScissorBoxColors[2].getBlue(), m_ScissorBoxColors[2].getAlpha());
				fillRect(0, container->getTranslateY() + (height * container->getScaleY()),
							dynamicWidth+1, (int) container->getTranslateY()+1);
							//container->getWidth() * container->getScaleX(), container->getTranslateY());
			//}

			setDrawColor(Color::white); 
		}

		
		void Renderer::__internalsDrawArray_triangleStrip(int first, int amount) {
			#ifdef ARK2D_RENDERER_OPENGL
				glDrawArrays(GL_TRIANGLE_STRIP, first, amount);
			#elif defined(ARK2D_RENDERER_DIRECTX)
				ARK2D::getLog()->w("Renderer::__internalsDrawArray_triangleStrip not implemented in Direct X.");
			#endif
			RendererStats::s_glCalls++;
		}
		void Renderer::__internalsDrawArray_triangles(int first, int amount) {
			#ifdef ARK2D_RENDERER_OPENGL
				glDrawArrays(GL_TRIANGLES, first, amount);
			#elif defined(ARK2D_RENDERER_DIRECTX)
				ARK2D::getLog()->w("Renderer::__internalsDrawArray_triangles not implemented in Direct X.");
			#endif
			RendererStats::s_glCalls++;
		}

		string Renderer::toString() {
			string s = "";
			s += StringUtil::append("RendererState::s_renderMode: ", RendererState::s_renderMode);
			s += ", \n";
			s += StringUtil::append("RendererState::s_textureId: ", RendererState::s_textureId);
			s += ", \n";
			s += StringUtil::append("RendererState::s_shaderId: ", RendererState::s_shaderId);
			//s += ", \n";
			//s += StringUtil::append("RendererState::s_multitextureId0: ", RendererState::s_multitextureId0);
			s += ", \n";
            s += StringUtil::append("Renderer::batching: ", Cast::boolToString(isBatching()));
			return s;
		}
	}
}
