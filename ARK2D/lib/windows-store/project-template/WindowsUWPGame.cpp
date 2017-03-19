#include "pch.h"
#include "WindowsUWPGame.h"
#include "../../src/%GAME_CLASS_NAME%.h"

#include <ppltasks.h>
#include <ARK2D/Core/Log.h>

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
	using namespace Windows::Phone::UI::Input;
#endif

// The main function is only used to initialize our IFrameworkView class.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	ARK2D::getLog()->setFilter(ARK::Core::Log::TYPE_ALL);
	ARK2D::getLog()->w("Game Start!");

	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new WindowsUWPGame();
}


WindowsUWPGame::WindowsUWPGame() :
	m_windowClosed(false),
	m_windowVisible(true)
{

}

// The first method called when the IFrameworkView is being created.
void WindowsUWPGame::Initialize(CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &WindowsUWPGame::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &WindowsUWPGame::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &WindowsUWPGame::OnResuming);

	// At this point we have access to the device.
	// We can create the device-dependent resources.
	%GAME_CLASS_NAME%* game = new %GAME_CLASS_NAME%("%GAME_NAME%");
	GameContainer* container = new GameContainer(*game, %GAME_WIDTH%, %GAME_HEIGHT%, 32, false);
}

// Called when the CoreWindow object is created (or re-created).
void WindowsUWPGame::SetWindow(CoreWindow^ window)
{
	window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &WindowsUWPGame::OnWindowSizeChanged);

	window->PointerCursor = ref new CoreCursor(CoreCursorType::Arrow, 0);

	//#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		// // Disable all pointer visual feedback for better performance when touching.
		// // This is not supported on Windows Phone applications.
	auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
	pointerVisualizationSettings->IsContactFeedbackEnabled = false;
	pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;
	//#endif

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &WindowsUWPGame::OnVisibilityChanged);

	window->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &WindowsUWPGame::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
	 	ref new TypedEventHandler<DisplayInformation^, Object^>(this, &WindowsUWPGame::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &WindowsUWPGame::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &WindowsUWPGame::OnDisplayContentsInvalidated);


	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsUWPGame::OnPointerPressed);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsUWPGame::OnPointerMoved);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WindowsUWPGame::OnPointerReleased);

	window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &WindowsUWPGame::OnKeyDown);
	window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &WindowsUWPGame::OnKeyUp);

	// Key events
	#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs^>(this, &WindowsUWPGame::OnBackButtonPressed);
	#endif

	ARK2D::getContainer()->getPlatformSpecific()->SetWindow(CoreWindow::GetForCurrentThread());
}

// Initializes scene resources, or loads a previously saved app state.
void WindowsUWPGame::Load(Platform::String^ entryPoint)
{
	Color* clearColor = new Color("%GAME_CLEAR_COLOR%");

	GameContainer* container = ARK2D::getContainer();
	container->setShowingFPS(false);
	container->setClearColor(*clearColor);
	container->setIcon("data/icon.ico");
	container->setCursorVisible(true);
	container->setWillLoadDefaultFont(true);

	container->start();
	container->setSize( CoreWindow::GetForCurrentThread()->Bounds.Width, CoreWindow::GetForCurrentThread()->Bounds.Height );

	ARK2D::getContainer()->getPlatformSpecific()->RegisterDeviceNotify(&notifier);
}

// This method is called after the window becomes active.
void WindowsUWPGame::Run()
{
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
			Scene* scene = ARK2D::getScene();

			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			if (!container->m_bRunning) { continue; }

			// Update ARK2D
			timer->tick();
			//container->processGamepadInput();

			// Update Game
			scene->update();

			// Update Input
			in->clearKeyPressedRecord();
			//for (unsigned int i = 0; i < container->m_gamepads.size(); i++) {
			//	container->m_gamepads.at(i)->clearButtonPressedRecord();
			//}

			// Rendering
			RendererStats::reset();
			container->getPlatformSpecific()->beginDXFrame();

			scene->render();

			container->getPlatformSpecific()->swapBuffers();
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
void WindowsUWPGame::Uninitialize()
{

}

// Application lifecycle event handlers.
void WindowsUWPGame::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void WindowsUWPGame::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	Sound::xa_stopEngine();

	create_task([this, deferral]()
	{
        ARK2D::getContainer()->getPlatformSpecific()->Trim();

		deferral->Complete();
	});
}

