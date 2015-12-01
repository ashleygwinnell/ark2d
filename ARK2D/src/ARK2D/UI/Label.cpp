/*
 * Label.cpp
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#include "Label.h"
#include "../Graphics/Renderer.h"

namespace ARK {
	namespace UI {
		Label::Label():
			AbstractUIComponent(),
			m_text(""),
			m_alignX(-1),
			m_alignY(-1) 
		{

		}

		Label::Label(string text):
			AbstractUIComponent(),
			m_text(text),
			m_alignX(-1),
			m_alignY(-1) 
		{
			setSize(ARK2D::getRenderer()->getFont()->getStringWidth(text), ARK2D::getRenderer()->getFont()->getLineHeight());
		}

		Label::Label(string text, signed int alignX, signed int alignY):
			AbstractUIComponent(),
			m_text(text),
			m_alignX(alignX),
			m_alignY(alignY)
		{
			setSize(ARK2D::getRenderer()->getFont()->getStringWidth(text), ARK2D::getRenderer()->getFont()->getLineHeight());
		}
		Label::Label(string text, signed int alignX, signed int alignY, float sc):
			AbstractUIComponent(),
			m_text(text),
			m_alignX(alignX),
			m_alignY(alignY)
		{
			setSize(ARK2D::getRenderer()->getFont()->getStringWidth(text), ARK2D::getRenderer()->getFont()->getLineHeight());
			transform.scale.set(sc, sc, sc);
		}

		void Label::setText(string text) {
			setText(text, false);
		}
		void Label::setText(string text, bool updateSize) {
			m_text = text;
			if (updateSize) {
				setSize(ARK2D::getRenderer()->getFont()->getStringWidth(text), ARK2D::getRenderer()->getFont()->getLineHeight());
			}
		}

		void Label::render() {
			Renderer* r = ARK2D::getRenderer();
			vector<string> lines = StringUtil::split(m_text, "\n");
			float innerWidth = 0.0f;
			float currentY = 0.0f;
           	for(unsigned int i = 0; i < lines.size(); i++) {
           		r->drawString(lines[i], 0.0f, currentY, m_alignX, m_alignX);
           		currentY += r->getFont()->getLineHeight();

           		float stringWidth = r->getFont()->getStringWidth(lines[i]);
           		if (stringWidth > innerWidth) {
           			innerWidth = stringWidth;
           		}
           	} 

           	setBounds(innerWidth, currentY, 0.0f);
		}
		void Label::renderBounds() {
			Renderer* g = ARK2D::getRenderer();
			g->drawRect(0, 0, m_width, m_height);
		}

		Label::~Label() {

		}
	}
}
