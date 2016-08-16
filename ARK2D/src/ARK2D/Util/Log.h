/*
 * Log.h
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARKLOG_H_
#define ARKLOG_H_

#include "../Core/ARK2D.h"
//#ifndef ARK2D_ANDROID
//	#include <string>
//	#include <list>
//#endif

//struct Graphics;
#include "../Namespaces.h"
#include "../Core/String.h"
#include "../UI/Button.h"
#include "../UI/TitledPanel.h"
#include "../UI/ScrollPanel.h"
#include "../UI/Label.h"

#include <list>


using namespace std;

#undef TYPE_BOOL

namespace ARK {
	namespace Util {

		class ARK2D_API LogMessage {
			public:
				string message;
				unsigned int level;
				LogMessage(): message(""), level(0) {

				}
		};

		class ARK2D_API WatchedVariable {
			public:
				string name;
				unsigned int type;
				void* data;

			public:
				static const unsigned int TYPE_UINT = 0;
				static const unsigned int TYPE_SINT = 1;
				static const unsigned int TYPE_FLOAT = 2;
				static const unsigned int TYPE_STR = 3;
				static const unsigned int TYPE_BOOL = 4;
		};

        class LogConsolePanel;
        class LogWatchedVariablesPanel;
        class LogRendererStatsPanel;

		/*!
		 * \brief Standard framework logging - contains UI element for log viewing and debugging.
		 *
		 * This class can be extended and overridden to replace the standard log.
		 * Use ARK2D::getLog();
		 *
		 * @see ARK2D
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Log : public SceneNode {
			friend class ARK::Core::GameContainer;
			friend class ARK2D;
			friend class LogConsolePanel;
			friend class LogWatchedVariablesPanel;
			friend class LogRendererStatsPanel;

			public:
				static Log* s_instance;
				static Log* getInstance();

			public:
				static const unsigned int TYPE_GAME = 4;
				static const unsigned int TYPE_VERBOSE = 3;
				static const unsigned int TYPE_INFORMATION = 2;
				static const unsigned int TYPE_WARNING = 1;
				static const unsigned int TYPE_ERROR = 0;
				static const unsigned int TYPE_THREAD = 5;
				static const unsigned int TYPE_ALL = 99999;

			protected:
				// logging
				list<LogMessage*> m_messages;
				vector<LogMessage*> m_messagesPool;
				unsigned int m_maxMessages;

				// watched variables
				vector<WatchedVariable*> m_watchedVariables;

				unsigned int m_filter;
				bool m_enabled;

				Color* m_backgroundColor;
				ARK::UI::Slider* m_gameSpeedSlider;
				ARK::UI::CheckBox* m_expoCheckbox;
				ARK::UI::Button* m_addGamepadButton;
				LogConsolePanel* m_consolePanel;

				bool m_logToFile;

			public:
				Log();
				void init(); // call this just before game.init
				void message(string s, unsigned int type);

				void e(const char* s);
				void w(const char* s);
				void i(const char* s);
				void v(const char* s);
				void g(const char* s);
				void t(const char* s);

				void e(string s);
				void w(string s);
				void i(string s);
				void v(string s);
				void g(string s);
				void t(string s);

				void e(String s);
				void w(String s);
				void i(String s);
				void v(String s);
				void g(String s);
				void t(String s);

				void backtrace();

				void watchVariable(string name, unsigned int type, void* data); // could templateise this?
				void addWatchedVariable(string name, unsigned int type, void* data); // could templateise this?
				void clearWatchedVariables();

				void setBackgroundColor(float r, float g, float b, float a);

				// only shows log statements lower than this value.
				void setFilter(unsigned int level);
				unsigned int getFilter();

				virtual void update();
				virtual void render();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void onResize();

				inline bool isVisible() { return visible; }
				inline bool isEnabled() { return visible; }

				bool isLoggingToFile();
				void setLoggingToFile(bool b);

				virtual ~Log();

			protected:
				string getTypeString(unsigned int type);
				wstring getTypeWString(unsigned int type);
		};

		void debug_addVirtualGamepad();



		class ARK2D_API LogConsolePanel : public Panel {
			public:
				LogConsolePanel();
				virtual void render();
		};
		class ARK2D_API LogSceneTreePanel : public ScrollPanel {
			public:
				LogSceneTreePanel();
                ARK::UI::Label tree;
				virtual void render();
		};
		class ARK2D_API LogWatchedVariablesPanel : public Panel {
			public:
				LogWatchedVariablesPanel();
				virtual void render();
		};
		class ARK2D_API LogRendererStatsPanel : public Panel {
			public:
				LogRendererStatsPanel();
				virtual void render();
		};
		class ARK2D_API GPButton : public ARK::UI::Button {
			public:
                GPButton(string text);
				unsigned int gpid;
				unsigned int gpbid;
		};
		class ARK2D_API GPAxisButton : public GPButton {
			public:
				unsigned int axisIdX;
				unsigned int axisIdY;
				float downX;
				float downY;
				float axisValueX;
				float axisValueY;
				GPAxisButton(string text);
				virtual void render();
				virtual void renderBackground();
				virtual void renderText(int x, int y);
				virtual void renderOverlay();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);
		};


	}
}

#endif /* ARKLOG_H_ */
