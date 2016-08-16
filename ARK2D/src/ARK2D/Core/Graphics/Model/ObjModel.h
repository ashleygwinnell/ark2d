/*
 * Cube3d.h
 *
 *  Created on: 5 Oct 2014
 *      Author: Ashley
 */

#ifndef ARK_GRAPHICS_MODEL_OBJMODEL_H_
#define ARK_GRAPHICS_MODEL_OBJMODEL_H_

#include "../../SceneGraph/Scene.h"
 
namespace ARK { 
	namespace Graphics { 
		namespace Model {

			//
			// Base code from
			// http://www.opengl-tutorial.org/download/
			//
			// This class does not load all triangles/faces/textures/etc of obj files.
			// This class is also not adapted to work on all platforms. e.g. Android.
			//
			class ObjModel : public SceneNode
			{ 
				public:
					string filename;
					vector<Vector3<float> > vertices;
					vector<Vector2<float> > uvs;
					vector<Vector3<float> > normals;
					vector<Vector4Template<unsigned char> > colors;
					
				public:
					ObjModel(string fn);  
					void loadObj();
					void render();
					virtual ~ObjModel();
			};
		}
	}
}

#endif /* ARK_GRAPHICS_MODEL_OBJMODEL_H_ */
