/*
 * Overlay.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: ashleygwinnell
 */

#include "Overlay.h"
//#include "../../../ARK.h"

//#include "../../Namespaces.h"
#include "../../Graphics/ImageIO/PNGImage.h"

namespace ARK {
	namespace GJ {
		namespace Next {

			

			void __internalGameJoltOverlayListener(ARK::GJ::Next::API* api, gjCallbackType type, void* data)
			{
				ARK2D::getLog()->v("--OVERLAY LISTENER--");
				switch(type) {
					case GJ_USERS_AUTH_RESULT: 
					{
						GJLoginState* ls = (GJLoginState*) ARK::GJ::Next::GameJolt::getInstance()->getState(ARK::GJ::Next::GameJolt::STATE_LOGIN);

						gjUsersAuthResult* result = (gjUsersAuthResult*) data;
						if (!result->success) {
							ls->onLoginFailed(result->message);
							return;
						}
						ls->onLoginSuccess();
						
						/*Overlay* o = Overlay::getInstance();
						if (o != NULL) {

						}*/
						break;
					}
					case GJ_HIGHSCORES_RESULT:
					{
						GJLeaderboardState* as = (GJLeaderboardState*) ARK::GJ::Next::GameJolt::getInstance()->getState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARD);
						
						gjHighscoresResult* result = (gjHighscoresResult*) data;
						unsigned int timestamp = ARK2D::getTimer()->unixTimestamp();
						if (timestamp > as->timestamp + 30) { // refresh every 30 seconds
							if (as->scores != NULL) { gjHighscoresResult_dispose(as->scores); }
							as->scores = gjHighscoresResult_copy(result);
							as->timestamp = timestamp;
						}
						break;
					}
					case GJ_ACHIEVEMENTS_RESULT:
					{
						GJAchievementsState* as = (GJAchievementsState*) ARK::GJ::Next::GameJolt::getInstance()->getState(ARK::GJ::Next::GameJolt::STATE_ACHIEVEMENTS);
						
						gjAchievementsResult* result = (gjAchievementsResult*) data;
						unsigned int timestamp = ARK2D::getTimer()->unixTimestamp();
						if (timestamp > as->timestamp + 30) { // refresh every 30 seconds
							if (as->achievements != NULL) { gjAchievementsResult_dispose(as->achievements); }
							as->achievements = gjAchievementsResult_copy(result);
							as->timestamp = timestamp;

							for(unsigned int i = 0; i < as->achievements->achievementsCount; ++i) {
								unsigned int id = as->achievements->achievements[i]->id;
								as->fetchIcon(id);
							}

						}
						break;
					}
					case GJ_HIGHSCORE_TABLES_RESULT:
					{
						GJLeaderboardsState* as = (GJLeaderboardsState*) ARK::GJ::Next::GameJolt::getInstance()->getState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARDS);
						
						gjHighscoreTablesResult* result = (gjHighscoreTablesResult*) data;
						unsigned int timestamp = ARK2D::getTimer()->unixTimestamp();
						if (timestamp > as->timestamp + 30) { // refresh every 30 seconds
							if (as->tables != NULL) { gjHighscoreTablesResult_dispose(as->tables); }
							as->tables = gjHighscoreTablesResult_copy(result);
							as->timestamp = timestamp;
							as->refreshUI();
						}
						break;
					}
					case GJ_GENERAL_RESULT:
					case GJ_HIGHSCORE_SUBMIT_RESULT:
					case GJ_HIGHSCORE_RANK_RESULT:
					case GJ_ACHIEVEMENT_SUBMIT_RESULT:
					case GJ_SESSION_OPEN_RESULT:
					case GJ_SESSION_PING_RESULT:
					case GJ_SESSION_CLOSE_RESULT:
						break;
				}
			}

			string gjUtil_strtoupper(string str) {
				string chars = "abcdefghijklmnopqrstuvwxyz";
				string replace = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

				for(int i = 0; i < str.length(); ++i) {
					for(unsigned int j = 0; j < chars.length(); ++j) {
						if (str[i] == chars[j]) {
							str.replace(i, 1,  &replace[j], 1);
						}	
					}
				}
				return str;
			}

			GJTextField::GJTextField():
				ARK::UI::TextField(),
				caretTimer(0.0f)
			{

			}
			void GJTextField::update() {
				caretTimer += ARK2D::getTimer()->getDelta();
				if (caretTimer > 1.0f) {
					caretTimer -= 1.0f;
				}
			}
			void GJTextField::renderBackground() {
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				Renderer* r = ARK2D::getRenderer();
				r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				r->fillRect(0, 0, m_width, m_height);

				m_textColor.set("#000000");
				m_textColor.setAlpha(gj->getAlpha());

				m_caretColor.set("#000000");
				m_caretColor.setAlpha(0.5f * gj->getAlpha());
			}
			void GJTextField::renderText(int x, int y) {
				x += 10;
				y += 2;
				TextField::renderText(x, y);
			}
			void GJTextField::renderCaret(int x1, int y1, int x2, int y2) {
			 	x1 += 10;
				x2 += 10;
				if (caretTimer < 0.5f) { 
			 		TextField::renderCaret(x1, y1, x2, y2);
			 	}
			}
			void GJTextField::renderOverlay() {

			}
			void GJTextField::setFocussed(bool b) {
				ARK2D::getLog()->e(StringUtil::append("gjtextfield focussed", Cast::boolToString(b)));
				TextField::setFocussed(b);

				#ifdef ARK2D_WINDOWS_PHONE_8
					if (b) {
						ARK::UI::Dialog::openInputDialog(0, "Edit text:", m_text.getc());
					}
				#endif
			}
			GJTextField::~GJTextField() {

			}


			GJButton::GJButton(): 
				ARK::UI::Button() 
			{ 

			}
			void GJButton::renderText(int x, int y) {
				y += 2;
				//ARK::UI::Button::renderText(x, y);
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				Renderer* g = ARK2D::getRenderer();
				g->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				g->drawString(m_text.get(), x, y);
			}
			void GJButton::renderBackground() {

				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				Renderer* g = ARK2D::getRenderer();
				g->setDrawColor("#2f7f6f", gj->getAlpha());
				g->fillRect(0, 0, m_width, m_height);
				g->setFont(gj->font);
			}
			void GJButton::renderOverlay() {

			}
			GJButton::~GJButton() { }


			// ------------------------------------------------------------------------------------------------








			GJScrollablePanel::GJScrollablePanel():
				m_x(0),
				m_y(0),
				m_width(0),
				m_height(0),   
				m_originalX(0),
				m_originalY(0),
				m_scrollY(0),
				m_scrollYReleased(0),
				m_scrollYSpeed(0.0f),
				m_scrollYDecelerate(200.0f),

				m_dragging(false),
				m_draggingStartY(0),
				m_draggingStart(0,0),
				m_draggingEnd(0,0),
				m_releaseTimer(0.15f),
				m_dragTimer(0.0f),

				m_innerHeight(0),
				m_actualInnerHeight(0)
			{
				
			}
			void GJScrollablePanel::init(int x, int y, int w, int h) {
				m_x = x;
				m_y = y;
				m_width = w;
				m_height = h;
				m_originalX = x;
				m_originalY = y;

				m_scrollY = 0;
				m_scrollYReleased = 0;
				m_scrollYSpeed = 0.0f;
				m_scrollYDecelerate = 200.0f;

				m_dragging = false;
				m_draggingStartY = 0;
				m_draggingStart.set(0,0);
				m_draggingEnd.set(0,0);
				m_releaseTimer = 0.15f;
				m_dragTimer = 0.0f;

				m_innerHeight = 0;
				m_actualInnerHeight = 0;
			}
			void GJScrollablePanel::setHeight(float innerHeight, float outerHeight) {
				m_height = outerHeight;
				m_innerHeight = innerHeight - m_height; //outerHeight;
				m_actualInnerHeight = innerHeight;
			}

