/*
 * Animation.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Image/Image.h"
#include "GameTimer.h"
#include "ARKVector.h"

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
		void drawFlipped(int x, int y, bool fx, bool fy);
		void drawCenteredFlipped(int x, int y, bool fx, bool fy);

		virtual ~Animation();

	private:
		ARKVector<Image*> m_frames;
		unsigned int m_currentFrameIndex;
		signed int m_nextFrameModifier;

		unsigned int m_timer;
		unsigned int m_frameTime;

		bool m_pingPong;
};

#endif /* ANIMATION_H_ */
