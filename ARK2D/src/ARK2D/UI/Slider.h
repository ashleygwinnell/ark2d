/*
 * Slider.h
 *
 *  Created on: 16 Apr 2014
 *      Author: Ashley
 */

#ifndef ARK_UI_SLIDER_H_
#define ARK_UI_SLIDER_H_

#include "../Includes.h"
#include "../Namespaces.h"
#include "AbstractUIComponent.h"

#include "../Core/String.h"
#include "../Controls/Input.h"
#include "../Graphics/Renderer.h"
#include "../SceneGraph/Scene.h"


namespace ARK {
	namespace UI {
		
		class ARK2D_API Slider : public AbstractUIComponent, public SceneNode { 
			public:

				Vector2<float> m_buttonLocation;

				Image* bar;
				Image* button;

				float m_value;
				float m_actualValue;
				float lowValue;
				float highValue;

				bool m_dragging;
				float m_alpha;

				bool m_snapping;
				float m_snapTo;

				Slider();
				void updateValue();
				virtual void setRange(float low, float high);
				virtual void setLocation(float x, float y);
				virtual void setLocationByCenter(float x, float y);
				virtual void render();
				inline void setAlpha(float a) { m_alpha = a; }
				inline float getAlpha() { return m_alpha; }

				inline float getValue() { return m_value; }
				inline float getActualValue() { return m_actualValue; }

				inline void setSnapping(bool b, float val = 0.0f) { m_snapping = b; m_snapTo = val; }
				inline bool isSnapping() { return m_snapping; }

				void setBarImage(Image* img);
				void setButtonImage(Image* img);
				void setBounds(float x, float y, float w, float h);
				void setButtonPosition(float percentage); // from 0 - 1. TODO: take in to account low/high value

				virtual void keyPressed(unsigned int key);
				virtual void keyReleased(unsigned int key);
				virtual void mouseMoved(int x, int y, int oldx, int oldy);

				virtual ~Slider();

			private:
				void refreshButtonPosition();
		};
	}
}

#endif /* ARK_UI_SLIDER_H_ */



