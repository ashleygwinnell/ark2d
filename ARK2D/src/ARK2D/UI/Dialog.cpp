/*
 * Dialog.cpp
 *
 *  Created on: 12 Feb 2013
 *      Author: Ashley
 */

#include "Dialog.h"
#include "../Core/Log.h"
#include "../Core/GameContainer.h"
#include "../Util/Callbacks.h"

#ifdef ARK2D_WINDOWS_PHONE_8

using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;

namespace ARK_Internal
{

	LoginResult::LoginResult() {

	}

	InputDialog::InputDialog(Platform::String^ title):
		HeaderBrush()
	{
		ARK2D::getLog()->w("new InputDialog");

		if (!title->IsEmpty() && title->Length() > 0) {
			Title = title;
		}
		UserNameTitle = "Name";
		LoginMessage = "Enter the information below to connect to your account.";

		ARK2D::getLog()->w("1");
		HeaderBrush.
		HeaderBrush.Get()->Color = Windows::UI::Colors::Blue; //ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Blue); //ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Blue);
		ARK2D::getLog()->w("2");
	}

	InputDialog::InputDialog(Platform::String^ title, Platform::String^ loginMessage)
	{
		ARK2D::getLog()->w("new InputDialog");

		if (!title->IsEmpty() && title->Length() > 0) {
			Title = title;
		}
		UserNameTitle = "Name";
		LoginMessage = "Enter the information below to connect to your account.";
		//HeaderBrush = ref new SolidColorBrush(Windows::UI::Colors::Blue);
		HeaderBrush.Get()->Color = Windows::UI::Colors::Blue;

		if (!loginMessage->IsEmpty() && loginMessage->Length() > 0)
		{
			LoginMessage = loginMessage;
		}
	}


	IAsyncOperation<LoginResult^ >^ InputDialog::ShowAsync()
	{
		ARK2D::getLog()->w("InputDialog::showAsync");

		_popup = ref new Popup();
		_popup->Child = CreateLogin();
		if (_popup->Child != nullptr)
		{
			SubscribeEvents();
			_popup->IsOpen = true;
		}
		//return IAsyncInfo:://Run(WaitForInput);
		//return ref new IAsyncOperation<LoginResult^>();
		return create_async([]() -> LoginResult^ {
			//WaitForInput
			return ref new LoginResult();
		});
	}

	/*Task<LoginResult> InputDialog::WaitForInput(CancellationToken token)
	{
	_taskCompletionSource = new TaskCompletionSource<LoginResult>();

	token.Register(OnCanceled);

	return _taskCompletionSource.Task;
	}*/

