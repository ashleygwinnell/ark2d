
#include "Scene.h"
#include "../Graphics/Renderer.h"
#include "../Geometry/Cube.h"
#include "../Graphics/Image.h"

namespace ARK {
	namespace SceneGraph {

		ARK::Geometry::Cube* SceneNode::s_dummyCube = new ARK::Geometry::Cube(0,0,0);

		SceneNode::SceneNode():
			parent(NULL),
			name("Node"),
			children(),
			pivot(0, 0, 0),
			position(0, 0, 0),
			scale(1, 1, 1),
			rotation(0),
			visible(true),
			type(TYPE_NODE)
		{

		}
		SceneNode::SceneNode(unsigned int nodeType, string nodeName):
			parent(NULL), 
			name(nodeName),
			children(),
			pivot(0, 0, 0),
			position(0, 0, 0),
			scale(1, 1, 1),
			rotation(0),
			visible(true),
			type(nodeType)
		{

		}
		void SceneNode::setName(string n) {
			name = n;
		}
		string SceneNode::getName() {
			return name;
		}
		void SceneNode::addChild(SceneNode* node) { 
			children.push_back(node);
			node->parent = this;
			node->onAdded(this);
			onChildAdded(node);
		}
		void SceneNode::removeChild(SceneNode* node) {
			for(unsigned int i = 0; i < children.size(); ++i) {
				if (children[i] == node) {
					children.erase(children.begin() + i);
       				break;
				}
			}
		}
		unsigned int SceneNode::countChildren(bool recursive) {
			if (!recursive) {
				return children.size();
			}
			unsigned int sz = children.size();
			for(unsigned int i = 0; i < children.size(); ++i) {
				sz += children[i]->countChildren(true);
			}
			return sz;
		}
		SceneNode* SceneNode::getChildByName(string n) {
			SceneNode* ret = NULL;
			for(unsigned int i = 0; i < children.size(); ++i) {
				if (children[i]->name == n) {
					ret = children[i];
					break;
				}
			}
			return ret;
		}

		SceneNode* SceneNode::setScale(float x, float y) {
			scale.set(x, y);
			return this;
		}
		SceneNode* SceneNode::setRotation(double degrees) {
			rotation = degrees;
			return this;
		}
		double SceneNode::getRotation() { 
			return rotation;
		}

		Vector3<float> SceneNode::localPositionToGlobalPositionInternal() {
			Renderer* r = ARK2D::getRenderer();
			float x = position.getX();
			float y = position.getY();
			float z = position.getZ();
			float w = 1.0f;
			if (r->getMatrix()->height() >= 1) {
				Vector4<float>::multMatrix44(x, y, z, w, *r->getMatrix()->at(r->getMatrix()->height()-1));
			}
			return Vector3<float>(x, y, z);
		}

		Vector3<float> SceneNode::localPositionToGlobalPosition() {
			// This method has to climb the scene graph to calculate the position. 
			// Likely slower than getWorldPosition() but can be used regardless of the current transformation matrix.
			Vector3<float> pos(position.getX(), position.getY(), position.getZ());
			if (parent != NULL) {

				pos *= parent->scale;

				ARK::Geometry::Cube* bounds = parent->getBounds();
				pos.m_x -= parent->pivot.m_x * bounds->getWidth() * parent->scale.getX();
				pos.m_y -= parent->pivot.m_y * bounds->getHeight() * parent->scale.getY();
				pos.m_z -= parent->pivot.m_z * bounds->getDepth() * parent->scale.getZ();

				MathUtil::rotatePointAroundPoint<float>(&pos.m_x, &pos.m_y, 0.0f, 0.0f, parent->rotation * 1.0f);

				// TODO: 
				// Positions are not calculated properly when rotation is set.
				// How do I sort this out?! 

				pos += parent->localPositionToGlobalPosition();
			} 
			return pos;
		}
		Vector3<float> SceneNode::localScaleToGlobalScale() {
			Vector3<float> sc(scale.getX(), scale.getY(), scale.getZ());
			if (parent != NULL) {
				sc *= parent->localScaleToGlobalScale();
			}
			return sc;
		}
		float SceneNode::localRotationToGlobalRotation() {
			float rot = rotation;
			if (parent != NULL) {
				rot += parent->localRotationToGlobalRotation();
			}
			return rot;
		}
		bool SceneNode::isGlobalPositionInBounds(float x, float y) {
			ARK::Geometry::Cube* bounds = getBounds();
			Vector3<float> globalpos = localPositionToGlobalPosition();
			Vector3<float> globalsc = localScaleToGlobalScale();
			float globalrot = localRotationToGlobalRotation();

			float minx = globalpos.getX() - pivot.getX() * bounds->getWidth()*globalsc.getX();
			float miny = globalpos.getY() - pivot.getY() * bounds->getHeight()*globalsc.getY();
			float tw = bounds->getWidth() * globalsc.getX();
			float th = bounds->getHeight() * globalsc.getY();

			// Make shape and globalposition relative to origin (0,0).
			x -= globalpos.getX();
			y -= globalpos.getY();

			MathUtil::rotatePointAroundPoint<float>(&x, &y, 0.0f, 0.0f, globalrot * -1.0f);

			return Shape<float>::collision_rectangleRectangle( 
				tw * pivot.getX() * -1.0f, 
				th * pivot.getY() * -1.0f, 
				tw, 
				th, 
				x, y, 1, 1);
		}

