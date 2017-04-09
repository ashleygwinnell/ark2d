/*
 * Mesh.h
 *
 *  Created on: 8 Apr 2017
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_MESH_H_
#define ARK_CORE_GRAPHICS_MESH_H_

#include "../../Namespaces.h"
#include "../GameObject.h"
#include "../Geometry/Vector2.h"
#include "../SceneGraph/Scene.h"

#include <string>
#include <vector>
using namespace std;

namespace ARK {
    namespace Core {
        namespace Graphics {
        	class ARK2D_API Mesh {
        		public:
        			vector<Vector3<float> > vertices;
					vector<Vector2<float> > uvs;
					vector<Vector3<float> > normals;
					vector<uint32_t> colors;
                    vector<unsigned short> indices;

					static Mesh* createFromObjFile(string filename, bool appendPath=true);
					static Mesh* createFromPNGFile(string filename, bool appendPath=true);
        	};

        	class ARK2D_API MeshRenderer : public GameComponent {
        		public:
        			Mesh* mesh;
                    MeshRenderer(Mesh* m);
        			virtual void render();
        	};
        }
    }
}

#endif
