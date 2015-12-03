/*
 * Renderer.h
 *
 *  Created on: 15-Oct-2009
 *      Author: user
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
#include "../Font/BMFont.h"
#include "Color.h"
#include "../Util/StringUtil.h"
#include "../Util/Matrix44.h"
#include "../Font/Font.h"

#include "../Includes.h"
#include "../Namespaces.h"



namespace ARK { 
	namespace Graphics {

		


		class ARK2D_API RendererStats 
		{
			public: 
				static unsigned int s_glCalls;
				//static unsigned int s_glDrawCalls;
				static unsigned int s_lines;
				static unsigned int s_tris;
				static unsigned int s_textureSwaps;	
				static unsigned int s_shaderSwaps;	
				static unsigned int s_textureAllocatedBytes;

				static unsigned int s_previousframe_glCalls;
				static unsigned int s_previousframe_lines;
				static unsigned int s_previousframe_tris;
				static unsigned int s_previousframe_textureSwaps;
				static unsigned int s_previousframe_shaderSwaps;
				static void reset();
				static void endframe();

				RendererStats();
				virtual ~RendererStats();
		};

		class ARK2D_API RendererState {
			public:
				static const int NONE = 0;
				static const int GEOMETRY = 1;
				static const int TEXTURE = 2;
				static const int SHADER = 3;
				//static const int MULTITEXTURE = 4;

				static int s_renderMode;
				static int s_textureId;
				static int s_shaderId;
				static int s_multitextureId0;
				static int s_multitextureId1;

				static Shader* start(int renderMode);
				static Shader* start(int renderMode, int textureId);
				//static void start(int renderMode, int textureId, int textureId2);
				//static void start(int renderMode, Shader* s);

				static void startNone();
				static void startGeometry();
				static void startTexture(int textureId);
				static void startShader(int shaderId);
				//static void startMultitexture(int textureId0, int textureId1);

                static void internalBindTexture(int textureId);
				
		};

		class ARK2D_API MatrixStack {
			public:
				unsigned int m_type;

				vector<Matrix44<float> > m_stack;
				unsigned int m_stackIndex;
				unsigned int m_root; // the lowest stack index after ark2d does its bits. e.g. letterboxing. 
									 // we reset to this instead of the identity matrix when starting new renderbatches.

				bool m_dirty;

				static const unsigned int TYPE_NULL = 0;
				static const unsigned int TYPE_PROJECTION = 1; 
				static const unsigned int TYPE_VIEW = 2;
				static const unsigned int TYPE_MODEL = 3;
	 
				MatrixStack(unsigned int type); 
				void translate(float x, float y, float z = 0.0f); 			// glTranslatef(x,y,0);
				void rotate(float angle, float x, float y, float z = 0.0f); // glRotatef(angle, 0, 0, 1);
				void scale(float x, float y, float z = 0.0f); 				// glScalef(x, y, 0.0f);
				void ortho2d(float l, float r, float b, float t, float n, float f);
				void frustum(float l, float r, float b, float t, float n, float f);
				void perspective(float fov, float aspect, float n, float f);
				void lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
				void identity();
				float* data(float* mx);
				void* pointer();  
				#ifdef ARK2D_RENDERER_DIRECTX
					DirectX::XMMATRIX d3dpointer();  
				#endif
				Matrix44<float>* current();
				Matrix44<float>* at(unsigned int index);
				void pushMatrix(bool setroot = false);
				void popMatrix();
				void toStringDialog();
				bool isDirty();
				void setDirty(bool b);
				unsigned int height();
				unsigned int root();

				virtual ~MatrixStack();

				
		};

		class ARK2D_API VBO {
			public:
				unsigned int m_id;
				unsigned int m_msize;
				#ifdef ARK2D_RENDERER_DIRECTX
					ID3D11Buffer* m_buffer; 
					D3D11_BUFFER_DESC m_bufferDesc;
					D3D11_SUBRESOURCE_DATA m_bufferData;
					HRESULT m_result;
					D3D11_BIND_FLAG m_indexType;
				#endif

				VBO();
				unsigned int getId();
				void init(); 
				virtual void setSize(unsigned int szbytes);
				virtual void setData(void* data, unsigned int size);
				virtual void bind();
				void unbind();
		};
		class ARK2D_API QuadVBO : public VBO {
			public:
				int m_width; // x,y,z
				int m_height; // for each vertex
				QuadVBO();
				void setWidth(int w);
				void setHeight(int h);
				void setData(void* data, unsigned int size);
				void setData(float* data);
				void setData(unsigned char* data);
		};
		class ARK2D_API Mat44VBO : public VBO {
			public:
				Mat44VBO();
				void setData(float* data);
				void bind();
		};
        class ARK2D_API Mat33VBO : public VBO {
            public:
                Mat33VBO();
                void setData(float* data);
                void bind();
        };

		class ARK2D_API VAO {
			public:
				static signed int s_currentVAO;
				unsigned int m_id;

				VAO();
				signed int getId();
				void init();
				bool bind(); 
		};

		

		/*!
		* \brief Batched rendering, automatically.
		* 
		* @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		*/
		//class ARK2D_API RendererBatchItem_StencilStart {
		class ARK2D_API RendererBatchItem_GeomTri {
			public:
				float vertexData[9];
				float normalData[9];
				unsigned char colorData[12];
				RendererBatchItem_GeomTri();
		};
		class ARK2D_API RendererBatchItem_TexTri {
			public:
				float vertexData[9];
				float normalData[9];
				float textureData[6];
				unsigned char colorData[12];
				RendererBatchItem_TexTri();
		};

		class ARK2D_API RendererBatchItem {
			public:
				static const unsigned int TYPE_GEOMETRY_TRIS = 0;
				static const unsigned int TYPE_TEXTURE_TRIS = 1;
				static const unsigned int TYPE_STENCIL_ENABLE = 2;
				static const unsigned int TYPE_STENCIL_START = 3;
				static const unsigned int TYPE_STENCIL_INVERSE = 4;
				static const unsigned int TYPE_STENCIL_STOP = 5; 
				static const unsigned int TYPE_STENCIL_DISABLE = 6;
				static const unsigned int TYPE_MULTISAMPLING_ENABLE = 7;
				static const unsigned int TYPE_MULTISAMPLING_DISABLE = 8;
				static const unsigned int TYPE_BACKFACECULLING_ENABLE = 9;
				static const unsigned int TYPE_BACKFACECULLING_DISABLE = 10;
				static const unsigned int TYPE_CUSTOM_OBJECT_FUNCTION = 11;
				static const unsigned int TYPE_CUSTOM_FUNCTION = 12;

			public:
				vector<RendererBatchItem_GeomTri> geomtris;
				vector<RendererBatchItem_TexTri> textris;
				unsigned int m_type;
				unsigned int m_textureId;
				unsigned int m_shaderId;
				void* m_objectPointer;
				void* m_functionPointer;

			public:
				RendererBatchItem();
				void clear();
				void render();
				string toString();
				~RendererBatchItem();

			private:
				void renderDX();
		};
		
		class ARK2D_API RendererBatch {
			public:
				vector<RendererBatchItem> items;
				bool enabled;

				// We don't need to do matrix multiplication if we don't change the matrices.
				unsigned int startedAtMatrixIndex; 

			public:
				RendererBatch();
				inline bool isEnabled() { return enabled; }
				void setEnabled(bool b, bool fromSceneGraph = false);

				void addGeometryTri(float* verts, float* normals, unsigned char* colors);
				void addGeometryTri(
					float x1, float y1, float z1,
					float x2, float y2, float z2,
					float x3, float y3, float z3,
					float nx1, float ny1, float nz1,
					float nx2, float ny2, float nz2,
					float nx3, float ny3, float nz3,
					unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
					unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
					unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a
				);
				void addGeometryQuad(
					float x1, float y1, float z1,
					float x2, float y2, float z2,
					float x3, float y3, float z3,
					float x4, float y4, float z4,
					float nx1, float ny1, float nz1,
					float nx2, float ny2, float nz2,
					float nx3, float ny3, float nz3,
					float nx4, float ny4, float nz4,
					unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
					unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
					unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a,
					unsigned char c4r, unsigned char c4g, unsigned char c4b, unsigned char c4a);
				
				void addTexturedTri(unsigned int texId, float* verts, float* normals, float* texcoords, unsigned char* colors);
				void addTexturedTri(
					unsigned int texId, 
					float x1, float y1, float z1,
					float x2, float y2, float z2,
					float x3, float y3, float z3,
					float nx1, float ny1, float nz1,
					float nx2, float ny2, float nz2,
					float nx3, float ny3, float nz3,
					float tx1, float ty1, float tx2, float ty2, float tx3, float ty3,
					unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
					unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
					unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a);

				void addTexturedQuad(unsigned int texId, float* verts, float* normals, float* texcoords, unsigned char* colors);
				void addTexturedQuad(
					unsigned int texId, 
					float x1, float y1, float z1,
					float x2, float y2, float z2,
					float x3, float y3, float z3,
					float x4, float y4, float z4,
					float nx1, float ny1, float nz1,
					float nx2, float ny2, float nz2,
					float nx3, float ny3, float nz3,
					float nx4, float ny4, float nz4,
					float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4,
					unsigned char c1r, unsigned char c1g, unsigned char c1b, unsigned char c1a,
					unsigned char c2r, unsigned char c2g, unsigned char c2b, unsigned char c2a,
					unsigned char c3r, unsigned char c3g, unsigned char c3b, unsigned char c3a,
					unsigned char c4r, unsigned char c4g, unsigned char c4b, unsigned char c4a);

				string toString();
				void render(); 
				~RendererBatch();
		};



		/*!
		 * \brief Contains rendering methods.
		 *
		 * @warning Primitive shapes are slow on most mobile devices so try to use textures where you can.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Renderer
		{
			public:

			// GL bits. :| 
			#if ( defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH) || defined(ARK2D_UBUNTU_LINUX) || defined(ARK2D_EMSCRIPTEN_JS) || (defined(ARK2D_WINDOWS) && defined(ARK2D_RENDERER_OPENGL)))

				#define GL_FRAMEBUFFER_ARK GL_FRAMEBUFFER
				#define GL_COLOR_ATTACHMENT0_ARK GL_COLOR_ATTACHMENT0
				#define GL_FRAMEBUFFER_COMPLETE_ARK GL_FRAMEBUFFER_COMPLETE

				#define glGenFramebuffersARK glGenFramebuffers
				#define glBindFramebufferARK glBindFramebuffer 
				#define glFramebufferTexture2DARK glFramebufferTexture2D
				#define glCheckFramebufferStatusARK glCheckFramebufferStatus

			
				#if (defined(ARK2D_ANDROID) || defined(ARK2D_IPHONE) || defined(ARK2D_EMSCRIPTEN_JS))
					typedef void (GL_APIENTRYP PFNGLGENVERTEXARRAYSOESPROC) (GLsizei n, GLuint *arrays);
					typedef void (GL_APIENTRYP PFNGLBINDVERTEXARRAYOESPROC) (GLuint n);
					
					PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
					PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;

					
					//void glGenVertexArrays(GLsizei n, GLuint* arrays);
					//void glBindVertexArray(GLuint n);

					// vaos
					#define glGenVertexArraysARK ARK2D::getRenderer()->glGenVertexArraysOES
					#define glBindVertexArrayARK ARK2D::getRenderer()->glBindVertexArrayOES
				
				#elif defined(ARK2D_UBUNTU_LINUX)

					#define glGenVertexArraysARK gl3wGenVertexArrays
					#define glBindVertexArrayARK gl3wBindVertexArray

				#else

					#define glGenVertexArraysARK glGenVertexArrays
					#define glBindVertexArrayARK glBindVertexArray

				#endif

				bool gluCheckExtensionARK(const GLubyte* extName, const GLubyte* extString);

			#else
				#define GL_FRAMEBUFFER_ARK GL_FRAMEBUFFER_EXT
				#define GL_COLOR_ATTACHMENT0_ARK GL_COLOR_ATTACHMENT0_EXT
				#define GL_FRAMEBUFFER_COMPLETE_ARK GL_FRAMEBUFFER_COMPLETE_EXT

				#define glGenFramebuffersARK glGenFramebuffersEXT
				#define glBindFramebufferARK glBindFramebufferEXT
				#define glFramebufferTexture2DARK glFramebufferTexture2DEXT
				#define glCheckFramebufferStatusARK glCheckFramebufferStatusEXT

				bool gluCheckExtensionARK(const GLubyte* extName, const GLubyte* extString);

				//#ifndef ARK2D_WINDOWS_VS
					void glGenVertexArrays(GLsizei n, GLuint* arrays);
					void glBindVertexArray(GLuint array);
				//#endif

				#define glGenVertexArraysARK ARK2D::getRenderer()->glGenVertexArrays
				#define glBindVertexArrayARK ARK2D::getRenderer()->glBindVertexArray

			#endif

			friend class ARK::Core::GameContainer;
			friend class ARK::Core::GameContainerPlatform;
			
			private:
				static const int DEFAULT_SEGMENTS = 50;

			// Image interpolation
			private:
				static signed int s_interpolation;
			public:
				static const signed int INTERPOLATION_NEAREST = 0;
				static const signed int INTERPOLATION_LINEAR = 1;
				//static const signed int INTERPOLATION_BICUBIC = 2;

				static signed int getInterpolation();
				static void setInterpolation(signed int i);
				static unsigned int getInterpolationGL();


			// Matrix bits.
			private:
				static MatrixStack* s_matrix;
				static MatrixStack* s_matrixProjection;
				static MatrixStack* s_matrixView;
				static MatrixStack* s_matrixModel;
				static Matrix33<float>* s_matrixNormal;
			public:
				static MatrixStack* getMatrix();
				static MatrixStack* getMatrix(unsigned int type);
				static void multiplyMatrix(Matrix44<float>* by);
				static void matrixMode(unsigned int mode);
				static Matrix33<float>* getNormalMatrix();

			#if defined (ARK2D_RENDERER_DIRECTX)
				static ID3D11Buffer* s_d3d_matrixBuffer;
				static CD3D11_BUFFER_DESC s_d3d_matrixBufferDesc;
				static Renderer_DX_ModelViewProjectionMatrixBuffer* s_d3d_matrixBufferData;
			#endif

 	
 			// Shader bits.
			public:
				static Shader* s_shaderCurrent;
				static Shader* getShader();
				
				
				static Shader* s_shaderBasicGeometry;
				static Shader* s_shaderBasicTexture;
				static Shader* s_shaderBasicGeometryDefault;
				static Shader* s_shaderBasicTextureDefault;
				static Shader* getBasicGeometryShader();
				static Shader* getBasicTextureShader();
				static void overrideBasicShaders(Shader* geometry, Shader* texture);
				static void resetBasicShaders();

			// VBO bits
			public:
				static Mat44VBO* s_vboMatrixProjection;
				static Mat44VBO* s_vboMatrixView;
				static Mat44VBO* s_vboMatrixModel;
                static Mat33VBO* s_vboMatrixNormal;
				static QuadVBO* s_vboQuadVerts;
				static QuadVBO* s_vboQuadTexCoords;
				static QuadVBO* s_vboQuadNormals;
				static QuadVBO* s_vboQuadColors;
				static VBO* s_vboIndices;
				static VAO* s_vaoQuad;


			// Batch processing bits
			public:
				static RendererBatch* s_batch;
				static inline RendererBatch* getBatch() { return s_batch; }
				static inline bool isBatching() { return (s_batch->isEnabled()); }


			public:
				static const signed int ALIGN_START = -1;
				static const signed int ALIGN_LEFT = -1;
				static const signed int ALIGN_TOP = -1;

				static const signed int ALIGN_CENTER = 0;

				static const signed int ALIGN_END = 1;
				static const signed int ALIGN_RIGHT = 1;
				static const signed int ALIGN_BOTTOM = 1;

				


			

				Renderer();
				void preinit();
				void init();

				// this call needs freeing afterwards.
				void* readPixels(int x, int y, int w, int h);
				void flush();  
				void finish(); 
 
				void setFont(ARK::Font::Font* f);
				void setDefaultFont(ARK::Font::Font* f);
				ARK::Font::Font* getFont() const ;
				ARK::Font::Font* getDefaultFont() const;

				void drawString(const std::string str, int x, int y) const;
				void drawString(const std::string str, float x, float y, signed int alignX, signed int alignY, float rotation=0.0f, float scale=1.0f);
				void drawStringCenteredAt(const std::string str, int x, int y) const;
				void drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing) const;
				void drawStringWordWrap(const std::string str, int x, int y, int maxWidth, int ySpacing, int alignX) const;

			public:

				void readPixels(void* data, int x, int y, int w, int h);

				void setDrawColor(string color);
				void setDrawColor(string color, float alpha);
				void setDrawColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
				void setDrawColorf(float r, float g, float b, float a);
				void setDrawColor(const Color& c);
				void setDrawColor(Color* c);
				const Color& getDrawColor() const;

				void setMaskColor(const Color& c);
				const Color& getMaskColor() const;

				void translate(int x, int y) const;
				void translate(float x, float y) const;
				void translate(float x, float y, float z) const;

				void rotate(int angle) const;
				void rotate(float angle) const;
				void rotate(float angle, float x, float y, float z) const;

				void scale(float x, float y) const;
				void scale(float x, float y, float z) const;

				void setScissorTestEnabled(bool b) const;
				void scissor(int x, int y, int w, int h) const;

				void viewport(int x, int y, int w, int h) const;

				void ortho2d(int x, int y, int w, int h);
				void ortho2d(int x, int y, int w, int h, float near, float far);

				void enableStencil() const;
				void startStencil() const;
				void inverseStencil() const;
				void stopStencil() const;
				void disableStencil() const;

				void enableMultisampling() const;
				void disableMultisampling() const; 

				void enableBackfaceCulling() const;
				void disableBackfaceCulling() const; 

				void pushMatrix(bool setasroot = false) const;
				void popMatrix() const;
				void loadIdentity() const;

				void fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle) const;
				void fillArc(float cx, float cy, int width, int height, float startAngle, float endAngle, int segs) const;

				void drawLine(int x1, int y1, int x2, int y2) const;
				void drawLine(float x1, float y1, float x2, float y2) const;
				void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
				void texturedLineOverlay_data(float* data, Image* img, float scale, float x1, float y1, float x2, float y2, float startX, float startY, float endX, float endY);
				void texturedLineOverlay(Image* img, float scale, float x1, float y1, float x2, float y2, float startX, float startY, float endX, float endY);

				void drawRect(ARK::Geometry::RectangleTemplate<int>* rect) const;
				void drawRect(ARK::Geometry::RectangleTemplate<float>* rect) const;
				void drawRect(float x, float y, int width, int height) const;
				void drawRect(float x, float y, float z, int width, int height) const;
				void drawRects(float rects[], int colors[] = NULL) const;
				
				void fillRect(float x, float y, int width, int height) const;
				void fillRect(float x, float y, float z, int width, int height) const;
				void fillRoundedRect(float x, float y, int width, int height, int radius) const;
				void fillRoundedRect(float x, float y, int width, int height, int radius, int segs) const;

				void fillGradientRect(float x, float y, float width, float height, Color* top, Color* bottom) const;
				void fillGradientRect(float x, float y, float z, float width, float height, Color* top, Color* bottom) const;

				void texturedRect(unsigned int texId, float x, float y, int width, int height) const;
				void texturedRect(unsigned int texId, float x, float y, float z, int width, int height) const;
				void texturedRectAlpha(unsigned int texId, float x, float y, int width, int height, float alpha) const;
				void texturedRectAlpha(unsigned int texId, float x, float y, float z, int width, int height, float alpha) const;
				//void texturedRectShader(Shader* useShader, const map<string, float>& vars, unsigned int texId, float x, float y, int width, int height) const;

				void fillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const;
				void fillQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4) const;
				void texturedQuad(unsigned int texId, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4);
				void texturedQuad(unsigned int texId, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float tx4, float ty4);
				void texturedQuads(unsigned int texId, float* verts, float* normals, float* texcoords, unsigned char* colors, unsigned int count);

				void fillTriangles(float* verts, float* normals, unsigned char* colors, unsigned int count, bool fromBatch=false);
				void texturedTriangles(unsigned int texId, float* verts, float* normals, float* texcoords, unsigned char* colors, unsigned int count, bool fromBatch=false);

				void fillTriangle(int x, int y, int width, int height) const;
				void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) const;
				void fillTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const;

				void drawPoint(int x, int y) const;
				void drawPoint(float x, float y) const;

				void drawCircle(ARK::Geometry::Circle<int>* circle);
				void drawCircle(ARK::Geometry::Circle<float>* circle);
				void drawCircle(float x, float y, int radius);
				void drawCircle(float x, float y, int radius, int points);
				void drawCircle(float x, float y, float z, int radius, int points);

				void fillCircle(float x, float y, int radius, int points) const;
				void fillCircle(float x, float y, float z, int radius, int points) const;

				void fillEllipse(float x, float y, float rx, float ry) const;
				void fillEllipse(float x, float y, float z, float rx, float ry) const;

				void drawCircleSpikey(float x, float y, int radius, int points) const;
				void fillCircleSpikey(float x, float y, int radius, int points) const;

				void setLineWidth(float i);
				float getLineWidth();

				void setPointSize(float i);
				float getPointSize();

				unsigned int getMaxTextureSize();

				void drawScissorBoxes();
				void setScissorBoxColors(const Color& top, const Color& left, const Color& bottom, const Color& right);

				static const unsigned int BLEND_NONE = 0;
				static const unsigned int BLEND_NORMAL = 1;
				static const unsigned int BLEND_ADDITIVE = 2;
				static const unsigned int BLEND_SPINE = 3;
				static const unsigned int BLEND_TEST = 4;
				static const unsigned int BLEND_MASK_SET = 5; // use white texture to change alpha mask
				static const unsigned int BLEND_MASK_USE = 6; // and then use it.
				void setBlendMode(unsigned int blendMode);
				unsigned int getBlendMode();

				void loadMatrix(Matrix44<float>* m) const;
				void loadMatrix(const Matrix44<float>& m) const;
				void resetMatrix() const;

				// internals... :|
				void __internalsDrawArray_triangleStrip(int first, int amount);
				void __internalsDrawArray_triangles(int first, int amount);
				static void __internalsDXUpdateMatrices();

				string toString();

			private:
				ARK::Font::Font* m_DefaultFont;
				ARK::Font::Font* m_Font;
				Color m_DrawColor;
				Color m_MaskColor;
				float m_LineWidth;
				float m_pointSize;
				unsigned int m_blendMode;

				Color m_ScissorBoxColors[4]; // top, left, bottom, right;
		};


	}
}

#endif /* GRAPHICS_H_ */
