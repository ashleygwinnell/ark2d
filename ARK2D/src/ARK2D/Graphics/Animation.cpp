/*
 * Animation.cpp
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#include "Animation.h"

namespace ARK {
	namespace Graphics {

		Animation::Animation(): 
			m_frames(),
			m_currentFrameIndex(0),
			m_nextFrameModifier(1),
			m_timer(0.0f),
			m_frameTime(0.0f),
			m_pingPong(false)
			{

		}

		void Animation::clear() {
			m_frames.clear();
			m_timer = 0;
			m_currentFrameIndex = 0;
			m_nextFrameModifier = 1;
		}

		unsigned int Animation::size() {
			return m_frames.size(); 
		}

		void Animation::setTime(float f) {
			m_timer = f;
		}

		void Animation::setAlpha(float f) {
			for(unsigned int i = 0 ; i < m_frames.size(); i++) {
				m_frames.get(i)->setAlpha(f);
			} 
		}

		void Animation::addImage(Image* image) {
			m_frames.add(image);
		} 
		Image* Animation::getCurrentFrame() {
			return m_frames.get(m_currentFrameIndex);
		}
		Image* Animation::getFrame(unsigned int i) {
			return m_frames.get(i);
		}

		Image* Animation::getNextFrame(unsigned int i) {
			i += m_currentFrameIndex;
			if (!m_pingPong) {
				while (i >= m_frames.size()) {
					i -= m_frames.size();
				}
			}
			return m_frames.get(i);
		}

		void Animation::setFrameTime(unsigned int ft) {
			m_frameTime = (ft / 1000.0f);
		}
		void Animation::setFrameTime(float ft) {
			m_frameTime = ft;
		}
		void Animation::setPingPong(bool b) {
			m_pingPong = b;
		}

		bool Animation::isPingPong() {
			return m_pingPong;
		}
		float Animation::getFrameTime() {
			return m_frameTime;
		}

		void Animation::update(GameTimer* timer) {
			if (m_frames.size() == 0) { return; }
			m_timer += timer->getDelta();
			if (m_timer > m_frameTime) {
				m_timer -= m_frameTime;

				// next frame (+1 or -1)
				m_currentFrameIndex += m_nextFrameModifier;

				if (m_pingPong) {
					if (m_currentFrameIndex >= (signed int) m_frames.size()) {
						m_currentFrameIndex = m_frames.size() - 1;
						m_nextFrameModifier *= -1;
					} else if (m_currentFrameIndex < 0) {
						m_currentFrameIndex = 1;
						m_nextFrameModifier *= -1;
					}

				} else {
					if (m_currentFrameIndex >= (signed int) m_frames.size()) {
						m_currentFrameIndex = 0;
					}
				}
			}
		}
		void Animation::draw(int x, int y) {
			m_frames.get(m_currentFrameIndex)->draw(x, y);
		}
		void Animation::drawCentered(int x, int y) {
			m_frames.get(m_currentFrameIndex)->drawCentered(x, y);
		}
		void Animation::drawFlipped(int x, int y, bool fx, bool fy) {
			m_frames.get(m_currentFrameIndex)->drawFlipped(x,y,fx,fy);
		}
		void Animation::drawCenteredFlipped(int x, int y, bool fx, bool fy) {
			Image* img = m_frames.get(m_currentFrameIndex);
			img->drawFlipped(x - (img->getWidth()/2), y - (img->getHeight()/2), fx, fy);
		}

		Animation::~Animation() {

		}
	}
}
