/*
 * GameObject.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "GameObject.h"

namespace ARK {
	namespace Core {

		GameComponent::GameComponent():
			dependencies(),
			gameObject(NULL)
			{

		}
		void GameComponent::update() {

		}
		void GameComponent::render() {

		}






		GameObject::GameObject():
			SceneNode(),
			components(),
			m_pendingRemoval(false),
			m_renderingLayer(0)
			{

		}

		bool GameObject::isPendingRemoval() {
			return m_pendingRemoval;
		}
		void GameObject::setPendingRemoval(bool b) {
			m_pendingRemoval = b;
		}
		void GameObject::onPrune() {

		}

		int GameObject::getRenderingLayer() {
			return m_renderingLayer;
		}
		void GameObject::setRenderingLayer(int i) {
			m_renderingLayer = i;
		}

		string GameObject::toString() {
			return "";
		}

		void GameObject::update(GameContainer* container, GameTimer* timer) {

		}

		void GameObject::render(GameContainer* container, Renderer* g) {

		}

		void GameObject::update() {
			SceneNode::update();
			for (auto it : components) {
    			GameComponent* component = it.second;
    			component->update();
    		}
		}
		void GameObject::render() {
			SceneNode::render();

			for (auto it : components) {
    			GameComponent* component = it.second;
    			component->render();
    		}
		}

		GameObject::~GameObject() {

		}

	}
}
