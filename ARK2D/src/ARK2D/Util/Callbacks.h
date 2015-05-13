/*
 * Callbacks.h
 *
 *  Created on: 13 Feb 2013
 *      Author: Ashley
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "../Includes.h"
#include "../Namespaces.h"

namespace ARK {
	namespace Util {

		class ARK2D_API Callback {
			public:
				unsigned int m_id; 
				void* m_functionPointer;
				string m_functionArgs;
				static string ARGS_NONE;

				Callback();
				inline unsigned int getId() { return m_id; }
				inline void setId(unsigned int id) { m_id = id; }
				inline void setFunctionPointer(void* pointer) { m_functionPointer = pointer; }
				inline void setFunctionArgs(string args) { m_functionArgs = args; }
				void invoke();
				void invoke(unsigned int param); 
				void invoke(string strparam); 
		}; 
 
		class ARK2D_API Callbacks {
			public:
				static const unsigned int CALLBACK_ANDROID_LICENSING_ALLOW = 1;
				static const unsigned int CALLBACK_ANDROID_LICENSING_DISALLOW = 2;
				static const unsigned int CALLBACK_ANDROID_SIGNIN_SUCCESSFUL = 11;
				static const unsigned int CALLBACK_ANDROID_SIGNIN_UNSUCCESSFUL = 12;
				static const unsigned int CALLBACK_ANDROID_THREAD_START = 21;

				static const unsigned int CALLBACK_GAMECENTER_SIGNIN_SUCCESSFUL = 31;

				static const unsigned int CALLBACK_OUYA_LICENSING_ALLOW = 41;
				static const unsigned int CALLBACK_OUYA_LICENSING_DISALLOW = 42;

				static const unsigned int CALLBACK_STEAM_OPENURLINBROWSER_LINUX = 51;

				static const unsigned int CALLBACK_GAMECIRCLE_INIT_SUCCESS = 61;
				static const unsigned int CALLBACK_GAMECIRCLE_INIT_FAIL = 62;
				static const unsigned int CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_SUCCESS = 63;
				static const unsigned int CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_FAIL = 64;
				static const unsigned int CALLBACK_GAMECIRCLE_UNLOCKACHIEVEMENT_SUCCESS = 65;
				static const unsigned int CALLBACK_GAMECIRCLE_UNLOCKACHIEVEMENT_FAIL = 66;

				static const unsigned int CALLBACK_WINDOWS_TABLETMODE_STARTED = 71;
				static const unsigned int CALLBACK_WINDOWS_DESKTOPMODE_STARTED = 72;
				static const unsigned int CALLBACK_WINDOWS_ORIENTATION_CHANGED = 73;
				static const unsigned int CALLBACK_WINDOWS_TOUCHMODE_CHANGED = 74;

				static const unsigned int CALLBACK_GAMEJOLT_OVERLAY_EDITNAME = 80;
				static const unsigned int CALLBACK_GAMEJOLT_OVERLAY_EDITTOKEN = 81;
				
			public:
				static vector<ARK::Util::Callback> s_callbacks;
				static void add(Callback c);
				static void invoke(unsigned int id);
				static void invoke(unsigned int id, unsigned int param);
				static void invoke(unsigned int id, string strparam);
		};

	}
}


#endif /* CALLBACKS_H_ */
