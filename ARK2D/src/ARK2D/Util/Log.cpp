/*
 * Log.cpp
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#include "Log.h"

#include "../ARK2D.h"
#include "../Core/String.h"
#include "../Core/GameContainer.h"
#include "../Graphics/Renderer.h"
#include "../UI/Slider.h"
#include "../UI/CheckBox.h"
#include "../UI/Label.h"
#include "../UI/ComboBox.h"
#include "../UI/ComboBoxItem.h"
#include "../Util/Strings.h"
#include "../UI/SplitPane.h"
#include "../UI/TitledPanel.h"

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
			SceneNode("log"),
			m_messages(),
			m_messagesPool(),
			m_maxMessages(256),

			m_watchedVariables(),

			m_filter(TYPE_ALL),
			m_enabled(true),
			m_backgroundColor(NULL),
			m_gameSpeedSlider(NULL),
			m_expoCheckbox(NULL),
			m_addGamepadButton(NULL),

			m_logToFile(false)
			{
				this->visible = false;


		}

		void debugLanguageChanged(ComboBox* box) {
		    //wstring val = StringUtil::stringToWstring( box->getSelected()->getValue() );
		    string val = box->getSelected()->getValue();
			ARK2D::getStrings()->setLanguage( Strings::textLanguageToConstLanguage(val) );
			//ARK2D::getStrings()->print();
		}

		void Log::onResize() {
			GameContainer* container = ARK2D::getContainer();
			setBounds(container->getWidth(), container->getHeight(), 0);

			SceneNode* node = getChildByName("split_horizontal");
			node->onResize();
		}

		void Log::init() {
			for(unsigned int i = 0; i < m_maxMessages; i++) {
				LogMessage* item = new LogMessage();
				m_messagesPool.push_back(item);
			}
 			m_backgroundColor = new Color(Color::black_50a);

 			//m_scene = new Scene();


 			GameContainer* container = ARK2D::getContainer();
 			setBounds(container->getWidth(), container->getHeight(), 0);

 			SplitPane* splitHorizontal = new SplitPane(SplitPane::SPLIT_HORIZONTAL);
 			splitHorizontal->setName("split_horizontal");
 			splitHorizontal->setSplitLocation(0.2f);




 				SplitPane* splitLeft = new SplitPane(SplitPane::SPLIT_VERTICAL);
 				splitLeft->setName("leftpanel_split_vertical");
 				splitLeft->setSplitLocation(0.5f);

 					TitledPanel* splitLeftTop = new TitledPanel();
 					splitLeftTop->setTitle("Debug");
 					splitLeftTop->setName("left_panel");

		                ARK::UI::Label* gameSpeedLabel = new ARK::UI::Label("Game Speed:", -1, 0);
		                gameSpeedLabel->transform.position.set(10, 20);
		   				splitLeftTop->addChild(gameSpeedLabel);

						m_gameSpeedSlider = new ARK::UI::Slider();
						m_gameSpeedSlider->setName("game_speed_slider");
						m_gameSpeedSlider->setSize(160, 3);
						m_gameSpeedSlider->transform.position.set(70, 20);
						m_gameSpeedSlider->setSnapping(true, 0.05f);
						m_gameSpeedSlider->setRange(0.0f, 2.0f);
						m_gameSpeedSlider->setButtonPosition(0.5f);
						splitLeftTop->addChild(m_gameSpeedSlider);

						ARK::UI::Label* expoModeLabel = new ARK::UI::Label("Expo Mode:", -1, 0);
		                expoModeLabel->transform.position.set(10, 60);
		                splitLeftTop->addChild(expoModeLabel);

						m_expoCheckbox = new ARK::UI::CheckBox();
						m_expoCheckbox->setName("expo_mode_checkbox");
						m_expoCheckbox->setSize(24, 24);
						m_expoCheckbox->transform.position.set(70, 60);
						splitLeftTop->addChild(m_expoCheckbox);

						ARK::UI::Label* virtualPadLabel = new ARK::UI::Label("Virtual Pad:", -1, 0);
		                virtualPadLabel->transform.position.set(10, 120);
		                splitLeftTop->addChild(virtualPadLabel);

						m_addGamepadButton = new ARK::UI::Button("+");
						m_expoCheckbox->setName("add_virtual_gamepad_button");
						m_addGamepadButton->setSize(30, 30);
						m_addGamepadButton->transform.position.set(70, 120);
						m_addGamepadButton->setEvent((void*) &debug_addVirtualGamepad);
						splitLeftTop->addChild(m_addGamepadButton);

						ARK::UI::Label* languageLabel = new ARK::UI::Label("Language:", -1, 0);
		                languageLabel->transform.position.set(10, 180);
		                splitLeftTop->addChild(languageLabel);

						ARK::UI::ComboBox* languageCombobox = new ARK::UI::ComboBox();
						languageCombobox->setName("language arena");
						languageCombobox->transform.position.set(70, 180);
						languageCombobox->setSize(160, 30);
						languageCombobox->setItemChangedEvent((void*) debugLanguageChanged);
						languageCombobox->addItem(new ComboBoxItem("English (UK)", "english_uk"));
						languageCombobox->addItem(new ComboBoxItem("English (US)", "english_us"));
						languageCombobox->addItem(new ComboBoxItem("French", "french_fr"));
						languageCombobox->addItem(new ComboBoxItem("Italian", "italian"));
						languageCombobox->addItem(new ComboBoxItem("German", "german"));
						languageCombobox->addItem(new ComboBoxItem("Spanish", "spanish"));
						languageCombobox->addItem(new ComboBoxItem("Swedish", "swedish"));
						languageCombobox->addItem(new ComboBoxItem("Hebrew", "hebrew"));
						//languageCombobox->addItem(new ComboBoxItem("Arabic", "arabic"));
						splitLeftTop->addChild(languageCombobox);

					TitledPanel* splitLeftBottomTitled = new TitledPanel(new LogSceneTreePanel(), "Scene Tree");






				splitLeft->setLeftComponent(splitLeftTop);
 				splitLeft->setRightComponent(splitLeftBottomTitled);
 				addChild(splitLeft);

			SplitPane* rightPanel = new SplitPane(SplitPane::SPLIT_VERTICAL);
			rightPanel->setName("right_panel_split_vertical");
			rightPanel->setSplitLocation(0.8f);

				TitledPanel* watchedVariables = new TitledPanel(new LogWatchedVariablesPanel(), "Watched Variables");
				TitledPanel* rendererStats = new TitledPanel(new LogRendererStatsPanel(), "Renderer Stats");
				SplitPane* bottomSplitRight = new SplitPane(SplitPane::SPLIT_HORIZONTAL, watchedVariables, rendererStats, 0.5f);

				SplitPane* bottomSplitLeft = new SplitPane(
												SplitPane::SPLIT_HORIZONTAL,
												new TitledPanel(new LogConsolePanel(), "Console"),
												bottomSplitRight,
												0.70f
											);



				rightPanel->setRightComponent(bottomSplitLeft);

			splitHorizontal->setLeftComponent(splitLeft);
 			splitHorizontal->setRightComponent(rightPanel);
			addChild(splitHorizontal);
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

			LogMessage* item = m_messagesPool.at(m_messagesPool.size()-1);
			if (item == NULL) {
				item = new LogMessage();
				m_messagesPool.push_back(item);
			}
			item->level = type;
			if (s.length() < 1024) {
				item->message = s;
			} else {
				item->message = s.substr(0, 1024);
			}
			m_messages.push_back(item);
			m_messagesPool.pop_back();

			if (isLoggingToFile()) {
				std::ofstream log_file("log.txt", std::ios_base::out | std::ios_base::app );
				log_file << "Log " << getTypeString(type) << ": " << s.c_str() << std::endl;
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

		void Log::backtrace() {
			#ifdef ARK2D_MACINTOSH
				e("--- BACKTRACE ---");

				void* callstack[128];
                int i, frames = ::backtrace(callstack, 128);
				char** strs = backtrace_symbols(callstack, frames);
				for (i = 0; i < frames; ++i) {
					printf("%s\n", strs[i]);
				}
				free(strs);

				//e("--- BACKTRACE SYMBOLS ---");
				//backtrace_symbols();
			#endif
		}

		void Log::watchVariable(string name, unsigned int type, void* data) {
			// check the memory location/variable is not being watched already!
			for(unsigned int i = 0; i < m_watchedVariables.size(); i++) {
				if (m_watchedVariables[i]->data == data) {
					ARK2D::getLog()->w(StringUtil::append("Variable is already being watched: ", name));
					return;
				}
			}

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
			SceneNode::update();

			//#if defined(ARK2D_DEBUG)
			//	return;
			//#endif

			#if defined(ARK2D_FLASCC)
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_SPACE) && i->isKeyDown(Input::KEY_9)) {
					this->visible = !this->visible;
				}
			#elif defined(ARK2D_WINDOWS)
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT) && i->isKeyDown(Input::KEY_LCONTROL)) {
					this->visible = !this->visible;
				}
			#else
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT)) {
					this->visible = !this->visible;
				}
			#endif

			if (this->visible) {
				ARK2D::getContainer()->setCursorVisible(true);
			}

			//m_scene->update();
		}
		void Log::render() {
			GameContainer* container = ARK2D::getContainer();
			Renderer* r = ARK2D::getRenderer();



			// remove old items
			if (m_messages.size() > m_maxMessages) {
				int removeCount = m_messages.size() - m_maxMessages;
				for(; removeCount > 0; removeCount--) {
					LogMessage* item = m_messages.front();
					m_messages.pop_front();
					m_messagesPool.push_back(item);
				}
			}

			if (this->visible) {
				Vector3<float> pos = find("right_panel_split_vertical")->localPositionToGlobalPosition();
				Game* g = ARK2D::getGame();
				ARK::Geometry::Cube* bounds = g->getBounds();


				g->transform.position.set(pos.getX(), 0, 0);

				float scaleX = std::min(1.0f, (container->getWidth() - pos.getX()) / float(container->getWidth()));
				float uniformScale = scaleX;
				g->transform.scale.set(uniformScale, uniformScale, uniformScale);
			} else {
				Game* g = ARK2D::getGame();
				g->transform.position.set(0, 0, 0);
				g->transform.scale.set(1.0f, 1.0f, 1.0f);
			}

			if (!this->visible) {
				return;
			}

			//r->disableMultisampling();

			//GameContainer* container = ARK2D::getContainer();
			//Renderer* g = ARK2D::getRenderer();
			//ARK::Font::Font* oldFont = r->getFont();
			//const Color& oldColor = r->getDrawColor();

			r->setDrawColor(*m_backgroundColor);
			//r->fillRect(container->getWidth()-150, 0, 150, 150);
			//r->fillRect(0, 0, 350, 200);
			//r->fillRect(0, 0, container->getWidth(), container->getHeight());

			ARK::Font::Font* defaultFont = r->getDefaultFont();
			if (defaultFont == NULL) {
				e("cannot display log. no default font loaded.");
				return;
				//defaultFont = oldFont;
			}

			r->setDrawColor(Color::white);
			r->setFont(defaultFont);

			// Adjust game speed
//			defaultFont->asBMFont()->drawString(StringUtil::appendf("Game Speed: ", m_gameSpeedSlider->getActualValue()), 10, 10, Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
			m_gameSpeedSlider->updateValue();
			//m_gameSpeedSlider->render();
			ARK2D::getTimer()->setDeltaModifier(m_gameSpeedSlider->getActualValue());

			r->setDrawColor(Color::white);

			//m_expoCheckbox->render();
			ARK2D::setExpoMode(m_expoCheckbox->isChecked());

			// draw renderer stats

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

			renderChildren();

			r->setDrawColor(Color::white);
			//r->setFont(defaultFont);
			//m_scene->render();

			//r->enableMultisampling();

			//r->setDrawColor(oldColor);
			//r->setFont(oldFont);
		}

		bool Log::keyPressed(unsigned int key) {
			if (!this->visible) { return false; }
			return SceneNode::keyPressed(key);
		}
		bool Log::keyReleased(unsigned int key) {
			if (!this->visible) { return false; }
			return SceneNode::keyReleased(key);
		}
		bool Log::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!this->visible) { return false; }
			return SceneNode::mouseMoved(x, y, oldx, oldy);
		}

		bool Log::isLoggingToFile() {
			return m_logToFile;
		}
		void Log::setLoggingToFile(bool b) {
			m_logToFile = b;
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
			SceneNode* panel = ARK2D::getLog()->find(pad->getName());
		    if (panel != NULL) {
		        ARK2D::getLog()->removeChild(panel);
		    }

			delete pad;
			pad = NULL;
		}



		LogSceneTreePanel::LogSceneTreePanel():
            ScrollPanel(),
            tree() {
            setName("scene_tree");

            addChild(&tree);
			//setTitle("Console");
		}
		void LogSceneTreePanel::render() {
			// Panel::render();

			return;

			Scene* scene = ARK2D::getContainer()->scene;
            tree.setText(scene->getRoot()->toListString());
            calculateSize();

			ScrollPanel::render();

			preRenderFromPivot();

			Log* l = ARK2D::getLog();
			Renderer* r = ARK2D::getRenderer();
			ARK::Geometry::Cube* bounds = getBounds();

			r->enableStencil();
			r->startStencil();
			r->fillRect(0,0,bounds->getWidth(), bounds->getHeight());
			r->stopStencil();



           	r->disableStencil();

			Panel::renderBorder();

			postRenderFromPivot();



			renderChildren();
		}

		LogConsolePanel::LogConsolePanel():
            Panel() {
            setName("console");
			//setTitle("Console");
		}
		void LogConsolePanel::render() {
			// Panel::render();

			preRenderFromPivot();

			Log* l = ARK2D::getLog();
			Renderer* r = ARK2D::getRenderer();
			ARK::Geometry::Cube* bounds = getBounds();

			r->enableStencil();
			r->startStencil();
			r->fillRect(0,0,bounds->getWidth(), bounds->getHeight());
			r->stopStencil();

            unsigned int i = 0;
            float totalH = 0;
            list<LogMessage*>::reverse_iterator it = l->m_messages.rbegin();
            while (it != l->m_messages.rend()) {
				r->drawString((*it)->message, 10.0f, bounds->getHeight() - (i*12.0f), Renderer::ALIGN_LEFT, Renderer::ALIGN_BOTTOM);
                it++;
                i++;
                totalH += 12.0f;
                if (i >= 10 || totalH > bounds->getHeight()) { break; }
			}

			r->disableStencil();

			Panel::renderBorder();

			postRenderFromPivot();
		}

		LogWatchedVariablesPanel::LogWatchedVariablesPanel():
            Panel() {
            setName("watchedVariables");
			//setTitle("Watched Variables");
		}
		void LogWatchedVariablesPanel::render() {
			//TitledPanel::render();

			Log* l = ARK2D::getLog();
			Renderer* r = ARK2D::getRenderer();

			preRenderFromPivot();

            for(unsigned int i = 0; i < l->m_watchedVariables.size(); ++i) {
				WatchedVariable* wv = l->m_watchedVariables[i];
				string displayName = wv->name;
				displayName += ": ";
				if (wv->type == WatchedVariable::TYPE_FLOAT) {
					float* d = (float*) wv->data;
					r->drawString(StringUtil::appendf(displayName, *d), 10, 0 + (12*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				} else if (wv->type == WatchedVariable::TYPE_UINT) {
					unsigned int* d = (unsigned int*) wv->data;
					r->drawString(StringUtil::append(displayName, *d), 10, 0 + (12*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				} else if (wv->type == WatchedVariable::TYPE_SINT) {
					signed int* d = (signed int*) wv->data;
					r->drawString(StringUtil::append(displayName, *d), 10, 0 + (12*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				} else if (wv->type == WatchedVariable::TYPE_BOOL) {
					bool* d = (bool*) wv->data;
					r->drawString(StringUtil::append(displayName, Cast::boolToString(*d)), 10, 0 + (12*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				} else if (wv->type == WatchedVariable::TYPE_STR) {
					string* d = (string*) wv->data;
					displayName += *d;
					r->drawString(displayName, 10, 0 + (12*i), Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
				}
			}

			Panel::renderBorder();

			postRenderFromPivot();
		}

		LogRendererStatsPanel::LogRendererStatsPanel():
            Panel() {
            setName("rendererStats");
			//setTitle("Renderer Stats");
		}
		void LogRendererStatsPanel::render() {
			//TitledPanel::render();

			Log* l = ARK2D::getLog();
			Renderer* r = ARK2D::getRenderer();

			preRenderFromPivot();

            float textX = 10;
            r->setDrawColor(Color::white);
			unsigned int rendererGLCalls = RendererStats::s_previousframe_glCalls;
			unsigned int rendererLines = RendererStats::s_previousframe_lines;
			unsigned int rendererTris = RendererStats::s_previousframe_tris;
			unsigned int rendererTextureSwaps = RendererStats::s_previousframe_textureSwaps;
			unsigned int rendererShaderSwaps = RendererStats::s_previousframe_shaderSwaps;
			float rendererTextureMemory = (float(RendererStats::s_textureAllocatedBytes) / 1024.0f) / 1024.0f;
			r->drawString(StringUtil::append("FPS: ", ARK2D::getTimer()->getFPS()), textX, 0, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

			r->drawString(StringUtil::append("Log Size: ", l->m_messagesPool.size()), textX, 10, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

			r->drawString(StringUtil::append("OpenGL Calls: ", rendererGLCalls), textX, 30, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::append("Lines: ", rendererLines), textX, 40, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::append("Tris: ", rendererTris), textX, 50, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::append("Texture Swaps: ", rendererTextureSwaps), textX, 60, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::append("Shader Swaps: ", rendererShaderSwaps), textX, 70, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);
			r->drawString(StringUtil::appendf("Texture Memory (MB): ", rendererTextureMemory), textX, 80, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

			Panel::renderBorder();

			postRenderFromPivot();

		}

		void GPAxisButton::render() {
			//GPButton::render();

			preRenderFromPivot();

			renderBackground();
			renderText(0,0);
			renderOverlay();

			Renderer* r = ARK2D::getRenderer();
			r->setDrawColor(Color::white);
		    r->fillCircle((m_width*0.5) + (axisValueX * m_width*0.5), (m_height*0.5) + (axisValueY * m_height*0.5), 5, 10);

		    postRenderFromPivot();

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
				Vector3<float> pos = globalPositionToLocalPosition(ARK2D::getInput()->getMouseX(), ARK2D::getInput()->getMouseY(), 0.0f);
				downX = pos.getX();
				downY = pos.getY();
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

				Vector3<float> pos = globalPositionToLocalPosition(x, y, 0.0f);
		        float angle = MathUtil::angle(downX, downY, pos.getX(), pos.getY());
		        float dist = MathUtil::distance(downX, downY, pos.getX(), pos.getY());
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
			gamepad->virtualpad = true;

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
			float panelY = 120 + (floor(double(gamepad->id)/3)*240);

            SceneNode* root = ARK2D::getLog();//->getScene()->getRoot();

				Panel* p = new Panel();

					// BACK, START
					GPButton* backButton = new GPButton("back");
					backButton->gpid = gamepad->id;
					backButton->gpbid = Gamepad::BUTTON_BACK;
					backButton->setEvent((void*) &debugGamepadButtonPress);
					backButton->setEventObj((void*) backButton);
					backButton->setSize(30, 30);
					backButton->transform.position.set(-5.0f, 0.0f);
					backButton->pivot.set(1.0, 0.5);
					p->addChild(backButton);

					GPButton* startButton = new GPButton("start");
					startButton->gpid = gamepad->id;
					startButton->gpbid = Gamepad::BUTTON_START;
					startButton->setEvent((void*) &debugGamepadButtonPress);
					startButton->setEventObj((void*) startButton);
					startButton->setSize(30, 30);
					startButton->transform.position.set(5, 0);
					startButton->pivot.set(0.0, 0.5);
					p->addChild(startButton);

					// DPAD
					GPButton* dpadButtonLeft = new GPButton("left");
					dpadButtonLeft->gpid = gamepad->id;
					dpadButtonLeft->gpbid = Gamepad::DPAD_LEFT;
					dpadButtonLeft->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonLeft->setEventObj((void*) dpadButtonLeft);
					dpadButtonLeft->setSize(30, 30);
					dpadButtonLeft->transform.position.set(-100, 50);
					dpadButtonLeft->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonLeft);

					GPButton* dpadButtonRight = new GPButton("right");
					dpadButtonRight->gpid = gamepad->id;
					dpadButtonRight->gpbid = Gamepad::DPAD_RIGHT;
					dpadButtonRight->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonRight->setEventObj((void*) dpadButtonRight);
					dpadButtonRight->setSize(30, 30);
					dpadButtonRight->transform.position.set(-50, 50);
					dpadButtonRight->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonRight);

					GPButton* dpadButtonUp = new GPButton("up");
					dpadButtonUp->gpid = gamepad->id;
					dpadButtonUp->gpbid = Gamepad::DPAD_UP;
					dpadButtonUp->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonUp->setEventObj((void*) dpadButtonUp);
					dpadButtonUp->setSize(30, 30);
					dpadButtonUp->transform.position.set(-75, 25);
					dpadButtonUp->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonUp);

					GPButton* dpadButtonDown = new GPButton("down");
					dpadButtonDown->gpid = gamepad->id;
					dpadButtonDown->gpbid = Gamepad::DPAD_DOWN;
					dpadButtonDown->setEvent((void*) &debugGamepadButtonPress);
					dpadButtonDown->setEventObj((void*) dpadButtonDown);
					dpadButtonDown->setSize(30, 30);
					dpadButtonDown->transform.position.set(-75, 75);
					dpadButtonDown->pivot.set(0.5, 0.5);
					p->addChild(dpadButtonDown);

					// ABXY
					GPButton* buttonB = new GPButton("B");
					buttonB->gpid = gamepad->id;
					buttonB->gpbid = Gamepad::BUTTON_B;
					buttonB->setEvent((void*) &debugGamepadButtonPress);
					buttonB->setEventObj((void*) buttonB);
					buttonB->setSize(30, 30);
					buttonB->transform.position.set(100, 50);
					buttonB->pivot.set(0.5, 0.5);
					p->addChild(buttonB);

					GPButton* buttonX = new GPButton("X");
					buttonX->gpid = gamepad->id;
					buttonX->gpbid = Gamepad::BUTTON_X;
					buttonX->setEvent((void*) &debugGamepadButtonPress);
					buttonX->setEventObj((void*) buttonX);
					buttonX->setSize(30, 30);
					buttonX->transform.position.set(50, 50);
					buttonX->pivot.set(0.5, 0.5);
					p->addChild(buttonX);

					GPButton* buttonY = new GPButton("Y");
					buttonY->gpid = gamepad->id;
					buttonY->gpbid = Gamepad::BUTTON_Y;
					buttonY->setEvent((void*) &debugGamepadButtonPress);
					buttonY->setEventObj((void*) buttonY);
					buttonY->setSize(30, 30);
					buttonY->transform.position.set(75, 25);
					buttonY->pivot.set(0.5, 0.5);
					p->addChild(buttonY);

					GPButton* buttonA = new GPButton("A");
					buttonA->gpid = gamepad->id;
					buttonA->gpbid = Gamepad::BUTTON_A;
					buttonA->setEvent((void*) &debugGamepadButtonPress);
					buttonA->setEventObj((void*) buttonA);
					buttonA->setSize(30, 30);
					buttonA->transform.position.set(75, 75);
					buttonA->pivot.set(0.5, 0.5);
					p->addChild(buttonA);

					// Shoulder buttons
					GPButton* leftBumperButton = new GPButton("LB");
					leftBumperButton->gpid = gamepad->id;
					leftBumperButton->gpbid = Gamepad::BUTTON_LBUMPER;
					leftBumperButton->setEvent((void*) &debugGamepadButtonPress);
					leftBumperButton->setEventObj((void*) leftBumperButton);
					leftBumperButton->setSize(60, 25);
					leftBumperButton->transform.position.set(-110.0f, -80.0f);
					leftBumperButton->pivot.set(0.5, 0.5);
					p->addChild(leftBumperButton);

					GPButton* rightBumperButton = new GPButton("RB");
					rightBumperButton->gpid = gamepad->id;
					rightBumperButton->gpbid = Gamepad::BUTTON_RBUMPER;
					rightBumperButton->setEvent((void*) &debugGamepadButtonPress);
					rightBumperButton->setEventObj((void*) rightBumperButton);
					rightBumperButton->setSize(60, 25);
					rightBumperButton->transform.position.set(110, -80);
					rightBumperButton->pivot.set(0.5, 0.5);
					p->addChild(rightBumperButton);

					// L & R Axes
					GPAxisButton* axisLeft = new GPAxisButton("L");
					axisLeft->gpid = gamepad->id;
					axisLeft->axisIdX = Gamepad::ANALOG_STICK_1_X;
					axisLeft->axisIdY = Gamepad::ANALOG_STICK_1_Y;
					axisLeft->setSize(55, 55);
					axisLeft->transform.position.set(-100, -30);
					axisLeft->pivot.set(0.5, 0.5);
					p->addChild(axisLeft);

					GPAxisButton* axisRight = new GPAxisButton("R");
					axisRight->gpid = gamepad->id;
					axisRight->axisIdX = Gamepad::ANALOG_STICK_2_X;
					axisRight->axisIdY = Gamepad::ANALOG_STICK_2_Y;
					axisRight->setSize(55, 55);
					axisRight->transform.position.set(100, -30);
					axisRight->pivot.set(0.5, 0.5);
					p->addChild(axisRight);

					// Remove gamepad button
					GPButton* closeButton = new GPButton("X");
					closeButton->gpid = gamepad->id;
					closeButton->setEvent((void*) &debugGamepadClose);
					closeButton->setEventObj((void*) closeButton);
					closeButton->setSize(40, 40);
					//closeButton->position.set(0.0f, -20.0f);
					closeButton->transform.scale.set(0.5f, 0.5f);
					closeButton->pivot.set(1.0, 0.0);
					p->addChild(closeButton);

				TitledPanel* pw = new TitledPanel(p, gamepad->name);
				pw->pivot.set(0.5, 0.5, 0.0);
                pw->setName(gamepad->name);
                pw->setBounds(300,240,0);
				pw->transform.position.set(panelX, panelY);

				closeButton->transform.position.set(pw->getWidth()*0.5f, -130.0f);

			root->addChild(pw);


		}

	}
}
