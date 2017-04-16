/*
 * Input.cpp
 *
 *  Created on: 12-Dec-2009
 *      Author: Ashley
 */

#ifdef __WIN32
	#include <windows.h>
#endif

#include "../../Namespaces.h"

#include "Input.h"
#include "Gamepad.h"
#include "../GameContainer.h"

namespace ARK {
    namespace Core {
        namespace Controls {

            #if !defined(ARK2D_WINDOWS_VS) // && !defined(ARK2D_UBUNTU_LINUX)
                const int Input::MOUSE_BUTTON_LEFT;
                const int Input::MOUSE_BUTTON_MIDDLE;
                const int Input::MOUSE_BUTTON_RIGHT;
                const int Input::KEY_SPACE;
                const int Input::KEY_PERIOD;
                const int Input::KEY_COMMA;
                const int Input::KEY_EQUALS;
                const int Input::KEY_HYPHEN;
                const int Input::KEY_F1;
                const int Input::KEY_F2;
                const int Input::KEY_F3;
                const int Input::KEY_F4;
                const int Input::KEY_F5;
                const int Input::KEY_F6;
                const int Input::KEY_F7;
                const int Input::KEY_F8;
                const int Input::KEY_F9;
                const int Input::KEY_F10;
                const int Input::KEY_F11;
                const int Input::KEY_F12;
                const int Input::KEY_INSERT;
                const int Input::KEY_LSHIFT;
                const int Input::KEY_LCONTROL;
                const int Input::KEY_BACKSPACE;
                const int Input::KEY_TAB;
                const int Input::KEY_ENTER;
                const int Input::MOBILE_BACK;
                const int Input::KEY_0;
                const int Input::KEY_1;
                const int Input::KEY_2;
                const int Input::KEY_3;
                const int Input::KEY_4;
                const int Input::KEY_5;
                const int Input::KEY_6;
                const int Input::KEY_7;
                const int Input::KEY_8;
                const int Input::KEY_9;
                const int Input::KEY_A;
                const int Input::KEY_B;
                const int Input::KEY_C;
                const int Input::KEY_D;
                const int Input::KEY_E;
                const int Input::KEY_F;
                const int Input::KEY_G;
                const int Input::KEY_H;
                const int Input::KEY_I;
                const int Input::KEY_J;
                const int Input::KEY_K;
                const int Input::KEY_L;
                const int Input::KEY_M;
                const int Input::KEY_N;
                const int Input::KEY_O;
                const int Input::KEY_P;
                const int Input::KEY_Q;
                const int Input::KEY_R;
                const int Input::KEY_S;
                const int Input::KEY_T;
                const int Input::KEY_U;
                const int Input::KEY_V;
                const int Input::KEY_W;
                const int Input::KEY_X;
                const int Input::KEY_Y;
                const int Input::KEY_Z;
                const int Input::KEY_LEFT;
                const int Input::KEY_RIGHT;
                const int Input::KEY_UP;
                const int Input::KEY_DOWN;
            #endif

            const char* Input::s_keyboardState = new char[256];

