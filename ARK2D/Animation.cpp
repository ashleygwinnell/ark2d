/*
 * Animation.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Animation.h"

Animation::Animation():
	m_frames(),
	m_currentFrameIndex(0),
	m_nextFrameModifier(1),
	m_timer(0),
	m_frameTime(0),
	m_pingPong(false)
	{

}

void Animation::addImage(Image* image) {
	m_frames.push_back(image);
}
Image* Animation::getCurrentFrame() {
	return m_frames[m_currentFrameIndex];
}

void Animation::setFrameTime(unsigned int ft) {
	m_frameTime = ft;
}
void Animation::setPingPong(bool b) {
	m_pingPong = b;
}

bool Animation::isPingPong() {
	return m_pingPong;
}
unsigned int Animation::getFrameTime() {
	return m_frameTime;
}

void Animation::update(GameTimer* timer) {
	m_timer += (unsigned int) (timer->getDelta() * 1000);
	if (m_timer > m_frameTime) {
		m_timer -= m_frameTime;

		// next frame (+1 or -1)
		m_currentFrameIndex += m_nextFrameModifier;

		if (m_pingPong) {
			if (m_currentFrameIndex >= m_frames.size()) {
				m_currentFrameIndex = m_frames.size() - 1;
				m_nextFrameModifier *= -1;
			} else if (m_currentFrameIndex < 0) {
				m_currentFrameIndex = 1;
				m_nextFrameModifier *= -1;
			}

		} else {
			if (m_currentFrameIndex >= m_frames.size()) {
				m_currentFrameIndex = 0;
			}
		}
	}
}
void Animation::draw(int x, int y) {
	m_frames[m_currentFrameIndex]->draw(x, y);
}
void Animation::drawCentered(int x, int y) {
	m_frames[m_currentFrameIndex]->drawCentered(x, y);
}

Animation::~Animation() {

}
