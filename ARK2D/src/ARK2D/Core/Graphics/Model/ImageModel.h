/*
 * ImageModel.h
 *
 *  Created on: 7 Oct 2014
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_MODEL_IMAGEMODEL_H_
#define ARK_GRAPHICS_MODEL_IMAGEMODEL_H_

#include "../../SceneGraph/Scene.h"
#include "../../Geometry/Cube.h"

namespace ARK {
	namespace Core {
		namespace Graphics {
				namespace Model {

				struct ImageModelTriangle {
					int vert1;
					int vert2;
					int vert3;
					int color1;
					int color2;
					int color3;
					int normal;
				};

				class ImageModel : public SceneNode
				{
					public:
						string filename;
						vector<Vector3<float> > vertices;
						vector<Vector3<float> > normals;
						vector<Vector4Template<unsigned char> > colors;
						vector<ImageModelTriangle*> indices;

						ARK::Core::Geometry::Cube bounds;

					public:
						ImageModel(string fn);
						void load();
						virtual ARK::Core::Geometry::Cube* getBounds() { return &bounds; }
						virtual void render();
						virtual ~ImageModel();

					private:
						unsigned int addVertex(float x, float y, float z);
						unsigned int addNormal(float x, float y, float z);

					private:
						static bool isFilledAt(PNGImage* png, unsigned int x, unsigned int y);

				};
			}
		}
	}
}

#endif /* ARK_GRAPHICS_MODEL_IMAGEMODEL_H_ */
