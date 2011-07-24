/*
 * GameObject.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "GameObject.h"

GameObject::GameObject(): m_isOnGround(false) {
	m_bounds = new GigaRectangle<float>();

	m_velocity = new Vector2<float>(0, 0);
	m_velocityMaximum = new Vector2<float>(0, 0);

	m_acceleration = new Vector2<float>(0, 0);
}

GigaRectangle<float>* GameObject::getBounds() {
	return m_bounds;
}

Vector2<float>* GameObject::getVelocity() {
	return m_velocity;
}

Vector2<float>* GameObject::getAcceleration() {
	return m_acceleration;
}

GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}
