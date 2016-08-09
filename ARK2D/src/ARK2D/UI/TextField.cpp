/*
 * TextField.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "TextField.h"
#include "../Util/Log.h"
#include <algorithm>

namespace ARK {
	namespace UI {
		TextField::TextField():
			AbstractUIComponent(),
			m_text(""),
			m_cursorPosition(0),
			m_selectedDir(0),
			m_selectedFrom(-1),
			m_selectedTo(-1),
			m_textAlignY(0),
			m_multiline(false),
			m_wrap(false),
			m_password(false),
			m_maxLength(0),
			m_textColor(Color::white),
			m_caretColor(Color::white_50a),
			m_restrictCharacters(RESTRICT_NONE)
			{

		}

		void TextField::setPasswordField(bool b) {
			m_password = b;
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
		const String& TextField::getText() {
			return m_text;
		}

		void TextField::setMultiline(bool b) {
			m_multiline = b;
		}
		void TextField::setRestrictedCharacterSet(unsigned int r) {
			m_restrictCharacters = r;
		}

		void TextField::render() {

			//AbstractUIComponent::preRender();

			const Renderer* g = ARK2D::getRenderer();
			renderBackground();

			if (hasSelection())
			{
				int sw  = g->getFont()->getStringWidth(m_text.substring(0, m_selectedFrom).get());
				int sw2 = g->getFont()->getStringWidth(m_text.substring(m_selectedFrom, m_selectedTo - m_selectedFrom).get());

				renderSelectedArea(
					0 + sw + getPaddingLeft(),
					0 + getPaddingTop(),
					sw2,
					m_height - getPaddingY()
				);
			}

			int renderTextX = 0 + getPaddingLeft();
			int renderTextY = 0;
			if (m_textAlignY == -1) {
				renderTextY = 0 + getPaddingTop();
			} else if (m_textAlignY == 1) {
				renderTextY = 0 + m_height - getPaddingBottom() - g->getFont()->getStringHeight(m_text.get());
			} else if (m_textAlignY == 0) {
				int tp = 0 + getPaddingTop();
				int bp = 0 + m_height - getPaddingBottom();
				int diff = bp - tp;
				renderTextY = tp + (diff/2) - (g->getFont()->getLineHeight()/2);
			}
			renderText(renderTextX, renderTextY);

			if (isFocussed()) {
				if (!m_multiline)
				{
					int sw = g->getFont()->getStringWidth(m_text.substring(0, m_cursorPosition).get());
					renderCaret(
						0 + getPaddingLeft() + sw,
						0 + getPaddingTop(),
						0 + getPaddingLeft() + sw,
						0 + m_height - getPaddingBottom()
					);
				}
				else
				{
					vector<String> lines = m_text.split("\n");
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

					String line = lines.at(curLine);

					int lh = g->getFont()->getLineHeight();
					int sw = g->getFont()->getStringWidth(line.substring(0, curPos).get());
					renderCaret(
						0 + getPaddingLeft() + sw,
						0 + getPaddingTop() + (curLine * lh),
						0 + getPaddingLeft() + sw,
						0 + getPaddingTop() + ((curLine+1) * lh)
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
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(0, 0, m_width, m_height);
		}
		void TextField::renderSelectedArea(int x, int y, int w, int h) {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::black_50a);
			g->fillRect(x, y, w, h);
		}
		void TextField::renderText(int x, int y) {
			Renderer* g = ARK2D::getRenderer();

			//g->setDrawColor(Color::white);
			//string dbg = Cast::toString<bool>(m_text.contains("\n"));
			//g->drawString(dbg, x, y - 50);
			//std::cout << "text contains slash-n: " << () << std::endl;st


			if (m_multiline == false || (m_multiline == true && !m_text.contains("\n"))) {
				
				string t = m_text.get();
				if (m_password) {
					string newt = "";
					for(unsigned int i = 0; i < t.length(); i++) {
						newt += "*";
					}
					t = newt;
				}
				g->setDrawColor(m_textColor);
				g->drawString(t, x, y);
			} else {




				g->setDrawColor(m_textColor);
				int cur_y = y;
				vector<String> lines = m_text.split("\n");
				for(unsigned int i = 0; i < lines.size(); i++) {
					g->drawString(lines.at(i).get(), x, cur_y);
					if (m_wrap) { /**/ }
					cur_y += (signed int) g->getFont()->getLineHeight();
				}
			}
		} 
		
		void TextField::renderCaret() {
			int sw = ARK2D::getRenderer()->getFont()->getStringWidth(m_text.substring(0, m_cursorPosition).get());
			renderCaret(  
				0 + getPaddingLeft() + sw,
				0 + getPaddingTop(),
				0 + getPaddingLeft() + sw,
				0 + m_height - getPaddingBottom()
			);
		}
		void TextField::renderCaret(int x1, int y1, int x2, int y2) {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(m_caretColor);
			g->drawLine(x1, y1, x2, y2);
		}
		void TextField::renderOverlay() {
			Renderer* g = ARK2D::getRenderer();
			g->setDrawColor(Color::white);
			g->drawRect(0, 0, m_width, m_height);
		}

		bool TextField::hasSelection() {
			if (m_selectedFrom != m_selectedTo) {
				return true;
			}
			return false;
		}
		bool TextField::keyReleased(unsigned int key) {
            return false;
		}
		bool TextField::keyPressed(unsigned int key) {
			Input* i = ARK2D::getInput();

			string k = i->getKeyChar(key);
			ARK2D::getLog()->i(k); //std::cout << k << std::endl;

			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				if (isGlobalPositionInBounds(i->getMouseX(), i->getMouseY())) {
					//if (UIComponent::s_currentFocus != NULL) {
					//	UIComponent::s_currentFocus->setFocussed(false);
					//}
					//
					//UIComponent::s_currentFocus = this;
					ARK2D::getLog()->e("focussed");
					setFocussed(true);
                    return true;
				} else {
					setFocussed(false);
				}
				return false;
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
                    return true;
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
                    return true;
				} else if (key == (unsigned int) Input::KEY_HOME) {
					m_cursorPosition = 0;
                    return true;
				} else if (key == (unsigned int) Input::KEY_END) {
					m_cursorPosition = m_text.length();
                    return true;
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
                    return true;
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
                    return true;
				} else if ((key == (unsigned int) Input::KEY_UP) && m_multiline) {
					// Find what line the cursor is on.
					// Find how far the cursor is through this line. (how_far_in)
					signed int how_far_in = -1;
					signed int prev_line_at = -1;
					signed int prev_line_length = 0;
					string s = m_text.getc();
					for(signed int i = m_cursorPosition - 1; i >= 0; i--) {
						if (s[i] == '\n') {
							if (how_far_in == -1) { 
								how_far_in = m_cursorPosition - i - 1; 
								prev_line_at = i;
							} else {
								prev_line_length = prev_line_at - i - 1;
								break;
							}
						} else if (i == 0) {
							if (prev_line_at != -1) {
								prev_line_length = prev_line_at;
							} else {
								how_far_in = m_cursorPosition;
								prev_line_at = 0;
							}
						}
					}
					ARK2D::getLog()->v(StringUtil::append("m_cursorPosition: ", m_cursorPosition));
					ARK2D::getLog()->v(StringUtil::append("how_far_in: ", how_far_in));
					ARK2D::getLog()->v(StringUtil::append("prev_line_at: ", prev_line_at));
					ARK2D::getLog()->v(StringUtil::append("prev_line_length: ", prev_line_length));
					
					// no previous line. must be at the top line. 
					if (how_far_in == -1 || prev_line_at == -1) {
						
					} else if (how_far_in != -1 && prev_line_at == 0) { // first line
						m_cursorPosition = 0;
					} else { 

						int newpos = m_cursorPosition;
						newpos -= how_far_in; 									// back to start of current line
						newpos -= 1; 											// to end of previous line (\n)
						if (how_far_in < prev_line_length) { 
							newpos -= prev_line_length - how_far_in;	// move cursor in, but only if we have to. (i.e. previous line is longer.)
						}
						m_cursorPosition = newpos;								// huzzah, end result 
					}
					ARK2D::getLog()->v(StringUtil::append("new m_cursorPosition: ", m_cursorPosition));
                    return true;

				} else if ((key == (unsigned int) Input::KEY_DOWN) && m_multiline) {
					// Find what line the cursor is on.
					// Find how far the cursor is through this line. (how_far_in)
					string s = m_text.getc();
					signed int how_far_in = -1;
					signed int curr_line_at = -1;
					for(signed int i = m_cursorPosition - 1; i >= 0; i--) {
						if (s[i] == '\n' && how_far_in == -1) {
							how_far_in = m_cursorPosition - i - 1; 
							curr_line_at = i+1;
							break;
						} else if (i == 0 && how_far_in == -1) {
							how_far_in = m_cursorPosition;
							curr_line_at = 0;
						}
					}
					if (m_cursorPosition == 0) {
						how_far_in = 0;
						curr_line_at = 0;
					}
					signed int current_line_length = -1;
					for(signed int i = curr_line_at; i < s.length(); i++) {
						if (s[i] == '\n' && current_line_length == -1) {
							current_line_length = i - curr_line_at;
							break;
						} else if (i == s.length()-1 && current_line_length == -1) {
							current_line_length = s.length() - curr_line_at;
						}
					}
					signed int next_line_start = curr_line_at + current_line_length + 1;
					signed int next_line_length = -1;
					for(signed int i = next_line_start; i < s.length(); i++) {
						if (s[i] == '\n' && next_line_length == -1) {
							next_line_length = i - next_line_start;
							break;
						} else if (i == s.length()-1 && next_line_length == -1) {
							next_line_length = s.length() - next_line_start;
						}
					}
					ARK2D::getLog()->v(StringUtil::append("m_cursorPosition: ", m_cursorPosition));
					ARK2D::getLog()->v(StringUtil::append("how_far_in: ", how_far_in));
					ARK2D::getLog()->v(StringUtil::append("curr_line_at: ", curr_line_at));
					ARK2D::getLog()->v(StringUtil::append("current_line_length: ", current_line_length));
					ARK2D::getLog()->v(StringUtil::append("next_line_start: ", next_line_start));
					ARK2D::getLog()->v(StringUtil::append("next_line_length: ", next_line_length));
					
					int newpos = m_cursorPosition;
					newpos -= how_far_in; 									// back to start of current line
					newpos += current_line_length; 							// end of current line.
					newpos += 1; 											// start of next line.
					newpos += std::min(how_far_in, next_line_length);	// move cursor in, but only if we have to. (i.e. previous line is longer.)
					m_cursorPosition = newpos;	


					ARK2D::getLog()->v(StringUtil::append("new m_cursorPosition: ", m_cursorPosition));
                    return true;

				} else if ((key == (unsigned int) Input::KEY_ENTER) && m_multiline) {
					m_text.insert("\n", m_cursorPosition);
					cursorRight();
					clearSelection();
                    return true;
				} else if (k.length() > 0) {

					if (m_maxLength > 0 && m_text.length() >= m_maxLength) {
						return true;
					}

					// do restrictions

					if (m_restrictCharacters == RESTRICT_ALPHANUMERIC || 
						m_restrictCharacters == RESTRICT_ALPHANUMERIC_SPACES || 
						m_restrictCharacters == RESTRICT_BASIC_PROGRAMMING
						) {

						string allowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
						
						if (m_restrictCharacters == RESTRICT_ALPHANUMERIC_SPACES) {
							allowedCharacters += " ";
						} else if (m_restrictCharacters == RESTRICT_BASIC_PROGRAMMING) {
							allowedCharacters += " +-*/<>=_[](){};:?\"!&|%\n\t\r.,#";
						}


						if (allowedCharacters.find(k) == string::npos) {
							return true;
						}
					}

					m_text.insert(k, m_cursorPosition);
					cursorRight();
					clearSelection();
                    return true;
				}
			}

            return false;


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

		void TextField::setFocussed(bool b) {
			UIComponent::setFocussed(b);
			if (b) {
				Input::setSoftwareKeyboardOpen(true);
			} else {
				Input::setSoftwareKeyboardOpen(false);
			}
			
		}

		TextField::~TextField() {

		}
	}
}
