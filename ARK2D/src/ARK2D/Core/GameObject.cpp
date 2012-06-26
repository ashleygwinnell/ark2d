/*
 * GameObject.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "GameObject.h"

namespace ARK {
	namespace Core {

		GameObject::GameObject():
			m_pendingRemoval(false), m_renderingLayer(0)
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

		GameObject::~GameObject() {

		}
	}
}
