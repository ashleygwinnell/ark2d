/*
 * TextureStore.h
 *
 *  Created on: 12 Dec 2012
 *      Author: Ashley
 */

#ifndef TEXTURESTORE_H_
#define TEXTURESTORE_H_

#include "../Includes.h"

namespace ARK {
	namespace Graphics {
		class Texture;

		/*!
		 * \brief Used to reload textures automatically on Android.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class TextureStore {
			private: 
				static TextureStore* s_textureStore;

			public:
				static TextureStore* getInstance();

			public:
				TextureStore();

				void addTexture(string ref, Texture* texture);
				void removeTexture(string ref); 
				Texture* getTexture(string ref); 
				bool hasTexture(string ref);

				void reloadTextures();
				void print();

				virtual ~TextureStore();

			private:
				map<string, Texture*> m_map;
		};
	}
}

#endif /* SOUNDSTORE_H_ */