            Input::Input():
                keyDownBuffer(),
                m_container(NULL),
                keyNames(),
                keyChars(),
                mouse_x(0),
                mouse_y(0),
                mouse_prev_x(0),
                mouse_prev_y(0),
                mouse_x_raw(0),
                mouse_y_raw(0),
                pressedEvents(),
                releasedEvents(),
                m_touchPointers()
            {
                keyNames[Input::MOUSE_BUTTON_LEFT] = "left mouse button";
                keyNames[Input::MOUSE_BUTTON_MIDDLE] = "middle mouse button";
                keyNames[Input::MOUSE_BUTTON_RIGHT] = "right mouse button";

                keyNames[Input::KEY_SPACE] = "space bar";
                keyChars[Input::KEY_SPACE] = " ";

                keyNames[Input::KEY_PERIOD] = "period/fullstop";
                keyChars[Input::KEY_PERIOD] = ".";

                keyNames[Input::KEY_COMMA] = "comma";
                keyChars[Input::KEY_COMMA] = ",";

                keyNames[Input::KEY_EQUALS] = "equals";
                keyChars[Input::KEY_EQUALS] = "=";

                keyNames[Input::KEY_HYPHEN] = "hyphen";
                keyChars[Input::KEY_HYPHEN] = "-";

                keyNames[Input::KEY_0] = keyChars[Input::KEY_0] = "0";
                keyNames[Input::KEY_1] = keyChars[Input::KEY_1] = "1";
                keyNames[Input::KEY_2] = keyChars[Input::KEY_2] = "2";
                keyNames[Input::KEY_3] = keyChars[Input::KEY_3] = "3";
                keyNames[Input::KEY_4] = keyChars[Input::KEY_4] = "4";
                keyNames[Input::KEY_5] = keyChars[Input::KEY_5] = "5";
                keyNames[Input::KEY_6] = keyChars[Input::KEY_6] = "6";
                keyNames[Input::KEY_7] = keyChars[Input::KEY_7] = "7";
                keyNames[Input::KEY_8] = keyChars[Input::KEY_8] = "8";
                keyNames[Input::KEY_9] = keyChars[Input::KEY_9] = "9";
                keyNames[Input::KEY_A] = keyChars[Input::KEY_A] = "a";
                keyNames[Input::KEY_B] = keyChars[Input::KEY_B] = "b";
                keyNames[Input::KEY_C] = keyChars[Input::KEY_C] = "c";
                keyNames[Input::KEY_D] = keyChars[Input::KEY_D] = "d";
                keyNames[Input::KEY_E] = keyChars[Input::KEY_E] = "e";
                keyNames[Input::KEY_F] = keyChars[Input::KEY_F] = "f";
                keyNames[Input::KEY_G] = keyChars[Input::KEY_G] = "g";
                keyNames[Input::KEY_H] = keyChars[Input::KEY_H] = "h";
                keyNames[Input::KEY_I] = keyChars[Input::KEY_I] = "i";
                keyNames[Input::KEY_J] = keyChars[Input::KEY_J] = "j";
                keyNames[Input::KEY_K] = keyChars[Input::KEY_K] = "k";
                keyNames[Input::KEY_L] = keyChars[Input::KEY_L] = "l";
                keyNames[Input::KEY_M] = keyChars[Input::KEY_M] = "m";
                keyNames[Input::KEY_N] = keyChars[Input::KEY_N] = "n";
                keyNames[Input::KEY_O] = keyChars[Input::KEY_O] = "o";
                keyNames[Input::KEY_P] = keyChars[Input::KEY_P] = "p";
                keyNames[Input::KEY_Q] = keyChars[Input::KEY_Q] = "q";
                keyNames[Input::KEY_R] = keyChars[Input::KEY_R] = "r";
                keyNames[Input::KEY_S] = keyChars[Input::KEY_S] = "s";
                keyNames[Input::KEY_T] = keyChars[Input::KEY_T] = "t";
                keyNames[Input::KEY_U] = keyChars[Input::KEY_U] = "u";
                keyNames[Input::KEY_V] = keyChars[Input::KEY_V] = "v";
                keyNames[Input::KEY_W] = keyChars[Input::KEY_W] = "w";
                keyNames[Input::KEY_X] = keyChars[Input::KEY_X] = "x";
                keyNames[Input::KEY_Y] = keyChars[Input::KEY_Y] = "y";
                keyNames[Input::KEY_Z] = keyChars[Input::KEY_Z] = "z";

            }

            // --------------------------------------------------
            // MULTI TOUCH!
            // --------------------------------------------------
            Pointer::Pointer() {
                #ifdef ARK2D_IPHONE
                    data = NULL;
                #endif
            }
            float Pointer::getX() {
                return x;
            }
            float Pointer::getY() {
                return y;
            }
            signed int Input::addTouch(Pointer pointer) {
                m_touchPointers.push_back(pointer);
                return (signed int) m_touchPointers.size()-1;
            }
            void Input::removeTouch(int index) {
                m_touchPointers.erase(m_touchPointers.begin() + index);
            }
            signed int Input::countTouches() {
                return (signed int) m_touchPointers.size();
            }
            Pointer* Input::getTouch(signed int index) {
                return &m_touchPointers.at(index);
            }
            #if defined(ARK2D_IPHONE)
                signed int Input::getTouchByInternalData(void* d) {
                    for(signed int i = 0; i < m_touchPointers.size(); i++) {
                        if (d == m_touchPointers.at(i).data) {
                            return i;
                        }
                    }
                    //ARK2D::getLog()->w("could not get touch by internal data.");
                    return -1;
                }
            #elif defined(ARK2D_WINDOWS)
                signed int Input::getTouchByInternalData(unsigned int d) {
                    for(signed int i = 0; i < m_touchPointers.size(); ++i) {
                        if (d == m_touchPointers[i].data) {
                            return i;
                        }
                    }
                    return -1;
                }
            #endif

