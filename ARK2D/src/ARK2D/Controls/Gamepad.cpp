 /*
 * Gamepad.cpp
 *
 *  Created on: 8 Nov 2010
 *      Author: Ashley
 */


#include "../Includes.h"

#include "../ARK2D.h"
#include "Gamepad.h" 
#include "../Util/Log.h" 
#include "../Core/GameContainer.h"
#include "../Core/Game.h"
 
#ifdef ARK2D_ANDROID
	#include "../Core/Platform/GameContainerAndroid.h" 
#endif 
   
namespace ARK { 
	namespace Controls {  

		vector<GamepadMapping>* Gamepad::s_gamepadMapping = NULL;
		void Gamepad::initMapping() {
			if (s_gamepadMapping == NULL) { 
				s_gamepadMapping = new vector<GamepadMapping>();
				String* s = Resource::get("ark2d/gamepads.json")->asString();
				if (s != NULL) { 
					JSONNode* root = libJSON::Parse(s->get());
					JSONNode* pads = root->GetNode(ARK2D::getPlatformString());

					for(unsigned int i = 0; i < pads->NodeSize(); i++) {
						JSONNode* pad = pads->Children[i];
						GamepadMapping mapping;
						mapping.name = pad->GetNode("name")->NodeAsString();
						mapping.vendorId  = pad->GetNode("vendorId")->NodeAsInt();
						mapping.productId  = pad->GetNode("productId")->NodeAsInt();
						
						mapping.buttons[(unsigned int) Gamepad::DPAD_LEFT]  = pad->GetNode("buttons")->GetNode("DPAD_LEFT")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::DPAD_RIGHT] = pad->GetNode("buttons")->GetNode("DPAD_RIGHT")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::DPAD_UP] 	 = pad->GetNode("buttons")->GetNode("DPAD_UP")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::DPAD_DOWN]  = pad->GetNode("buttons")->GetNode("DPAD_DOWN")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_A]   = pad->GetNode("buttons")->GetNode("A")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_B]  = pad->GetNode("buttons")->GetNode("B")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_X]  = pad->GetNode("buttons")->GetNode("X")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_Y]  = pad->GetNode("buttons")->GetNode("Y")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_LBUMPER]  = pad->GetNode("buttons")->GetNode("LBUMPER")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_RBUMPER]  = pad->GetNode("buttons")->GetNode("RBUMPER")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_LTRIGGER]  = pad->GetNode("buttons")->GetNode("LTRIGGER")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_RTRIGGER]  = pad->GetNode("buttons")->GetNode("RTRIGGER")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_L3]  = pad->GetNode("buttons")->GetNode("L3")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_R3]  = pad->GetNode("buttons")->GetNode("R3")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_BACK]  = pad->GetNode("buttons")->GetNode("BACK")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_START] = pad->GetNode("buttons")->GetNode("START")->NodeAsInt();
						mapping.buttons[(unsigned int) Gamepad::BUTTON_ACTIVATE] = pad->GetNode("buttons")->GetNode("ACTIVATE")->NodeAsInt();

						mapping.axes[(unsigned int) Gamepad::ANALOG_STICK_1_X] = pad->GetNode("axes")->GetNode("LX")->NodeAsInt();
						mapping.axes[(unsigned int) Gamepad::ANALOG_STICK_1_Y] = pad->GetNode("axes")->GetNode("LY")->NodeAsInt();
						mapping.axes[(unsigned int) Gamepad::ANALOG_STICK_2_X] = pad->GetNode("axes")->GetNode("RX")->NodeAsInt();
						mapping.axes[(unsigned int) Gamepad::ANALOG_STICK_2_Y] = pad->GetNode("axes")->GetNode("RY")->NodeAsInt();
						mapping.axes[(unsigned int) Gamepad::TRIGGER_1] = pad->GetNode("axes")->GetNode("LT")->NodeAsInt();
						mapping.axes[(unsigned int) Gamepad::TRIGGER_2] = pad->GetNode("axes")->GetNode("RT")->NodeAsInt();
						
						mapping.shared_triggers_axis = true;
						JSONNode* sharedTriggersNode = pad->GetNode("shared_triggers_axis");
						if (sharedTriggersNode) {
							mapping.shared_triggers_axis = sharedTriggersNode->NodeAsBool();
						}
						

						mapping.toInverse();

						s_gamepadMapping->push_back(mapping);
						
					}

					
				}
				
			} 
		}
		void GamepadMapping::toInverse() {
			map<unsigned int, signed int>::iterator it;
			for(it = buttons.begin(); it != buttons.end(); it++) {
				buttonsInverse[it->second] = it->first;
			}

			map<unsigned int, signed int>::iterator it2;
			for(it2 = axes.begin(); it2 != axes.end(); it2++) {
				axesInverse[it2->second] = it2->first;
			}
		}
		void GamepadMapping::toRegular() {
			map<signed int, unsigned int>::iterator it;
			for(it = buttonsInverse.begin(); it != buttonsInverse.end(); it++) {
				buttons[it->second] = it->first;
			}

			map<signed int, unsigned int>::iterator it2;
			for(it2 = axesInverse.begin(); it2 != axesInverse.end(); it2++) {
				axes[it2->second] = it2->first;
			}
		}
		string GamepadMapping::toString() {
			string nl = "\r\n";
			string s = "{"; s += nl; 

				s += "	\"name\": \""; s += name; s += "\", "; s += nl;
				s += "	\"vendorId\": "; s += Cast::toString<unsigned int>(vendorId); s += ", "; s += nl;
				s += "	\"productId\": "; s += Cast::toString<unsigned int>(productId); s += ", "; s += nl;

				s += "	\"buttons\": { "; s += nl;

					s += "		\"DPAD_LEFT\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::DPAD_LEFT] ); s += ", "; s += nl;
					s += "		\"DPAD_RIGHT\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::DPAD_RIGHT] ); s += ", "; s += nl;
					s += "		\"DPAD_UP\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::DPAD_UP] ); s += ", "; s += nl;
					s += "		\"DPAD_DOWN\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::DPAD_DOWN] ); s += ", "; s += nl;
					s += "		\"A\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_A] ); s += ", "; s += nl;
					s += "		\"B\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_B] ); s += ", "; s += nl;
					s += "		\"X\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_X] ); s += ", "; s += nl;
					s += "		\"Y\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_Y] ); s += ", "; s += nl;
					s += "		\"LBUMPER\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_LBUMPER] ); s += ", "; s += nl;
					s += "		\"RBUMPER\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_RBUMPER] ); s += ", "; s += nl;
					s += "		\"LTRIGGER\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_LTRIGGER] ); s += ", "; s += nl;
					s += "		\"RTRIGGER\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_RTRIGGER] ); s += ", "; s += nl;
					s += "		\"L3\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_L3] ); s += ", "; s += nl;
					s += "		\"R3\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_R3] ); s += ", "; s += nl;
					s += "		\"BACK\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_BACK] ); s += ", "; s += nl;
					s += "		\"START\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_START] ); s += ", "; s += nl;
					s += "		\"ACTIVATE\": "; s += Cast::toString<signed int>( buttons[(signed int) Gamepad::BUTTON_ACTIVATE] ); s += " "; s += nl;

				s += "	},"; s += nl;
				
				s += "	\"axes\": { "; s += nl;

					s += "		\"LX\": "; s += Cast::toString<signed int>( axes[(signed int) Gamepad::ANALOG_STICK_1_X] ); s += ", "; s += nl;
					s += "		\"LY\": "; s += Cast::toString<signed int>( axes[(signed int) Gamepad::ANALOG_STICK_1_Y] ); s += ", "; s += nl;
					s += "		\"RX\": "; s += Cast::toString<signed int>( axes[(signed int) Gamepad::ANALOG_STICK_2_X] ); s += ", "; s += nl;
					s += "		\"RY\": "; s += Cast::toString<signed int>( axes[(signed int) Gamepad::ANALOG_STICK_2_Y] ); s += ", "; s += nl;
					s += "		\"LT\": "; s += Cast::toString<signed int>( axes[(signed int) Gamepad::TRIGGER_1] ); s += ", "; s += nl;
					s += "		\"RT\": "; s += Cast::toString<signed int>( axes[(signed int) Gamepad::TRIGGER_2] ); s += " "; s += nl;

				s += "	}"; s += nl;
			s += "}"; 
			return s;

			
		
		}

		Gamepad::Gamepad():   
			m_triggersSendBumperEvents(true),
			m_previousLTriggerValue(0.0f),
			m_previousRTriggerValue(0.0f), 
			m_sharedTriggerAxis(true),

			pressedEvents(),
			releasedEvents(),
 
			initted(false),
			id(0),
			productId(0), 
			vendorId(0),
			name(""), 

			#if defined(ARK2D_WINDOWS)

				povXAxisIndex(0),
				povYAxisIndex(0),

			#endif

			dpadPosition(DPAD_NONE),

			numButtons(0),
			buttons(),

			numAxes(0),
			axes()
		{

		}
		int Gamepad::getDPadPosition() {
			return dpadPosition;
		}

		void Gamepad::update() {
 
			float currentLBumperValue = getAxisValue(Gamepad::TRIGGER_1);
			if (currentLBumperValue > 0.5f && m_previousLTriggerValue < 0.5f) {
				pressButton(Gamepad::BUTTON_LBUMPER);
			} else if (currentLBumperValue < 0.5f && m_previousLTriggerValue > 0.5f) {
				releaseButton(Gamepad::BUTTON_LBUMPER);	
			}

			float currentRBumperValue = getAxisValue(Gamepad::TRIGGER_2);
			if (currentRBumperValue > 0.5f && m_previousRTriggerValue < 0.5f) {
				pressButton(Gamepad::BUTTON_RBUMPER);
			} else if (currentRBumperValue < 0.5f && m_previousRTriggerValue > 0.5f) {
				releaseButton(Gamepad::BUTTON_RBUMPER);	
			} 
			m_previousLTriggerValue = currentLBumperValue;
			m_previousRTriggerValue = currentRBumperValue;
			
		}

		void Gamepad::clearButtonPressedRecord() {
			pressedEvents.clear();
			releasedEvents.clear();
		}
		bool Gamepad::hasButton(unsigned int b) {
			#if defined(ARK2D_ANDROID)
				bool found = false;
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 
				return found;
			#else 
				bool found = false;
				unsigned int nb = convertButtonToId(this, b);
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 
				return found;
			#endif
		}
		void Gamepad::moveAxis(unsigned int axis, float val) {
			bool found = false;
			for(unsigned int i = 0; i < axes.size(); ++i) {
				if (axes.at(i)->id == axis) {
					axes.at(i)->value = val;
					found = true;
					break;
				}
			} 

			if (!found) {
				ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad axis (on move) with id: ", axis));
				return;
			}

			Game* g = ARK2D::getGame();
			GamepadListener* gl = NULL;
			gl = dynamic_cast<GamepadListener*>(g);
			if (gl != NULL) {
				gl->axisMoved(this, axis, val);
			}
		}
		void Gamepad::pressButton(unsigned int b) {

			#if defined(ARK2D_ANDROID)
				bool found = false;
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on press) with id: ", b));
					return;
				}
				pressedEvents.insert(b);

			#else

				//ARK2D::getLog()->v(StringUtil::append("buttons size: ", buttons.size()));
				bool found = false;
				unsigned int nb = convertButtonToId(this, b);
				
				#if defined(ARK2D_WINDOWS)
					ARK2D::getLog()->i(StringUtil::append("button press initial: ", b));
					ARK2D::getLog()->i(StringUtil::append("button press converted: ", nb));
				#endif

				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						buttons.at(i)->down = true;
						found = true;
						break;
					}
				} 

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on press) with id: ", b));
					return;
				}

				//buttons[b]->down = true;
				pressedEvents.insert(b);

				Game* g = ARK2D::getGame();
				GamepadListener* gl = NULL;
				gl = dynamic_cast<GamepadListener*>(g);
				if (gl != NULL) {
					gl->buttonPressed(this, b);
				}
			#endif
		} 

		void Gamepad::releaseButton(unsigned int b) {
			#if defined(ARK2D_ANDROID)
				bool found = false;
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						buttons.at(i)->down = false;
						found = true;
						break;
					}
				} 

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on release) with id: ", b));
					return;
				}
				releasedEvents.insert(b);

			#else

				bool found = false;
				unsigned int nb = convertButtonToId(this, b, false);
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						buttons.at(i)->down = false;
						found = true; 
						break;
					}
				}

				if (!found && b != BUTTON_NULL) {
					ARK2D::getLog()->e(StringUtil::append("Could not find Gamepad button (on release) with id: ", b));
					return;
				}

				//buttons[b]->down = false;
				releasedEvents.insert(b);

				Game* g = ARK2D::getGame();
				GamepadListener* gl = NULL;
				gl = dynamic_cast<GamepadListener*>(g);
				if (gl != NULL) {
					gl->buttonReleased(this, b); 
				}
			#endif
		}

		bool Gamepad::isButtonPressed(unsigned int b) {
			std::set<int>::iterator it = pressedEvents.find(b);
			if (it != pressedEvents.end()) {
				return true;
			} else {
				return false;
			}
		}
		bool Gamepad::isAnyButtonPressed() {
			return (pressedEvents.size() > 0);
		}

		bool Gamepad::isButtonReleased(unsigned int b) {
			std::set<int>::iterator it = releasedEvents.find(b);
			if (it != releasedEvents.end()) {
				return true;
			} else {
				return false;
			}
		}

		bool Gamepad::isButtonDown(unsigned int b) {
			#if defined(ARK2D_ANDROID)
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == b) {
						return buttons.at(i)->down;
					}
				}
				return false; 
			#else 
				//return buttons[b]->down;
				unsigned int nb = convertButtonToId(this, b, false);
				for(unsigned int i = 0; i < buttons.size(); ++i) {
					if (buttons.at(i)->id == nb) {
						return buttons.at(i)->down;
					}
				}
				return false; 
				//return buttonStates.at(b);
				//return buttonDownBuffer[b];
			#endif
		}

		bool Gamepad::isAxis(unsigned int axisId) {
			unsigned int newAxisId = convertAxisToId(this, axisId);
			for(unsigned int i = 0; i < axes.size(); ++i) {
				if (axes.at(i)->id == newAxisId) {
					return true;
				}
			}
			return false;
		}
		float Gamepad::getAxisValue(unsigned int axisId) {

			unsigned int newAxisId = axisId; //convertAxisToId(this, axisId);
			for(unsigned int i = 0; i < axes.size(); ++i) {
				if (axes.at(i)->id == newAxisId) { 
 					return axes.at(i)->value; 
				}
			}
			return 0.0f;
		}

		GamepadMapping* Gamepad::getMapping() {
			unsigned int len = s_gamepadMapping->size();
			for(unsigned int i = 0; i < len; ++i) 
			{
				GamepadMapping* mapping = &s_gamepadMapping->at(i);
				if (mapping->vendorId == this->vendorId && mapping->productId == this->productId) {
					return mapping;
				}
			}
			ARK2D::getLog()->e("Could not get mapping for gamepad. returning null.");
			return NULL;
		}
		unsigned int Gamepad::convertSystemButtonToARKButton(Gamepad* gamepad, unsigned int button) {
			if (s_gamepadMapping == NULL) { ARK2D::getLog()->e("GamepadMapping not initialised."); return button; }

			#if defined(ARK2D_WINDOWS) || defined(ARK2D_MACINTOSH)
				bool foundControllerMapping = false;
				unsigned int len = s_gamepadMapping->size();
				for(unsigned int i = 0; i < len; ++i) {
					const GamepadMapping& mapping = s_gamepadMapping->at(i);
					if (mapping.vendorId == gamepad->vendorId && 
						mapping.productId == gamepad->productId) {
						foundControllerMapping = true;

						map<signed int, unsigned int>::const_iterator it = mapping.buttonsInverse.find(button);
						if (it != mapping.buttonsInverse.end()) {
							return it->second;
						}
					}
				}
				//if (!foundControllerMapping) {
					//ARK2D::getLog()->w("Mapping not found for this controller/button.");
				//}

				/*bool isSaitekCyborgRumble = StringUtil::str_contains(gamepad->name, "Saitek Cyborg Rumble Pad");
				if (isSaitekCyborgRumble) { 
					switch (button) {
						case 1:
							return BUTTON_A; 
						case 2:
							return BUTTON_B;
						case 0:
							return BUTTON_X; 
						case 3:
							return BUTTON_Y;
					}
				}*/
			#elif defined(ARK2D_ANDROID)
				if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->firetv_isAmazonFireTV()) {
					switch(button) {
						case 106:
							return BUTTON_L3;
						case 107:
							return BUTTON_R3;
						case 96:
							return BUTTON_A;
						case 97:
							return BUTTON_B;
						case 99:
							return BUTTON_X;
						case 100:
							return BUTTON_Y;
						case 102:
							return BUTTON_LBUMPER;
						case 103:
							return BUTTON_RBUMPER;
					}
					return id; 
				} else if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->ouya_isOuya()) {
					switch(button) {
						case 19:
							return DPAD_UP;
						case 20:
							return DPAD_DOWN;
						case 21:
							return DPAD_LEFT;
						case 22:
							return DPAD_RIGHT;
						case 82:
							return BUTTON_ACTIVATE;
						case 102:
							return BUTTON_LBUMPER;
						case 103:
							return BUTTON_RBUMPER;
						case 108:
							return BUTTON_START;
						case 96:
							return BUTTON_A;
						case 97:
							return BUTTON_B;
						case 99: 
							return BUTTON_X;
						case 100:
							return BUTTON_Y;
						case 106:
							return BUTTON_L3;
						case 107:
							return BUTTON_R3;
						case 104:
							return BUTTON_LTRIGGER;
						case 105:
							return BUTTON_RTRIGGER;

					}
				} else { 
					switch(button) {
						case 104:
							return DPAD_UP;
						case 105:
							return DPAD_DOWN;
						case 109:
							return DPAD_LEFT;
						case 108:
							return DPAD_RIGHT;
						case 102:
							return BUTTON_L3;
						case 103:
							return BUTTON_R3;
						case 96:
							return BUTTON_A;
						case 99:
							return BUTTON_B;
						case 97:
							return BUTTON_X;
						case 98:
							return BUTTON_Y;
						case 100:
							return BUTTON_LBUMPER;
						case 101:
							return BUTTON_RBUMPER;
						//case 110:
						//	return TRIGGER_1;
						//case 106:
						//	return TRIGGER_2;
						case 107: 
							return BUTTON_ACTIVATE;
					}
				}
			
			#endif
				return button;
		}
		unsigned int Gamepad::convertSystemAxisToARKAxis(Gamepad* gamepad, unsigned int axis) {
			#if defined(ARK2D_WINDOWS) || defined(ARK2D_MACINTOSH)
				if (s_gamepadMapping == NULL) { ARK2D::getLog()->e("GamepadMapping not initialised."); return axis; }
				unsigned int len = s_gamepadMapping->size();
				for(unsigned int i = 0; i < len; ++i) {
					const GamepadMapping& mapping = s_gamepadMapping->at(i);
					if (mapping.vendorId == gamepad->vendorId && 
						mapping.productId == gamepad->productId) {

						map<signed int, unsigned int>::const_iterator it = mapping.axesInverse.find(axis);
						if (it != mapping.axesInverse.end()) {
							return it->second;
						}

					}
				}
			#endif
			return axis;
		}

		// Fire TV
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 0 to gamepad id: 14 at index: 0 (X1)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 1 to gamepad id: 14 at index: 1 (Y1)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 11 to gamepad id: 14 at index: 2 (X2)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 14 to gamepad id: 14 at index: 3 (Y2)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 22 to gamepad id: 14 at index: 4 (RT)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 23 to gamepad id: 14 at index: 5 (LT)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 15 to gamepad id: 14 at index: 6 (DPAD X)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 16 to gamepad id: 14 at index: 7 (DPAD Y)
		// 12-01 12:39:41.111: I/ARK2D(10663): : Add axis 32 to gamepad id: 14 at index: 8 (no idea...)




		unsigned int Gamepad::convertAxisToId(Gamepad* gamepad, unsigned int axis) {
			#if defined(ARK2D_MACINTOSH)
				bool isPS3 = StringUtil::str_contains(gamepad->name, "PLAYSTATION(R)3");
				if (isPS3) {
					if (axis == TRIGGER_1) { 
						return 14;  
					} else if (axis == TRIGGER_2) {
						return 15;
					} 
				}
				return axis;
			#elif defined(ARK2D_WINDOWS) 
				switch(axis) {
					case ANALOG_STICK_2_X: 
						return TRIGGER_1;
					case TRIGGER_1:
						return ANALOG_STICK_2_X;
					
				} 
			#elif defined(ARK2D_ANDROID)
				
				if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->firetv_isAmazonFireTV()) {
					if (axis == TRIGGER_1) {
						return 5;
					} else if (axis == TRIGGER_2) {
						return 4;
					} 
				} 

				/*if (axis == ANALOG_STICK_1_X) {
					return axis;
				} else if (axis == ANALOG_STICK_1_Y) {
					return axis;
				} else if (axis == ANALOG_STICK_2_X) {
					return TRIGGER_1;
				} else if (axis == ANALOG_STICK_2_Y) {
					return ANALOG_STICK_2_X;
				} else if (axis == TRIGGER_1) {
					return ANALOG_STICK_2_Y;
				} else if (axis == TRIGGER_2) {
					return axis;
				}*/

				if (axis == ANALOG_STICK_1_X) {
					return axis;
				} else if (axis == ANALOG_STICK_1_Y) {
					return axis;
				} else if (axis == ANALOG_STICK_2_X) {
					return ANALOG_STICK_2_Y;
				} else if (axis == ANALOG_STICK_2_Y) {
					return TRIGGER_1;
				} else if (axis == TRIGGER_1) {
					return ANALOG_STICK_2_X;
				} else if (axis == TRIGGER_2) {
					return axis;
				}


				/*static const unsigned int ANALOG_STICK_1_X = 0;
				static const unsigned int ANALOG_STICK_1_Y = 1;
				static const unsigned int ANALOG_STICK_2_X = 2;
				static const unsigned int ANALOG_STICK_2_Y = 3;
				static const unsigned int TRIGGER_1 = 4;
				static const unsigned int TRIGGER_2 = 5;

				10-20 20:30:55.583: I/ARK2D(24226): Index: 0. Axis Id: 0. Value: 0.027451.		// 0, LS_X
				10-20 20:30:55.583: I/ARK2D(24226): Index: 1. Axis Id: 1. Value: 0.00392163.	// 1, LS_Y
				10-20 20:30:55.583: I/ARK2D(24226): Index: 2. Axis Id: 11. Value: -1.			// 2, LT
				10-20 20:30:55.583: I/ARK2D(24226): Index: 3. Axis Id: 12. Value: 0.00392163.	// 3, RS_X
				10-20 20:30:55.583: I/ARK2D(24226): Index: 4. Axis Id: 13. Value: 0.00392163.	// 4, RS_Y
				10-20 20:30:55.583: I/ARK2D(24226): Index: 5. Axis Id: 14. Value: -1.			// 5, RT
*/


			#endif
			return axis;
		}

		// CONTROLLER KEY IDS (android)
			// DPAD_UP 		104
			// DPAD_DOWN 	105
			// DPAD_LEFT	109
			// DPAD_RIGHT	108
			// L3			102
			// R3			103
			// BUTTON_A 	96 		(button o)
			// BUTTON_B 	99 		(button a)
			// BUTTON_X		97		(button u)
			// BUTTON_Y		98		(button y)
			// LBUMPER		100
			// RBUMPER		101
			// LTRIGGER		110
			// RTRIGGER		106
			// ACTIVATE		107		(button OUYA)
		// CONTROLLER KEY IDS (OUYA)

		// CONTROLLER KEY IDS (android / firetv)
			// L3			106
			// R3			107
			// BUTTON_A 	96 		
			// BUTTON_B 	97 		
			// BUTTON_X		99		
			// BUTTON_Y		100		
			// LBUMPER		102
			// RBUMPER		103

		//unsigned int Gamepad::convertIdToButton(Gamepad* gamepad, unsigned int id) {
			
		//}

		

		unsigned int Gamepad::convertButtonToId(Gamepad* gamepad, unsigned int button, bool printInfo) {
			
			#if defined(ARK2D_ANDROID)
				if (ARK2D::getContainer()->getPlatformSpecific()->getPluggable()->ouya_isOuya()) {
					switch (button) {
						case DPAD_UP:
							return 19;
						case DPAD_DOWN:
							return 20;
						case DPAD_LEFT:
							return 21;
						case DPAD_RIGHT:
							return 22;
						case BUTTON_LBUMPER:
							return 102;
						case BUTTON_RBUMPER:
							return 103;
						case BUTTON_START:
							return 108;
						case BUTTON_A:
							return 96;
						case BUTTON_B:
							return 97;
						case BUTTON_X: 
							return 99;
						case BUTTON_Y:
							return 100;
						case BUTTON_L3:
							return 106;
						case BUTTON_R3:
							return 107; 
						case BUTTON_LTRIGGER:
							return 104;
						case BUTTON_RTRIGGER:
							return 105;
						case BUTTON_ACTIVATE:
							return 82;
					}
				} else { 
					switch(button) {
						case DPAD_UP: 
							return 104;
						case DPAD_DOWN:
							return 105;
						case DPAD_LEFT:
							return 109;
						case DPAD_RIGHT:
							return 108;
						case BUTTON_L3:
							return 102;
						case BUTTON_R3:
							return 103;
						case BUTTON_A:
							return 96;
						case BUTTON_B:
							return 99;
						case BUTTON_X:
							return 97;
						case BUTTON_Y:
							return 98;
						case BUTTON_LBUMPER:
							return 100;
						case BUTTON_RBUMPER:
							return 101;
						//case 110:
						//	return TRIGGER_1;
						//case 106:
						//	return TRIGGER_2;
						case BUTTON_ACTIVATE: 
							return 107;
					}
				}
			#endif
			return button;
		}

		#define REG_STRING_MAX 255

		#ifdef ARK2D_WINDOWS
			const char* Gamepad::getGamepadDescription(unsigned int joystickID, JOYINFOEX into, JOYCAPS caps) {

				char * description = "";
				char subkey[REG_STRING_MAX];
				HKEY topKey, key;
				LONG result;

				snprintf(subkey, REG_STRING_MAX, "%s\\%s\\%s", REGSTR_PATH_JOYCONFIG, caps.szRegKey, REGSTR_KEY_JOYCURR);
					result = RegOpenKeyEx(topKey = HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &key);
					if (result != ERROR_SUCCESS) {
						result = RegOpenKeyEx(topKey = HKEY_CURRENT_USER, subkey, 0, KEY_READ, &key);
					}
					if (result == ERROR_SUCCESS) {
						char value[REG_STRING_MAX];
						char nam[REG_STRING_MAX];
						DWORD nameSize;

						snprintf(value, REG_STRING_MAX, "Joystick%d%s", joystickID + 1, REGSTR_VAL_JOYOEMNAME);
						nameSize = sizeof(nam);
						result = RegQueryValueEx(key, value, NULL, NULL, (LPBYTE) nam, &nameSize);
						RegCloseKey(key);

						if (result == ERROR_SUCCESS) {
							snprintf(subkey, REG_STRING_MAX, "%s\\%s", REGSTR_PATH_JOYOEM, nam);
							result = RegOpenKeyEx(topKey, subkey, 0, KEY_READ, &key);

							if (result == ERROR_SUCCESS) {
								nameSize = sizeof(nam);
								result = RegQueryValueEx(key, REGSTR_VAL_JOYOEMNAME, NULL, NULL, NULL, &nameSize);

								if (result == ERROR_SUCCESS) {
									description = (char*) malloc(nameSize);
									result = RegQueryValueEx(key, REGSTR_VAL_JOYOEMNAME, NULL, NULL, (LPBYTE) description, &nameSize);
								}
								RegCloseKey(key);

								if (result == ERROR_SUCCESS) {
									return description;
								}
								free(description);
							}
						}
					}

					description = (char*) malloc(strlen(caps.szPname) + 1);
					strcpy(description, caps.szPname);

					return description;

			}
		#elif defined(ARK2D_MACINTOSH)
			string Gamepad::getGamepadDescription(IOHIDDeviceRef device) {
				CFStringRef cfProductName;
				string s = "";

				CFStringRef cfProductKey = CFSTR(kIOHIDProductKey);
				cfProductName = (CFStringRef) IOHIDDeviceGetProperty(device, cfProductKey);
				if (cfProductName == NULL || CFGetTypeID(cfProductName) != CFStringGetTypeID()) {
					s = string("Unknown");
				} else {

					const char* controllerName = CFStringGetCStringPtr(cfProductName, CFStringGetSmallestEncoding(cfProductName));
					if (controllerName != NULL) {
						s += string(controllerName);
					}

				}
				return s;
			}

			unsigned int Gamepad::convertCookieToButton(unsigned int cookie) {
				bool isPS3 = StringUtil::str_contains(this->name, string("PLAYSTATION(R)3"));
				//ARK2D::getLog()->v("name...");
				//ARK2D::getLog()->v(this->name);
				if (isPS3) {
					//ARK2D::getLog()->v("ps3 convertCookieToButton");
					switch(cookie) {
						case 7:
							return BUTTON_BACK;
						case 8:
							return BUTTON_L3;
						case 9:
							return BUTTON_R3;
						case 10:
							return BUTTON_START;
						case 11:
							return DPAD_UP;
						case 12: 
							return DPAD_RIGHT;
						case 13:
							return DPAD_DOWN;
						case 14:
							return DPAD_LEFT;
						case 15: 
							// PS3 LEFT TRIGGER
							return BUTTON_NULL;
						case 16: 
							// PS3 RIGHT TRIGGER
							return BUTTON_NULL;
						case 17:
							return BUTTON_LBUMPER;
						case 18:
							return BUTTON_RBUMPER; 
						case 19:
							return BUTTON_Y;
						case 20:
							return BUTTON_B;
						case 21:
							return BUTTON_A;
						case 22:
							return BUTTON_X;
						case 23: 
							return BUTTON_ACTIVATE;
					}
					//ARK2D::getLog()->w("Button press on PS3 controller not recognised.");
					return BUTTON_ACTIVATE;
				} 
				switch(cookie) {
					case 10:
						return DPAD_UP;
					case 11:
						return DPAD_DOWN;
					case 12:
						return DPAD_LEFT;
					case 13: 
						return DPAD_RIGHT;
					case 14:
						return BUTTON_START;
					case 15:
						return BUTTON_BACK;
					case 16:
						return BUTTON_L3;
					case 17:
						return BUTTON_R3;
					case 18:
						return BUTTON_LBUMPER;
					case 19:
						return BUTTON_RBUMPER; 
					case 20: 
						return BUTTON_ACTIVATE;
					case 21:
						return BUTTON_A;
					case 22:
						return BUTTON_B;
					case 23:
						return BUTTON_X;
					case 24:
						return BUTTON_Y;
				}
				return BUTTON_ACTIVATE;
			}

			
		#elif defined(ARK2D_UBUNTU_LINUX)
			const char* Gamepad::getGamepadDescription(unsigned int joystickID) {
				string burp = ARK2D::getInput()->getGamepad(joystickID)->description;
				return burp.c_str();
			}
		#else
			const char* Gamepad::getGamepadDescription(unsigned int joystickID) {
				return "gamepad desc";
			}
		#endif

		/*
		double Gamepad::currentTime() {
			// HACK: No timestamp data from joyGetInfoEx, so we make it up
			static LARGE_INTEGER frequency;
			LARGE_INTEGER currentTime;

			if (frequency.QuadPart == 0) {
				QueryPerformanceFrequency(&frequency);
			}
			QueryPerformanceCounter(&currentTime);

			return (double) currentTime.QuadPart / frequency.QuadPart;
		}
		*/

		bool Gamepad::isXbox360Controller() {
			bool isXbox360Controller = StringUtil::str_contains(this->name, "XBOX 360");
			bool isXbox360Controller2 = StringUtil::str_contains(this->name, "Xbox 360");
			return (isXbox360Controller || isXbox360Controller2);
		}
		bool Gamepad::isPS4Controller() {
			#if defined(ARK2D_WINDOWS) || defined(ARK2D_MACINTOSH)
				return (this->vendorId == 1356 && this->productId == 1476);
			#endif
			return false;
		}
		bool Gamepad::isConfigured() {
			unsigned int len = s_gamepadMapping->size();
			for(unsigned int i = 0; i < len; ++i) 
			{
				const GamepadMapping& mapping = s_gamepadMapping->at(i);
				if (mapping.vendorId == this->vendorId && 
					mapping.productId == this->productId) 
				{
					return true;
				}
			}
			return false;
		}

		unsigned int Gamepad::s_nextGamepadId = 0;
		unsigned int Gamepad::getNextGamepadId() 
		{
			vector<Gamepad*>* pads = ARK2D::getContainer()->getGamepads();
			if (pads->size() == 0) {
				s_nextGamepadId++;
				return 0;
			}

			unsigned int maxId = 0;
			for(unsigned int i = 0; i < pads->size(); ++i) {
				if (pads->at(i)->id >= maxId) {
					maxId = pads->at(i)->id;
				}
			}

			for(unsigned int i = 0; i <= maxId; ++i) {
				Gamepad* boop = ARK2D::getInput()->getGamepad(i);
				if (boop == NULL) {
					//ARK2D::getLog()->i(StringUtil::append("getNextGamepadId boop: ", i));
					return i;
				}
			}

			//ARK2D::getLog()->i(StringUtil::append("getNextGamepadId return: ", m_gamepadId));
			s_nextGamepadId = maxId + 1;
			return s_nextGamepadId;

		}

		string Gamepad::toString() {

			string nl = "\n";
			string s = "";

			s += "{" + nl;
			s += "	\"id\": \"" + Cast::toString<unsigned int>(id) + "\"," + nl;
			s += "	\"productId\": \"" + Cast::toString<signed int>(productId) + "\"," + nl;
			s += "	\"vendorId\": \"" + Cast::toString<signed int>(vendorId) + "\"," + nl;
			s += "	\"name\": \"" + name + "\"," + nl;
			//s += "	\"description\": \"" + description + "\"," + nl;

			#ifdef ARK2D_ANDROID
				s += "	\"deviceId\": \"" + Cast::toString<unsigned int>(deviceId) + "\"," + nl;
			#endif

			s += "	\"numButtons\": \"" + Cast::toString<unsigned int>(numButtons) + "\"," + nl;
			s += "	\"buttons\": [ "+ nl;
			for(unsigned int i = 0; i < buttons.size(); ++i) {
				s += "		{ "+ nl;
				
				s += "			\"id\": \"" + Cast::toString<unsigned int>(buttons.at(i)->id) + "\"" + nl;
				#ifdef ARK2D_MACINTOSH
					//s += "			\"cookie\": \"" + buttons.at(i)->cookie + "\"," + nl;
				#endif

				if (i != buttons.size() - 1) {
					s += "		},"+ nl;
				} else {
					s += "		}" + nl;
				}
			}
			s += "	], "+ nl;
			
			s += "	\"numAxes\": \"" + Cast::toString<signed int>(numAxes) + "\"," + nl;
			s += "	\"axes\": [ "+ nl;
			for(unsigned int i = 0; i < axes.size(); ++i) {
				s += "		{ "+ nl;
				
				s += "			\"id\": \"" + Cast::toString<unsigned int>(axes.at(i)->id) + "\"," + nl;
				#ifdef ARK2D_WINDOWS
					s += "			\"axisId\": \"" + Cast::toString<unsigned int>(axes.at(i)->axisId) + "\"," + nl;
					s += "			\"rangeMin\": \"" + Cast::toString<unsigned int>(axes.at(i)->rangeMin) + "\"," + nl;
					s += "			\"rangeMax\": \"" + Cast::toString<unsigned int>(axes.at(i)->rangeMax) + "\"," + nl;
				#endif
				s += "			\"attached\": \"true\"" + nl;

				if (i != axes.size() - 1) {
					s += "		},"+ nl;
				} else {
					s += "		}" + nl;
				}
			}
			s += "	]"+ nl;
			s += "}"+ nl;
			return s;
		}
		

		Gamepad::~Gamepad() {

		}
	}
}
