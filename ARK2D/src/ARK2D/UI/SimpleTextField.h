
#ifndef SIMPLETEXTFIELD_H_
#define SIMPLETEXTFIELD_H_

#include "TextField.h"

class ARK2D_API SimpleTextField : public TextField {
	public: 
		SimpleTextField();

		virtual bool keyPressed(unsigned int key);
    
		virtual void render();
		virtual void renderCaret();
		virtual void renderCaret(int x1, int y1, int x2, int y2);
 
		virtual ~SimpleTextField();
};

#endif
