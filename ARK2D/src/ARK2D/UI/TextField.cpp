/*
 * TextField.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "TextField.h"

TextField::TextField():
	AbstractUIComponent(),
	m_text(""),
	m_cursorPosition(0),
	m_selectedDir(0),
	m_selectedFrom(-1),
	m_selectedTo(-1),
	m_textAlignY(0),
	m_multiline(false),
	m_wrap(false)
	{

}

void TextField::setText(string s) {
	m_text.clear();
	m_text += s;

	clearSelection();
	m_cursorPosition = m_text.length();
}
void TextField::setText(int i) {
	m_text.clear();
	m_text += i;

	clearSelection();
	m_cursorPosition = m_text.length();
}
void TextField::setText(float f) {
	m_text.clear();
	m_text += f;

	clearSelection();
	m_cursorPosition = m_text.length();
}
const ARKString& TextField::getText() {
	return m_text;
}

void TextField::setMultiline(bool b) {
	m_multiline = b;
}
void TextField::render() {

	//AbstractUIComponent::preRender();

	const Graphics* g = ARK2D::getGraphics();
	renderBackground();

	if (hasSelection())
	{
		int sw  = g->getFont()->getStringWidth(m_text.substring(0, m_selectedFrom).get());
		int sw2 = g->getFont()->getStringWidth(m_text.substring(m_selectedFrom, m_selectedTo - m_selectedFrom).get());

		renderSelectedArea(
			m_x + sw + getPaddingLeft(),
			m_y + getPaddingTop(),
			sw2,
			m_height - getPaddingY()
		);
	}

	int renderTextX = m_x + getPaddingLeft();
	int renderTextY = m_y;
	if (m_textAlignY == -1) {
		renderTextY = m_y + getPaddingTop();
	} else if (m_textAlignY == 1) {
		renderTextY = m_y + m_height - getPaddingBottom() - g->getFont()->getStringHeight(m_text.get());
	} else if (m_textAlignY == 0) {
		int tp = m_y + getPaddingTop();
		int bp = m_y + m_height - getPaddingBottom();
		int diff = bp - tp;
		renderTextY = tp + (diff/2) - (g->getFont()->getLineHeight()/2);
	}
	renderText(renderTextX, renderTextY);

	if (isFocussed()) {
		if (!m_multiline)
		{
			int sw = g->getFont()->getStringWidth(m_text.substring(0, m_cursorPosition).get());
			renderCaret(
				m_x + getPaddingLeft() + sw,
				m_y + getPaddingTop(),
				m_x + getPaddingLeft() + sw,
				m_y + m_height - getPaddingBottom()
			);
		}
		else
		{
			vector<ARKString> lines = m_text.split("\n");
			int prePos = 0;
			int curPos = 0;
			int curLine = 0;
			for(unsigned int i = 0; i < lines.size(); i++) {
				curLine = i;
				prePos = curPos;
				curPos += lines[i].length() + 1;
				if (prePos <= m_cursorPosition && curPos > m_cursorPosition) {
					curPos = (m_cursorPosition - prePos);
					//curPos -= m_cursorPosition;
					break;
				}

			}

			ARKString line = lines.at(curLine);

			int lh = g->getFont()->getLineHeight();
			int sw = g->getFont()->getStringWidth(line.substring(0, curPos).get());
			renderCaret(
				m_x + getPaddingLeft() + sw,
				m_y + getPaddingTop() + (curLine * lh),
				m_x + getPaddingLeft() + sw,
				m_y + getPaddingTop() + ((curLine+1) * lh)
			);
		}
	}

	renderOverlay();

	//AbstractUIComponent::postRender();
}
void TextField::setTextAlignY(signed int align) {
	m_textAlignY = align;
}
void TextField::renderBackground() {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::black_50a);
	g->fillRect(m_x, m_y, m_width, m_height);
}
void TextField::renderSelectedArea(int x, int y, int w, int h) {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::black_50a);
	g->fillRect(x, y, w, h);
}
void TextField::renderText(int x, int y) {
	Graphics* g = ARK2D::getGraphics();

	//g->setDrawColor(Color::white);
	//string dbg = Cast::toString<bool>(m_text.contains("\n"));
	//g->drawString(dbg, x, y - 50);
	//std::cout << "text contains slash-n: " << () << std::endl;st


	if (m_multiline == false || (m_multiline == true && !m_text.contains("\n"))) {
		g->setDrawColor(Color::white);
		g->drawString(m_text.get(), x, y);
	} else {




		g->setDrawColor(Color::white);
		int cur_y = y;
		vector<ARKString> lines = m_text.split("\n");
		for(unsigned int i = 0; i < lines.size(); i++) {
			g->drawString(lines.at(i).get(), x, cur_y);
			if (m_wrap) { /**/ }
			cur_y += (signed int) g->getFont()->getLineHeight();
		}
	}
}
void TextField::renderCaret(int x1, int y1, int x2, int y2) {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white_50a);
	g->drawLine(x1, y1, x2, y2);
}
void TextField::renderOverlay() {
	Graphics* g = ARK2D::getGraphics();
	g->setDrawColor(Color::white);
	g->drawRect(m_x, m_y, m_width, m_height);
}

