/*
 * Log.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#include "Log.h"

#include "../ARK2D.h"
#include "../Core/GameContainer.h"
#include "../Core/String.h"
#include "../Graphics/Renderer.h"
#include "../UI/Slider.h"
#include "../UI/CheckBox.h"

namespace ARK { 
	namespace Util {

		Log* Log::s_instance = NULL;

		Log* Log::getInstance() {
			if (s_instance == NULL) {
				s_instance = new Log();
			}
			return s_instance;
		}

		Log::Log():
			m_messages(),
			m_messagesPool(), 
			m_maxMessages(128),
			
			m_watchedVariables(),

			m_filter(TYPE_ALL),
			m_visible(false),
			m_enabled(true),
			m_backgroundColor(NULL),
			m_gameSpeedSlider(NULL),
			m_expoCheckbox(NULL),
			m_addGamepadButton(NULL)
			{

			for(unsigned int i = 0; i < m_maxMessages; i++) {
				LogMessage* item = new LogMessage();
				m_messagesPool.push_back(item);
			}
 			m_backgroundColor = new Color(Color::black_50a);

 			m_scene = new Scene();

			m_gameSpeedSlider = new ARK::UI::Slider();
			m_gameSpeedSlider->setSize(240, 3);
			m_gameSpeedSlider->position.set(20, 30);
			m_gameSpeedSlider->setSnapping(true, 0.05f);
			m_gameSpeedSlider->setRange(0.0f, 2.0f);
			m_gameSpeedSlider->setButtonPosition(0.5f);
			m_scene->getRoot()->addChild(m_gameSpeedSlider);

			m_expoCheckbox = new ARK::UI::CheckBox();
			m_expoCheckbox->setSize(24, 24);
			m_expoCheckbox->position.set(20, 70); 
			m_scene->getRoot()->addChild(m_expoCheckbox);

			m_addGamepadButton = new ARK::UI::Button("+ Virtual Gamepad");
			m_addGamepadButton->setSize(160, 30);
			m_addGamepadButton->position.set(20, 140); 
			m_addGamepadButton->setEvent((void*) &debug_addVirtualGamepad);
			m_scene->getRoot()->addChild(m_addGamepadButton);
		}

		#define  LOGLOGLOG(...)  __android_log_print(ANDROID_LOG_INFO,"ARK2D",__VA_ARGS__)

		#if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS_VS) 
			void WINPHONELOG(const char* ch, int len) {

				#if NTDDI_PHONE8 
					LPCSTR lps = ch;
					OutputDebugString(lps);
					OutputDebugString("\r\n");
				#elif NTDDI_WINBLUE
					

					#if defined(ARK2D_WINDOWS)
						LPCSTR lps = ch;
						OutputDebugString(lps);
						OutputDebugString("\r\n");
					#else					
						WCHAR str[1024];
						MultiByteToWideChar(CP_UTF8, 0, ch, len, str, 1024);
						str[len] = L'\0';
						LPCWSTR lps = str;
						OutputDebugString(lps);
						OutputDebugString(L"\r\n");
					#endif


					
				#endif

			}
		#endif

		void Log::setBackgroundColor(float r, float g, float b, float a) {
			m_backgroundColor->set(r, g, b, a);
		}

		void Log::setFilter(unsigned int level) {
			m_filter = level;
		}
		unsigned int Log::getFilter() {
			return m_filter;
		}

		void Log::message(string s, unsigned int type) {

			/*#if !defined(ARK2D_DEBUG)
				if (type == TYPE_VERBOSE) { 
					return;
				}
			#endif*/ 

			if (type > m_filter) {
				return; 
			}

			#if defined(ARK2D_ANDROID)
				if (s.length() < 1000) { 
					LOGLOGLOG("%s: %s", getTypeString(type).c_str(), s.c_str());
				} else {
					LOGLOGLOG("%s: %s", getTypeString(type).c_str(), "Long string: ");
					for (unsigned int i = 0; i < s.length(); i += 1024) {
						LOGLOGLOG("%s",  s.substr(i, 1024).c_str());
					}
				}
				/*if (s.length() < 1024) { 
					__android_log_print(ANDROID_LOG_INFO, "ARK2D", s.c_str());
				} else {
					__android_log_print(ANDROID_LOG_INFO, "ARK2D", "Long string: ");
					for (unsigned int i = 0; i < s.length(); i += 1024) {
						__android_log_print(ANDROID_LOG_INFO, "ARK2D", s.substr(i, 1024).c_str());
					}
				}*/
				
			#elif defined(ARK2D_FLASCC)
				/*string newStr = StringUtil::append(getTypeString(type), s);
				printf("Log ");
				printf(getTypeString(type).c_str());
				printf(": ");
				printf(s.c_str());
				printf("\n");*/
			#elif defined(ARK2D_WINDOWS_PHONE_8) 

				if (s.length() < 1023) {
					WINPHONELOG(s.c_str(), s.length());
				}
				else {
					WINPHONELOG("Long string: ", 13);
					for (unsigned int i = 0; i < s.length(); i += 1023) {
						WINPHONELOG(s.substr(i, 1023).c_str(), s.substr(i, 1023).length());
					}
				}

			#elif defined(ARK2D_WINDOWS_VS) 

				std::cout << "Log " << getTypeString(type) << ": " << s << std::endl;
				if (s.length() < 1023) {
					WINPHONELOG(s.c_str(), s.length());
				}
				else {
					WINPHONELOG("Long string: ", 13);
					for (unsigned int i = 0; i < s.length(); i += 1023) {
						WINPHONELOG(s.substr(i, 1023).c_str(), s.substr(i, 1023).length());
					}
				}

				
				
			#else
				std::cout << "Log " << getTypeString(type) << ": " << s << std::endl;
			#endif

			if (type == TYPE_THREAD) { return; }   

			if (m_messagesPool.size() == 0) {
				for(unsigned int i = 0; i < 6; i++) { 
					LogMessage* item = new LogMessage();
					m_messagesPool.push_back(item);

				}
			}

			if (s.length() < 1024) { 
				LogMessage* item = m_messagesPool.back();
				item->level = type;
				item->message = s;
				m_messages.push_back(item);
				m_messagesPool.pop_back();
			} else {
				LogMessage* item = m_messagesPool.back();
				item->level = type;
				item->message = s.substr(0, 1024);
				m_messages.push_back(item);
				m_messagesPool.pop_back();
			}

			
		}
		void Log::e(const char* s) {
			string str(s);
			message(str, TYPE_ERROR);
		}
		void Log::w(const char* s) {
			string str(s);
			message(str, TYPE_WARNING);
		}
		void Log::i(const char* s) {
			string str(s);
			message(str, TYPE_INFORMATION);
		}
		void Log::v(const char* s) {
			string str(s);
			message(str, TYPE_VERBOSE);
		} 
		void Log::g(const char* s) {
			string str(s);
			message(str, TYPE_GAME);
		}
		void Log::t(const char* s) {
			string str(s);
			message(str, TYPE_THREAD);
		}
		
		void Log::e(string s) {
			message(s, TYPE_ERROR);
		}
		void Log::w(string s) {
			message(s, TYPE_WARNING);
		}
		void Log::i(string s) {
			message(s, TYPE_INFORMATION);
		}
		void Log::v(string s) {
			message(s, TYPE_VERBOSE);
		}
		void Log::g(string s) {
			message(s, TYPE_GAME);
		}
		void Log::t(string s) {
			message(s, TYPE_THREAD);
		}

		
		void Log::e(String s) {
			message(s.get(), TYPE_ERROR);
		}
		void Log::w(String s) {
			message(s.get(), TYPE_WARNING);
		}
		void Log::i(String s) {
			message(s.get(), TYPE_INFORMATION);
		}
		void Log::v(String s) {
			message(s.get(), TYPE_VERBOSE);
		}
		void Log::g(String s) {
			message(s.get(), TYPE_GAME);
		}
		void Log::t(String s) { 
			message(s.get(), TYPE_THREAD);
		}


		void Log::watchVariable(string name, unsigned int type, void* data) {
			WatchedVariable* wv = new WatchedVariable();
			wv->name = name;
			wv->type = type;
			wv->data = data;
			m_watchedVariables.push_back(wv);
		}
		void Log::addWatchedVariable(string name, unsigned int type, void* data) {
			watchVariable(name, type, data);
		}
		void Log::clearWatchedVariables() {
			for(unsigned int i = 0; i < m_watchedVariables.size(); ++i) {
				WatchedVariable* wv = m_watchedVariables[i];
				delete wv;
				wv = NULL; 
			}
			m_watchedVariables.clear();
		}
		
		void Log::update() {

			//#if defined(ARK2D_DEBUG) 
			//	return;
			//#endif

			#if defined(ARK2D_FLASCC)
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_SPACE) && i->isKeyDown(Input::KEY_9)) {
					m_visible = !m_visible;
				} 
			#elif defined(ARK2D_WINDOWS)
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT) && i->isKeyDown(Input::KEY_LCONTROL)) {
					m_visible = !m_visible;
				}
			#else 
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT)) {
					m_visible = !m_visible;
				}
			#endif

			if (m_visible) {
				ARK2D::getContainer()->setCursorVisible(true); 
			}

			m_scene->update();
		}	
		void Log::render(GameContainer* container, Renderer* r) {

			// remove old items
			if (m_messages.size() > m_maxMessages) {
				int removeCount = m_messages.size() - m_maxMessages;
				for(; removeCount > 0; removeCount--) {
					LogMessage* item = m_messages.front();
					m_messages.pop_front();
					m_messagesPool.push_back(item);
				}
			}

			if (!m_visible) {
				return;
			}

			//GameContainer* container = ARK2D::getContainer();
			//Renderer* g = ARK2D::getRenderer();
			ARK::Font::Font* oldFont = r->getFont();
			const Color& oldColor = r->getDrawColor();

			r->setDrawColor(*m_backgroundColor);
			r->fillRect(container->getWidth()-150, 0, 150, 150);
			r->fillRect(0, 0, 350, 200);
			//r->fillRect(0, 0, container->getWidth(), container->getHeight());

			ARK::Font::Font* defaultFont = r->getDefaultFont();
			if (defaultFont == NULL) {
				defaultFont = oldFont;
			} 

			r->setDrawColor(Color::white);
			r->setFont(defaultFont);

			// Adjust game speed
			r->drawString(StringUtil::appendf("Game Speed: ", m_gameSpeedSlider->getActualValue()), 10, 10, Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
			m_gameSpeedSlider->updateValue();
			//m_gameSpeedSlider->render();
			ARK2D::getTimer()->setDeltaModifier(m_gameSpeedSlider->getActualValue());

			r->setDrawColor(Color::white);
			r->drawString("EXPO Mode:", 10, 60, Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
			//m_expoCheckbox->render();
			ARK2D::setExpoMode(m_expoCheckbox->isChecked());

			// draw watched variables
			r->drawString("Watched Variables:", 10, 100, Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
			for(unsigned int i = 0; i < m_watchedVariables.size(); ++i) {
				WatchedVariable* wv = m_watchedVariables[i];
				string displayName = wv->name;
				displayName += ": ";
				if (wv->type == WatchedVariable::TYPE_FLOAT) {
					float* d = (float*) wv->data;
					r->drawString(StringUtil::appendf(displayName, *d), 10, 110 + (10*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
				} else if (wv->type == WatchedVariable::TYPE_UINT) {
					unsigned int* d = (unsigned int*) wv->data;
					r->drawString(StringUtil::append(displayName, *d), 10, 110 + (10*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
				} else if (wv->type == WatchedVariable::TYPE_SINT) {
					signed int* d = (signed int*) wv->data;
					r->drawString(StringUtil::append(displayName, *d), 10, 110 + (10*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
				} else if (wv->type == WatchedVariable::TYPE_BOOL) {
					bool* d = (bool*) wv->data;
					r->drawString(StringUtil::append(displayName, Cast::boolToString(*d)), 10, 110 + (10*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
				} else if (wv->type == WatchedVariable::TYPE_STR) {
					string* d = (string*) wv->data;
					displayName += *d;
					r->drawString(displayName, 10, 110 + (10*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
				}
			}
			
			// draw renderer stats
			r->setDrawColor(Color::white);
			unsigned int rendererGLCalls = RendererStats::s_glCalls;
			unsigned int rendererLines = RendererStats::s_lines;
			unsigned int rendererTris = RendererStats::s_tris;
			unsigned int rendererTextureSwaps = RendererStats::s_textureSwaps;
			unsigned int rendererShaderSwaps = RendererStats::s_shaderSwaps;
			float rendererTextureMemory = (float(RendererStats::s_textureAllocatedBytes) / 1024.0f) / 1024.0f;
			r->drawString(StringUtil::append("FPS: ", ARK2D::getTimer()->getFPS()), container->getWidth() - 10, 10, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			
			r->drawString(StringUtil::append("Log Size: ", m_messagesPool.size()), container->getWidth() - 10, 30, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);

			r->drawString(StringUtil::append("OpenGL Calls: ", rendererGLCalls), container->getWidth() - 10, 50, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			r->drawString(StringUtil::append("Lines: ", rendererLines), container->getWidth() - 10, 60, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			r->drawString(StringUtil::append("Tris: ", rendererTris), container->getWidth() - 10, 70, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			r->drawString(StringUtil::append("Texture Swaps: ", rendererTextureSwaps), container->getWidth() - 10, 80, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			r->drawString(StringUtil::append("Shader Swaps: ", rendererShaderSwaps), container->getWidth() - 10, 90, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			r->drawString(StringUtil::appendf("Texture Memory (MB): ", rendererTextureMemory), container->getWidth() - 10, 100, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			 
 			// container widths and heights...
 			int cy = 120;
 			//r->drawString(StringUtil::append("FPS: ", ARK2D::getTimer()->getFPS()), container->getWidth() - 10, 10, Renderer::ALIGN_RIGHT, Renderer::ALIGN_TOP, 0.0f, 0.5f);
			
			 
 
			// render log.
			/*list<LogMessage*>::iterator it;
			int actualHeight = 0;
			for(it = m_messages.begin(); it != m_messages.end(); it++) {
				actualHeight += defaultFont->getLineHeight() * 0.5f;
			}
			int y = -10;
			if (actualHeight > (signed int) container->getHeight()) { y -= (actualHeight - container->getHeight()); }
			for(it = m_messages.begin(); it != m_messages.end(); it++) {
				LogMessage* m = (*it);
				r->drawString(m->message, 10, y, Renderer::ALIGN_LEFT, Renderer::ALIGN_LEFT, 0.0f, 0.5f);
				y += defaultFont->getLineHeight() * 0.5f;
			}*/

			r->setDrawColor(Color::white);
			r->setFont(defaultFont);
			m_scene->render();


			r->setDrawColor(oldColor);
			r->setFont(oldFont);
		}

		void Log::keyPressed(unsigned int key) {
			if (!m_visible) { return; }
			//m_gameSpeedSlider->keyPressed(key);
			//m_expoCheckbox->keyPressed(key);
			m_scene->keyPressed(key);
		}
		void Log::keyReleased(unsigned int key) {
			if (!m_visible) { return; }
			//m_gameSpeedSlider->keyReleased(key);
			//m_expoCheckbox->keyReleased(key);
			m_scene->keyReleased(key);
		}
		void Log::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!m_visible) { return; }
			//m_gameSpeedSlider->mouseMoved(x, y, oldx, oldy);
			//m_expoCheckbox->mouseMoved(x, y, oldx, oldy);
			m_scene->mouseMoved(x, y, oldx, oldy);
		}

   		wstring Log::getTypeWString(unsigned int type) {
            return StringUtil::stringToWstring(getTypeString(type));
        }
		string Log::getTypeString(unsigned int type) {
			#if defined(ARK2D_ANDROID)
				switch(type) {
					case ANDROID_LOG_INFO:
						return "INFO";
						break;
				}
				return "";
			#else
				switch(type) {
					case TYPE_ERROR:
						return "ERROR";
						break;
					case TYPE_WARNING:
						return "WARNING";
						break;
					case TYPE_INFORMATION:
						return "INFO";
						break;
					case TYPE_VERBOSE:
						return "VERBOSE";
						break;
					case TYPE_THREAD:
						return "THREAD";
						break;
					case TYPE_GAME:
						return "GAME";
						break;
				}
				return "";
			#endif
		}

		Log::~Log() {

		}


		GPAxisButton::GPAxisButton(string text): 
			GPButton(text),
			downX(0.0f),
			downY(0.0f),
			axisValueX(0.0f),
			axisValueY(0.0f) {

		}
		GPButton::GPButton(string text): 
			Button(text) {

		}

		void debugGamepadButtonPress(GPButton* butt) { 
			Gamepad* pad = ARK2D::getInput()->getGamepad(butt->gpid);
			pad->pressButton(butt->gpbid); 
		}
		void debugGamepadClose(GPButton* butt) {
			bool closed = false;
			unsigned int gamepadId = butt->gpid;
			vector<Gamepad* >* pads = ARK2D::getInput()->getGamepads();
			Gamepad* pad = NULL;
			for(unsigned int i = 0; i < pads->size(); i++) { 
				if (pads->at(i)->id == gamepadId) { 
					pad = pads->at(i);
					closed = true;

					pads->erase(pads->begin() + i);
					break;
				}
			}

			if (!closed) {
				ARK2D::getLog()->e("Could not remove virtual gamepad.");
				return;
			}

			// send event to game.
			Game* g = ARK2D::getGame();
			GamepadListener* gl = NULL;
			gl = dynamic_cast<GamepadListener*>(g);
			if (gl != NULL) {
				gl->gamepadDisconnected(pad);
			}

			// Remove the panel from the debug scene
		    Scene* scene = ARK2D::getLog()->getScene();
			SceneNode* panel = scene->find(pad->getName());
		    if (panel != NULL) {
		        scene->removeChild(panel);
		    }

			delete pad;
			pad = NULL;
		}

		TitledPanel::TitledPanel(): 
			Panel(), 
			m_title("") {

		}
		void TitledPanel::setTitle(string title) {
			m_title = title;
		}
		void TitledPanel::render() {
			Renderer* r = ARK2D::getRenderer();
			r->setDrawColor(Color::black_50a);
			r->fillRect(0, 0, m_width, m_height);

			r->setDrawColor(Color::white);
			r->drawLine(0, 20, m_width, 20);
			
			r->drawString(m_title, 1, 1, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP, 0.0f, 0.5f);

			renderChildren();

			Panel::renderBorder();
		}
		void GPAxisButton::render() {
			//GPButton::render();

			renderBackground();
			renderText(0,0); 
			renderOverlay();

			Renderer* r = ARK2D::getRenderer();
			r->setDrawColor(Color::white);
		    r->fillCircle((m_width*0.5) + (axisValueX * m_width*0.5), (m_height*0.5) + (axisValueY * m_height*0.5), 5, 10);

			renderChildren();
		} 
		void GPAxisButton::renderText(int x, int y) {
			//Renderer* r = ARK2D::getRenderer();
			//r->setDrawColor(Color::white);
			//r->drawString(m_text.get(), x + (m_width * 0.5f), y + (m_height * 0.5f) );

		}
		void GPAxisButton::renderBackground() {
			Renderer* r = ARK2D::getRenderer();
			r->setDrawColor(Color::black_50a);
			r->fillCircle(m_width * 0.5f, m_height * 0.5f, (m_width + m_height)*0.25, 20);
		}
		void GPAxisButton::renderOverlay() {
			Renderer* r = ARK2D::getRenderer();
			r->setDrawColor(Color::white);
			if (m_state == STATE_OVER || m_state == STATE_DOWN) {
				r->setDrawColor(Color::white_50a);
			}
			r->drawCircle(m_width * 0.5f, m_height * 0.5f, (m_width + m_height)*0.25, 20);
		}
		bool GPAxisButton::keyPressed(unsigned int key) {
			bool didEvent = GPButton::keyPressed(key);
			if (didEvent) {
				downX = ARK2D::getInput()->getMouseX();
				downY = ARK2D::getInput()->getMouseY();
				return true;
			}
			return false;
		}
		bool GPAxisButton::keyReleased(unsigned int key) {


			downX = -1.0f;
			downY = -1.0f;
			axisValueX = 0.0f;
			axisValueY = 0.0f; 

			Gamepad* pad = ARK2D::getInput()->getGamepad(this->gpid);
		    pad->moveAxis(axisIdX, axisValueX);
			pad->moveAxis(axisIdY, axisValueY);
				
			bool didEvent = GPButton::keyReleased(key);
			if (didEvent) {
				return true;
			}
			return false;
		}
		bool GPAxisButton::mouseMoved(int x, int y, int oldx, int oldy) {
			bool didEvent = GPButton::mouseMoved(x, y, oldx, oldy);
			if (didEvent && m_state == STATE_DOWN) { 
				
		        float angle = MathUtil::angle(downX, downY, x, y);
		        float dist = MathUtil::distance(downX, downY, x, y);
		        if (dist > m_width * 0.5) { dist = m_width * 0.5; }

		        axisValueX = 0.0f;
				axisValueY = 0.0f;
		        MathUtil::moveAngle<float>(axisValueX, axisValueY, angle, dist / float(m_width*0.5f)); 
		         
		        Gamepad* pad = ARK2D::getInput()->getGamepad(this->gpid);
		        pad->moveAxis(axisIdX, axisValueX);
		        pad->moveAxis(axisIdY, axisValueY);
		        return true;
			} else {
				axisValueX = 0.0f;
				axisValueY = 0.0f;
			}
			return false;
		}

		void debug_addVirtualGamepad() {
			GameContainer* container = ARK2D::getContainer(); 
			Gamepad* gamepad = new Gamepad();
			gamepad->id = ARK2D::getInput()->countGamepads();
			gamepad->name = StringUtil::append("Virtual Gamepad ", gamepad->id+1);

			// DPAD
			GamepadButton* dpad_button = new GamepadButton();
			dpad_button->id = Gamepad::DPAD_UP;
			dpad_button->down = false;  
			gamepad->buttons.push_back(dpad_button);

			dpad_button = new GamepadButton();
			dpad_button->id = Gamepad::DPAD_DOWN;
			dpad_button->down = false;  
			gamepad->buttons.push_back(dpad_button);

			dpad_button = new GamepadButton();
			dpad_button->id = Gamepad::DPAD_LEFT;
			dpad_button->down = false;  
			gamepad->buttons.push_back(dpad_button);

			dpad_button = new GamepadButton();
			dpad_button->id = Gamepad::DPAD_RIGHT;
			dpad_button->down = false;  
			gamepad->buttons.push_back(dpad_button);

			// Face Buttons
			GamepadButton* a_button = new GamepadButton();
			a_button->id = Gamepad::BUTTON_A;
			a_button->down = false;  
			gamepad->buttons.push_back(a_button);

			GamepadButton* b_button = new GamepadButton();
			b_button->id = Gamepad::BUTTON_B;
			b_button->down = false;  
			gamepad->buttons.push_back(b_button);

			GamepadButton* x_button = new GamepadButton();
			x_button->id = Gamepad::BUTTON_X;
			x_button->down = false;  
			gamepad->buttons.push_back(x_button);

			GamepadButton* y_button = new GamepadButton();
			y_button->id = Gamepad::BUTTON_Y;
			y_button->down = false;  
			gamepad->buttons.push_back(y_button);

			GamepadButton* lb_button = new GamepadButton();
			lb_button->id = Gamepad::BUTTON_LBUMPER;
			lb_button->down = false;  
			gamepad->buttons.push_back(lb_button);

			GamepadButton* rb_button = new GamepadButton();
			rb_button->id = Gamepad::BUTTON_RBUMPER;
			rb_button->down = false;  
			gamepad->buttons.push_back(rb_button);

			GamepadButton* back_button = new GamepadButton();
			back_button->id = Gamepad::BUTTON_BACK;
			back_button->down = false;  
			gamepad->buttons.push_back(back_button);

			GamepadButton* start_button = new GamepadButton();
			start_button->id = Gamepad::BUTTON_START;
			start_button->down = false;  
			gamepad->buttons.push_back(start_button);

			GamepadButton* l3_button = new GamepadButton();
			l3_button->id = Gamepad::BUTTON_L3;
			l3_button->down = false;  
			gamepad->buttons.push_back(l3_button);

			GamepadButton* r3_button = new GamepadButton();
			r3_button->id = Gamepad::BUTTON_R3;
			r3_button->down = false;  
			gamepad->buttons.push_back(r3_button);

			GamepadButton* activate_button = new GamepadButton();
			activate_button->id = Gamepad::BUTTON_R3;
			activate_button->down = false;  
			gamepad->buttons.push_back(activate_button);

			GamepadButton* lt_button = new GamepadButton();
			lt_button->id = Gamepad::BUTTON_LTRIGGER;
			lt_button->down = false;  
			gamepad->buttons.push_back(lt_button);

			GamepadButton* rt_button = new GamepadButton();
			rt_button->id = Gamepad::BUTTON_RTRIGGER;
			rt_button->down = false;  
			gamepad->buttons.push_back(rt_button);

			// Axes
			GamepadAxis* xAxis = new GamepadAxis();
			xAxis->id = Gamepad::ANALOG_STICK_1_X;
			gamepad->axes.push_back(xAxis);

			GamepadAxis* yAxis = new GamepadAxis(); 
			yAxis->id = Gamepad::ANALOG_STICK_1_Y;
			gamepad->axes.push_back(yAxis);
			 
			GamepadAxis* rAxis = new GamepadAxis();
			rAxis->id = Gamepad::ANALOG_STICK_2_X;
			gamepad->axes.push_back(rAxis);
			
			GamepadAxis* uAxis = new GamepadAxis();
			uAxis->id = Gamepad::ANALOG_STICK_2_Y;
			gamepad->axes.push_back(uAxis);

			// Triggers
			GamepadAxis* zAxis = new GamepadAxis();
			zAxis->id = Gamepad::TRIGGER_1;
			gamepad->axes.push_back(zAxis);

			GamepadAxis* vAxis = new GamepadAxis(); // rtrigger
			vAxis->id = Gamepad::TRIGGER_2;
			gamepad->axes.push_back(vAxis);

			gamepad->numAxes = gamepad->axes.size(); 

			ARK2D::getLog()->i(gamepad->toString());
			container->getGamepads()->push_back(gamepad);

			// Give event to game
			Game* g = ARK2D::getGame();
			GamepadListener* gl = NULL;
			gl = dynamic_cast<GamepadListener*>(g);
			if (gl != NULL) {
				gl->gamepadConnected(gamepad);
			}

			float panelX = 500 + ((gamepad->id%3)*300);
			float panelY = 120 + (floor(gamepad->id/3)*240);

			SceneNode* root = ARK2D::getLog()->getScene()->getRoot();
				TitledPanel* p = new TitledPanel();
				p->setName(gamepad->name);
				p->setTitle(gamepad->name);
				p->setSize(300,240);
				p->position.set(panelX, panelY);
				p->scale.set(1.0f, 1.0f);
				p->pivot.set(0.5f, 0.5f);

					// BACK, START
					GPButton* backButton = new GPButton("back");
					backButton->gpid = gamepad->id;
					backButton->gpbid = Gamepad::BUTTON_BACK;
					backButton->setEvent((void*) &debugGamepadButtonPress);
					backButton->setEventObj((void*) backButton);
					backButton->setSize(30, 30);
					backButton->position.set(-5.0f, 10.0f);
					backButton->pivot.set(1.0, 0.5);
					p->addChild(backButton);

					GPButton* startButton = new GPButton("start");
					startButton->gpid = gamepad->id;
					startButton->gpbid = Gamepad::BUTTON_START;
					startButton->setEvent((void*) &debugGamepadButtonPress);
					startButton->setEventObj((void*) startButton);
					startButton->setSize(30, 30);
					startButton->position.set(5, 10);
					startButton->pivot.set(0.0, 0.5);
					p->addChild(startButton);

					// DPAD
					GPButton* dpadButtonLeft = new GPButton("left");
					dpadButtonLeft->gpid = gamepad->id;
					dpadButtonLeft->gpbid = Gamepad::DPAD_LEFT;
					dpadButtonLeft->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonLeft->setEventObj((void*) dpadButtonLeft);
					dpadButtonLeft->setSize(30, 30);
					dpadButtonLeft->position.set(-100, 60);
					dpadButtonLeft->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonLeft);

					GPButton* dpadButtonRight = new GPButton("right");
					dpadButtonRight->gpid = gamepad->id;
					dpadButtonRight->gpbid = Gamepad::DPAD_RIGHT;
					dpadButtonRight->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonRight->setEventObj((void*) dpadButtonRight);
					dpadButtonRight->setSize(30, 30);
					dpadButtonRight->position.set(-50, 60);
					dpadButtonRight->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonRight);

					GPButton* dpadButtonUp = new GPButton("up");
					dpadButtonUp->gpid = gamepad->id;
					dpadButtonUp->gpbid = Gamepad::DPAD_UP;
					dpadButtonUp->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonUp->setEventObj((void*) dpadButtonUp);
					dpadButtonUp->setSize(30, 30);
					dpadButtonUp->position.set(-75, 35);
					dpadButtonUp->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonUp);

					GPButton* dpadButtonDown = new GPButton("down");
					dpadButtonDown->gpid = gamepad->id;
					dpadButtonDown->gpbid = Gamepad::DPAD_DOWN;
					dpadButtonDown->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonDown->setEventObj((void*) dpadButtonDown);
					dpadButtonDown->setSize(30, 30);
					dpadButtonDown->position.set(-75, 85);
					dpadButtonDown->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonDown);

					// ABXY
					GPButton* buttonB = new GPButton("B");
					buttonB->gpid = gamepad->id;
					buttonB->gpbid = Gamepad::BUTTON_B;
					buttonB->setEvent((void*) &debugGamepadButtonPress);
					buttonB->setEventObj((void*) buttonB);
					buttonB->setSize(30, 30);
					buttonB->position.set(100, 60);
					buttonB->pivot.set(0.5, 0.5);
					p->addChild(buttonB);

					GPButton* buttonX = new GPButton("X");
					buttonX->gpid = gamepad->id;
					buttonX->gpbid = Gamepad::BUTTON_X;
					buttonX->setEvent((void*) &debugGamepadButtonPress);
					buttonX->setEventObj((void*) buttonX);
					buttonX->setSize(30, 30);
					buttonX->position.set(50, 60);
					buttonX->pivot.set(0.5, 0.5);
					p->addChild(buttonX);

					GPButton* buttonY = new GPButton("Y");
					buttonY->gpid = gamepad->id;
					buttonY->gpbid = Gamepad::BUTTON_Y;
					buttonY->setEvent((void*) &debugGamepadButtonPress);
					buttonY->setEventObj((void*) buttonY);
					buttonY->setSize(30, 30);
					buttonY->position.set(75, 35);
					buttonY->pivot.set(0.5, 0.5);
					p->addChild(buttonY);

					GPButton* buttonA = new GPButton("A");
					buttonA->gpid = gamepad->id;
					buttonA->gpbid = Gamepad::BUTTON_A;
					buttonA->setEvent((void*) &debugGamepadButtonPress);
					buttonA->setEventObj((void*) buttonA);
					buttonA->setSize(30, 30);
					buttonA->position.set(75, 85);
					buttonA->pivot.set(0.5, 0.5);
					p->addChild(buttonA);

					// Shoulder buttons
					GPButton* leftBumperButton = new GPButton("LB");
					leftBumperButton->gpid = gamepad->id;
					leftBumperButton->gpbid = Gamepad::BUTTON_LBUMPER;
					leftBumperButton->setEvent((void*) &debugGamepadButtonPress);
					leftBumperButton->setEventObj((void*) leftBumperButton);
					leftBumperButton->setSize(60, 25);
					leftBumperButton->position.set(-110.0f, -70.0f);
					leftBumperButton->pivot.set(0.5, 0.5);
					p->addChild(leftBumperButton);

					GPButton* rightBumperButton = new GPButton("RB");
					rightBumperButton->gpid = gamepad->id;
					rightBumperButton->gpbid = Gamepad::BUTTON_RBUMPER;
					rightBumperButton->setEvent((void*) &debugGamepadButtonPress);
					rightBumperButton->setEventObj((void*) rightBumperButton);
					rightBumperButton->setSize(60, 25);
					rightBumperButton->position.set(110, -70);
					rightBumperButton->pivot.set(0.5, 0.5);
					p->addChild(rightBumperButton);

					// L & R Axes
					GPAxisButton* axisLeft = new GPAxisButton("L");
					axisLeft->gpid = gamepad->id;
					axisLeft->axisIdX = Gamepad::ANALOG_STICK_1_X;
					axisLeft->axisIdY = Gamepad::ANALOG_STICK_1_Y;
					axisLeft->setSize(55, 55);
					axisLeft->position.set(-100, -20);
					axisLeft->pivot.set(0.5, 0.5);
					p->addChild(axisLeft);

					GPAxisButton* axisRight = new GPAxisButton("R");
					axisRight->gpid = gamepad->id;
					axisRight->axisIdX = Gamepad::ANALOG_STICK_2_X;
					axisRight->axisIdY = Gamepad::ANALOG_STICK_2_Y;
					axisRight->setSize(55, 55);
					axisRight->position.set(100, -20);
					axisRight->pivot.set(0.5, 0.5);
					p->addChild(axisRight);

					// Remove gamepad button
					GPButton* closeButton = new GPButton("X");
					closeButton->gpid = gamepad->id;
					closeButton->setEvent((void*) &debugGamepadClose);
					closeButton->setEventObj((void*) closeButton);
					closeButton->setSize(40, 40);
					closeButton->position.set(p->getWidth()*0.5f, p->getHeight()*-0.5f);
					closeButton->scale.set(0.5f, 0.5f);
					closeButton->pivot.set(1.0, 0.0);
					p->addChild(closeButton);
		 


			root->addChild(p);


		}

	}
}
