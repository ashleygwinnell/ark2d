/*
 * SceneGraph.h
 *
 *  Created on: 24 May 2015
 *      Author: Ashley
 */

#ifndef ARK_SCENEGRAPH_SCENE_H_
#define ARK_SCENEGRAPH_SCENE_H_

#include "../Common/DLL.h"
#include <string>
#include <vector>
#include "../Geometry/Vector3.h"

namespace ARK {
	namespace SceneGraph {

		class ARK2D_API SceneNode {
			public:
				static const unsigned int TYPE_NODE = 0;
				static const unsigned int TYPE_GROUP = 1;
				static const unsigned int TYPE_IMAGE = 2;
			public:
				SceneNode* parent;
				string name;
				vector<SceneNode*> children;
				Vector3<float> pivot;
				Vector3<float> position;
				Vector3<float> scale;
				double rotation;
				bool visible;
				unsigned int type;

                SceneNode();
                SceneNode(unsigned int type, string name);

				void setName(string n);
				string getName();

				void addChild(SceneNode* node);
				void removeChild(SceneNode* node);
				unsigned int countChildren(bool recursive);
				SceneNode* getChildByName(string n);

				virtual SceneNode* setScale(float x, float y);
				virtual SceneNode* setRotation(double degrees);
				double getRotation();
		
				virtual void update();
				virtual void render();

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);

				virtual void onAdded(SceneNode* newparent);
		};

		class ARK2D_API SceneGroup : public SceneNode {
				
		};
		

		class ARK2D_API Scene {
			public: 
				SceneNode* root;
				Scene();
				void setRoot(SceneNode* node);
				SceneNode* getRoot();
				void addChild(SceneNode* node);
				void removeChild(SceneNode* node);
				SceneNode* getChildByName(string n);
				void update();
				void render();

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);
		};
	}
}

#endif