	Grid^ InputDialog::CreateLogin()
	{
		ARK2D::getLog()->w("InputDialog::CreateLogin");
		FrameworkElement^ content = (FrameworkElement^)Window::Current->Content;// as FrameworkElement;
		if (content == nullptr)
		{
			// The dialog is being shown before content has been created for the window
			m_windowActivatedToken = Window::Current->Activated += ref new WindowActivatedEventHandler(this, &InputDialog::OnWindowActivated);

			return nullptr;
		}

		Style^ basicTextStyle = (Style^) Application::Current->Resources->Lookup("BaseTextBlockStyle");// as Style;
		Style^ subHeaderTextStyle = (Style^) Application::Current->Resources->Lookup("SubheaderTextBlockStyle");// as Style;

		double width = Window::Current->Bounds.Width;
		double height = Window::Current->Bounds.Height;

		Grid^ rootPanel = ref new Grid();
		rootPanel->Width = width;
		rootPanel->Height = height;

		auto overlay = ref new Grid();
		overlay->Background = ref new SolidColorBrush(Windows::UI::Colors::Black);
		overlay->Opacity = 0.2;
		rootPanel->Children->Append(overlay);

		Grid^ dialog = ref new Grid();
		dialog->VerticalAlignment = VerticalAlignment::Center;
		dialog->RequestedTheme = ElementTheme::Light;

		RowDefinition^ row1 = ref new RowDefinition();
		row1->Height = GridLength(1, GridUnitType::Auto);
		dialog->RowDefinitions->Append(row1);

		RowDefinition^ row2 = ref new RowDefinition();
		row2->Height = GridLength(1, GridUnitType::Star);
		dialog->RowDefinitions->Append(row2);

		RowDefinition^ row3 = ref new RowDefinition();
		row3->Height = GridLength(1, GridUnitType::Auto);
		dialog->RowDefinitions->Append(row3);

		ColumnDefinition^ column1 = ref new ColumnDefinition();
		column1->Width = GridLength(410.0, GridUnitType::Star);
		dialog->ColumnDefinitions->Append(column1);

		ColumnDefinition^ column2 = ref new ColumnDefinition();
		column2->Width = GridLength(1, GridUnitType::Auto);
		dialog->ColumnDefinitions->Append(column2);

		ColumnDefinition^ column3 = ref new ColumnDefinition();
		column3->Width = GridLength(500, GridUnitType::Star);
		dialog->ColumnDefinitions->Append(column3);

		rootPanel->Children->Append(dialog);

		auto titleBorder = ref new Border();
		titleBorder->Background = HeaderBrush.Get();
		titleBorder->Height = 80.0;
		Grid::SetColumnSpan(titleBorder, 3);
		dialog->Children->Append(titleBorder);

		_titleTextBlock = ref new TextBlock();
		_titleTextBlock->Text = Title;
		_titleTextBlock->Style = subHeaderTextStyle;
		_titleTextBlock->Margin = Thickness(0, 0, 0, 20);
		_titleTextBlock->VerticalAlignment = VerticalAlignment::Bottom;
		Grid::SetColumn(_titleTextBlock, 1);
		dialog->Children->Append(_titleTextBlock);

		auto infoBorder = ref new Border();
		infoBorder->Background = ref new SolidColorBrush(Windows::UI::Colors::White);
		Grid::SetRow(infoBorder, 1);
		Grid::SetColumnSpan(infoBorder, 3);
		Grid::SetRowSpan(infoBorder, 2);
		dialog->Children->Append(infoBorder);

		auto grid = ref new Grid();
		grid->MinHeight = 220;
		Grid::SetRow(grid, 1);
		Grid::SetColumn(grid, 1);
		dialog->Children->Append(grid);

		StackPanel^ informationPanel = ref new StackPanel();
		informationPanel->Margin = Thickness(0, 20, 0, 30);
		informationPanel->Width = 456.0;
		Grid::SetColumn(informationPanel, 1);
		Grid::SetRow(informationPanel, 1);

		auto descriptionTextBlock = ref new TextBlock();
		descriptionTextBlock->Text = LoginMessage;
		descriptionTextBlock->Style = basicTextStyle;
		descriptionTextBlock->Width = 456.0;
		informationPanel->Children->Append(descriptionTextBlock);

		_userNameTextBlock = ref new TextBlock();
		_userNameTextBlock->Text = UserNameTitle;
		_userNameTextBlock->Style = basicTextStyle;
		_userNameTextBlock->Margin = Thickness(0, 20, 0, 4);
		informationPanel->Children->Append(_userNameTextBlock);

		_userTextBox = ref new TextBox();
		_userTextBox->BorderBrush = ref new SolidColorBrush(Windows::UI::Colors::Black);
		_userTextBox->BorderThickness = Thickness(1);
		informationPanel->Children->Append(_userTextBox);

		TextBlock^ passwordTextBlock = ref new TextBlock();
		passwordTextBlock->Text = "Password";
		passwordTextBlock->Style = basicTextStyle;
		passwordTextBlock->FontSize = 16.0;
		passwordTextBlock->Margin = Thickness(0, 16, 0, 0);
		informationPanel->Children->Append(passwordTextBlock);

		_passwordTextBox = ref new PasswordBox();
		_passwordTextBox->BorderBrush = ref new SolidColorBrush(Windows::UI::Colors::Black);
		_passwordTextBox->BorderThickness = Thickness(1);
		_passwordTextBox->KeyUp += ref new KeyEventHandler(this, &InputDialog::PasswordTextBoxOnKeyUp);
		informationPanel->Children->Append(_passwordTextBox);

		grid->Children->Append(informationPanel);

		Windows::UI::Xaml::Controls::Button^ connectButton = ref new Windows::UI::Xaml::Controls::Button();
		connectButton->BorderThickness = Thickness();
		connectButton->Padding = Thickness(10, 5, 10, 5);
		// TODO: Fill with user input
		connectButton->Content = "Connect";
		connectButton->HorizontalAlignment = HorizontalAlignment::Right;
		connectButton->Background = HeaderBrush.Get();
		connectButton->Margin = Thickness(0, 0, 0, 20);
		connectButton->MinWidth = 90.0;
		connectButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &InputDialog::OnCompleted);//(okSender, okArgs) = > OnCompleted();
		Grid::SetColumn(connectButton, 1);
		Grid::SetRow(connectButton, 2);
		dialog->Children->Append(connectButton);

