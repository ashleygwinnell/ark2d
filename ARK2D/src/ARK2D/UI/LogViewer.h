/*
 * LogViewer.h
 *
 *  Created on: Oct 27, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARK_UI_LOGVIEWER_H_
#define ARK_UI_LOGVIEWER_H_

#include "../Namespaces.h"

#include "../Core/ARK2D.h"
#include "../Core/String.h"
#include "Button.h"
#include "TitledPanel.h"
#include "ScrollPanel.h"
#include "Label.h"

#include <list>
using namespace std;

#undef TYPE_BOOL

namespace ARK {
	namespace UI {

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
		class ARK2D_API LogViewer : public SceneNode {
			friend class ARK::Core::GameContainer;
			friend class ARK2D;
			friend class LogConsolePanel;
			friend class LogWatchedVariablesPanel;
			friend class LogRendererStatsPanel;

			protected:

				bool m_enabled;

				Color* m_backgroundColor;
				ARK::UI::Slider* m_gameSpeedSlider;
				ARK::UI::CheckBox* m_expoCheckbox;
				ARK::UI::Button* m_addGamepadButton;
				LogConsolePanel* m_consolePanel;

			public:
				LogViewer();

				void setBackgroundColor(float r, float g, float b, float a);

				virtual void init();
				virtual void update();
				virtual void render();

				virtual bool keyPressed(unsigned int key);
				virtual bool keyReleased(unsigned int key);
				virtual bool mouseMoved(int x, int y, int oldx, int oldy);

				virtual void onResize();

				inline bool isVisible() { return visible; }
				inline bool isEnabled() { return visible; }

				virtual ~LogViewer();
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
