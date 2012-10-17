#ifndef ARK_GRAPHICS_FBO_H_
#define ARK_GRAPHICS_FBO_H_

namespace ARK { 
	namespace Graphics {
		
		class FBO {
			private:
				unsigned int fbo;
				unsigned int fbo_depth;
				unsigned int fbo_texture;
				unsigned int window_width;
				unsigned int window_height;
				static bool m_supported;
				static bool m_supportedChecked;

			public:
				FBO();	
				FBO(unsigned int w, unsigned int h);

				void bind();
				void unbind(); 
				
				unsigned int getTextureId();

				virtual ~FBO();

			private:
				void init();

			public:
				static bool isSupported();
		};

	}
}

#endif