		Windows::UI::Xaml::Controls::Button^ cancelButton = ref new Windows::UI::Xaml::Controls::Button();
		cancelButton->BorderThickness = Thickness();
		cancelButton->Padding = Thickness(10, 5, 10, 5);
		cancelButton->Content = "Cancel";
		cancelButton->HorizontalAlignment = HorizontalAlignment::Left;
		cancelButton->Background = ref new SolidColorBrush(Windows::UI::Colors::Red);
		cancelButton->Margin = Thickness(20, 0, 0, 20);
		cancelButton->MinWidth = 90.0;
		cancelButton->Click += ref new Windows::UI::Xaml::RoutedEventHandler(this, &InputDialog::OnCanceled);
		Grid::SetColumn(cancelButton, 2);
		Grid::SetRow(cancelButton, 2);
		dialog->Children->Append(cancelButton);
		return rootPanel;
	}

	void InputDialog::PasswordTextBoxOnKeyUp(Object^ sender, KeyRoutedEventArgs^ e)
	{
		if (e->Key == VirtualKey::Enter) {
			OnCompleted(sender, e);
		}
	}

	// adjust for different view states
	void InputDialog::OnWindowSizeChanged(Object^ sender, WindowSizeChangedEventArgs^ e)
	{
		if (_popup->IsOpen == false) return;

		//var child = _popup->Child as FrameworkElement;
		//if (child == null) return;
		FrameworkElement^ child = (FrameworkElement^)_popup->Child;

		child->Width = e->Size.Width;
		child->Height = e->Size.Height;
	}

	// Adjust the name/password textboxes for the virtual keyuboard
	void InputDialog::OnInputShowing(Windows::UI::ViewManagement::InputPane^ sender, InputPaneVisibilityEventArgs^ args)
	{
		FrameworkElement^ child = (FrameworkElement^)_popup->Child;// as FrameworkElement;
		//if (child == null) return;

		GeneralTransform^ transform = _passwordTextBox->TransformToVisual(child);
		Point topLeft = transform->TransformPoint(Point(0, 0));

		// Need to be able to view the entire textblock (plus a little more)
		auto buffer = _passwordTextBox->ActualHeight + 10;
		if ((topLeft.Y + buffer) > sender->OccludedRect.Top)
		{
			auto margin = topLeft.Y - sender->OccludedRect.Top;
			margin += buffer;
			child->Margin = Thickness(0, -margin, 0, 0);
		}
	}

	void InputDialog::OnInputHiding(InputPane^ sender, InputPaneVisibilityEventArgs^ args)
	{
		FrameworkElement^ child = (FrameworkElement^)_popup->Child;
		//if (child == null) return;

		child->Margin = Thickness(0);
	}

	void InputDialog::OnKeyDown(Object^ sender, KeyRoutedEventArgs^ e)
	{
		if (e->Key == VirtualKey::Escape)
		{
			OnCanceled(sender, e);
		}
	}

	void InputDialog::OnWindowActivated(Object^ sender, WindowActivatedEventArgs^ windowActivatedEventArgs)
	{
		Window::Current->Activated -= m_windowActivatedToken;

		SubscribeEvents();
		_popup->Child = CreateLogin();
		_popup->IsOpen = true;
	}

	void InputDialog::OnCompleted(Object^ o, RoutedEventArgs^ e)
	{
		UnsubscribeEvents();

		LoginResult^ result = ref new LoginResult();
		result->Name = _userTextBox->Text;

		_popup->IsOpen = false;
		//_taskCompletionSource.SetResult(result);
	}

	void InputDialog::OnCanceled(Object^ o, RoutedEventArgs^ e)
	{
		UnsubscribeEvents();
		// null to indicate dialog was canceled
		LoginResult^ result = ref new LoginResult(); // null;

		_popup->IsOpen = false;
		//_taskCompletionSource.SetResult(result);
	}

	void InputDialog::SubscribeEvents()
	{
	#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		m_sizeChangedToken = Window::Current->SizeChanged += ref new TypedEventHandler<Object^, WindowSizeChangedEventArgs^>(this, &InputDialog::OnWindowSizeChanged);
	#endif

		m_keyDownToken = Window::Current->Content->KeyDown += ref new KeyEventHandler(this, &InputDialog::OnKeyDown);

		auto input = InputPane::GetForCurrentView();
		m_inputShowingToken = input->Showing += ref new TypedEventHandler<InputPane^, InputPaneVisibilityEventArgs^>(this, &InputDialog::OnInputShowing);
		m_inputHidingToken = input->Hiding += ref new TypedEventHandler<InputPane^, InputPaneVisibilityEventArgs^>(this, &InputDialog::OnInputHiding);;
	}

	void InputDialog::UnsubscribeEvents()
	{
	#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		Window::Current->SizeChanged -= m_sizeChangedToken;
	#endif

		Window::Current->Content->KeyDown -= m_keyDownToken;

		InputPane^ input = InputPane::GetForCurrentView();
		input->Showing -= m_inputShowingToken;//OnInputShowing;
		input->Hiding -= m_inputHidingToken;// OnInputHiding;
	}

}

