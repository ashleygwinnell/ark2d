/*
 * TextField.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef TEXTFIELD_H_
#define TEXTFIELD_H_

#include "../MyString.h"
#include "AbstractUIComponent.h"

/**
 * Unlike HTML, padding does not (currently) affect height.
 */
class TextField : public AbstractUIComponent {
	public:
		MyString m_text;
		int m_cursorPosition;

		int m_selectedDir; // -1 for left, 0 for none and +1 for right.
		int m_selectedFrom;
		int m_selectedTo;

		Vector4<int> m_padding;
		//int m_textAlignX; // -1 for left, 1 for right - not implemented
		int m_textAlignY; // -1 for top, 0 for middle, +1 for bottom.

		TextField():
			AbstractUIComponent(),
			m_text(""),
			m_cursorPosition(0),
			m_selectedDir(0),
			m_selectedFrom(-1),
			m_selectedTo(-1),
			m_padding(0, 0, 0, 0), // left, top, right, bottom.
			m_textAlignY(0)
			{

		}
		void setText(string s) {
			m_text.clear();
			m_text += s;

			clearSelection();
			m_cursorPosition = m_text.length();
		}
		void setText(int i) {
			m_text.clear();
			m_text += i;

			clearSelection();
			m_cursorPosition = m_text.length();
		}
		const MyString& getText() {
			return m_text;
		}
		void setPadding(int l, int t, int r, int b) {
			m_padding.set(l, t, r, b);
		}
		void setPadding(int all) {
			m_padding.set(all, all, all, all);
		}
		int getPaddingLeft() {
			return m_padding.getX();
		}
		int getPaddingTop() {
			return m_padding.getY();
		}
		int getPaddingRight() {
			return m_padding.getZ();
		}
		int getPaddingBottom() {
			return m_padding.getW();
		}
		int getPaddingX() {
			return m_padding.getX() + m_padding.getZ();
		}
		int getPaddingY() {
			return m_padding.getY() + m_padding.getW();
		}
		void render() {

			Graphics* g = ARK2D::getGraphics();
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
				int sw = g->getFont()->getStringWidth(m_text.substring(0, m_cursorPosition).get());

				renderCaret(
					m_x + getPaddingLeft() + sw,
					m_y + getPaddingTop(),
					m_x + getPaddingLeft() + sw,
					m_y + m_height - getPaddingBottom()
				);
			}

			renderOverlay();
		}
		void renderBackground() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::black_50a);
			g->fillRect(m_x, m_y, m_width, m_height);
		}
		void renderSelectedArea(int x, int y, int w, int h) {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::black_50a);
			g->fillRect(x, y, w, h);
		}
		void renderText(int x, int y) {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->drawString(m_text.get(), x, y);
		}
		void renderCaret(int x1, int y1, int x2, int y2) {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white_50a);
			g->drawLine(x1, y1, x2, y2);
		}
		void renderOverlay() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->drawRect(m_x, m_y, m_width, m_height);
		}

		bool hasSelection() {
			if (m_selectedFrom != m_selectedTo) {
				return true;
			}
			return false;
		}
		void keyPressed(unsigned int key) {
			Input* i = ARK2D::getInput();

			string k = i->getKeyChar(key);
			std::cout << k << std::endl;

			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				if (GigaRectangle<int>::s_contains(m_x, m_y, m_width, m_height, i->getMouseX(), i->getMouseY())) {
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
				} else if (k.length() > 0) {
					m_text.insert(k, m_cursorPosition);
					cursorRight();
					clearSelection();
				}
			}




		}
		void clearSelection() {
			m_selectedFrom = -1;
			m_selectedTo = -1;
			m_selectedDir = 0;
		}
		void cursorLeft() {
			m_cursorPosition -= 1;
			if (m_cursorPosition <= 0) {
				m_cursorPosition = 0;
			}
		}
		void cursorRight() {
			m_cursorPosition += 1;
			if (m_cursorPosition >= m_text.length()) {
				m_cursorPosition = m_text.length();
			}
		}
		virtual ~TextField() {

		}
};

#endif /* TEXTFIELD_H_ */
