
#include "SimpleTextField.h" 

SimpleTextField::SimpleTextField(): TextField() {
	TextField::setRestrictedCharacterSet(TextField::RESTRICT_ALPHANUMERIC);
}
void SimpleTextField::keyPressed(unsigned int key) {
	if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
		return;
	}
	//setFocussed(true); 
	TextField::keyPressed(key);
}
void SimpleTextField::render() { 
	int x = m_x + (m_width/2);
	int y = m_y + (m_height/2);
	x -= (ARK2D::getRenderer()->getFont()->getStringWidth(m_text.get())/2);
	y -= (ARK2D::getRenderer()->getFont()->getLineHeight()/2);
	renderText(x, y); 
	renderCaret();
} 
void SimpleTextField::renderCaret() {
	//TextField::renderCaret(m_x, m_y, m_x + 40, m_y);

	int x = m_x + (m_width/2);
	int y = m_y + (m_height/2);
	x -= (ARK2D::getRenderer()->getFont()->getStringWidth(m_text.get())/2);
	y += (ARK2D::getRenderer()->getFont()->getLineHeight()/2);

	int sw = ARK2D::getRenderer()->getFont()->getStringWidth(m_text.substring(0, m_cursorPosition).get());
	ARK2D::getRenderer()->setDrawColor(Color::white_50a);
	ARK2D::getRenderer()->fillRect(x + sw, y, 10, 2);
	ARK2D::getRenderer()->setDrawColor(Color::white);
}
void SimpleTextField::renderCaret(int x1, int y1, int x2, int y2) {
	TextField::renderCaret(x1, y1, x2, y2);
}
SimpleTextField::~SimpleTextField() {

}
