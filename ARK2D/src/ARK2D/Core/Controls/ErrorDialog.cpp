/*
 * ErrorDialog.cpp
 *
 *  Created on: 23-Dec-2009
 *      Author: Ashley
 */


#include "ErrorDialog.h"
#include "../ARK2D.h"

#include "../GameContainer.h"
#include "../Graphics/TextureStore.h"
#include "../Graphics/ShaderStore.h"

#include "../Log.h"

namespace ARK {
	namespace Core {
		namespace Controls {

			void ErrorDialog::createAndShow(string message) {
				#if defined(ARK2D_WINDOWS_PHONE_8) || defined(ARK2D_WINDOWS_STORE)

					ARK2D::getLog()->e(message);

					using namespace Windows::UI::Popups;

					string title = "Error: ";
					string contents = message;
					wchar_t* wideTitle = Cast::charToWideChar(title.c_str());
					wchar_t* wideContents = Cast::charToWideChar(contents.c_str());

					Platform::String^ titleStr = ref new Platform::String(wideTitle);
					Platform::String^ contentsStr = ref new Platform::String(wideContents);
					MessageDialog^ md = ref new MessageDialog(contentsStr, titleStr);
					md->ShowAsync();

					delete wideTitle;
					delete wideContents;

				#elif defined(ARK2D_EMSCRIPTEN_JS)
					ARK2D::getLog()->e(message);

				#elif defined(ARK2D_ANDROID)
					ARK2D::getLog()->e(message);
					if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->ouya_isOuya()) {
						ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->openErrorDialog(message);
					}
				#elif defined(ARK2D_WINDOWS)
					MessageBox(NULL, message.c_str(), NULL, MB_OK | MB_ICONEXCLAMATION);
					ARK2D::getLog()->e(message);
				#elif defined(ARK2D_UBUNTU_LINUX)
					ARK2D::getLog()->e(message);

					#if defined(ARK2D_SDL2)
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error: ", message.c_str(), NULL);
					#else
						string note = "notify-send \"";
						note += message;
						note += "\"";
						int ret = system(note.c_str());
					#endif
				#elif defined(ARK2D_IPHONE)

					NSString* nsmessage = [NSString stringWithCString:message.c_str() encoding:[NSString defaultCStringEncoding]];
					UIAlertView* alert = [[[UIAlertView alloc] initWithTitle: @"Error:" message: nsmessage delegate: NULL cancelButtonTitle: @"OK" otherButtonTitles: NULL] autorelease];
	    			[alert show];


				#elif defined(ARK2D_MACINTOSH)

	    			ARK2D::getLog()->e(message);

	    			const char* messageCStr = message.c_str();
	    			unsigned int messageLen = message.length();

					CFStringRef header_ref   = CFStringCreateWithCString( NULL, "",   strlen("")    );
					CFStringRef message_ref  = CFStringCreateWithCString( NULL, messageCStr,  messageLen );

					CFOptionFlags result;
					CFUserNotificationDisplayAlert(
						0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL,
						header_ref, message_ref,
						NULL, // default button
						NULL, // alternative button 1
						NULL, // anternative button 2
						&result
					);

					if (header_ref != NULL) {  CFRelease( header_ref ); }
					if (message_ref != NULL) {  CFRelease( message_ref ); }

					// kCFUserNotificationDefaultResponse // default button pressed
					// kCFUserNotificationAlternateResponse // alternative button 1 pressed
					// kCFUserNotificationOtherResponse // alternative button 2 pressed.
					// kCFUserNotificationCancelResponse // notification timed out.

					/*
					 * confirm dialog box
					 *
					//convert the strings from char* to CFStringRef
					CFStringRef header_ref    = CFStringCreateWithCString( NULL, "",   strlen("")    );
					CFStringRef message_ref  = CFStringCreateWithCString( NULL, message.c_str(),  strlen(message.c_str()) );

					CFOptionFlags result;  //result code from the message box

					//launch the message box
					CFUserNotificationDisplayAlert(
						0, // no timeout
						kCFUserNotificationNoteAlertLevel, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
						NULL, //icon url, use default, you can change it depending message_type flags
						NULL, //not used
						NULL, //localization of strings
						header_ref, //header text
						message_ref, //message text
						NULL, //default "ok" text in button
						CFSTR("Cancel"), //alternate button title
						NULL, //other button title, null--> no other button
						&result //response flags
					);

					//Clean up the strings
					CFRelease( header_ref );
					CFRelease( message_ref );
					*/

				#endif
			}

			void ErrorDialog::showAnyGlErrorAndExit() {
				showAnyGlErrorAndExit( __FILE__, __LINE__ );
			}
			void ErrorDialog::showAnyGlErrorAndExit(const char* fname, int line) {
				//#ifdef ARK2D_DEBUG
				if (ARK2D::getLog()->getFilter() == ARK::Core::Log::TYPE_ALL)
				{
					#if defined(ARK2D_RENDERER_DIRECTX)
						/*string s = "dx: ";
						s += fname;
						s += " - ";
						s += Cast::toString<int>(line);
						s += ".";
						ARK2D::getLog()->v(s);*/
						return;
					#elif defined(ARK2D_RENDERER_OPENGL)
						RendererStats::s_glCalls++;
						int e = glGetError();
						if (e != GL_NO_ERROR) {
							ARK2D::getLog()->e("-----------!!!-----------");
							ARK2D::getLog()->e("A GL error. :( ");
							ARK2D::getLog()->e("Renderer state:" );
							ARK2D::getLog()->e(ARK2D::getRenderer()->toString());
							TextureStore::getInstance()->print();
	            			ShaderStore::getInstance()->print();

							RendererStats::s_glCalls++;
							string s = getGlErrorString(e);
							s += " : ";
							s += fname;
							s += Cast::toString<int>(line);
							s += ".";
							ErrorDialog::createAndShow(s);
							exit(0);
						}
					#endif
				}
				//#endif
			}

			string ErrorDialog::getGlErrorString(int error) {
				#if defined(ARK2D_RENDERER_OPENGL)
					switch(error) {
						case GL_INVALID_ENUM:
							return "GL_INVALID_ENUM";
							break;
						case GL_INVALID_VALUE:
							return "GL_INVALID_VALUE";
							break;
						#if !defined(ARK2D_ANDROID)
							case GL_INVALID_FRAMEBUFFER_OPERATION:
								return "GL_INVALID_FRAMEBUFFER_OPERATION";
								break;
						#endif
						case GL_INVALID_OPERATION:
							return "GL_INVALID_OPERATION";
							break;
						case GL_OUT_OF_MEMORY:
							return "GL_OUT_OF_MEMORY";
							break;

						/*case GL_STACK_OVERFLOW:
							return "GL_STACK_OVERFLOW";
							break;
						case GL_STACK_UNDERFLOW:
							return "GL_STACK_UNDERFLOW";
							break;
						case GL_TABLE_TOO_LARGE:
							return "GL_TABLE_TOO_LARGE";
							break;*/
					}
					return "GL_UNKNOWN_ERROR";
				#else
					return "DIRECTX_ERROR_NOT_IMPLEMENTED";
				#endif
			}
		}
	}
}
