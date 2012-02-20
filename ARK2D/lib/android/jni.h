
#include <jni.h>
	
#ifndef INCLUDED_TEST_PROJECT_H
#define INCLUDED_TEST_PROJECT_H
	
	#ifdef __cplusplus
		extern "C" {
	#endif
		/*
		 * Class:     org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer
		 * Method:    nativeInit
		 * Signature: ()V
		 */
		JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeInit
		  (JNIEnv *, jclass, jstring);
		
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
		
	#ifdef __cplusplus
		}
	#endif
#endif
