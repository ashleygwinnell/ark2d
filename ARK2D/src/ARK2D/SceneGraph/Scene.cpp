
#include "Scene.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace SceneGraph {

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
		void SceneNode::render() {
			Renderer* r = ARK2D::getRenderer();
			r->pushMatrix();
			for(unsigned int i = 0; i < children.size(); ++i) {
				children[i]->render();
			}
			r->popMatrix();
		}

		void SceneNode::onAdded(SceneNode* newparent) {

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
				r->getBatch()->setEnabled(true);
				root->render();
				r->getBatch()->render();
				r->getBatch()->setEnabled(false);
			}
		}

	}
}