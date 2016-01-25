/*
 * GameContainerAndroid.h
 *
 *  Created on: Feb 13, 2012
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERANDROID_H_
#define GAMECONTAINERANDROID_H_

#include "../../ARK2D.h"


namespace ARK {
	namespace Core {

		class GameContainer;
		using namespace std;

		class AndroidPluggable {
			public:
				string m_keyChar;
				virtual string urlRequest(JNIEnv* env, string url);
				virtual string urlRequestThreaded(string url);
				virtual bool isNetworkAvailable();
				
				virtual void openBrowserToUrl(string url);
				virtual void openGalleryToImageUrl(string url); 
				virtual void openGooglePlayStore(string packagename);
				virtual void openErrorDialog(string message);
				virtual void openSoftwareKeyboard();
				virtual void closeSoftwareKeyboard();
				virtual void openInputDialog(unsigned int jniCallbackId, string title, string defaultStr);
				virtual string getInputDialogText();

				virtual bool vibrator_hasVibrator(); 
				virtual void vibrator_vibrate(int millis);
    			virtual void vibrator_cancel();

				virtual void ga_sendSocial(string network, string action, string targeturl);
				virtual void ga_sendEvent(string category, string action, string label, long opt_value);
				virtual void ga_sendTiming(long loadTime, string category, string name, string label);

				virtual void share_googleplus(string text);
				virtual void thread_start(unsigned int thread_id);

				virtual void googleplaygameservices_signIn();
				virtual void googleplaygameservices_signOut();
				virtual bool googleplaygameservices_isSignedIn();
				virtual bool googleplaygameservices_isSigningIn();
				virtual void googleplaygameservices_viewAchievements();
				virtual void googleplaygameservices_unlockAchievement(string id);
				virtual void googleplaygameservices_viewScores(string id);
				virtual void googleplaygameservices_submitScore(string id, int score);

				virtual void container_close();
				//virtual bool googleplaygameservices_isConnected();
				//virtual bool googleplaygameservices_isConnecting();

				virtual bool ouya_isOuya();
				virtual void ouya_requestPurchase(string name);
				virtual string ouya_getUsername();

				virtual bool firetv_isAmazonFireTV();
				virtual string firetv_getUsername();
				virtual void firetv_viewAchievements();
				virtual void firetv_unlockAchievement(string id);
				virtual void firetv_viewScores();
				virtual void firetv_viewScores(string id);
				virtual void firetv_submitScore(string id, int score);
		};


		class GameContainerPlatform {
			friend class GameContainer;

			public:
				GameContainer* m_container;
				AndroidPluggable* m_pluggable;
				string m_externalDataStr;

				static bool s_nativeResizing;
				static bool s_gamePaused;

				unsigned int m_gamepadId;

				JavaVM* m_jvm;

			public:
				GameContainerPlatform();
				void setTitle(string title);
				void initGL(string clearColorStr, int w, int h);
				void initGL2D(int w, int h);
				bool initOpenAL();
				bool deinitOpenAL();
				string getResourcePath() const;
				string getExternalResourcePath() const;
				virtual ~GameContainerPlatform();

				inline AndroidPluggable* getPluggable() { return m_pluggable; }
				inline bool isRetina() { return false; } 

				static void* getARK2DResource(int resourceId, int resourceType);
		};

		

	}
}

#endif /* GAMECONTAINERANDROID_H_ */
