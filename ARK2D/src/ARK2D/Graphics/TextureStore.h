/*
 * TextureStore.h
 *
 *  Created on: 12 Dec 2012
 *      Author: Ashley
 */

#ifndef TEXTURESTORE_H_
#define TEXTURESTORE_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include <map>
using std::map;

namespace ARK {
	namespace Graphics {
		class Texture;

		/*!
		 * \brief Used to reload textures automatically on Android.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API TextureStore {
			private:
				static TextureStore* s_textureStore;

			public:
				static TextureStore* getInstance();

			public:
				TextureStore();

				void addTexture(string ref, Texture* texture);
				void removeTexture(string ref);
				Texture* getTexture(string ref);
				Texture* getTexture(unsigned int id);
				bool hasTexture(string ref);
				bool hasTexture(unsigned int ref);

				void reloadTextures();
				void print();

				virtual ~TextureStore();

			private:
				map<string, Texture*> m_map;
				map<unsigned int, Texture*> m_mapById;
		};



		/*!
		 * \brief Used to reload FBOs automatically on Android.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class FBO;
		class FBOStore {
			private:
				static FBOStore* s_fboStore;

			public:
				static FBOStore* getInstance();

			public:
				FBOStore();

				void addFBO(unsigned int ref, FBO* texture);
				void removeFBO(unsigned int ref);
				FBO* getFBO(unsigned int ref);
				bool hasFBO(unsigned int ref);

				void reloadFBOs();
				void print();

				virtual ~FBOStore();

			private:
				map<unsigned int, FBO*> m_map;
		};
	}
}

#endif /* TEXTURESTORE_H_ */