		void SceneNode::update() {
			for(unsigned int i = 0; i < children.size(); ++i) {
				children[i]->update();
			}
		}
		
		void SceneNode::preRender() {
			ARK::Geometry::Cube* bounds = getBounds();
			Renderer* r = ARK2D::getRenderer();
			r->pushMatrix();
			r->translate(position.getX(), position.getY(), position.getZ());
            r->rotate(float(rotation));
            r->translate(pivot.getX() * bounds->getWidth() * -1.0f * scale.getX(), pivot.getY() * bounds->getHeight() * -1.0f * scale.getY(), pivot.getZ() * bounds->getDepth() * -1.0f * scale.getZ());
            r->scale(scale.getX(), scale.getY(), scale.getZ());
		}
		void SceneNode::render() {
			renderChildren();
		}
		void SceneNode::renderChildren() {
            for(unsigned int i = 0; i < children.size(); ++i) {
				children[i]->preRender();
				children[i]->render();
				children[i]->postRender();
			}
		}
		void SceneNode::postRender() {
			Renderer* r = ARK2D::getRenderer();
			r->popMatrix();
		}

		void SceneNode::onAdded(SceneNode* newparent) {

		}
		void SceneNode::onChildAdded(SceneNode* newChild) {
			
		}

		bool SceneNode::keyPressed(unsigned int key) { 
			for(unsigned int i = 0; i < children.size(); ++i) {
				if (children[i]->keyPressed(key)) { 
					return true; 
				}
			}
			return false;
		}
		bool SceneNode::keyReleased(unsigned int key) { 
			for(unsigned int i = 0; i < children.size(); ++i) {
				if (children[i]->keyReleased(key)) {
					return true;
				}
			}
			return false;
		}
		bool SceneNode::mouseMoved(int x, int y, int oldx, int oldy) { 
			for(unsigned int i = 0; i < children.size(); ++i) {
				if (children[i]->mouseMoved(x, y, oldx, oldy)) {
					return true;
				}
			}
			return false;
		} 
		Image* SceneNode::asImage() {
			return dynamic_cast<Image*>(this);
		}
		ARK::Geometry::Cube* SceneNode::getBounds() {
			return s_dummyCube;
		}
		


		





		Scene::Scene():
			root(NULL)
		{
			root = new SceneGroup();
			root->setName("Root");
		}
		void Scene::setRoot(SceneNode* node) {
			root = node;
		}
		SceneNode* Scene::getRoot() {
			return root;
		}
		void Scene::addChild(SceneNode* node) {
			root->addChild(node);
		}
		void Scene::removeChild(SceneNode* node) {
			root->removeChild(node);
		}
		SceneNode* Scene::getChildByName(string n) {
			return root->getChildByName(n);
		}
		void Scene::update() {
			if (root != NULL) { 
				root->update();
			}
		}
		void Scene::render() {
			if (root != NULL) { 
				Renderer* r = ARK2D::getRenderer();
				r->getBatch()->setEnabled(true, true);
				root->preRender();
				root->render();
				root->postRender();
				r->getBatch()->setEnabled(false, true);
				r->getBatch()->render();
			}
		}
		bool Scene::keyPressed(unsigned int key) { 
			return root->keyPressed(key);
		}
		bool Scene::keyReleased(unsigned int key) {
            return root->keyReleased(key);
		}
		bool Scene::mouseMoved(int x, int y, int oldx, int oldy) {
			return root->mouseMoved(x, y, oldx, oldy);
		}

        
		


	}
}