#endif

namespace ARK {
	namespace UI {

		#if defined(ARK2D_WINDOWS)

			char mystaticdialogproc_szItemName[80];
			INT_PTR CALLBACK MyStaticDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
			{
				//ARK2D::getLog()->t("mystaticdialogproc");
				switch (message)
			    {
			        case WM_COMMAND:
			            switch (LOWORD(wParam))
			            {
			                case IDOK:
			                    if (!GetDlgItemText(hwndDlg, ARK2D_DIALOG_INPUT_TEXT_CONTENTS, mystaticdialogproc_szItemName, 80))
			                         *mystaticdialogproc_szItemName=0;

			                    // Fall through.

			                case IDCANCEL:
			                    EndDialog(hwndDlg, wParam);
			                    return TRUE;

			                //default:
			               		//return DefDlgProc(hwndDlg, message, wParam, lParam);
			            }
			    }
			    return FALSE;

				//return TRUE;
			}

			#define ID_HELP   150
			#define ID_TEXT   200

			LPWORD lpwAlign(LPWORD lpIn)
			{
			    ULONG ul;

			    ul = (ULONG)lpIn;
			    ul ++;
			    ul >>=1;
			    ul <<=1;
			    return (LPWORD)ul;
			}

			LRESULT DisplayMyMessage(HINSTANCE hinst, HWND hwndOwner, LPSTR lpszMessage)
			{
			    HGLOBAL hgbl;
			    LPDLGTEMPLATE lpdt;
			    LPDLGITEMTEMPLATE lpdit;
			    LPWORD lpw;
			    LPWSTR lpwsz;
			    LRESULT ret;
			    int nchar;

			    hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
			    if (!hgbl)
			        return -1;

			    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);

			    // Define a dialog box.

			    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
			    lpdt->cdit = 3;         // Number of controls
			    lpdt->x  = 10;  lpdt->y  = 10;
			    lpdt->cx = 100; lpdt->cy = 100;

			    lpw = (LPWORD)(lpdt + 1);
			    *lpw++ = 0;             // No menu
			    *lpw++ = 0;             // Predefined dialog box class (by default)

			    lpwsz = (LPWSTR)lpw;
			    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "My Dialog", -1, lpwsz, 50);
			    lpw += nchar;

			    //-----------------------
			    // Define an OK button.
			    //-----------------------
			    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
			    lpdit = (LPDLGITEMTEMPLATE)lpw;
			    lpdit->x  = 10; lpdit->y  = 70;
			    lpdit->cx = 80; lpdit->cy = 20;
			    lpdit->id = IDOK;       // OK button identifier
			    lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

			    lpw = (LPWORD)(lpdit + 1);
			    *lpw++ = 0xFFFF;
			    *lpw++ = 0x0080;        // Button class