bool TextField::hasSelection() {
	if (m_selectedFrom != m_selectedTo) {
		return true;
	}
	return false;
}
void TextField::keyPressed(unsigned int key) {
	Input* i = ARK2D::getInput();

	string k = i->getKeyChar(key);
	std::cout << k << std::endl;

	if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
		//if (GigaRectangle<int>::s_contains(m_x, m_y, m_width, m_height, i->getMouseX(), i->getMouseY())) {
		if (GigaRectangle<int>::s_contains(getOnScreenX(), getOnScreenY(), m_width, m_height, i->getMouseX(), i->getMouseY())) {
			if (UIComponent::s_currentFocus != NULL) {
				UIComponent::s_currentFocus->setFocussed(false);
			}

			UIComponent::s_currentFocus = this;
			setFocussed(true);
		} else {
			setFocussed(false);
		}
	}


	if (isFocussed()) {
		if (key == (unsigned int) Input::KEY_BACKSPACE) {
			if (hasSelection()) {
				int l = m_selectedTo - m_selectedFrom;
				m_text.remove(m_selectedFrom, l);

				if (m_selectedDir == 1) {
					for(int ii = 0; ii < l; ii++) {
						cursorLeft();
					}
				}
				clearSelection();

			} else {
				m_text.remove(m_cursorPosition-1, 1);
				cursorLeft();
			}
		} else if (key == (unsigned int) Input::KEY_DELETE) {
			if (hasSelection()) {
				int l = m_selectedTo - m_selectedFrom;
				m_text.remove(m_selectedFrom, l);

				if (m_selectedDir == 1) {
					for(int ii = 0; ii < l; ii++) {
						cursorLeft();
					}
				}
				clearSelection();
			} else {
				m_text.remove(m_cursorPosition, 1);
			}
		} else if (key == (unsigned int) Input::KEY_HOME) {
			m_cursorPosition = 0;
		} else if (key == (unsigned int) Input::KEY_END) {
			m_cursorPosition = m_text.length();
		} else if (key == (unsigned int) Input::KEY_LEFT) {
			if (i->isKeyDown(Input::KEY_SHIFT)) {
				if (m_selectedDir == 1) {
					m_selectedTo--;
					if (m_selectedFrom == m_selectedTo) {
						clearSelection();
					}
				} else {
					if (m_cursorPosition != 0) {
						if (m_selectedTo == -1) { // only set this if there is no selection.
							m_selectedTo = m_cursorPosition;
							m_selectedDir = -1;
						}
						m_selectedFrom = m_cursorPosition - 1;
					}
				}
			} else {
				clearSelection();
			}
			cursorLeft();
		} else if (key == (unsigned int) Input::KEY_RIGHT) {
			if (i->isKeyDown(Input::KEY_SHIFT)) {
				if (m_selectedDir == -1) {
					m_selectedFrom++;
					if (m_selectedFrom == m_selectedTo) {
						clearSelection();
					}
				} else {
					if (m_cursorPosition != m_text.length()) {
						if (m_selectedFrom == -1) { // only set this if there is no selection.
							m_selectedFrom = m_cursorPosition;
							m_selectedDir = 1;
						}
						m_selectedTo = m_cursorPosition + 1;
					}
				}

			} else {
				clearSelection();
			}
			cursorRight();
		} else if ((key == (unsigned int) Input::KEY_ENTER) && m_multiline) {
			m_text.insert("\n", m_cursorPosition);
			cursorRight();
			clearSelection();
		} else if (k.length() > 0) {
			m_text.insert(k, m_cursorPosition);
			cursorRight();
			clearSelection();
		}
	}




}
void TextField::clearSelection() {
	m_selectedFrom = -1;
	m_selectedTo = -1;
	m_selectedDir = 0;
}
void TextField::cursorLeft() {
	m_cursorPosition -= 1;
	if (m_cursorPosition <= 0) {
		m_cursorPosition = 0;
	}
}
void TextField::cursorRight() {
	m_cursorPosition += 1;
	if (m_cursorPosition >= m_text.length()) {
		m_cursorPosition = m_text.length();
	}
}
TextField::~TextField() {

}
