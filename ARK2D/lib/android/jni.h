
#include <jni.h>
#include "ARK.h"

#ifndef INCLUDED_TEST_PROJECT_H
#define INCLUDED_TEST_PROJECT_H

	#include <string>
	using namespace std;
	class MyAndroidPluggable : public ARK::Core::AndroidPluggable {
		public:
			virtual string urlRequest(JNIEnv* env, string url);
			virtual string urlRequestThreaded(string url); // android native threads must have be attached to the VM to use it.
			virtual bool isNetworkAvailable();
			virtual void openBrowserToUrl(string url);
			virtual void openGalleryToImageUrl(string url);
			virtual void openGooglePlayStore(string packageName);
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

			virtual bool googleplaybilling_isSetup();
			virtual void googleplaybilling_startPurchase(string id, int referenceNumber, string extraToken);
			virtual bool googleplaybilling_hasPurchase(string sku);
			virtual void googleplaybilling_consumePurchase(string id);
			virtual void googleplaybilling_queryPurchases();

			virtual bool ironsource_isRewardedAdAvailable();
			virtual void ironsource_showRewardedAd(string placement);

			virtual string getISO6391Language();

			//virtual bool googleplaygameservices_isConnected();
			//virtual bool googleplaygameservices_isConnecting();
			virtual void container_close();

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


	#ifdef __cplusplus
		extern "C" {
	#endif
		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeInit
		 * Signature: ()V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeInit
		  (JNIEnv *, jclass, jstring, jstring);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeResize
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResize
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeRender
		 * Signature: ()V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeRender
		  (JNIEnv *, jclass);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativePause
		 * Signature: ()V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativePause
		  (JNIEnv *, jclass);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeResume
		 * Signature: ()V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResume
		  (JNIEnv *, jclass);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeBackPressed
		 * Signature: ()V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeBackPressed
		  (JNIEnv *, jclass);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeCallbackById
		 * Signature: (I)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeCallbackById
		  (JNIEnv *, jclass, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeCallbackByIdIntParam
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeCallbackByIdIntParam
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeCallbackByIdStringParam
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeCallbackByIdStringParam
		  (JNIEnv *, jclass, jint, jstring);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeErrorDialog
		 * Signature: (Ljava.lang.String)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeErrorDialog
		  (JNIEnv *, jclass, jstring);


		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeTouchDown
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchDown
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeTouchMove
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchMove
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeTouchUp
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchUp
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeKeyDown
		 * Signature: (I)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeKeyDown
		  (JNIEnv *, jclass, jint, jstring);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeKeyUp
		 * Signature: (I)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeKeyUp
		  (JNIEnv *, jclass, jint, jstring);


		// GAMEPAD BITS
		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadAdd
		 * Signature: (I)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAdd
		  (JNIEnv *, jclass, jint, jstring);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadRemove
		 * Signature: (I)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadRemove
		  (JNIEnv *, jclass, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadRemove
		 * Signature: (I)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadRemove
		  (JNIEnv *, jclass, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadExists
		 * Signature: (I)Z
		 */
		JNIEXPORT bool JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadExists
		  (JNIEnv *, jclass, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadNumAxes
		 * Signature: (I)I
		 */
		JNIEXPORT int JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadNumAxes
		  (JNIEnv *, jclass, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadAddAxis
		 * Signature: (III)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAddAxis
		  (JNIEnv *, jclass, jint, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadAxisAtIndex
		 * Signature: (II)I
		 */
		JNIEXPORT int JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAxisAtIndex
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadAxisChanged
		 * Signature: (IIF)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAxisChanged
		  (JNIEnv *, jclass, jint, jint, jfloat);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadKeyDown
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadKeyDown
		  (JNIEnv *, jclass, jint, jint);

		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeGamepadKeyUp
		 * Signature: (II)V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadKeyUp
		  (JNIEnv *, jclass, jint, jint);


	#ifdef __cplusplus
		}
	#endif



#endif
