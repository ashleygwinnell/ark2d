#include "pch.h"
#include "WindowsPhone81Game.h"
#include "../../src/%GAME_CLASS_NAME%.h"

#include <ppltasks.h>

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::Phone::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

WindowsPhone81Game::WindowsPhone81Game() :
	m_windowClosed(false),
	m_windowVisible(true)
{

}

// The first method called when the IFrameworkView is being created.
void WindowsPhone81Game::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &WindowsPhone81Game::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &WindowsPhone81Game::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &WindowsPhone81Game::OnResuming);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	//m_deviceResources = std::make_shared<DX::DeviceResources>();
}

// Called when the CoreWindow object is created (or re-created).
void WindowsPhone81Game::SetWindow(CoreWindow^ window)
{
	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &WindowsPhone81Game::OnVisibilityChanged);

	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &WindowsPhone81Game::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &WindowsPhone81Game::OnDisplayContentsInvalidated);

	#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		window->SizeChanged +=
			ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &WindowsPhone81Game::OnWindowSizeChanged);

		currentDisplayInformation->DpiChanged +=
			ref new TypedEventHandler<DisplayInformation^, Object^>(this, &WindowsPhone81Game::OnDpiChanged);

		currentDisplayInformation->OrientationChanged +=
			ref new TypedEventHandler<DisplayInformation^, Object^>(this, &WindowsPhone81Game::OnOrientationChanged);

		// Disable all pointer visual feedback for better performance when touching.
		// This is not supported on Windows Phone applications.
		auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
		pointerVisualizationSettings->IsContactFeedbackEnabled = false; 
		pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;
	#endif

	//m_deviceResources->SetWindow(window);


	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsPhone81Game::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsPhone81Game::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsPhone81Game::OnPointerReleased);

	// Key events
	HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs^>(this, &WindowsPhone81Game::OnBackButtonPressed);

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
	container->getPlatformSpecific()->initialize(CoreWindow::GetForCurrentThread(), this);
	container->start(); 
	container->setSize(windowWidth, windowHeight);
	
}

// Initializes scene resources, or loads a previously saved app state.
void WindowsPhone81Game::Load(Platform::String^ entryPoint)
{
	//if (m_main == nullptr)
	//{
	//	m_main = std::unique_ptr<App2Main>(new App2Main(m_deviceResources));
	//}
}

// This method is called after the window becomes active.
void WindowsPhone81Game::Run()
{
	/*while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			m_main->Update();

			if (m_main->Render())
			{
				m_deviceResources->Present();
			}
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}*/
	while (!m_windowClosed)
	{  
		if (m_windowVisible)
		{ 
			GameContainer* container = ARK2D::getContainer();
			Game* game = ARK2D::getGame();
			GameTimer* timer = ARK2D::getTimer();
			Log* logg = ARK2D::getLog();
			Input* in = ARK2D::getInput();
			Renderer* r = ARK2D::getRenderer();

			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

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
			//timer->limit(60);

		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void WindowsPhone81Game::Uninitialize()
{

}


// Window event handlers.
void WindowsPhone81Game::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void WindowsPhone81Game::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
void WindowsPhone81Game::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	//m_deviceResources->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	//m_main->CreateWindowSizeDependentResources();
}
#endif



// Mouse events

void WindowsPhone81Game::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	//ARK2D::getLog()->v("OnPointerPressed");

	GameContainer* container = ARK2D::getContainer();
	if (!container->m_bRunning) { return; }

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

void WindowsPhone81Game::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	//ARK2D::getLog()->v("OnPointerMoved");

	GameContainer* container = ARK2D::getContainer();
	if (!container->m_bRunning) { return; }

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

void WindowsPhone81Game::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
	//ARK2D::getLog()->v("OnPointerReleased");

	GameContainer* container = ARK2D::getContainer();
	if (!container->m_bRunning) { return; }

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

// Key events
void WindowsPhone81Game::OnBackButtonPressed(Object^ sender, BackPressedEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (!container->m_bRunning) { return; }

	Input* in = ARK2D::getInput();
	in->pressKey(Input::MOBILE_BACK);
	in->releaseKey(Input::MOBILE_BACK);

	args->Handled = true;

	//Windows::ApplicationModel::Core::CoreApplication::Exit();
	//Windows::ApplicationModel::Core::CoreWindow::GetForCurrentThread()->Close();
}


// Application lifecycle event handlers.
void WindowsPhone81Game::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void WindowsPhone81Game::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	Sound::xa_stopEngine();

	create_task([this, deferral]()
	{
        //m_deviceResources->Trim();

		// Insert your code here.

		deferral->Complete();
	});
}

void WindowsPhone81Game::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
	Sound::xa_startEngine();
}


// DisplayInformation event handlers.
void WindowsPhone81Game::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	//m_deviceResources->ValidateDevice();
}


#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
void WindowsPhone81Game::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	//m_deviceResources->SetDpi(sender->LogicalDpi);
	//m_main->CreateWindowSizeDependentResources();
}

void WindowsPhone81Game::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	//m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	//m_main->CreateWindowSizeDependentResources();
}
#endif

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	ARK2D::getLog()->setFilter(ARK::Util::Log::TYPE_WARNING);
	ARK2D::getLog()->v("Game Start!"); 

	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new WindowsPhone81Game();
}
