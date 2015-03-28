#include "pch.h"
#include "WindowsPhone8Game.h"
#include "../../src/DefaultGame.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace concurrency;

WindowsPhone8Game::WindowsPhone8Game() :
	m_windowClosed(false),
	m_windowVisible(true)
{
	
}
 
void WindowsPhone8Game::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &WindowsPhone8Game::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &WindowsPhone8Game::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &WindowsPhone8Game::OnResuming);

}

void WindowsPhone8Game::SetWindow(CoreWindow^ window)
{
	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &WindowsPhone8Game::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &WindowsPhone8Game::OnWindowClosed);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsPhone8Game::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsPhone8Game::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsPhone8Game::OnPointerReleased);

	int windowWidth = CoreWindow::GetForCurrentThread()->Bounds.Width;
	int windowHeight = CoreWindow::GetForCurrentThread()->Bounds.Height;
 
	ARK2D::getLog()->v(StringUtil::append("window width: ", windowWidth));
	ARK2D::getLog()->v(StringUtil::append("window height: ", windowHeight));


	Color* clearColor = new Color("%GAME_CLEAR_COLOR%");
	%GAME_CLASS_NAME%* game = new %GAME_CLASS_NAME%("%GAME_NAME%");
	GameContainer* container = new GameContainer(*game, %GAME_WIDTH%, %GAME_HEIGHT%, 32, false);
	container->setShowingFPS(false); // container.setShowingFPS(true);
	container->setClearColor(*clearColor);
	container->setIcon("data/icon.ico");
	container->setCursorVisible(true);
	container->setWillLoadDefaultFont(false);
	container->getPlatformSpecific()->initialize(CoreWindow::GetForCurrentThread());
	container->start(); 
	container->setSize(windowWidth, windowHeight);
	
}   
  
void WindowsPhone8Game::Load(Platform::String^ entryPoint) 
{

} 

void WindowsPhone8Game::Run() 
{
	while (!m_windowClosed)
	{  
		if (m_windowVisible)
		{ 
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			
			GameContainer* container = ARK2D::getContainer();
			Game* game = ARK2D::getGame();
			GameTimer* timer = ARK2D::getTimer();
			Log* logg = ARK2D::getLog();
			Input* in = ARK2D::getInput();
			Renderer* r = ARK2D::getRenderer();

			if (!container->m_bRunning) { continue; }

			// Update ARK2D
			timer->tick(); 
			//container->processGamepadInput();
			logg->update();

			// Update Game
			game->preUpdate(container, timer);
			game->update(container, timer);
			game->postUpdate(container, timer);
		
			// Update Input
			in->clearKeyPressedRecord();
			//for (unsigned int i = 0; i < container->m_gamepads.size(); i++) {
			//	container->m_gamepads.at(i)->clearButtonPressedRecord();
			//}

			// Rendering
			RendererStats::reset();
			container->getPlatformSpecific()->beginDXFrame(); 

			game->preRender(container, r);
			game->render(container, r);
			logg->render(container, r);
			game->postRender(container, r);
			
			container->getPlatformSpecific()->swapBuffers(); 
			timer->limit(60);

		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void WindowsPhone8Game::Uninitialize()
{
}

void WindowsPhone8Game::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void WindowsPhone8Game::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void WindowsPhone8Game::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	float thisx = (float) args->CurrentPoint->Position.X;
	float thisy = (float) args->CurrentPoint->Position.Y;
	thisx -= container->getTranslateX();
	thisy -= container->getTranslateY();

	thisx /= container->getScale();
	thisy /= container->getScale();

	Input* i = ARK2D::getInput();
	i->mouse_x = (int) thisx;
	i->mouse_y = (int) thisy;
	i->pressKey(Input::MOUSE_BUTTON_LEFT);

}

void WindowsPhone8Game::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	float thisx = (float) args->CurrentPoint->Position.X;
	float thisy = (float) args->CurrentPoint->Position.Y;
	
	thisx -= container->getTranslateX();
	thisy -= container->getTranslateY();

	thisx /= container->getScale();
	thisy /= container->getScale();

	Input* i = ARK2D::getInput();
	ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
	ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
	i->mouse_x = (int) thisx;
	i->mouse_y = (int) thisy;
}

void WindowsPhone8Game::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	float thisx = (float) args->CurrentPoint->Position.X;
	float thisy = (float) args->CurrentPoint->Position.Y;

	thisx -= container->getTranslateX();
	thisy -= container->getTranslateY();

	thisx /= container->getScale();
	thisy /= container->getScale();

	Input* i = ARK2D::getInput();
	i->mouse_x = (int) thisx;
	i->mouse_y = (int) thisy;
	i->releaseKey(Input::MOUSE_BUTTON_LEFT);
}

void WindowsPhone8Game::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void WindowsPhone8Game::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();
	// m_renderer->ReleaseResourcesForSuspending();
	Sound::xa_stopEngine();

	create_task([this, deferral]()
	{
		// Insert your code here.

		deferral->Complete();
	});
}
 
void WindowsPhone8Game::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
	// m_renderer->CreateWindowSizeDependentResources();
	Sound::xa_startEngine();
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new WindowsPhone8Game();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	ARK2D::getLog()->setFilter(ARK::Util::Log::TYPE_ALL);
	ARK2D::getLog()->v("GO!"); 
	 
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource); 
	return 0;
} 

