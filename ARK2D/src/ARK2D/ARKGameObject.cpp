/*
 * ARKGameObject.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "ARKGameObject.h"

ARKGameObject::ARKGameObject() {

}

bool ARKGameObject::isPendingRemoval() {
	return m_pendingRemoval;
}
void ARKGameObject::setPendingRemoval(bool b) {
	m_pendingRemoval = b;
}

ARKGameObject::~ARKGameObject() {

}