			void GJScrollablePanel::update() { 
				
				GameTimer* timer = ARK2D::getContainer()->getTimer();


			 
				// target page x;
				float targetY = 0;
				if (!m_dragging) {

					m_dragTimer = 0.0f;

					if (m_scrollYSpeed != 0.0f) {
						m_scrollYReleased += m_scrollYSpeed * timer->getDelta();

						if (m_scrollYSpeed > 0.0f) {
							m_scrollYSpeed -= m_scrollYDecelerate * timer->getDelta();
							if (m_scrollYSpeed < 0.0f) {
								m_scrollYSpeed = 0.0f;
							}
						} else if (m_scrollYSpeed < 0.0f) {
							m_scrollYSpeed += m_scrollYDecelerate * timer->getDelta();
							if (m_scrollYSpeed > 0.0f) {
								m_scrollYSpeed = 0.0f;
							}
						}

						//m_scrollY = m_scrollYReleased;
					}

					if (m_scrollYReleased < 0) { 
						m_scrollY = targetY;
						

						if (m_releaseTimer.getTime() > 0.0f) 
						{
							m_scrollY = Easing::ease(Easing::QUADRATIC_OUT, m_releaseTimer.getTime(), m_scrollYReleased, targetY - m_scrollYReleased, m_releaseTimer.getDuration());

							if (m_releaseTimer.update()) {
								m_releaseTimer.reset();
								m_scrollYSpeed = 0.0f;
								//m_page = m_pageTarget;				
							}
						} 
					} else if (m_scrollYReleased > m_innerHeight) {
						targetY = m_innerHeight;
						m_scrollY = targetY;


						if (m_releaseTimer.getTime() > 0.0f) 
						{
							m_scrollY = Easing::ease(Easing::QUADRATIC_OUT, m_releaseTimer.getTime(), m_scrollYReleased, targetY - m_scrollYReleased, m_releaseTimer.getDuration());

							if (m_releaseTimer.update()) {
								m_releaseTimer.reset();
								m_scrollYSpeed = 0.0f;
								//m_page = m_pageTarget;				
							}
						} 
					} else {
						m_scrollY = m_scrollYReleased;
					}


				
				} 

				
			 	else {  
				//if (m_dragging) {
					m_scrollY = m_scrollYReleased;
					m_dragTimer += timer->getDelta();
				}

				//ARK2D::getLog()->e(StringUtil::append("m_actualInnerHeight: ", m_actualInnerHeight));
				//ARK2D::getLog()->e(StringUtil::append("m_height: ", m_height));
				if (m_actualInnerHeight < m_height) {
					m_scrollY = 0;
				}

			}
			/*void GJScrollablePanel::render() {
				
				GameContainer* container = dg->getContainer();
				Renderer* r = ARK2D::getRenderer();
				int tx = ARK2D::getContainer()->getTranslateX();
				int ty = ARK2D::getContainer()->getTranslateY();
				float sc = ARK2D::getContainer()->getScale();
			  
				r->pushMatrix();  
				r->setScissorTestEnabled(true);
				r->scissor(tx + (m_x * sc), ty + (m_y * sc), m_width * sc, m_height * sc);
				
				int curY = m_y - m_scrollY;

				//r->fillGradientRect(m_x, m_y, m_width, 20, const_cast<Color*>(dg->backdrop->getTextColor()), const_cast<Color*>(&Color::black_0a));
				int overlayImageY = m_y + 4;
				if (m_scrollY > 0) {
					overlayImageY -= m_scrollY;
				} else {
					//overlayImageY += m_scrollY;
				}

				r->getBatch()->setEnabled(true);

				dg->stateShop->m_overlayImage->drawCentered(m_x + (m_width/2), overlayImageY);


				curY += START_SPACING;
				curY += START_SPACING;

				// set alphas
				dg->font_small->getImage()->setAlpha(m_alphaModifier);

				// AVAILABLE
				//dg->font_small->drawString("Available", m_x, curY, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				//curY += TEXT_SPACING_SMALLTITLE; //TEXT_SPACING;
				//r->setLineWidth(2 * dg->getContainer()->getScale());
				//r->drawLine(m_x, curY-LINE_BACKSPACE, m_x + m_width, curY-LINE_BACKSPACE);
				//dg->stateShop->m_lineImage->drawCentered(m_x + (m_width/2), curY-LINE_BACKSPACE);
				for(unsigned int i = 0; i < m_buttons.size(); ++i) {
					if (i > 0 && i % m_columns == 0) { curY += BUTTON_SPACING; }
					m_buttons.get(i)->render();
					//curY += BUTTON_SPACING; 
					//if (i % 2 == 1) { curY += BUTTON_SPACING; }
			 
				} 

				

				r->getBatch()->render();
				r->getBatch()->setEnabled(false);
				curY += BUTTON_SPACING;

			 curY += GROUP_SPACING;
				curY += GROUP_SPACING; 

				if (m_buttons.size() == 16) {
				//	curY += BUTTON_SPACING; 
				}

				curY += END_SPACING;
			  
			  	int overlayImageY2 = curY - 4;
				if (m_scrollY > m_innerHeight) {
					overlayImageY2 += (m_scrollY - m_innerHeight);
				} 

				//if (m_itemType == StoreItem::TYPE_WEAPON) {
				//	overlayImageY2 = curY - 4;
				//}
				dg->stateShop->m_overlayImage->drawCenteredFlipped(m_x + (m_width/2), overlayImageY2, false, true);
				dg->stateShop->m_overlayImage->setAlpha(1.0f);
				

			  	r->scissor(0,0,container->getDynamicWidth(), container->getDynamicHeight());
				r->setScissorTestEnabled(false);
				r->popMatrix(); 
				//r->drawRect(m_x, m_y, m_width, m_height);
				 
				r->setDrawColor(Color::white);
				dg->stateShop->m_lineImage->setColor(NULL);
				dg->stateShop->m_lineImage->setAlpha(1.0f * m_alphaModifier);
				//dg->stateShop->m_lineImage->drawCentered(m_x + (m_width/2), m_y);
				//dg->stateShop->m_lineImage->drawCentered(m_x + (m_width/2), m_y + m_height);
				
			 
			 	r->getBatch()->setEnabled(true);
				
				Image* img = dg->stateShop->m_lineImage;
				r->setDrawColorf(1.0f, 1.0f, 1.0f, m_alphaModifier);
				r->texturedLineOverlay(img, 1.0f, m_x, m_y-1, m_x+m_width, m_y-1, 0, 0, img->getWidth(), 0);
				r->texturedLineOverlay(img, 1.0f, m_x, m_y+m_height-1, m_x+m_width, m_y+m_height-1, 0, 0, img->getWidth(), 0);

				r->getBatch()->render();
				r->getBatch()->setEnabled(false);

				dg->stateShop->m_lineImage->setAlpha(0.5f * m_alphaModifier);


				// reset alphas
				dg->font_small->getImage()->setAlpha(1.0f);

				//r->drawString(Cast::toString<float>(m_scrollYSpeed), 30, 30);
			}*/
			bool GJScrollablePanel::keyPressed(unsigned int key) {
				if (!Shape<int>::collision_rectangleRectangle(m_x, m_y, m_width, m_height, ARK2D::getInput()->getMouseX(), ARK2D::getInput()->getMouseY(), 1, 1)) { return false; }
				
				Input* in = ARK2D::getInput();
				if (key == (signed int) Input::MOUSE_BUTTON_LEFT) {
					if (!m_dragging) { 
						m_dragging = true;
						m_draggingStartY = in->getMouseY();
						m_draggingStart.set(in->getMouseX(), in->getMouseY() + m_scrollY );
						m_scrollYReleased = m_scrollY;

						return true;
					}
				}

				// for(unsigned int i = 0; i < m_customisationButtons.size(); i++) {
				// 	m_customisationButtons.get(i)->keyPressed(key);
				// } 

				if (m_actualInnerHeight < m_height) { 
					m_dragging = false;
					m_scrollYReleased = -100;
					//m_draggingStartY = 0;
					//m_scrollY = 0;
					//m_scrollYReleased = 0;
				}
				return false;
			}
			bool GJScrollablePanel::keyReleased(unsigned int key) {
				//if (!Shape<int>::collision_rectangleRectangle(m_x, m_y, m_width, m_height, ARK2D::getInput()->getMouseX(), ARK2D::getInput()->getMouseY(), 1, 1)) { return; }
				

				
				//DefaultGame* dg = DefaultGame::getInstance();
				bool wasDragging = m_dragging;// && dg->stateShop->m_dragType != ShopState2::DRAG_CALCULATING;

				Input* in = ARK2D::getInput();   
				if (key == (signed int) Input::MOUSE_BUTTON_LEFT) {
					if (m_dragging) {
						m_dragging = false;
						m_draggingEnd.set(in->getMouseX(), in->getMouseY());

						m_releaseTimer.reset();
						m_releaseTimer.update(); 
			 
						if (m_dragTimer != 0) {
							m_scrollYSpeed = (m_draggingStartY - m_draggingEnd.getY()) / m_dragTimer;
						}
						
						/*if (m_scrollYSpeed > 200) {
							m_scrollYSpeed = 200;
						}
						if (m_scrollYSpeed < -200) {
							m_scrollYSpeed = -200; 
						}*/

						//if (m_draggingStart.getY() > m_draggingEnd.getY()) {
						//	m_scrollYSpeed *= -1.0f;
						//} else {
			//
			//			}
			 		 	return true;
			 
					}
				}	
			 

			 	if (m_actualInnerHeight < m_height) {
					String thisStr("inner height: ");
					thisStr += m_actualInnerHeight;
					thisStr += " height: "; 
					thisStr += m_height;
					//ErrorDialog::createAndShow(thisStr.get());

					/*m_releaseTimer.setTime(m_releaseTimer.getDuration()); 
					m_scrollY = 0;
					m_scrollYReleased = m_height - m_actualInnerHeight;
					m_scrollYSpeed = 0;
					m_draggingEnd.set(0, 0); 
					m_draggingStart.set(0, 0);
			*/
					m_dragging = false;
					m_scrollYReleased = -100;
					wasDragging = false;
					//m_draggingStartY = m_height - m_actualInnerHeight;
					//m_scrollY = m_height - m_actualInnerHeight;
					//m_scrollYReleased = m_height - m_actualInnerHeight;
				} 
				
				//bool dragWasSuperSmall = m_dragging && (MathUtil::distance(m_draggingStart.getX(), m_draggingStart.getY() + m_scrollY, m_draggingEnd.getX(), m_draggingEnd.getY())  < 5.0f * ARK2D::getContainer()->getScale());
				//if (!m_dragging || dragWasSuperSmall) {
				if (!wasDragging) {
			//		for (unsigned int i = 0; i < m_buttons.size(); i++) { 
			//			m_buttons.get(i)->keyReleased(key); 
			//		} 
				}
			 

				

				// for(unsigned int i = 0; i < m_customisationButtons.size(); i++) { 
				// 	m_customisationButtons.get(i)->keyReleased(key);
				// } 
				return false;
			}
			bool GJScrollablePanel::mouseMoved(int x, int y, int oldx, int oldy) {
			//	for(unsigned int i = 0; i < m_buttons.size(); i++) {
			//		m_buttons.get(i)->mouseMoved(x, y, oldx, oldy);
			//	} 


			 

				if (m_dragging) {
					m_scrollYReleased = (m_draggingStart.getY() - y);;//in->getMouseY() + m_scrollY;
					//m_scrollY = (m_draggingStart.getY() - y);
					//m_scrollY =  oldy - y; 

					 
						
					

						// limit left side
						int SCROLL_UNDERFLOW = -30;
						if (m_scrollYReleased < SCROLL_UNDERFLOW) {
							m_scrollYReleased = SCROLL_UNDERFLOW;
						} else if (m_scrollYReleased > m_innerHeight - SCROLL_UNDERFLOW) {
							m_scrollYReleased = m_innerHeight - SCROLL_UNDERFLOW;
						}
			 
					

					m_releaseTimer.reset();
					m_releaseTimer.update();
					return true;
				}
			 

			 	if (m_actualInnerHeight < m_height) {
					m_dragging = false;
					m_scrollYReleased = -100;
				}
				
				return false;
			}
			GJScrollablePanel::~GJScrollablePanel() {
				
			} 