            // --------
            // Game pads!!!
            // --------
            vector<Gamepad*>* Input::getGamepads() {
                return m_container->getGamepads();
            }
            signed int Input::countGamepads() {
                return m_container->getGamepads()->size();
            }
            Gamepad* Input::getGamepad(unsigned int id) {
                vector<Gamepad*>* gamepads = m_container->getGamepads();
                for(unsigned int i = 0; i < gamepads->size(); ++i) {
                    if (gamepads->at(i)->id == id) {
                        return gamepads->at(i);
                    }
                }

                //ARK2D::getLog()->e(StringUtil::append("Could not get gamepad with id: ", id));
                return NULL;
            }
            Gamepad* Input::getGamepadByIndex(unsigned int index) {
                vector<Gamepad*>* gamepads = ARK2D::getContainer()->getGamepads();
                if (index >= gamepads->size()) {
                    //ARK2D::getLog()->e(StringUtil::append("Could not get gamepad at index: ", id));
                    return NULL;
                }
                return gamepads->at(index);
            }

            bool Input::isGamepadButtonDown(unsigned int button) {
                vector<Gamepad*>* gamepads = getGamepads();
                for (unsigned int i = 0; i < gamepads->size(); i++) {
                    bool r = gamepads->at(i)->isButtonDown(button);
                    if (r) {
                        return true;
                    }
                }
                return false;
            }

            bool Input::isGamepadButtonPressed(unsigned int button) {
                vector<Gamepad*>* gamepads = getGamepads();
                for (unsigned int i = 0; i < gamepads->size(); i++) {
                    bool r = gamepads->at(i)->isButtonPressed(button);
                    if (r) {
                        return true;
                    }
                }
                return false;
            }


            bool Input::isGamepadButtonDown(Gamepad* gamepad, unsigned int button) {
                return gamepad->isButtonDown(button);
            }

            bool Input::isGamepadButtonPressed(Gamepad* gamepad, unsigned int button) {
                return gamepad->isButtonPressed(button);
            }
            bool Input::isAnyGamepadButtonPressed() {
                vector<Gamepad*>* gamepads = getGamepads();
                for (unsigned int i = 0; i < gamepads->size(); i++) {
                    bool r = gamepads->at(i)->isAnyButtonPressed();
                    if (r) {
                        return true;
                    }
                }
                return false;
            }

            bool Input::isKeyDown(unsigned int key) {
                return keyDownBuffer[key];
            }
            bool Input::isKeyPressed(unsigned int key) {
                std::set<int>::iterator it = pressedEvents.find(key);
                if (it != pressedEvents.end()) {
                    return true;
                } else {
                    return false;
                }
            }
            bool Input::isKeyReleased(unsigned int key) {
                std::set<int>::iterator it = releasedEvents.find(key);
                if (it != releasedEvents.end()) {
                    return true;
                } else {
                    return false;
                }
            }

            void Input::pressKey(unsigned int key) {
                this->keyDownBuffer[key] = true;
                pressedEvents.insert(key);

                //ARK2D::getLog()->keyPressed(key);
                ARK2D::getScene()->keyPressed(key);

                //#ifdef ARK2D_UBUNTU_LINUX
                //#ifdef ARK2D_ANDROID
                //	ARK2D::getLog()->e(StringUtil::append("key press: ", getKeyName(key)));
                //#endif
            }
            void Input::releaseKey(unsigned int key) {
                this->keyDownBuffer[key] = false;
                releasedEvents.insert(key);

                //ARK2D::getLog()->keyReleased(key);
                ARK2D::getScene()->keyReleased(key);
            }

