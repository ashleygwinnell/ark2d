
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
		SceneNode::SceneNode(string nodeName):
			parent(NULL), 
			name(nodeName),
			children(),
			pivot(0, 0, 0),
			position(0, 0, 0),
			scale(1, 1, 1),
			rotation(0),
			visible(true),
			type(TYPE_NODE)
		{

		}
		SceneNode::SceneNode(string nodeName, unsigned int nodeType):
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
					
					// sure we want to deallocate memory? 
					this->onChildRemoved(node);
					node->onRemoved(this);

					delete node;
					node = NULL;

					children.erase(children.begin() + i);
       				break;
				} else {
					children[i]->removeChild(node);
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
		SceneNode* SceneNode::find(string n) {
			SceneNode* ret = NULL;
			for(unsigned int i = 0; i < children.size(); ++i) {
				if (children[i]->name == n) {
					ret = children[i];
					break;
				} else {
					SceneNode* ch = children[i]->find(n);
					if (ch != NULL) {
						ret = ch;
						break;
					}
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

		

		vector<SceneNode*> SceneNode::getPathToRoot() {
			vector<SceneNode*> path;
			getPathToRoot(&path);
			return path;
		}
		void SceneNode::getPathToRoot(vector<SceneNode* >* path) {
			path->push_back(this);
			if (parent != NULL) {
				parent->getPathToRoot(path);
			}
		}

		Vector3<float> SceneNode::globalPositionToLocalPosition(float gx, float gy, float gz, bool fromTopLeft) {
			vector<SceneNode*> path = getPathToRoot();
			std::reverse(path.begin(), path.end());

			Vector3<float> currentScale(1.0f, 1.0f, 1.0f);


			for(signed int i = 0; i < path.size(); i++) {

				ARK::Geometry::Cube* bounds = path[i]->getBounds();

				gx -= path[i]->position.getX() * currentScale.getX();
				gy -= path[i]->position.getY() * currentScale.getY();
				gz -= path[i]->position.getZ() * currentScale.getZ();

				MathUtil::rotatePointAroundPoint<float>(&gx, &gy, 0.0f, 0.0f, path[i]->rotation * -1.0f);

				currentScale *= path[i]->scale;
				if (path[i] == this) { 
					gx /= currentScale.getX();
					gy /= currentScale.getY();
					gz /= currentScale.getZ();
					break;
				}
			}

			if (fromTopLeft) {
                ARK::Geometry::Cube* bounds = getBounds();
				gx += pivot.getX() * bounds->getWidth();// * -1.0f;// * path[i]->scale.getX();
				gy += pivot.getY() * bounds->getHeight();// * -1.0f;// * path[i]->scale.getY();
				gz += pivot.getZ() * bounds->getDepth();// * -1.0f;// * path[i]->scale.getZ();
			}

			return Vector3<float>(gx, gy, gz);

		}
		Vector3<float> SceneNode::globalPositionToLocalPositionInternal(float gx, float gy, float gz, bool fromTopLeft) {
			/*Renderer* r = ARK2D::getRenderer();
			Matrix44<float>* mat = r->getMatrix()->at(r->getMatrix()->height()-1);
			Vector3<float> thisGlobalPosition = localPositionToGlobalPositionInternal();
			float invDet = 1.0f / (mat->col[0][0] * mat->col[1][1] - mat->col[0][1] * mat->col[1][0]);
			
			float dx = gx - thisGlobalPosition.getX();
			float dy = gy - thisGlobalPosition.getY();

			Vector3<float> local;
			local.setX(dx * mat->col[0][0] * invDet - dy * mat->col[0][1] * invDet);
			local.setY(dy * mat->col[1][1] * invDet - dx * mat->col[1][0] * invDet);
			local.setZ(0.0f);
			return local;*/
			return Vector3<float>(0,0,0);

			/*
			float x = position.getX();
			float y = position.getY();
			float z = position.getZ();
			float w = 1.0f;
			if (r->getMatrix()->height() >= 1) {
				Vector4<float>::multMatrix44(x, y, z, w, *);
			}
			return Vector3<float>(x, y, z);*/
		}

		Vector3<float> SceneNode::localPositionToGlobalPositionInternal() {
			Renderer* r = ARK2D::getRenderer();
			float x = position.getX();
			float y = position.getY();
			float z = position.getZ();
			float w = 1.0f;
			if (r->getMatrix()->height() >= 1) {
				Vector4<float>::multMatrix44(x, y, z, w, *r->getMatrix()->at(r->getMatrix()->height()-2));
			}
			return Vector3<float>(x, y, z);
		}


		Vector3<float> SceneNode::localPositionToGlobalPosition() {
			// This method has to climb the scene graph to calculate the position. 
			// Likely slower than getWorldPosition() but can be used regardless of the current transformation matrix.
			/*Vector3<float> pos(position.getX(), position.getY(), position.getZ());
			if (parent != NULL) {

				ARK::Geometry::Cube* bounds = parent->getBounds();

				pos.m_x += parent->pivot.m_x * bounds->getWidth() * parent->scale.getX();
				pos.m_y += parent->pivot.m_y * bounds->getHeight() * parent->scale.getY();
				pos.m_z += parent->pivot.m_z * bounds->getDepth() * parent->scale.getZ();

				pos *= parent->scale;

				pos.m_x -= parent->pivot.m_x * bounds->getWidth() * parent->scale.getX();
				pos.m_y -= parent->pivot.m_y * bounds->getHeight() * parent->scale.getY();
				pos.m_z -= parent->pivot.m_z * bounds->getDepth() * parent->scale.getZ();

				MathUtil::rotatePointAroundPoint<float>(&pos.m_x, &pos.m_y, 0.0f, 0.0f, parent->rotation * 1.0f);

				// TODO: 
				// Positions are not calculated properly when rotation is set.
				// How do I sort this out?! 

				pos += parent->localPositionToGlobalPosition();
			} 
			return pos;*/

			
			// The vector we will return.
			Vector3<float> pos(0, 0, 0);

			// Get path from root to this node. 
			vector<SceneNode* > path = getPathToRoot();
			std::reverse(path.begin(), path.end());

			float currentRotation = 0.0f;
			Vector3<float> currentScale(1.0f,1.0f,1.0f);
			Vector3<float> lastPos(0,0,0);
			
			// Do the same things we do in preRender.
			for(signed int i = 0; i < path.size(); i++) {
				
				if (path[i]->parent == NULL) { continue; }

				//ARK::Geometry::Cube* bounds = path[i]->getBounds();

				Vector3<float> localisedPosition(0,0,0);
				localisedPosition += path[i]->position;
				localisedPosition *= path[i]->parent->scale; 

				currentRotation += path[i]->parent->rotation;
				currentScale *= path[i]->parent->scale;
				MathUtil::rotatePointAroundPoint<float>(&localisedPosition.m_x, &localisedPosition.m_y, 0.0f, 0.0f, currentRotation);

				pos += localisedPosition;

				
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
			/*ARK::Geometry::Cube* bounds = getBounds();
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
			*/

			ARK::Geometry::Cube* bounds = getBounds();
			Vector3<float> localPosition = globalPositionToLocalPosition(x,y,0,true);
			return Shape<float>::collision_rectangleRectangle( 
				0, 
				0, 
				bounds->getWidth(), 
				bounds->getHeight(), 
				localPosition.getX(), localPosition.getY(), 1, 1);
		}

		void SceneNode::update() {
			for(unsigned int i = 0; i < children.size(); ++i) {
				children[i]->update();
			}
		}

		void SceneNode::preRenderFromPivot() {
			ARK::Geometry::Cube* bounds = getBounds();
			Renderer* r = ARK2D::getRenderer();
			r->pushMatrix();
			r->translate(pivot.getX() * bounds->getWidth() * -1.0f, pivot.getY() * bounds->getHeight() * -1.0f, pivot.getZ() * bounds->getDepth() * -1.0f);
		}
		void SceneNode::postRenderFromPivot() {
			ARK::Geometry::Cube* bounds = getBounds();
			Renderer* r = ARK2D::getRenderer();
			r->popMatrix();
		}
		
		void SceneNode::preRender() {
			if (!visible) { return; }
			Renderer* r = ARK2D::getRenderer();
			r->pushMatrix();
			r->translate(position.getX(), position.getY(), position.getZ());
            r->rotate(float(rotation));
            r->scale(scale.getX(), scale.getY(), scale.getZ());
		}
		void SceneNode::render() {
			if (!visible) { return; }
			renderChildren();
		}
		void SceneNode::renderChildren() {
			if (!visible) { return; }
			Renderer* r = ARK2D::getRenderer();
   			ARK::Geometry::Cube* bounds = getBounds();
   			//r->translate(pivot.getX() * bounds->getWidth() * scale.getX(), pivot.getY() * bounds->getHeight() * scale.getY(), pivot.getZ() * bounds->getDepth() * scale.getZ());
			 for(unsigned int i = 0; i < children.size(); ++i) {
				children[i]->preRender();
				children[i]->render();
				children[i]->postRender();
			}
			//r->translate(pivot.getX() * bounds->getWidth() * scale.getX() * -1.0f, pivot.getY() * bounds->getHeight() * scale.getY() * -1.0f, pivot.getZ() * bounds->getDepth() * scale.getZ() * -1.0f);
		}
		void SceneNode::renderBounds() {
			Renderer* r = ARK2D::getRenderer();
			ARK::Geometry::Cube* bounds = getBounds();
			r->setDrawColor(Color::white);
			r->drawRect(0, 0, bounds->getWidth(), bounds->getHeight());
		}
		void SceneNode::postRender() {
			if (!visible) { return; }
			Renderer* r = ARK2D::getRenderer();
			r->popMatrix();
		}

		void SceneNode::onAdded(SceneNode* newparent) {

		}
		void SceneNode::onChildAdded(SceneNode* newChild) {
			
		}
		void SceneNode::onRemoved(SceneNode* removingFrom) {

		}
		void SceneNode::onChildRemoved(SceneNode* removingChild) {

		}

		bool SceneNode::keyPressed(unsigned int key) { 
			for(signed int i = children.size() - 1; i >= 0; --i) {
				if (children[i]->keyPressed(key)) { 
					return true; 
				}
			}
			return false;
		}
		bool SceneNode::keyReleased(unsigned int key) { 
			for(signed int i = children.size() - 1; i >= 0; --i) {
				if (children[i]->keyReleased(key)) {
					return true;
				}
			}
			return false;
		}
		bool SceneNode::mouseMoved(int x, int y, int oldx, int oldy) { 
			for(signed int i = children.size() - 1; i >= 0 ; --i) {
				if (children[i]->mouseMoved(x, y, oldx, oldy)) {
					return true;
				} 
			}
			return false;
		} 
		Image* SceneNode::asImage() {
			return dynamic_cast<Image*>(this);
		}
		void SceneNode::setBounds(float w, float h, float d) {
			// STUB
		}
		ARK::Geometry::Cube* SceneNode::getBounds() {
			return s_dummyCube;
		}
		SceneNode::~SceneNode() {
			
		}
		void SceneNode::setVisible(bool b) {
			visible = b;
		}
		void SceneNode::setVisibleStatic(SceneNode* obj, bool b) {
			obj->setVisible(b);
		}
		
		string SceneNode::toListString() {
			return toListString(0);
		}
		string SceneNode::toListString(signed int indent) {
			string nl = "\n";
        	string s = "";
        	for(signed int i = 0; i < indent; i++) {
        		s += "----";
        	}
        	s += " ";
        	s += string(name);
        	s += nl;
        	for(unsigned int i = 0; i < children.size(); i++) {
        		s += children[i]->toListString(indent + 1);
        	}
        	return s;
        }

		





		Scene::Scene():
			root(NULL)
		{
			root = new SceneGroup();
			root->setName("root");
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
		SceneNode* Scene::find(string n) {
			return root->find(n);
		}
		void Scene::update() {
			if (root != NULL) { 
				root->update();
			}
		}
		void Scene::render() {
			if (root != NULL) { 
				Renderer* r = ARK2D::getRenderer();
				r->setDrawColor(Color::white);
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