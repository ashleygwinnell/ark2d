
#include "GameContainer.h"
#include "GameContainerLinux.h"

#ifdef ARK2D_UBUNTU_LINUX

	GLint GameContainerPlatform::att[5] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	GameContainer::GameContainer(Game& game, int width, int height, int bpp, bool fullscreen):
		m_timer(),
		m_game(game),
		m_input(),
		m_graphics(),
		m_gamepads(),
		m_width(width),
		m_height(height),
		m_bpp(bpp),
		m_fullscreen(fullscreen),
		m_clearColor(Color::black),
		m_resizeBehaviour(RESIZE_BEHAVIOUR_SCALE),
		m_platformSpecific()
		{
		m_input.setGameContainer(this);

		// NULL means the current computer.
		m_platformSpecific.dpy = XOpenDisplay(NULL);
		if (m_platformSpecific.dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(0);
		}

		// Root window is "desktop background" window.
		m_platformSpecific.root = DefaultRootWindow(m_platformSpecific.dpy);

		// Pick a display mode: this can fail, so type glxinfo into the command line for detail.
		m_platformSpecific.vi = glXChooseVisual(m_platformSpecific.dpy, 0, m_platformSpecific.att);
		if (m_platformSpecific.vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(0);
		} else {
			printf("\n\tvisual %p selected\n", (void*) m_platformSpecific.vi->visualid);
		} /* %p creates hexadecimal output like in glxinfo */

		// make a colourmap for the window.
		m_platformSpecific.cmap = XCreateColormap(m_platformSpecific.dpy, m_platformSpecific.root, m_platformSpecific.vi->visual, AllocNone);

		// something else
		m_platformSpecific.swa.colormap = m_platformSpecific.cmap;
		m_platformSpecific.swa.event_mask = ExposureMask | KeyPressMask; // respond to these events...

		// create the window
		m_platformSpecific.win =
			XCreateWindow(
					m_platformSpecific.dpy,
					m_platformSpecific.root,
					0, 0, // initial x,y pos
					width, height, 0, // w, h, border...
					m_platformSpecific.vi->depth, InputOutput,
					m_platformSpecific.vi->visual, CWColormap | CWEventMask, &m_platformSpecific.swa);

		// make the window appear...
		XMapWindow(m_platformSpecific.dpy, m_platformSpecific.win);

		// title bar name
		XStoreName(m_platformSpecific.dpy, m_platformSpecific.win, "VERY SIMPLE APPLICATION");

		// create an opengl context and bind it to the window
		m_platformSpecific.glc = glXCreateContext(m_platformSpecific.dpy, m_platformSpecific.vi, NULL, GL_TRUE); // GL_TRUE for rendering enabled.
		glXMakeCurrent(m_platformSpecific.dpy, m_platformSpecific.win, m_platformSpecific.glc);

		// tssk. start game.
		glEnable(GL_DEPTH_TEST);

		while(1) {
			XNextEvent(m_platformSpecific.dpy, &m_platformSpecific.xev);
			if (m_platformSpecific.xev.type == Expose) {
				XGetWindowAttributes(m_platformSpecific.dpy, m_platformSpecific.win, &m_platformSpecific.gwa);
				glViewport(0, 0, m_platformSpecific.gwa.width, m_platformSpecific.gwa.height);
				//DrawAQuad();
				glXSwapBuffers(m_platformSpecific.dpy, m_platformSpecific.win);
			} else if(m_platformSpecific.xev.type == KeyPress) {
				glXMakeCurrent(m_platformSpecific.dpy, None, NULL);
				glXDestroyContext(m_platformSpecific.dpy, m_platformSpecific.glc);
				XDestroyWindow(m_platformSpecific.dpy, m_platformSpecific.win);
				XCloseDisplay(m_platformSpecific.dpy);
				exit(0);
			}
		}
	}

	void GameContainer::setIcon(const string& icon) {

	}

	void GameContainer::setCursorVisible(bool b) {

	}

	void GameContainer::start() {

	}

	void GameContainer::close() const {

	}

	void GameContainerPlatform::setTitle(std::string title) {

	}


	int GameContainer::getGlobalMouseX() const {
		return 0;
	}
	int GameContainer::getGlobalMouseY() const {
		return 0;
	}

#endif
