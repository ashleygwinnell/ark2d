/*
 * UISimpleButton.h
 *
 *  Created on: 26-May-2010
 *      Author: Ashley
 */

#ifndef UISIMPLEBUTTON_H_
#define UISIMPLEBUTTON_H_

#include "UIButton.h"
#include "GameContainer.h"
#include "Graphics.h"

class UISimpleButton : public UIButton
{
	public:
		UISimpleButton(std::string text, signed int x, signed int y, unsigned int width, unsigned int height);
		void setX(signed int x);
		void setY(signed int y);
		void setWidth(unsigned int width);
		void setHeight(unsigned int height);
		void setText(std::string text);
		void render(GameContainer& container, Graphics& g);

	private:
		signed int m_x;
		signed int m_y;
		unsigned int m_width;
		unsigned int m_height;
		std::string m_text;

		enum MouseState {OVER, OFF, DOWN, DOWN_COMPLETE};
		MouseState m_mousestate;

		void updateInputState(GameContainer& container);
};

#endif /* UISIMPLEBUTTON_H_ */
