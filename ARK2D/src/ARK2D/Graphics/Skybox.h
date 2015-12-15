/*
 * Skybox.h
 *
 *  Created on: 1 Dec 2015
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_SKYBOX_H_
#define ARK_GRAPHICS_SKYBOX_H_

#include "../Common/DLL.h"
#include "../SceneGraph/Scene.h"

namespace ARK {
	namespace Graphics {
		class Image;

		class ARK2D_API Skybox : public SceneNode
		{ 
			public:
				Image* front;
				Image* back;
				Image* top;
				Image* bottom;
				Image* left;
				Image* right;

			protected:
				float sz;
				
			public:
				Skybox(string boxImageFilename);
				Skybox(Image* boxImage);
				Skybox(Image* front, Image* back, Image* left, Image* right, Image* top, Image* bottom);
				void set(Image* boxImage);
				void set(Image* front, Image* back, Image* left, Image* right, Image* top, Image* bottom);
				void setSize(float f);
				virtual void render();
				virtual ~Skybox();
		};

	}
}

#endif /* ARK_GRAPHICS_SKYBOX_H_ */
