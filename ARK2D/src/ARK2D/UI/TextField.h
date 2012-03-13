/*
 * TextField.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef TEXTFIELD_H_
#define TEXTFIELD_H_

#include "AbstractUIComponent.h"

#include "../ARKString.h"
#include "../Input.h"
#include "../Graphics.h"
#include "../Graphics/Color.h"
#include "../Geometry/GigaRectangle.h"

/**
 * Unlike HTML, padding does not (currently) affect height.
 */


class TextField : public AbstractUIComponent {
	public:
		ARKString m_text;
		int m_cursorPosition;

		int m_selectedDir; // -1 for left, 0 for none and +1 for right.
		int m_selectedFrom;
		int m_selectedTo;

		//int m_textAlignX; // -1 for left, 1 for right - not implemented
		int m_textAlignY; // -1 for top, 0 for middle, +1 for bottom.

		bool m_multiline;
		bool m_wrap;

		// align vars
		static const signed int ALIGN_START = -1;
		static const signed int ALIGN_TOP = -1;
		static const signed int ALIGN_LEFT = -1;

		static const signed int ALIGN_MIDDLE = 0;

		static const signed int ALIGN_RIGHT = 1;
		static const signed int ALIGN_BOTTOM = 1;
		static const signed int ALIGN_END = 1;

		TextField();
		void setText(string s);
		void setText(int i);
		void setText(float f);
		const ARKString& getText();

		void setMultiline(bool b);
		void setTextAlignY(signed int align);

		void render();
		void renderBackground();
		void renderSelectedArea(int x, int y, int w, int h);
		void renderText(int x, int y);
		void renderCaret(int x1, int y1, int x2, int y2);
		void renderOverlay();

		bool hasSelection();
		void keyPressed(unsigned int key);

		void clearSelection();
		void cursorLeft();
		void cursorRight();
		virtual ~TextField();
};

#endif /* TEXTFIELD_H_ */
