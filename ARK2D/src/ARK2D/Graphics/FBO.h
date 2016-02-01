#ifndef ARK_GRAPHICS_FBO_H_
#define ARK_GRAPHICS_FBO_H_

#include "../Includes.h"
#include "../Graphics/Color.h"


namespace ARK { 
	namespace Graphics {
		class FBOStore;
		
		class ARK2D_API FBO {
			friend class FBOStore;
			private:
				unsigned int _internalId;
				static unsigned int _internalIdCount;
			public:
				string name;
			private:
				unsigned int fbo;
				unsigned int fbo_depth;
				unsigned int fbo_texture;
				unsigned int window_width;
				unsigned int window_height;

				unsigned int original_width;
				unsigned int original_height;

				static bool m_supported;
				static bool m_supportedNPO2;
				static bool m_supportedVAOs;
				static bool m_supportedChecked;

				bool m_dummy; 
				bool m_clearScreenOnBind;

			public:
				Color clearColor;

			public:
				FBO();	
				FBO(bool dummy);
				FBO(unsigned int w, unsigned int h);

				void addFBOToStore();

				unsigned int getId();

				void bind(); 
				void bind(unsigned int vw, unsigned int vh); 
				void unbind(); 

				void bind_2d();
				void bind_2d(unsigned int w, unsigned int h);
				void unbind_2d();

				inline void setClearScreenOnBind(bool b) { m_clearScreenOnBind = b; }

				unsigned int getGLID() { return fbo; }
				
				void resize(int width, int height);
				
				unsigned int getTextureId();
				unsigned int getWidth() { return window_width; }
				unsigned int getHeight() { return window_height; }
				unsigned int getOriginalWidth() { return original_width; }
				unsigned int getOriginalHeight() { return original_height; }
				float getTextureMaxXAt() { return float(original_width) / float(window_width); }
				float getTextureMaxYAt() { return float(original_height) / float(window_height); }

				virtual ~FBO();

			private:
				void init();
				void deinit();

			public:
				static bool isSupported();
				static bool isNPO2Supported() { return m_supportedNPO2; }
				static bool isVAOSupported() { return m_supportedVAOs; }
		};

	}
}

#endif
