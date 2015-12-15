/*
 * ShaderStore.h
 *
 *  Created on: 1 Dec 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SHADERSTORE_H_
#define ARK_GRAPHICS_SHADERSTORE_H_

#include "../Common/DLL.h"
#include <map>

namespace ARK {
	namespace Graphics {
		class Shader;

		/*!
		 * \brief Used to bind/unbind shaders in the Renderer.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API ShaderStore {
			private: 
				static ShaderStore* s_instance;

			public:
				static ShaderStore* getInstance();

			public:
				ShaderStore();

				void addShader(unsigned int ref, Shader* shader);
				void removeShader(unsigned int ref); 
				Shader* getShader(unsigned int ref);
				bool hasShader(unsigned int ref);

				void print();

				virtual ~ShaderStore();

			private:
				std::map<unsigned int, Shader*> m_map;
		};

	}
}

#endif /* ARK_GRAPHICS_SHADERSTORE_H_ */
