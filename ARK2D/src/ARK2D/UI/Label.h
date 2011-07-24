/*
 * Label.h
 *
 *  Created on: 23 Jul 2011
 *      Author: Ashley
 */

#ifndef LABEL_H_
#define LABEL_H_

#include "AbstractUIComponent.h"

#include <string>
using namespace std;

/**
 * TODO: text alignment left top right bottom etc.
 */
class Label : public AbstractUIComponent {
	private:
		string m_text;
	public:
		Label();
		Label(string text);
		void setText(string text);
		void render();
		virtual ~Label();
};

#endif /* LABEL_H_ */