			// ------------------------------------------------------------------------------------------------

			void gjLoginState_editNameCallback() {
				string result = Dialog::getInputDialogText();
				result = StringUtil::restrictToAlphanumericsSpaces(result);

				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				GJLoginState* ls = (GJLoginState*) gj->getState(ARK::GJ::Next::GameJolt::STATE_LOGIN);
				ls->username->setText(result);
			}
			void gjLoginState_editTokenCallback() {
				
			}
			GJLoginState::GJLoginState(): GameState() {
				ARK::Util::Callback gamejoltNameCallback;
				gamejoltNameCallback.setId(ARK::Util::Callbacks::CALLBACK_GAMEJOLT_OVERLAY_EDITNAME); 
				gamejoltNameCallback.setFunctionPointer((void*) &gjLoginState_editNameCallback);
				ARK::Util::Callbacks::add(gamejoltNameCallback);

				ARK::Util::Callback gamejoltTokenCallback;
				gamejoltTokenCallback.setId(ARK::Util::Callbacks::CALLBACK_GAMEJOLT_OVERLAY_EDITTOKEN); 
				gamejoltTokenCallback.setFunctionPointer((void*) &gjLoginState_editTokenCallback);
				ARK::Util::Callbacks::add(gamejoltTokenCallback);
			}

			void GJLoginState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
			void GJLoginState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

			unsigned int GJLoginState::id() {
				return ARK::GJ::Next::GameJolt::STATE_LOGIN;
			}
			void GJLoginState::init(GameContainer* container, StateBasedGame* game) {

				username = new GJTextField();
				username->setSize(280, 40);
				username->setText("");
				username->setFocussed(true);

				usertoken = new GJTextField();
				usertoken->setSize(280, 40);
				usertoken->setText("");
				usertoken->setPasswordField(true);

				//Dialog::openInputDialog(Callbacks::CALLBACK_GAMEJOLT_OVERLAY_EDITNAME, "Edit Username:", "");
				//Dialog::openInputDialog(Callbacks::CALLBACK_GAMEJOLT_OVERLAY_EDITTOKEN, "Edit Token:", "");

				login = new GJButton();
				login->setText("LOGIN");
				login->setSize(280, 40);
				login->setEvent((void*) onLogin);
				login->setEventObj(this);

				signup = new GJButton();
				signup->setText("SIGNUP");
				signup->setSize(280, 40);
				signup->setEvent((void*) onSignup);
				signup->setEventObj(this);

				help = new GJButton();
				help->setText("HELP");
				help->setSize(80, 40);
				
				help->setEvent((void*) onHelp);
				help->setEventObj(this);
				
				close = new GJButton();
				close->setText("CLOSE");
				close->setSize(90, 40);
				close->setEvent((void*) onClose);
				close->setEventObj(this);	
			}
			void GJLoginState::onLogin(GJLoginState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->api->authUser(ls->username->getText().getc(), ls->usertoken->getText().getc());
			}
			void GJLoginState::onSignup(GJLoginState* ls) {
				FileUtil::openBrowserToURL("http://gamejolt.com/auth/sign_up/");
			}
			void GJLoginState::onHelp(GJLoginState* ls) {
				FileUtil::openBrowserToURL("http://help.gamejolt.com/tokens");
			}
			void GJLoginState::onClose(GJLoginState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->close();
			}
			void GJLoginState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				Input* in = ARK2D::getInput();
				if (in->isKeyPressed(Input::MOBILE_BACK) || in->isKeyPressed(Input::KEY_ESCAPE)) { close->doEvent(); }

				username->update();
				usertoken->update();

				m_iconX = container->getWidth()*0.5f;
				m_iconY = DisplayUtil::adjustY(80);
				m_iconScale = 6.0f;
				username->setLocation(20, DisplayUtil::adjustY2(165));	
				usertoken->setLocation(20, DisplayUtil::adjustY2(230));	
				login->setLocation(20, DisplayUtil::adjustY2(300));	
				signup->setLocation(20, DisplayUtil::adjustY2(350));	
				help->setLocation(container->getWidth() - 20 - 80, container->getHeight() - 20 - 40);	
				close->setLocation(20, container->getHeight() - 20 - 40);