void WindowsUWPGame::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
	Sound::xa_startEngine();
}

void WindowsUWPGame::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (container != NULL) {
		container->getPlatformSpecific()->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	}
	else {
		ErrorDialog::createAndShow("Could not OnWindowSizeChanged. GameContainer was null.");
	}
}


// Window event handlers.
void WindowsUWPGame::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void WindowsUWPGame::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void WindowsUWPGame::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (container != NULL) {
		container->getPlatformSpecific()->SetDpi(sender->LogicalDpi);
		container->getPlatformSpecific()->CreateWindowSizeDependentResources();
	}
	else {
		ErrorDialog::createAndShow("Could not OnDpiChanged. GameContainer was null.");
	}
}

void WindowsUWPGame::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (container != NULL) {
		container->getPlatformSpecific()->SetCurrentOrientation(sender->CurrentOrientation);
		container->getPlatformSpecific()->CreateWindowSizeDependentResources();
	}
	else {
		ErrorDialog::createAndShow("Could not OnOrientationChanged. GameContainer was null.");
	}
}

// DisplayInformation event handlers.
void WindowsUWPGame::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	//m_deviceResources->ValidateDevice();
	GameContainer* container = ARK2D::getContainer();
	if (container != NULL) {
		container->getPlatformSpecific()->ValidateDevice();
	}
	else {
		ErrorDialog::createAndShow("Could not OnDisplayContentsInvalidated. GameContainer was null.");
	}
}

// Mouse events
void WindowsUWPGame::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
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

void WindowsUWPGame::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (!container->m_bRunning) { return; }

	float thisx = (float) args->CurrentPoint->Position.X;
	float thisy = (float) args->CurrentPoint->Position.Y;

	thisx -= container->getTranslateX();
	thisy -= container->getTranslateY();

	thisx /= container->getScale();
	thisy /= container->getScale();

	Input* i = ARK2D::getInput();
	//ARK2D::getLog()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
	ARK2D::getGame()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
	i->mouse_x = (int) thisx;
	i->mouse_y = (int) thisy;
}

void WindowsUWPGame::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
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

void WindowsUWPGame::OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (container == NULL || !container->m_bRunning) { return; }

	// VirtualKey enum maps to win32 keys.
	ARK2D::getInput()->pressKey((int)args->VirtualKey);
}

void WindowsUWPGame::OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{
	GameContainer* container = ARK2D::getContainer();
	if (container == NULL || !container->m_bRunning) { return; }

	// VirtualKey enum maps to win32 keys.
	ARK2D::getInput()->releaseKey((int)args->VirtualKey);
}


// Key events
#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
void WindowsUWPGame::OnBackButtonPressed(Object^ sender, BackPressedEventArgs^ args)
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
#endif


// Notifies renderers that device resources need to be released.
void WindowsUWPGame_DeviceNotifier::OnDeviceLost()
{

}

// Notifies renderers that device resources may now be recreated.
void WindowsUWPGame_DeviceNotifier::OnDeviceRestored()
{
	//m_sceneRenderer->CreateDeviceDependentResources();
	//m_fpsTextRenderer->CreateDeviceDependentResources();

	GameContainer* container = ARK2D::getContainer();
	if (container != NULL) {
		ARK2D::getLog()->e("OnDeviceRestored");
		container->getPlatformSpecific()->CreateWindowSizeDependentResources();
	}
	else {
		ErrorDialog::createAndShow("Could not OnDeviceRestored. GameContainer was null.");
	}
}



