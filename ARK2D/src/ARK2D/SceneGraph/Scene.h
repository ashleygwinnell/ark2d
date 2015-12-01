/*
 * SceneGraph.h
 *
 *  Created on: 24 May 2015
 *      Author: Ashley
 */

#ifndef ARK_SCENEGRAPH_SCENE_H_
#define ARK_SCENEGRAPH_SCENE_H_

#include "../Common/DLL.h"
#include "../Namespaces.h"
#include <string>
#include <vector>
#include "../Geometry/Vector3.h"
#include "../Geometry/Transform.h"
#include "../Controls/KeyListener.h"

namespace ARK { 
	namespace Graphics {
		class Image;
	}
}
using std::string;
using std::vector;

namespace ARK {
	namespace SceneGraph {

		class ARK2D_API SceneNode : public KeyListener {
			public:
				static ARK::Geometry::Cube* s_dummyCube;

			public:
				static const unsigned int TYPE_NODE = 0;
				static const unsigned int TYPE_GROUP = 1;
				static const unsigned int TYPE_IMAGE = 2;
				static const unsigned int TYPE_CAMERA = 3;
			public:
				SceneNode* parent;
				string name;
				vector<SceneNode*> children;
				Vector3<float> pivot;
				//Vector3<float> position;
				//Vector3<float> scale;
				//double rotation;
				Transform transform;
				bool visible;
				unsigned int type;
				
                SceneNode();
                SceneNode(string name);
                SceneNode(string name, unsigned int type);

				void setName(string n);
				string getName();

				void addChild(SceneNode* node);
				void removeChild(SceneNode* node);
				unsigned int countChildren(bool recursive);
				SceneNode* getChildByName(string n);

				SceneNode* find(string n);

				virtual SceneNode* setScale(float x, float y);
				virtual SceneNode* setRotation(double degrees);
				double getRotation();

			protected:
				Vector3<float> localPositionToGlobalPositionInternal();
			public:
				Vector3<float> localPositionToGlobalPosition();
				Vector3<float> localScaleToGlobalScale();
				float localRotationToGlobalRotation();
				virtual bool isGlobalPositionInBounds(float x, float y);

				Vector3<float> globalPositionToLocalPosition(float gx, float gy, float gz, bool fromTopLeft = false);
				Vector3<float> globalPositionToLocalPositionInternal(float gx, float gy, float gz, bool fromTopLeft = false);

				vector<SceneNode*> getPathToRoot();
				void getPathToRoot(vector<SceneNode* >* path);
		
				virtual void update();

				virtual void render();
				virtual void preRender();
				virtual void postRender();
				virtual void renderChildren();
				virtual void renderBounds();

				void setVisible(bool b);
				static void setVisibleStatic(SceneNode* obj, bool b);

				virtual void setBounds(float w, float h, float d);
				virtual ARK::Geometry::Cube* getBounds();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void onAdded(SceneNode* newParent); // called on the child when it is added.
				virtual void onChildAdded(SceneNode* newChild); // called on the parent when it is added.
				virtual void onRemoved(SceneNode* removingFrom); // called on the child when it is removed.
				virtual void onChildRemoved(SceneNode* removingChild); // called on the parent when it is removed.

				Image* asImage();
				virtual ~SceneNode();

				string toListString();
				string toListString(signed int indent);

				void preRenderFromPivot();
				void postRenderFromPivot();
		};



		class ARK2D_API SceneGroup : public SceneNode {
				
		};
		

		class ARK2D_API Scene : public KeyListener {
			public:
				SceneNode* root;
				bool batching;
			public: 
				Scene();
				void setRoot(SceneNode* node);
				SceneNode* getRoot();
				void addChild(SceneNode* node);
				void removeChild(SceneNode* node);
				SceneNode* getChildByName(string n);
				SceneNode* find(string n);
				void update();
				void render();

				bool isBatching();
				void setBatching(bool b);

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);
		};
	}
}

#endif