				if (container->getWidth() > container->getHeight()) {
       				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
       				m_iconScale = 9.0f;
					m_iconX = (container->getWidth()*0.499f) - (gj->icon->getWidth()*m_iconScale*0.5f) - 20;
					m_iconY = container->getHeight()*0.5f;

					username->setX( (container->getWidth()*0.499f) + 10 );	
					usertoken->setX( (container->getWidth()*0.499f) + 10 );	
					login->setX( (container->getWidth()*0.499f) + 10 );	
					signup->setX( (container->getWidth()*0.499f) + 10 );

					float ch = container->getHeight() * 0.5f;
					username->setY( ch - 100 );	 
					usertoken->setY( ch - 35 );	
					login->setY( ch + 20);	
					signup->setY( ch + 70 );
				}
			}
			void GJLoginState::onLoginFailed(string message) {
				//ErrorDialog::createAndShow(message);
				// show GJ Growl...?
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				gj->m_loggedin = false;
			}
			void GJLoginState::onLoginSuccess() {

				
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				gj->m_loggedin = true;

				gj->file->add("username", gj->api->m_userName);
				gj->file->add("usertoken", gj->api->m_userToken);
				gj->file->save();

				gj->api->sessionOpen();

				if (gj->getCurrentState() == this) {
					gj->enterState(ARK::GJ::Next::GameJolt::STATE_OVERVIEW);
				}
			}
			void GJLoginState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				float alpha = gj->getAlpha();

				if (container->getWidth() > container->getHeight()) {

				} else {

				}

				r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
				gj->icon->drawCenteredScaled(m_iconX, m_iconY, m_iconScale, m_iconScale);

				r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
				r->drawString("USERNAME: ", username->getMinX(), username->getMinY() - 5, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 0.5f);
				username->render();

				r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
				r->drawString("TOKEN: ", usertoken->getMinX(), usertoken->getMinY() - 5, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 0.5f);
				usertoken->render();

				r->setDrawColorf(1.0f, 1.0f, 1.0f, alpha);
				login->render();
				signup->render();
				help->render();
				close->render();
			}
			bool GJLoginState::keyPressed(unsigned int key) {
				if (username->keyPressed(key)) return true;
				if (usertoken->keyPressed(key)) return true;
				if (login->keyPressed(key)) return true;
				if (signup->keyPressed(key)) return true;
				if (help->keyPressed(key)) return true;
				if (close->keyPressed(key)) return true;
				return false;
			}
			bool GJLoginState::keyReleased(unsigned int key) {
				if (username->keyReleased(key)) return true;
				if (usertoken->keyReleased(key)) return true;
				if (login->keyReleased(key)) return true;
				if (signup->keyReleased(key)) return true;
				if (help->keyReleased(key)) return true;
				if (close->keyReleased(key)) return true;
				return false;
			}
			bool GJLoginState::mouseMoved(int x, int y, int oldx, int oldy) {
				if (username->mouseMoved(x, y, oldx, oldy)) return true;
				if (usertoken->mouseMoved(x, y, oldx, oldy)) return true;
				if (login->mouseMoved(x, y, oldx, oldy)) return true;
				if (signup->mouseMoved(x, y, oldx, oldy)) return true;
				if (help->mouseMoved(x, y, oldx, oldy)) return true;
				if (close->mouseMoved(x, y, oldx, oldy)) return true;
				return false;
			}
			GJLoginState::~GJLoginState() {

			}
			 






			GJOverviewState::GJOverviewState(): GameState() {

			}

			void GJOverviewState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { }
			void GJOverviewState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

			unsigned int GJOverviewState::id() {
				return ARK::GJ::Next::GameJolt::STATE_OVERVIEW;
			}
			void GJOverviewState::init(GameContainer* container, StateBasedGame* game) {
				achievements = new GJButton();
				achievements->setText("TROPHIES");
				achievements->setSize(container->getWidth()-40, 40);
				
				achievements->setEvent((void*) onAchievements);
				achievements->setEventObj(this);

				leaderboards = new GJButton();
				leaderboards->setText("LEADERBOARDS");
				leaderboards->setSize(container->getWidth()-40, 40);
				
				leaderboards->setEvent((void*) onLeaderboards);
				leaderboards->setEventObj(this);

				stats = new GJButton();
				stats->setText("STATS");
				stats->setSize(container->getWidth()-40, 40);
				stats->setEvent((void*) onStats);
				stats->setEventObj(this);

				close = new GJButton();
				close->setText("CLOSE");
				close->setSize(80, 40);
				close->setEvent((void*) onClose);
				close->setEventObj(this);	

				logout = new GJButton();
				logout->setText("LOG OUT");
				logout->setSize(100, 40);
				logout->setEvent((void*) onLogout);
				logout->setEventObj(this);	
				
				
			}
			void GJOverviewState::onAchievements(GJOverviewState* ls) {
				//ARK2D::getLog()->e("achievements");
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_ACHIEVEMENTS);
			}
			void GJOverviewState::onLeaderboards(GJOverviewState* ls) {
				//ARK2D::getLog()->e("leaderboards");	
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARDS);
			}
			void GJOverviewState::onStats(GJOverviewState* ls) {
				//ARK2D::getLog()->e("stats");	
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_STATS);
			}
			void GJOverviewState::onLogout(GJOverviewState* ls) {
				ARK2D::getLog()->v("-- logout --");	
 
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				gj->m_loggedin = false;

				gj->file->add("username", "");
				gj->file->add("usertoken", "");
				gj->file->save();

				gj->api->sessionClose();

				ARK::GJ::Next::GameJolt::getInstance()->api->logout();
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_LOGIN);
			}
			void GJOverviewState::onClose(GJOverviewState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->close();
			}
			void GJOverviewState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				Input* in = ARK2D::getInput();
				if (in->isKeyPressed(Input::MOBILE_BACK) || in->isKeyPressed(Input::KEY_BACKSPACE)) { close->doEvent(); }

				achievements->setLocation(20, DisplayUtil::adjustY2(250));	
				leaderboards->setLocation(20, DisplayUtil::adjustY2(300));	
				stats->setLocation(20, DisplayUtil::adjustY2(350));	
				close->setLocation(20, container->getHeight() - 20 - 40);	
				logout->setLocation(container->getWidth() - 20 - 100, container->getHeight() - 20 - 40);	
			}
			void GJOverviewState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				r->drawString("YOU ARE LOGGED IN AS: ", container->getWidth()/2, DisplayUtil::adjustY(144), Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM, 0.0f, 0.5f);
				r->drawString("GWINNELL", container->getWidth()/2, DisplayUtil::adjustY(150), Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP);
				
				//r->drawString("ACHIEVEMENTS: ", achievements->getMinX(), achievements->getMinY() - 5, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 0.5f);
				//r->drawString("LEADERBOARDS: ", leaderboards->getMinX(), leaderboards->getMinY() - 5, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 0.5f);
				//r->drawString("STATS: ", stats->getMinX(), stats->getMinY() - 5, Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM, 0.0f, 0.5f);
				achievements->render();
				leaderboards->render();
				stats->render();

				logout->render();
				close->render();
			}
			bool GJOverviewState::keyPressed(unsigned int key) {
				if (achievements->keyPressed(key)) return true;
				if (leaderboards->keyPressed(key)) return true;
				if (stats->keyPressed(key)) return true;
				if (logout->keyPressed(key)) return true;
				if (close->keyPressed(key)) return true;
				return false;
			}
			bool GJOverviewState::keyReleased(unsigned int key) {
				if (achievements->keyReleased(key)) return true;
				if (leaderboards->keyReleased(key)) return true;
				if (stats->keyReleased(key)) return true;
				if (logout->keyReleased(key)) return true;
				if (close->keyReleased(key)) return true;
				return false;
			}
			bool GJOverviewState::mouseMoved(int x, int y, int oldx, int oldy) {
				if (achievements->mouseMoved(x, y, oldx, oldy)) return true;
				if (leaderboards->mouseMoved(x, y, oldx, oldy)) return true;
				if (stats->mouseMoved(x, y, oldx, oldy)) return true;
				if (logout->mouseMoved(x, y, oldx, oldy)) return true;
				if (close->mouseMoved(x, y, oldx, oldy)) return true;
				return false;
			}
			GJOverviewState::~GJOverviewState() {

			}






			GJAchievementsState::GJAchievementsState(): 
				GameState(),
				back(NULL),
				scrollablePanel(NULL),
				achievements(NULL),
				achievementIcons(),
				timestamp(0),
				m_requests(),
				m_callbacks(),
				m_callbackAddMutex(NULL)
			{
				m_callbackAddMutex = new Mutex();
			}
			void GJAchievementsState::fetchIcon(unsigned int id) {
				// if not in map, fetch request.
				if (achievementIcons.find(id) == achievementIcons.end()) {
					gjAchievement* ach = findAchievementById(id);
					string url = string(ach->imageUrl);

					string ext = StringUtil::getExtension(url);
					if (ext == "png") {

						ARK2D::getLog()->e("-- fetch icon --");
						gjUrlRequest* req = gjUrlRequest_create();
						gjUrlRequest_setUrl(req, url.c_str());
						gjUrlRequest_start(req, (void*) &fetchIconInternal, this);
						m_requests.push_back(req);

					} else {
						ARK2D::getLog()->e("-- extension --");
						ARK2D::getLog()->e(ext);
						achievementIcons[id] = getImageForTrophy(id);
					}
				}
			}
			void GJAchievementsState::fetchIconInternal(GJAchievementsState* as, string result, gjUrlRequest* req) 
			{
				// Check for errors
				if (req->request->hasError()) 
				{
					ARK::GJ::Next::GameJolt* gamejolt = ARK::GJ::Next::GameJolt::getInstance();
					ARK::GJ::Next::API::removeRequestStatic(&as->m_requests, req);
					return;
				}

				return;

				// TODO: wide byte chars ors omething.
				ARK2D::getLog()->e("-- icon png --");
				ARK2D::getLog()->e(result);


                ARK::Graphics::ImageIO::PNGImage* png = new ARK::Graphics::ImageIO::PNGImage("");
				int ok = png->load((void*) result.c_str());
				if (ok != 0) {
					ARK2D::getLog()->e("failed");
				}
				/*as->m_callbackAddMutex->lock();
				gjCallback* cb = gjCallback_create(GJOVERLAY_USERS_ICON_RESULT, NULL);
				as->m_callbacks.push_back(cb);
				as->m_callbackAddMutex->unlock();*/

			}
			Image* GJAchievementsState::getImageForTrophy(unsigned int id) {
				if (achievementIcons.find(id) == achievementIcons.end()) {
					gjAchievement* ach = findAchievementById(id);
					if (ach->difficulty == GJ_DIFFICULTY_BRONZE) {
						return trophyBronze;
					} else if (ach->difficulty == GJ_DIFFICULTY_SILVER) {
						return trophySilver;
					} else if (ach->difficulty == GJ_DIFFICULTY_GOLD) {
						return trophyGold;
					} else if (ach->difficulty == GJ_DIFFICULTY_PLATINUM) {
						return trophyPlatinum;
					} 
					return trophyBronze;
				}
				Image* temp = achievementIcons.find(id)->second;
				return temp;
			}
			ARK::GJ::Next::gjAchievement* GJAchievementsState::findAchievementById(unsigned int id) 
			{
				for(unsigned int i = 0; i < achievements->achievementsCount; ++i) {
					if (achievements->achievements[i]->id == id) {
						return achievements->achievements[i];
					}
				}
				ARK2D::getLog()->e(StringUtil::append("could not findAchievementById: ", id));
				return NULL;
			}
			void GJAchievementsState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { 
				ARK::GJ::Next::GameJolt* gamejolt = ARK::GJ::Next::GameJolt::getInstance();
				gamejolt->api->getAchievements();

				scrollablePanel->init(0,0,container->getWidth(), container->getHeight());

			}
			void GJAchievementsState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

			unsigned int GJAchievementsState::id() {
				return ARK::GJ::Next::GameJolt::STATE_ACHIEVEMENTS;
			}
			void GJAchievementsState::init(GameContainer* container, StateBasedGame* game) {
				back = new GJButton();
				back->setText("BACK");
				back->setSize(80, 40);
				back->setEvent((void*) onBackPressed);
				back->setEventObj(this);

				scrollablePanel = new GJScrollablePanel();
				scrollablePanel->init(0, 0, container->getWidth(), container->getHeight());

				trophyBronze = Resource::get("ark2d/gamejolt-overlay/trophy-bronze.png")->asImage();
				trophySilver = Resource::get("ark2d/gamejolt-overlay/trophy-silver.png")->asImage();
				trophyGold = Resource::get("ark2d/gamejolt-overlay/trophy-gold.png")->asImage();
				trophyPlatinum = Resource::get("ark2d/gamejolt-overlay/trophy-platinum.png")->asImage();
			}
			void GJAchievementsState::onBackPressed(GJAchievementsState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_OVERVIEW);
			}
			void GJAchievementsState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				Input* in = ARK2D::getInput();
				//if (in->isKeyPressed(Input::MOBILE_BACK)) { back->doEvent(); }
				if (in->isKeyPressed(Input::MOBILE_BACK) || in->isKeyPressed(Input::KEY_BACKSPACE)) { back->doEvent(); }
				scrollablePanel->update();

				back->setLocation(20, container->getHeight() - 20 - 40);	
			}
			void GJAchievementsState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();

				//r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				//back->render();
				float offsetY = scrollablePanel->getOffsetY();

				r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				r->drawString("TROPHIES", container->getWidth()/2, offsetY + 80, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);

				r->drawLine(20.0f, offsetY + 100, container->getWidth() - 20, offsetY + 100);

				float y = offsetY + 100;
				if (achievements != NULL) {
					

					for(unsigned int i = 0; i < achievements->achievementsCount; ++i) {
						
						//
						Image* trophyImage = getImageForTrophy(achievements->achievements[i]->id);
						trophyImage->setAlpha(((achievements->achievements[i]->achieved)?1.0f:0.5f));
						trophyImage->draw(29, y + 10, 60, 60);	
						//if (achievements->achievements[i]->achieved) {
						//	r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
							//r->fillRect(29, y+10, 60, 60);		
							//r->drawRect(24, y+5, 70, 70);
						//}

						string name = string(achievements->achievements[i]->name);
						string description = string(achievements->achievements[i]->description);

						name = gjUtil_strtoupper(name);
						description = gjUtil_strtoupper(description);

						float totalHeight = 0.0f;
						totalHeight += 26;
						vector<string> lines = StringUtil::getLinesForWordWrap(description, 220);
						for(unsigned int j = 0; j < lines.size(); j++) { 
							totalHeight += 13;
						}
						float startY = y + 43 - (totalHeight/2);

						r->drawString(name, 100, startY);
						startY += 26;
						for(unsigned int j = 0; j < lines.size(); j++) { 
							r->drawString(lines[j], 100, startY, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
							startY += 13;
						}
						y += 80;

						if (i != achievements->achievementsCount - 1) {
							r->setDrawColorf(0.5f, 0.5f, 0.5f, gj->getAlpha());
							r->setLineWidth(1);
						} else {
							r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
							r->setLineWidth(2);
						}
						r->drawLine(20.0f, y, container->getWidth() - 20.0f, y);

						scrollablePanel->setHeight(y + 90 - offsetY, container->getHeight());

					}
					if (achievements->achievementsCount == 0) { 
						r->drawString("NO TROPHIES!", container->getWidth()/2, (container->getHeight() / 2), Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
					}
				} else {
					r->drawString("LOADING...", container->getWidth()/2, (container->getHeight() / 2), Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
				}	

				r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				back->render();

			}
			bool GJAchievementsState::keyPressed(unsigned int key) {
				if (back->keyPressed(key)) return true;
				if (scrollablePanel->keyPressed(key)) return true;
				return false;
			}
			bool GJAchievementsState::keyReleased(unsigned int key) {
				if (back->keyReleased(key)) return true;
				if (scrollablePanel->keyReleased(key)) return true;
				return false;
			}
			bool GJAchievementsState::mouseMoved(int x, int y, int oldx, int oldy) {
				if (back->mouseMoved(x, y, oldx, oldy)) return true;
				if (scrollablePanel->mouseMoved(x, y, oldx, oldy)) return true;
				return false;
			}
			GJAchievementsState::~GJAchievementsState() {

			}




			GJLeaderboardsState::GJLeaderboardsState(): 
				GameState(),
				back(NULL),
				scrollablePanel(NULL),
				tables(),
				timestamp(0),
				buttons() 
			{

			}

			void GJLeaderboardsState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { 
				ARK::GJ::Next::GameJolt* gamejolt = ARK::GJ::Next::GameJolt::getInstance();
				gamejolt->api->getHighscoreTables();

				scrollablePanel->init(0, 0, container->getWidth(), container->getHeight());
			}
			void GJLeaderboardsState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

			unsigned int GJLeaderboardsState::id() {
				return ARK::GJ::Next::GameJolt::STATE_LEADERBOARDS;
			}
			void GJLeaderboardsState::refreshUI() {
				// move all buttons.
				for(unsigned int i = 0; i < buttons.size(); i++) {
					ARK::UI::Button* b = buttons[i];
					delete b;
					b = NULL;
				}
				buttons.clear();

				// add buttons from tables data
				if (tables != NULL) {
					for(unsigned int i = 0; i < tables->tablesCount; i++) {
						GJButton* b = new GJButton();
						b->setText("");
						b->setSize(280, 100);
						b->setEvent((void*) onLeaderboardPressed);
						b->setEventObj(this);
						buttons.push_back(b);
					}
				}
			}
			void GJLeaderboardsState::init(GameContainer* container, StateBasedGame* game) {
				back = new GJButton();
				back->setText("BACK");
				back->setSize(80, 40);
				back->setEvent((void*) onBackPressed);
				back->setEventObj(this);

				scrollablePanel = new GJScrollablePanel();
				scrollablePanel->init(0, 0, container->getWidth(), container->getHeight());

				refreshUI();
			}
			void GJLeaderboardsState::onBackPressed(GJLeaderboardsState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_OVERVIEW);
			}
			void GJLeaderboardsState::onLeaderboardPressed(GJLeaderboardsState* ls) {
				for(unsigned int i = 0; i < ls->buttons.size(); i++) {
					if (ls->buttons[i]->isPointerOver()) {
						GJLeaderboardState* ls2 = (GJLeaderboardState*) ARK::GJ::Next::GameJolt::getInstance()->getState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARD);
						ls2->setTableId(ls->tables->tables[i]->id);
						ARK::GJ::Next::GameJolt::getInstance()->enterState(ls2);
						
						break;
					}
				}
			}
			void GJLeaderboardsState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				Input* in = ARK2D::getInput();
				//if (in->isKeyPressed(Input::MOBILE_BACK)) { back->doEvent(); }
				if (in->isKeyPressed(Input::MOBILE_BACK) || in->isKeyPressed(Input::KEY_BACKSPACE)) { back->doEvent(); }
				scrollablePanel->update();
				back->setLocation(20, container->getHeight() - 20 - 40);	
			}
			void GJLeaderboardsState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();

				
				float startY = scrollablePanel->getOffsetY();

				r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				r->drawString("LEADERBOARDS", container->getWidth()/2, startY + 80, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);

				r->setLineWidth(2);
				r->drawLine(20.0f, startY + 100, container->getWidth() - 20, startY + 100);

				if (tables != NULL) {
					float y = startY + 100;
					for(unsigned int i = 0; i < tables->tablesCount; ++i) {
						if (i < buttons.size()) {
							ARK::UI::Button* b = buttons.at(i);
							if (b != NULL) {
								b->setLocation(20, y + 10);
								b->render();
							}
						}

						r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
						
						string name = string(tables->tables[i]->name);
						string description = string(tables->tables[i]->description);

						name = gjUtil_strtoupper(name);
						description = gjUtil_strtoupper(description);

						float totalHeight = 0.0f;
						totalHeight += 30;
						vector<string> lines = StringUtil::getLinesForWordWrap(description, 280);
						for(unsigned int j = 0; j < lines.size(); j++) { 
							totalHeight += 15;
						}
						float startY = y + 62 - (totalHeight/2);

						r->drawString(name, container->getWidth()/2.0f, startY, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP);
						startY += 30;
						for(unsigned int j = 0; j < lines.size(); ++j) { 
							r->drawString(lines[j], container->getWidth()/2.0f, startY, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP, 0.0f, 0.5f);
							startY += 15;
						}

						//r->drawString(StringUtil::appendf("h: ", totalHeight), container->getWidth()/2.0f, startY + 40, Renderer::ALIGN_CENTER, Renderer::ALIGN_TOP, 0.0f, 0.5f);
						
						

						y += 120;


						if (i != tables->tablesCount - 1) {
							r->setDrawColorf(0.5f, 0.5f, 0.5f, gj->getAlpha());
							r->setLineWidth(1);
						} else {
							r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
							r->setLineWidth(2);
						}
							r->drawLine(20.0f, y, container->getWidth() - 20.0f, y);
						//}

						scrollablePanel->setHeight(y + 90 - startY, container->getHeight());

					}

					if (tables->tablesCount == 0) {
						r->drawString("NO LEADERBOARDS!", container->getWidth()/2, (container->getHeight() / 2) + 10, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
					}
				} else {
					r->drawString("LOADING...", container->getWidth()/2, (container->getHeight() / 2) + 10, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
				}

				r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
				back->render();

			}
			gjHighscoreTable* GJLeaderboardsState::findTableById(unsigned int tableId) {
				if (tables == NULL) { 
					ARK2D::getLog()->e(StringUtil::append("could not findTableById. tables not loaded?: ", tableId));
					return NULL; 
				}
				for(unsigned int i = 0; i < tables->tablesCount; ++i) {
					if (tableId == tables->tables[i]->id) {
						return tables->tables[i];
					}	
				}
				ARK2D::getLog()->e(StringUtil::append("could not findTableById: ", tableId));
				return NULL;
			}
			bool GJLeaderboardsState::keyPressed(unsigned int key) {
				if (back->keyPressed(key)) return true;
				if (scrollablePanel->keyPressed(key)) return true;
				for(unsigned int i = 0; i < buttons.size(); i++) { 
					if (buttons[i]->keyPressed(key)) return true;
				}
				return false;
			}
			bool GJLeaderboardsState::keyReleased(unsigned int key) {
				if (back->keyReleased(key)) return true;
				if (scrollablePanel->keyReleased(key)) return true;
				for(unsigned int i = 0; i < buttons.size(); i++) { 
					if (buttons[i]->keyReleased(key)) return true;
				}
				return false;
			}
			bool GJLeaderboardsState::mouseMoved(int x, int y, int oldx, int oldy) {
				if (back->mouseMoved(x, y, oldx, oldy)) return true;
				if (scrollablePanel->mouseMoved(x, y, oldx, oldy)) return true;
				for(unsigned int i = 0; i < buttons.size(); i++) { 
					if (buttons[i]->mouseMoved(x, y, oldx, oldy)) return true;
				}
				return false;
			}
			GJLeaderboardsState::~GJLeaderboardsState() {

			}






			GJLeaderboardState::GJLeaderboardState(): 
				GameState(),
				back(NULL),
				scrollablePanel(NULL),
				tableId(0),
				scores(NULL),
				timestamp(0)
				{

			}
			void GJLeaderboardState::setTableId(unsigned int id) {
				if (id != tableId) { timestamp = 0; }
				tableId = id;
			}

			void GJLeaderboardState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { 
				ARK::GJ::Next::GameJolt* gamejolt = ARK::GJ::Next::GameJolt::getInstance();
				gamejolt->api->getHighscores(tableId, 0, 100);

				scrollablePanel->init(0, 0, container->getWidth(), container->getHeight());

			}
			void GJLeaderboardState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

			unsigned int GJLeaderboardState::id() {
				return ARK::GJ::Next::GameJolt::STATE_LEADERBOARD;
			}
			void GJLeaderboardState::init(GameContainer* container, StateBasedGame* game) {
				back = new GJButton();
				back->setText("BACK");
				back->setSize(80, 40);
				back->setEvent((void*) onBackPressed);
				back->setEventObj(this);

				scrollablePanel = new GJScrollablePanel();
				scrollablePanel->init(0, 0, container->getWidth(), container->getHeight());


			/*	ARK2D::getLog()->watchVariable("m_height", WatchedVariable::TYPE_SINT, &scrollablePanel->m_height);
				ARK2D::getLog()->watchVariable("m_originalY", WatchedVariable::TYPE_SINT, &scrollablePanel->m_originalY);
				ARK2D::getLog()->watchVariable("m_scrollY", WatchedVariable::TYPE_FLOAT, &scrollablePanel->m_scrollY);
				ARK2D::getLog()->watchVariable("m_dragging", WatchedVariable::TYPE_BOOL, &scrollablePanel->m_dragging);
				ARK2D::getLog()->watchVariable("m_draggingStartY", WatchedVariable::TYPE_FLOAT, &scrollablePanel->m_draggingStartY);
				ARK2D::getLog()->watchVariable("m_innerHeight", WatchedVariable::TYPE_FLOAT, &scrollablePanel->m_innerHeight);
				ARK2D::getLog()->watchVariable("m_actualInnerHeight", WatchedVariable::TYPE_FLOAT, &scrollablePanel->m_actualInnerHeight);*/
			}
			void GJLeaderboardState::onBackPressed(GJLeaderboardState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARDS);
			}
			void GJLeaderboardState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				Input* in = ARK2D::getInput();
				if (in->isKeyPressed(Input::MOBILE_BACK) || in->isKeyPressed(Input::KEY_BACKSPACE)) { back->doEvent(); }
				scrollablePanel->update();
				back->setLocation(20, container->getHeight() - 20 - 40);	
			}
			void GJLeaderboardState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
				
				ARK::GJ::Next::GameJolt* gj = ARK::GJ::Next::GameJolt::getInstance();
				if (scores != NULL && timestamp > 0) { 


					r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
					float startY = scrollablePanel->getOffsetY();

					GJLeaderboardsState* parentState = (GJLeaderboardsState*) ARK::GJ::Next::GameJolt::getInstance()->getState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARDS);
					string tableName = string(parentState->findTableById(tableId)->name);
					tableName = gjUtil_strtoupper(tableName);

					r->drawString(tableName, container->getWidth()/2, startY + 80, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
					r->drawLine(20.0f, startY + 100.0f, container->getWidth() - 20.0f, startY + 100.0f);

					float y = startY + 100;
					for(unsigned int i = 0; i < scores->scoresCount; ++i) {
						
						string name = string( gjHighscore_getName(scores->scores[i]) );
						while (r->getFont()->getStringWidth(name) > 120) {
							name = name.substr(0, name.length() - 1);
						}
						float middle = (container->getWidth()*0.5f);//+15.0f;
						r->drawString(name, middle - 10, y + 12, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP);
						r->drawString(Cast::toString<unsigned int>(scores->scores[i]->score), middle+10, y + 12, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f );
						y += 30;

						
						if (i == scores->scoresCount - 1) { 
							r->drawLine(20.0f, y + 12, container->getWidth() - 20.0f, y + 12);
						}
					}
					scrollablePanel->setHeight(y + 90 - startY, container->getHeight());

					if (scores->scoresCount == 0) {
						r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
						r->drawString("NO HIGH SCORES!", container->getWidth()/2, (container->getHeight() / 2), Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
					}
				} else {
					r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
					r->drawString("LOADING...", container->getWidth()/2, (container->getHeight() / 2), Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
				}

				//if (container->isTouchMode() && ARK2D::getPlatform() == ARK2D::PLATFORM_IPHONE) {
				/*	r->setDrawColor("#222222", gj->getAlpha());
					r->fillRect(0, back->getMinY() - 20, container->getWidth(), back->getHeight() + 41);

					r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
					r->drawLine(0.0f, back->getMinY() - 20, float(container->getWidth()), back->getMinY() - 20);*/

					r->setDrawColorf(1.0f, 1.0f, 1.0f, gj->getAlpha());
					back->render();
				//}

				
			}
			bool GJLeaderboardState::keyPressed(unsigned int key) {
				if (back->keyPressed(key)) return true;
				if (scrollablePanel->keyPressed(key)) return true;
				return false;
			}
			bool GJLeaderboardState::keyReleased(unsigned int key) {
				if (back->keyReleased(key)) return true;
				if (scrollablePanel->keyReleased(key)) return true;
				return false;
			}
			bool GJLeaderboardState::mouseMoved(int x, int y, int oldx, int oldy) {
				if (back->mouseMoved(x, y, oldx, oldy)) return true;
				if (scrollablePanel->mouseMoved(x, y, oldx, oldy)) return true;
				return false;
			}
			GJLeaderboardState::~GJLeaderboardState() {

			}






			GJStatsState::GJStatsState(): 
				GameState(),
				back(NULL) {

			}

			void GJStatsState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { 
				ARK::GJ::Next::GameJolt* gamejolt = ARK::GJ::Next::GameJolt::getInstance();
				//gamejolt->api->getHighscores(1);

			}
			void GJStatsState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

			unsigned int GJStatsState::id() {
				return ARK::GJ::Next::GameJolt::STATE_STATS;
			}
			void GJStatsState::init(GameContainer* container, StateBasedGame* game) {
				back = new GJButton();
				back->setText("BACK");
				back->setSize(80, 40);
				back->setEvent((void*) onBackPressed);
				back->setEventObj(this);
			}
			void GJStatsState::onBackPressed(GJStatsState* ls) {
				ARK::GJ::Next::GameJolt::getInstance()->enterState(ARK::GJ::Next::GameJolt::STATE_OVERVIEW);
			}
			void GJStatsState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
				Input* in = ARK2D::getInput();
				if (in->isKeyPressed(Input::MOBILE_BACK) || in->isKeyPressed(Input::KEY_BACKSPACE)) { back->doEvent(); }
				back->setLocation(20, container->getHeight() - 20 - 40);	 
			}
			void GJStatsState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
				back->render();

				r->drawString("STATS", container->getWidth()/2, 80, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
				r->drawString("UNDER CONSTRUCTION!", container->getWidth()/2, (container->getHeight() / 2) + 10, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);

				r->drawLine(20, 100, container->getWidth() - 20, 100);

				/*float y = 100;
				for(unsigned int i = 0; i < achievements.size(); ++i) {
					r->drawLine(20.0f, y, container->getWidth() - 20.0f, y);
					r->drawRect(20, y+5, 50, 50);

					if (achievements[i]->achieved) {
						r->fillRect(25, y+10, 40, 40);			
					}

					r->drawString(achievements[i]->name, 80, y + 5);
					r->drawString(achievements[i]->description, 80, y + 35, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
					y += 60;

					r->drawLine(20.0f, y, container->getWidth() - 20.0f, y);
				}*/
				
			}
			bool GJStatsState::keyPressed(unsigned int key) {
				return back->keyPressed(key);
			}
			bool GJStatsState::keyReleased(unsigned int key) {
				return back->keyReleased(key);
			}
			bool GJStatsState::mouseMoved(int x, int y, int oldx, int oldy) {
				return back->mouseMoved(x, y, oldx, oldy);
			}
			GJStatsState::~GJStatsState() {

			}










			GameJolt* GameJolt::s_instance = NULL;
			GameJolt::GameJolt(int gameId, string gameKey):
				StateBasedGame("Game Jolt"),
				api(NULL),
				m_loggedin(false),
				m_sessionPingTimer(0.0f),
				m_sessionPingDuration(30.0f)
			{
				api = new API(gameId, gameKey);
				api->m_overlayListener = __internalGameJoltOverlayListener;

				s_instance = this;
				init(	ARK2D::getContainer());
			}
			void GameJolt::initStates(GameContainer* container) {
				Renderer::setInterpolation(Renderer::INTERPOLATION_NEAREST);
				logo = Resource::get("ark2d/gamejolt-overlay/logo-clean.png")->asImage();
				icon = Resource::get("ark2d/gamejolt-overlay/icon.png")->asImage();

				//font = Resource::get("gamejolt-overlay/Press-Start-K.fnt")->asFont()->asBMFont();
				font = Resource::get("ark2d/gamejolt-overlay/nokia.fnt")->asFont()->asBMFont();
				file = Resource::get("ark2d/gamejolt-overlay/save.kpf")->asKeyPairFile();
			
				m_openTimer = 0.0f;
				m_openDuration = 0.3f;
				m_openState = STATE_CLOSED;
				ARK2D::getLog()->watchVariable("openState", WatchedVariable::TYPE_UINT, &m_openState);

				colorGreen = new Color("#ccff00");
				colorBackground = new Color("#333333");
				
				GJLoginState* loginState = new GJLoginState();
				loginState->init(container, this);
				addState(loginState);

				GJOverviewState* overviewState = new GJOverviewState();
				overviewState->init(container, this);
				addState(overviewState);

				GJAchievementsState* achievementsState = new GJAchievementsState();
				achievementsState->init(container, this);
				addState(achievementsState);

				GJLeaderboardsState* leaderboardsState = new GJLeaderboardsState();
				leaderboardsState->init(container, this);
				addState(leaderboardsState);

				GJLeaderboardState* leaderboardState = new GJLeaderboardState();
				leaderboardState->init(container, this);
				addState(leaderboardState);

				GJStatsState* statsState = new GJStatsState();
				statsState->init(container, this);
				addState(statsState);

				enterState(loginState);

				ARK2D::getLog()->e("Auto logging in?");
				string userName = file->getString("username", "");
				string userToken = file->getString("usertoken", "");
				if (userName.length() > 0 && userToken.length() > 0) {
					string s = "Auto log-in for user '";
					s += userName;
					s += "' with token '";
					s += userToken;
					s += "'.";
					ARK2D::getLog()->v(s);
					api->authUser(userName, userToken);
				}
			}
			void GameJolt::open() {
				//m_open = true;
				if (m_openState == STATE_CLOSED) {
					m_openTimer = 0.001f;	
					m_openState = STATE_OPENING;
				}
			}
			void GameJolt::close() {
				if (m_openState == STATE_OPEN) {
					m_openTimer = 0.001f;
					m_openState = STATE_CLOSING;
				}
			}
			void GameJolt::toggle() {
				if (m_openState == STATE_OPEN) {
					close();
				} else if (m_openState == STATE_CLOSED) {
					open();
				}
			}
			void GameJolt::showAchievements() {
				if (m_loggedin) {
					enterState(STATE_ACHIEVEMENTS);
				} else {
					enterState(STATE_LOGIN);
				}
				open();
			}
			void GameJolt::showHighscoreTable() {
				if (m_loggedin) {
					enterState(STATE_LEADERBOARDS);
				} else {
					enterState(STATE_LOGIN);
				}
				open();
			}
			void GameJolt::showHighscoreTable(unsigned int id) {
				if (m_loggedin) {
					GJLeaderboardsState* ls = (GJLeaderboardsState*) getState(STATE_LEADERBOARD);
					if (ls != NULL && ls->findTableById(id) != NULL) { 
						
						GJLeaderboardState* leaderboardState = (GJLeaderboardState*) getState(ARK::GJ::Next::GameJolt::STATE_LEADERBOARD);
						leaderboardState->setTableId(id);
						enterState(leaderboardState);

						//enterState(STATE_LEADERBOARD);
					} else {
						enterState(STATE_LEADERBOARDS);
					}
				} else {
					enterState(STATE_LOGIN);
				}
				open();
			}
			void GameJolt::showStats() {
				if (m_loggedin) {
					enterState(STATE_STATS);
				} else {
					enterState(STATE_LOGIN);
				}
				open();
			}
			void GameJolt::tick() {
				update(ARK2D::getContainer(), ARK2D::getTimer());
				api->update();
			}
			void GameJolt::render() {
				if (m_openState == STATE_CLOSED) { return; }
				render(ARK2D::getContainer(), ARK2D::getRenderer());

			}
		

			void GameJolt::update(GameContainer* container, GameTimer* timer) {
				StateBasedGame::update(container, timer);

				if (m_openTimer > 0.0f) {
					m_openTimer += timer->getDelta();
					if (m_openTimer >= m_openDuration) {
						m_openTimer = 0.0f;

						if (m_openState == STATE_OPENING) {
							m_openState = STATE_OPEN;
						} else if (m_openState == STATE_CLOSING) {
							m_openState = STATE_CLOSED;
						}
					}
				}

				if (m_loggedin) {
					m_sessionPingTimer += timer->getDelta();
					if (m_sessionPingTimer >= m_sessionPingDuration) {
						m_sessionPingTimer -= m_sessionPingDuration;
						api->sessionPing(true);
					}
				}
			}
			void GameJolt::render(GameContainer* container, Renderer* g) {
                ARK::Font::Font* previousFont = g->getFont();

				g->setFont(font);

				preRender(container, g);
				StateBasedGame::render(container, g);
				postRender(container, g);

				g->setFont(previousFont);
			}
			void GameJolt::preRender(GameContainer* container, Renderer* r) {
				r->setDrawColorf(0.05f, 0.05f, 0.05f, 0.8f * getAlpha());
				r->fillRect(0, 0, container->getWidth(), container->getHeight());
			}
			void GameJolt::postRender(GameContainer* container, Renderer* r) {
				if (getCurrentStateID() == STATE_LOGIN) { return; }
				float alpha = getAlpha();
				r->setDrawColorf(colorBackground->getRedf(), colorBackground->getGreenf(), colorBackground->getBluef(), colorBackground->getAlphaf() * alpha);
				r->fillRect(0,0,container->getWidth(), 50);

				r->setDrawColor(Color::white);
				logo->setAlpha(alpha);
				logo->draw(10, 14);

				r->setDrawColorf(colorGreen->getRedf(), colorGreen->getGreenf(), colorGreen->getBluef(), colorGreen->getAlphaf() * alpha);
				r->setLineWidth(2);
				r->drawLine(0, 50, container->getWidth(), 50);;
			}
			float GameJolt::getAlpha() {
				if (m_openState == STATE_CLOSED) {
					return 0.0f;
				} else if (m_openState == STATE_OPENING) {
					return Easing::ease(Easing::QUADRATIC_OUT, m_openTimer, 0.0f, 1.0f, m_openDuration);
				} else if (m_openState == STATE_CLOSING) {
					return Easing::ease(Easing::QUADRATIC_OUT, m_openTimer, 1.0f, -1.0f, m_openDuration);
				}
				return 1.0f;
			}
			void GameJolt::resize(GameContainer* container, int width, int height) {
				StateBasedGame::resize(container, width, height);
			}
			bool GameJolt::isOpen() {
				if (m_openState != STATE_CLOSED) {
					return true;
				}
				return false;
			}
			bool GameJolt::isLoggedIn() {
				return m_loggedin;
			}

			bool GameJolt::keyPressed(unsigned int key) {
				if (m_openState == STATE_OPEN) {
					return StateBasedGame::keyPressed(key);
				}
				return false;
			}
			bool GameJolt::keyReleased(unsigned int key) {
				if (m_openState == STATE_OPEN) {
					return StateBasedGame::keyReleased(key);
				}
				return false;
			}
			bool GameJolt::mouseMoved(int x, int y, int oldx, int oldy) {
				if (m_openState == STATE_OPEN) {
					return StateBasedGame::mouseMoved(x, y, oldx, oldy);	
				}
				return false;
			}

			GameJolt::~GameJolt() {

			}

			GameJolt* GameJolt::getInstance() {
				return s_instance;
			}


		}
	}
}