            const string Input::getKeyName(unsigned int key) {
                map<int, string>::iterator it = keyNames.find(key);
                if (it != keyNames.end()) {
                    return keyNames[key];
                }
                return string("unknown");
            }
            const string Input::getKeyChar(unsigned int key) {
                /*map<int, string>::iterator it = keyChars.find(key);
                if (it != keyChars.end()) {
                    return keyChars[key];
                }
                return string("");*/

                #if defined(ARK2D_FLASCC)

                    map<int, string>::iterator it = keyChars.find(key);
                    if (it != keyChars.end()) {
                        return keyChars[key];
                    }
                    return string("");

                #elif defined(ARK2D_ANDROID)

                    if (key == (unsigned int) KEY_ENTER) {
                        return "\n";
                    }

                    return ARK2D::getContainer()->m_platformSpecific.m_pluggable->m_keyChar;


                #elif defined( ARK2D_WINDOWS )

                    if (key == (unsigned int) MOUSE_BUTTON_LEFT // left mouse
                        || key == (unsigned int) MOUSE_BUTTON_RIGHT // right mouse
                        || key == (unsigned int) MOUSE_BUTTON_MIDDLE // middle mouse
                        || key == (unsigned int) KEY_BACKSPACE // backspace
                        || key == (unsigned int) KEY_DELETE // backspace
                        || key == (unsigned int) KEY_SHIFT // shift
                        || key == (unsigned int) KEY_LSHIFT // shift
                        || key == (unsigned int) KEY_RSHIFT // shift
                        || key == (unsigned int) KEY_CONTROL // ctrl
                        || key == (unsigned int) KEY_CAPSLOCK // caps
                        || key == (unsigned int) KEY_LWIN // lwin
                        || key == (unsigned int) KEY_RWIN // rwin
                        || key == (unsigned int) KEY_INSERT // insert
                        || key == (unsigned int) KEY_HOME // home
                        || key == (unsigned int) KEY_END // end
                        || key == (unsigned int) KEY_ALT // alt
                        || key == (unsigned int) KEY_PAGEUP // pg up
                        || key == (unsigned int) KEY_NUMLOCK // numlock
                        || key == (unsigned int) KEY_PAGEDOWN // pg down
                        || key == (unsigned int) KEY_SELECT // select key (right click menu)
                        || (key >= (unsigned int) KEY_F1 && key <= (unsigned int) KEY_F12) // Function keys
                        || (key >= (unsigned int) KEY_LEFT && key <= (unsigned int) KEY_DOWN) // arrows: l, u, r, d
                        || key == (unsigned int) KEY_ESCAPE
                        || isKeyDown((unsigned int) KEY_CONTROL)
                        ) {
                        return string("");
                    }

                    unsigned char* out = new unsigned char[3];
                    out[1] = '\0';

                    GetKeyboardState((BYTE*) s_keyboardState);
                    int didConvert = ToAscii(key, MapVirtualKey(key, MAPVK_VK_TO_VSC), (BYTE*) s_keyboardState, (WORD*) out, 0);


                    if (didConvert == 0) {
                        ARK2D::getLog()->e("could not convert key code. returning from map. ");
                        map<int, string>::iterator it = keyChars.find(key);
                        if (it != keyChars.end()) {
                            return keyChars[key];
                        }
                        return "";
                    }

                    //std::cout << out << " - " << (unsigned int)(out[0]) << " - " << key << std::endl;
                    string returnString = "";

                    if (int(out[0]) == 0// other
                            || int(out[0]) <= 26 ) { // uh...

                    } else if (int(out[0]) == KEY_TAB) {
                        returnString.append("\t");
                    } else if (int(out[0]) == KEY_ENTER) {
                        returnString.append("\n");
                    } else if (int(s_keyboardState[VK_CONTROL]) == -127) {

                    } else {
                        returnString.append((char*)out);
                    }

                    //ARK2D::getLog()->e(StringUtil::append("char length: ", returnString.length()));

                    delete out;



                    return returnString;

                #elif defined(ARK2D_MACINTOSH)

                    // temp fix for scripting keys.
                    if (isKeyDown(KEY_LSHIFT) || isKeyDown(KEY_RSHIFT)) {
                        switch (key) {
                            case KEY_9:
                                return "(";
                            case KEY_0:
                                return ")";
                            case KEY_HYPHEN:
                                return "_";
                            case KEY_APOSTROPHE:
                                return "\"";
                        }
                    }

                    if (key == (unsigned int) KEY_ENTER) {
                        return "\n";
                    }

                    NSEvent* e = ARK2D::getContainer()->m_platformSpecific.m_listener->m_latestKeyUpEvent;
                    if (e != NULL) {
                        NSString* characterNSString = [e charactersIgnoringModifiers];

                        const char* ptr = [characterNSString cStringUsingEncoding:NSUTF8StringEncoding];
                        return string(ptr);
                    } else {
                        map<int, string>::iterator it = keyChars.find(key);
                        if (it != keyChars.end()) {
                            return keyChars[key];
                        }
                    }

                    //return string("");

                    //! @todo: get this from the OS.

                    //NSString*test = [NSString stringWithCharacters:ch length:unistrlen];

                    /*TISInputSourceRef isref = TISCopyCurrentKeyboardInputSource();
                    CFDataRef uchr = uchr = (CFDataRef)TISGetInputSourceProperty( _isref,  kTISPropertyUnicodeKeyLayoutData );
                    UCKeyboardLayout* keyLayoutPtr = (UCKeyboardLayout *)CFDataGetBytePtr( _uchr );

                    int deadKeyState = 0;
                    int actualLength;

                    OSStatus status = UCKeyTranslate(
                        keyLayoutPtr,
                        key,
                        kUCKeyActionDown,
                        0,//modifierKeyState,
                        LMGetKbdType(),
                        0,
                        &deadKeyState,
                        1, // sizeof(str)/sizeof(unichar),
                        &actualLength,
                        uchr
                    );*/

                    //return (string) uchr;

                    /*unsigned long state;

                    unichar str[10];
                    UInt32 deadKeyState = 0;
                    UniCharCount actualLength;

                    char restype[] = "uchr";
                    ResType* rt = (ResType*)restype;

                    SInt16 currentKeyScript = GetScriptManagerVariable(smKeyScript);
                    SInt16 lastKeyLayoutID = GetScriptVariable(currentKeyScript, smScriptKeys);

                    Handle hh = GetResource(*rt, lastKeyLayoutID);

                    if (hh) {

                        OSStatus status = UCKeyTranslate(
                            keyLayoutPtr,
                            key,
                            kUCKeyActionDown,
                            0,//modifierKeyState,
                            LMGetKbdType(),
                            0,
                            &deadKeyState,
                            1, // sizeof(str)/sizeof(unichar),
                            &actualLength,
                            str
                        );

                    }
                    return string(str[0]);*/


                #endif

                return "";
            }
            bool Input::isKeyAlphanumeric(unsigned int key) {
                return (key >= 48 && key <= 90);
            }
            bool Input::isCapsLockOn() {
                #ifdef __WIN32
                    return ((GetKeyState(KEY_CAPSLOCK) & 0x0001)!=0);
                #endif
                return false;
            }

