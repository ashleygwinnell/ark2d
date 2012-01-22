/*
 * ARKGameObject.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "ARKGameObject.h"

ARKGameObject::ARKGameObject():
	m_pendingRemoval(false), m_renderingLayer(0)
	{

}

bool ARKGameObject::isPendingRemoval() {
	return m_pendingRemoval;
}
void ARKGameObject::setPendingRemoval(bool b) {
	m_pendingRemoval = b;
}
void ARKGameObject::onPrune() {

}

int ARKGameObject::getRenderingLayer() {
	return m_renderingLayer;
}
void ARKGameObject::setRenderingLayer(int i) {
	m_renderingLayer = i;
}

ARKGameObject::~ARKGameObject() {

}
