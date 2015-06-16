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
			m_expoCheckbox(NULL)
			{

			for(unsigned int i = 0; i < m_maxMessages; i++) {
				LogMessage* item = new LogMessage();
				m_messagesPool.push_back(item);
			}
 			m_backgroundColor = new Color(Color::black_50a);

 			m_scene = new Scene();

			m_gameSpeedSlider = new ARK::UI::Slider();
			m_gameSpeedSlider->setSize(240, 3);
			m_gameSpeedSlider->setLocation(20, 30);
			m_gameSpeedSlider->setSnapping(true, 0.05f);
			m_gameSpeedSlider->setRange(0.0f, 2.0f);
			m_gameSpeedSlider->setButtonPosition(0.5f);

			m_expoCheckbox = new ARK::UI::CheckBox();
			m_expoCheckbox->setSize(24, 24);
			m_expoCheckbox->setLocation(20, 70); 
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
			//#elif defined(ARK2D_WINDOWS)
			//	Input* i = ARK2D::getInput();
			//	if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_A)) {
			//		m_visible = !m_visible;
			//	}
			#else 
				Input* i = ARK2D::getInput();
				if (i->isKeyPressed(Input::KEY_D) && i->isKeyDown(Input::KEY_LSHIFT)) {
					m_visible = !m_visible;
				}
			#endif
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
			m_gameSpeedSlider->render();
			ARK2D::getTimer()->setDeltaModifier(m_gameSpeedSlider->getActualValue());

			r->setDrawColor(Color::white);
			r->drawString("EXPO Mode:", 10, 60, Renderer::ALIGN_LEFT, Renderer::ALIGN_CENTER, 0.0f, 0.5f);
			m_expoCheckbox->render();
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
			m_gameSpeedSlider->keyPressed(key);
			m_expoCheckbox->keyPressed(key);
			m_scene->keyPressed(key);
		}
		void Log::keyReleased(unsigned int key) {
			if (!m_visible) { return; }
			m_gameSpeedSlider->keyReleased(key);
			m_expoCheckbox->keyReleased(key);
			m_scene->keyReleased(key);
		}
		void Log::mouseMoved(int x, int y, int oldx, int oldy) {
			if (!m_visible) { return; }
			m_gameSpeedSlider->mouseMoved(x, y, oldx, oldy);
			m_expoCheckbox->mouseMoved(x, y, oldx, oldy);
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

	}
}