            void Input::update() {
                if (!mouse_fetched_this_frame) {
                    mouse_fetched_this_x = 0;
                    mouse_fetched_this_y = 0;
                    CGGetLastMouseDelta(&mouse_fetched_this_x, &mouse_fetched_this_y);
                    mouse_fetched_this_frame = true;
                }
            }
            void Input::clearKeyPressedRecord() {
                pressedEvents.clear();
                releasedEvents.clear();
            }
            void Input::endFrame() {
                mouse_prev_x = mouse_x;
                mouse_prev_y = mouse_y;
                mouse_fetched_this_frame = false;

                // Mouse lock.
                if (m_container->isCursorLocked()) {
                    #ifdef ARK2D_MACINTOSH
                        NSWindow* window = m_container->m_platformSpecific.m_window;
                        NSRect windowFrame = [window frame];
                        CGPoint centerOfScreen = CGPointMake(windowFrame.origin.x + (m_container->getDynamicWidth() / 2), windowFrame.origin.y + (m_container->getDynamicHeight() / 2));
                        CGWarpMouseCursorPosition(centerOfScreen);
                    #endif
                }
            }

            int Input::getGlobalMouseX() {
                return m_container->getGlobalMouseX();
            }
            int Input::getGlobalMouseY() {
                return m_container->getGlobalMouseY();
            }

            int Input::getMouseX() const {
                return mouse_x;
            }
            int Input::getMouseY() const {
                return mouse_y;
            }
            int Input::getLastMouseX() const {
                return mouse_prev_x;
            }
            int Input::getLastMouseY() const {
                return mouse_prev_y;
            }
            int Input::getMouseChangeX() const {
                #ifdef ARK2D_MACINTOSH
                    return mouse_fetched_this_x;
                #endif
                return mouse_x - mouse_prev_x;
            }
            int Input::getMouseChangeY() const {
                #ifdef ARK2D_MACINTOSH
                    return mouse_fetched_this_y;
                #endif
                return mouse_y - mouse_prev_y;
            }
            int Input::getMouseXRaw() const {
                return mouse_x_raw;
            }
            int Input::getMouseYRaw() const {
                return mouse_y_raw;
            }
            void Input::setGameContainer(ARK::Core::GameContainer* c) {
                m_container = c;
            }

            void Input::setSoftwareKeyboardOpen(bool b) {
                #if defined( ARK2D_ANDROID )
                    if (b) {
                        ARK2D::getContainer()->m_platformSpecific.m_pluggable->openSoftwareKeyboard();
                    } else {
                        ARK2D::getContainer()->m_platformSpecific.m_pluggable->closeSoftwareKeyboard();
                    }
                #endif
            }


            Input::~Input() {

            }
        }
	}
}