			    lpwsz = (LPWSTR)lpw;
			    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "OK", -1, lpwsz, 50);
			    lpw += nchar;
			    *lpw++ = 0;             // No creation data

			    //-----------------------
			    // Define a Help button.
			    //-----------------------
			    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
			    lpdit = (LPDLGITEMTEMPLATE)lpw;
			    lpdit->x  = 55; lpdit->y  = 10;
			    lpdit->cx = 40; lpdit->cy = 20;
			    lpdit->id = ID_HELP;    // Help button identifier
			    lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

			    lpw = (LPWORD)(lpdit + 1);
			    *lpw++ = 0xFFFF;
			    *lpw++ = 0x0080;        // Button class atom

			    lpwsz = (LPWSTR)lpw;
			    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "Help", -1, lpwsz, 50);
			    lpw += nchar;
			    *lpw++ = 0;             // No creation data

			    //-----------------------
			    // Define a static text control.
			    //-----------------------
			    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
			    lpdit = (LPDLGITEMTEMPLATE)lpw;
			    lpdit->x  = 10; lpdit->y  = 10;
			    lpdit->cx = 40; lpdit->cy = 20;
			    lpdit->id = ID_TEXT;    // Text identifier
			    lpdit->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

			    lpw = (LPWORD)(lpdit + 1);
			    *lpw++ = 0xFFFF;
			    *lpw++ = 0x0082;        // Static class

			    for (lpwsz = (LPWSTR)lpw; *lpwsz++ = (WCHAR)*lpszMessage++;);
			    lpw = (LPWORD)lpwsz;
			    *lpw++ = 0;             // No creation data

			    GlobalUnlock(hgbl);
			    ret = DialogBoxIndirect(hinst,
			                           (LPDLGTEMPLATE)hgbl,
			                           hwndOwner,
			                           (DLGPROC)MyStaticDialogProc);
			    GlobalFree(hgbl);
			    return ret;
			}

		#endif





		string Dialog::s_inputDialogText = "";

		void Dialog::openInputDialog(unsigned int callbackId, string title, string defaultStr) {
			#if defined(ARK2D_WINDOWS_PHONE_8)

				/*using namespace Windows::UI::Popups;

				string contents = message;
				wchar_t* wideTitle = Cast::charToWideChar(title.c_str());
				wchar_t* wideContents = Cast::charToWideChar(contents.c_str());

				Platform::String^ titleStr = ref new Platform::String(wideTitle);
				Platform::String^ contentsStr = ref new Platform::String(wideContents);
				MessageDialog^ md = ref new MessageDialog(contentsStr, titleStr);
				md->ShowAsync();

				delete wideTitle;
				delete wideContents;*/
				ARK2D::getLog()->w("open dialog input");
				GameContainer* container = ARK2D::getContainer();

				auto window = Windows::UI::Core::CoreWindow::GetForCurrentThread();
				auto dispatcher = window->Dispatcher;

				//container->getPlatformSpecific()->m_uiDispatcher->
				//auto dispatcher = container->getPlatformSpecific()->m_uiDispatcher;

				//create_async([dispatcher]() {
					//ARK2D::getLog()->w("open dialog input async 1");
					dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([](){
						ARK2D::getLog()->w("open dialog input async 2");
						ARK_Internal::InputDialog^ d = ref new ARK_Internal::InputDialog(ref new Platform::String(L"Input"));
						ARK2D::getLog()->w("3");
						d->ShowAsync();
					}));
				//});




				//create_async([]() -> void {
				//
				//	return;
				//});


			#elif defined(ARK2D_FLASCC)
				unsigned int titlelength = title.length();
				unsigned int defaultStrlength = defaultStr.length();

				/*inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.s_console.openInputDialog(%0, CModule.readString(%1, %2), CModule.readString(%3, %4));\n"
					: : "r"(callbackId), "r"(title), "r"(titlelength), "r"(defaultStr), "r"(defaultStrlength)
				); */

				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"Console.s_console.openInputDialog(%0, CModule.readString(%1, %2), CModule.readString(%3, %4));\n"\
					: :  "r"(callbackId), "r"(title.c_str()), "r"(titlelength), "r"(defaultStr.c_str()), "r"(defaultStrlength)
				);

			#elif defined(ARK2D_IPHONE)

				s_inputDialogText = "";

				InputDialogViewController* dele = [[InputDialogViewController alloc] init];
				dele.m_callbackId = callbackId;

				NSString* titleNSStr = [NSString stringWithCString:title.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* okNSStr = [NSString stringWithCString:"OK" encoding:[NSString defaultCStringEncoding]];
				NSString* defaultNSStr = [NSString stringWithCString:defaultStr.c_str() encoding:[NSString defaultCStringEncoding]];

				UIAlertView * alert = [[UIAlertView alloc] initWithTitle:titleNSStr message:titleNSStr delegate:dele  cancelButtonTitle:okNSStr otherButtonTitles:nil];
				alert.alertViewStyle = UIAlertViewStylePlainTextInput;
				UITextField * alertTextField = [alert textFieldAtIndex:0];
				alertTextField.keyboardType = UIKeyboardTypeNumberPad;
				//alertTextField.placeholder = @"Enter your name";
				alertTextField.text = defaultNSStr;
				[alert show];
				[alert release];


			#elif defined(ARK2D_MACINTOSH)

				s_inputDialogText = "";



				NSString* titleNSStr = [NSString stringWithCString:title.c_str() encoding:[NSString defaultCStringEncoding]];
				NSString* defaultNSStr = [NSString stringWithCString:defaultStr.c_str() encoding:[NSString defaultCStringEncoding]];

				NSAlert* alert = [NSAlert alertWithMessageText: titleNSStr
					defaultButton:@"OK"
					alternateButton:@"Cancel"
					otherButton:nil
					informativeTextWithFormat:@""];

				NSTextField* input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 200, 24)];
				[input setStringValue:defaultNSStr];
				[input autorelease];
				//[alert orderFrontRegardless];
				[alert setAccessoryView:input];

				//[[NSRunningApplication currentApplication] activateWithOptions:NSApplicationActivateIgnoringOtherApps];

				NSWindow* topWindow = ARK2D::getContainer()->getPlatformSpecific()->m_window;
				[topWindow orderOut:alert];

				NSInteger button = [alert runModal];
				if (button == NSAlertDefaultReturn) {
					[input validateEditing];
					s_inputDialogText = [[input stringValue] cStringUsingEncoding:[NSString defaultCStringEncoding]];
					Callbacks::invoke(callbackId);
					[topWindow orderFrontRegardless];
					[topWindow makeKeyWindow];
					ARK2D::getInput()->mouse_x = -1;
					ARK2D::getInput()->mouse_y = -1;
					return;
				} else if (button == NSAlertAlternateReturn) {
					s_inputDialogText = defaultStr;
					[topWindow orderFrontRegardless];
					[topWindow makeKeyWindow];
					ARK2D::getInput()->mouse_x = -1;
					ARK2D::getInput()->mouse_y = -1;
					return;
				} else {
					ARK2D::getLog()->w("Invalid input dialog button");
					[topWindow orderFrontRegardless];
					[topWindow makeKeyWindow];
					ARK2D::getInput()->mouse_x = -1;
					ARK2D::getInput()->mouse_y = -1;
					return;
				}


			#elif defined(ARK2D_WINDOWS)

				//ErrorDialog::createAndShow("not implemented");
				//return;

				HINSTANCE dllModule = LoadLibrary("libARK2D.dll");
				if (dllModule == NULL) {
					ErrorDialog::createAndShow("Could not load Input Dialog");
				} else {

					int ret = DialogBox(
						dllModule, //ARK2D::getContainer()->m_platformSpecific.m_hInstance, 	// handle to application instance
						MAKEINTRESOURCE(ARK2D_DIALOG_INPUT_TEXT), 				// identifies dialog box template
						ARK2D::getContainer()->m_platformSpecific.m_hWindow,	// handle to owner window
						(DLGPROC) MyStaticDialogProc 							// pointer to dialog box procedure
					);
					ARK2D::getLog()->t(StringUtil::append("DialogBox ret: ", ret));
					if (ret == -1) {
						ARK2D::getLog()->t(StringUtil::append("DialogBox ret 2: ", GetLastError()));
					}

					if (ret == IDOK) {
						s_inputDialogText = string(mystaticdialogproc_szItemName);
						Callbacks::invoke(callbackId);
					} else {
						s_inputDialogText = defaultStr;
					}
				}
				FreeLibrary(dllModule);

				return;

			#elif defined(ARK2D_ANDROID)
				ARK2D::getContainer()->m_platformSpecific.m_pluggable->openInputDialog(callbackId, title, defaultStr);
				return;
			#endif
		}
		string Dialog::getInputDialogText() {




			#if defined(ARK2D_FLASCC)

				const char* inputText = NULL;
 				inline_as3(
					"import com.adobe.flascc.Console;\n"\
					"%0 = Console.s_console.getInputDialogText();\n"
					: "=r"(inputText) :
				);

 				string inputTextNew = string(inputText);
 				ARK2D::getLog()->t(inputTextNew);

			#elif defined(ARK2D_ANDROID)
				return ARK2D::getContainer()->m_platformSpecific.m_pluggable->getInputDialogText();
			#endif
			return s_inputDialogText;
		}

	}
}
