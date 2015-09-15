/*
 * VideoGameState.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: ashleygwinnell
 */

#include "VideoGameState.h"
#include "../Core/GameObject.h"
#include "../Util/Containers/Vector.h"
#include "../Util/Containers/Pool.h"

#ifdef ARK2D_MACINTOSH

	@implementation QTMovieView (TFOverrideDrag)

	- (void)mouseDown:(NSEvent *)theEvent
	{
	    //[self.superview becomeFirstResponder];
	    NSLog(@"MDown");
	    ARK2D::getInput()->pressKey(Input::MOUSE_BUTTON_LEFT);
	} 

	- (void)mouseDragged:(NSEvent *)theEvent
	{ 
	    //[self.superview becomeFirstResponder];
	    NSLog(@"MDrag");
	}

	- (void)rightMouseDown:(NSEvent *)theEvent
	{
	    //[self.superview becomeFirstResponder];
	    NSLog(@"RMouse");
	} 
	- (void)keyDown:(NSEvent *)theEvent
	{
	   // [self.nextResponder keyDown:theEvent];
		[ARK2D::getContainer()->getPlatformSpecific()->m_listener keyDown:theEvent]; //  ARK2D::getInput()->pressKey();
	    NSLog(@"keyDown");
	}
	- (void)keyUp:(NSEvent *)theEvent
	{
		[ARK2D::getContainer()->getPlatformSpecific()->m_listener keyUp:theEvent]; //  ARK2D::getInput()->pressKey();
	    NSLog(@"keyUp");
	}

	@end

#endif


namespace ARK {
	namespace State {

		VideoGameState::VideoGameState(): 
			GameState(),
			m_source(""),
			m_loaded(false) 
			#if defined(ARK2D_MACINTOSH)
				,
				m_movie(NULL),
				m_movieView(NULL)
				
			#endif
		{

		}

 
		void VideoGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {
			GameState::enter(container, game, from);

			if (!m_loaded) { 
				load();
			}

		 	//SoundStore::getInstance()->setVolumeByGroupId(0, 0.0f); 

			#if defined(ARK2D_MACINTOSH)
				[NSOpenGLContext clearCurrentContext];
				[ARK2D::getContainer()->getPlatformSpecific()->m_window setContentView:m_movieView];
			#endif
			

		}
		void VideoGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {
			GameState::leave(container, game, to);

			stop();	

			#if defined(ARK2D_MACINTOSH)
	  
				NSView* view = [ARK2D::getContainer()->getPlatformSpecific()->m_context view];
				[ARK2D::getContainer()->getPlatformSpecific()->m_window setContentView: view];
	 
				ARK2D::getContainer()->getPlatformSpecific()->makeContextCurrent(
					ARK2D::getContainer()->getPlatformSpecific()->m_window, 
					ARK2D::getContainer()->getPlatformSpecific()->m_context
				); 

				[ARK2D::getContainer()->getPlatformSpecific()->m_window setDelegate: ARK2D::getContainer()->getPlatformSpecific()->m_listener];

			#endif
		}

		bool VideoGameState::keyPressed(unsigned int key) {
			return GameState::keyPressed(key);
		}
		bool VideoGameState::keyReleased(unsigned int key) {
			return GameState::keyReleased(key);
		}
		bool VideoGameState::mouseMoved(int x, int y, int oldx, int oldy) {
			return GameState::mouseMoved(x, y, oldx, oldy);
		}

		void VideoGameState::setSource(string s) {
			m_source = s;
		}
		string VideoGameState::getSource() {
			return m_source;
		}

		void VideoGameState::init(GameContainer* container, StateBasedGame* game) {

		}
		void VideoGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

		}
		void VideoGameState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
			#if defined(ARK2D_WINDOWS)
				r->setDrawColor(Color::white);
				r->drawString("Video playback is currently unavailable on Windows platforms.", container->getWidth()/2, container->getWidth()/2, Renderer::ALIGN_CENTER, Renderer::ALIGN_CENTER);
				return;
			#endif
		}

		void VideoGameState::load() {

			#if defined(ARK2D_MACINTOSH)

				//GameContainerIPhoneGLViewController* glview = ARK2D::getContainer()->getPlatformSpecific()->m_appDelegate.glViewController;
				//glview.view = m_movieView;

				string filec = ARK2D::getContainer()->getResourcePath() + m_source;
				ARK2D::getLog()->v(StringUtil::append("movie: ", filec));
				NSString* fileName = [NSString stringWithCString:filec.c_str() encoding:[NSString defaultCStringEncoding]];;
				NSError* tempError;

				//[fileName autorelease ];

				bool cando = [QTMovie canInitWithFile:fileName];
				if (cando) { 
					ARK2D::getLog()->v("Loading movie");
					m_movie = [QTMovie movieWithFile:fileName error:&tempError];
				} else {
					ARK2D::getLog()->v("Couldn't load movie"); 
				}

				NSRect windowFrame = [ARK2D::getContainer()->getPlatformSpecific()->m_window frame];
				NSRect contentRect;
					    	contentRect = [NSWindow contentRectForFrameRect: windowFrame
													styleMask: NSTitledWindowMask];
				//CGRect frame = [[NSScreen mainScreen] bounds];
				m_movieView = [[QTMovieView alloc] initWithFrame:contentRect];
				[m_movieView setMovie:m_movie];
				[m_movieView setEditable:NO]; 
				[m_movieView setControllerVisible:NO];
				[m_movieView setPreservesAspectRatio:YES];

			#endif

			m_loaded = true;
			
		}
		void VideoGameState::unload() {
			//m_movie;
			//m_movieView
		}

		void VideoGameState::play() {
			#if defined(ARK2D_MACINTOSH)
				[m_movie play];
			#endif
		}
		//void VideoGameState::pause() {
			//[m_movie pause];
		//}
		void VideoGameState::stop() {
			#if defined(ARK2D_MACINTOSH)
				[m_movie stop];
			#endif
		}

		float VideoGameState::getCurrentTime() {
			#if defined(ARK2D_MACINTOSH)
				return (unsigned int) [m_movie currentTime].timeValue;
			#else
				return false;
			#endif
		}

		void VideoGameState::setCurrentTime(float pos) {
			#if defined(ARK2D_MACINTOSH)
				QTTime t = QTMakeTime( (long long) pos, [m_movie currentTime].timeScale );
				[m_movie setCurrentTime:t];
			#endif
		}

		float VideoGameState::getVolume() {
			#if defined(ARK2D_MACINTOSH)
				return [m_movie volume];
			#endif
			return 0.0f;
		}
		void VideoGameState::setVolume(float f) {
			#if defined(ARK2D_MACINTOSH)
				[m_movie setVolume:f];
			#endif
		}

		bool VideoGameState::isCompleted() {
			#if defined(ARK2D_MACINTOSH)
				return [m_movie currentTime].timeValue >= [m_movie duration].timeValue;
			#endif
			return true;
		}

		VideoGameState::~VideoGameState() {

		}

	}
}



