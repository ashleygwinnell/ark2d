
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
			node->parent = this;
			node->onAdded(this);
			children.push_back(node);
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
            r->translate(pivot.getX() * bounds->getWidth() * -1.0f, pivot.getY() * bounds->getHeight() * -1.0f, pivot.getZ() * bounds->getDepth() * -1.0f);
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