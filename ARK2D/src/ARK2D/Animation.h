/*
 * Animation.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>

#include "Image.h"
#include "GameTimer.h"

class Animation {
	public:
		Animation();

		void addImage(Image* image);
		Image* getCurrentFrame();
		Image* getFrame(unsigned int i);

		void update(GameTimer* timer);

		void setFrameTime(unsigned int ft);
		unsigned int getFrameTime();

		void setPingPong(bool b);
		bool isPingPong();

		void draw(int x, int y);
		void drawCentered(int x, int y);

		virtual ~Animation();

	private:
		vector<Image*> m_frames;
		unsigned int m_currentFrameIndex;
		signed int m_nextFrameModifier;

		unsigned int m_timer;
		unsigned int m_frameTime;

		bool m_pingPong;
};

#endif /* ANIMATION_H_ */
