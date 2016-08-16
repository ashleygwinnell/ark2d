/*
 * Dialog.h
 *
 *  Created on: 12 Feb 2013
 *      Author: Ashley
 */

#ifndef ARK_UI_DIALOG_H_
#define ARK_UI_DIALOG_H_

#include "../Core/ARK2D.h"
#include <string>
using namespace std;

#ifdef __WIN32

#elif defined(ARK2D_MACINTOSH)
#import <Cocoa/Cocoa.h>
#endif


	#ifdef ARK2D_WINDOWS_PHONE_8

namespace ARK_Internal
{

	ref class LoginResult sealed
	{
	public:
		LoginResult();
		property Platform::String^ Name;
		//public string Name { get; set; }
		//public string Password { get; set; }
	};


	ref class InputDialog sealed// : public Windows::ApplicationModel::Core::
	{
	private:
		//TaskCompletionSource<LoginResult>^ _taskCompletionSource;
		Windows::UI::Xaml::Controls::PasswordBox^ _passwordTextBox;
		Windows::UI::Xaml::Controls::TextBlock^ _titleTextBlock;
		Windows::UI::Xaml::Controls::TextBlock^ _userNameTextBlock;
		Windows::UI::Xaml::Controls::TextBox^ _userTextBox;
		Windows::UI::Xaml::Controls::Primitives::Popup^ _popup;

	public:
		InputDialog(Platform::String^ title);
		InputDialog(Platform::String^ title, Platform::String^ loginMessage);
		
		Windows::Foundation::IAsyncOperation<LoginResult^ >^ ShowAsync();

	private:
		property Platform::String^ LoginMessage;// { get; set; }
		Platform::Agile<Windows::UI::Xaml::Media::SolidColorBrush> HeaderBrush;// { get; set; }

		property Platform::String^ Title;// { get; set; }

		property Platform::String^ UserNameTitle;// { get; set; }

		
		//Windows::Foundation::IAsyncOperation<Windows::UI::Popups::UICommand^> ShowAsync();


	private:
		//Task<LoginResult> WaitForInput(CancellationToken token);

		Windows::UI::Xaml::Controls::Grid^ CreateLogin();

		void PasswordTextBoxOnKeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ keyRoutedEventArgs);

		// adjust for different view states
		void OnWindowSizeChanged(Platform::Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e);

		// Adjust the name/password textboxes for the virtual keyuboard
		void OnInputShowing(Windows::UI::ViewManagement::InputPane^ sender, Windows::UI::ViewManagement::InputPaneVisibilityEventArgs^ args);

		void OnInputHiding(Windows::UI::ViewManagement::InputPane^ sender, Windows::UI::ViewManagement::InputPaneVisibilityEventArgs^ args);

		void OnKeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);

		void OnWindowActivated(Platform::Object^ sender, Windows::UI::Core::WindowActivatedEventArgs^ windowActivatedEventArgs);

		void OnCompleted(Platform::Object^ o, Windows::UI::Xaml::RoutedEventArgs^ e);

		void OnCanceled(Platform::Object^ o, Windows::UI::Xaml::RoutedEventArgs^ e);

		Windows::Foundation::EventRegistrationToken m_windowActivatedToken;
		Windows::Foundation::EventRegistrationToken m_keyDownToken;
		Windows::Foundation::EventRegistrationToken m_sizeChangedToken;
		Windows::Foundation::EventRegistrationToken m_inputShowingToken;
		Windows::Foundation::EventRegistrationToken m_inputHidingToken;
		void SubscribeEvents();
		void UnsubscribeEvents();
	};
}
	#endif

namespace ARK {
	namespace UI {

		/*!
		 * \brief Dialogs such as modal input window.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API Dialog { 
			private:
				static string s_inputDialogText;
			public: 

				static void openInputDialog(unsigned int callbackId, string title, string defaultStr);
				static string getInputDialogText(); 

				static void setInputDialogText(string t) { s_inputDialogText = t; }
		};
	}
}

#endif /* ARK_UI_DIALOG_H_ */
