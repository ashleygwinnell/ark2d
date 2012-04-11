/*
 * Animation.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../Namespaces.h"
#include "Image.h"
#include "../Core/GameTimer.h"
#include "../Util/Containers/Vector.h"

namespace ARK {
	namespace Graphics {

		/*!
		 * \brief A bunch of Image frames thrown together to produce an animation.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
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
				Vector<Image*> m_frames;
				unsigned int m_currentFrameIndex;
				signed int m_nextFrameModifier;

				unsigned int m_timer;
				unsigned int m_frameTime;

				bool m_pingPong;
		};
	}
}

#endif /* ANIMATION_H_ */
