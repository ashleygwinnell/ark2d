/*
 * FloatingFadingText.h
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#ifndef FLOATINGFADINGTEXT_H_
#define FLOATINGFADINGTEXT_H_

#include "ARK.h"
#include "GameObject.h"

#include <string>
using namespace std;

class FloatingFadingText : public GameObject {
	private:
		string m_text;
		float m_alpha;

	public:
		//Color* m_color;
		FloatingFadingText(string s);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);

		virtual ~FloatingFadingText();
};

#endif /* FLOATINGFADINGTEXT_H_ */
