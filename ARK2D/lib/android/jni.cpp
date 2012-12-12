#include <string.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>
#include <stdint.h>

#include "jni.h"
#include "ARK.h"
#include "%GAME_CLASS_NAME%.h"

GameContainer* container = NULL;
Renderer* r = NULL;
GameTimer* timer = NULL;
%GAME_CLASS_NAME%* game = NULL;
Log* arklog = NULL;
JNIEnv* s_env = NULL;

bool s_initted = false;

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeInit(JNIEnv* env, jclass cls, jstring apkPath, jstring externalDataPath) {
	//__android_log_print(ANDROID_LOG_INFO, "%GAME_CLASS_NAME%Activity", "native init");

	// set globals?
	s_env = env;

	// set apk name
	const char* apkstr;
	jboolean isCopy;
	apkstr = env->GetStringUTFChars(apkPath, &isCopy);
	Resource::apkZipName = string(apkstr);

	// init game stuff
	game = new %GAME_CLASS_NAME%("%GAME_CLASS_NAME%");
	container = new GameContainer(*game, %GAME_WIDTH%, %GAME_HEIGHT%, 32, false);
	r = ARK2D::getRenderer();
	timer = container->getTimer();
	arklog = ARK2D::getLog();
	arklog->i("native init (ark2d) done");

	// set external data path
	const char* externalDataStr;
	jboolean isCopy2;
	externalDataStr = env->GetStringUTFChars(externalDataPath, &isCopy2);
	container->m_platformSpecific.m_externalDataStr = string(externalDataStr);

	// random init.
	arklog->i("seed random");
	MathUtil::seedRandom();

	// init pluggable?
	//delete container->m_platformSpecific.m_pluggable;
	container->setClearColor(Color::darker_grey);
	container->m_platformSpecific.m_pluggable = new MyAndroidPluggable();

	// init opengl
	container->m_platformSpecific.initGL("%GAME_CLEAR_COLOR%", %GAME_WIDTH%, %GAME_HEIGHT%);
	container->m_platformSpecific.initGL2D(%GAME_WIDTH%, %GAME_HEIGHT%);


	arklog->i("init fonts");
	ARK::Font::BMFont* fnt = Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); //new BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
	r->setDefaultFont(fnt);
	r->setFont(fnt);
	// TODO: init default font.

	arklog->i("init openal");
	// TODO: init openal
	bool b = container->m_platformSpecific.initOpenAL();
	if (!b) {
		arklog->i("openal init failed");
		//exit(0);
	}

	arklog->i("init game class");
	game->init(container);
	arklog->i("game class initialised!");

	s_initted = true;
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResize(JNIEnv* env, jclass  thiz, jint w, jint h) {
	//__android_log_print(ANDROID_LOG_INFO, "%GAME_CLASS_NAME%Activity", "resize w=%d h=%d", w, h);
	if (container != NULL) {
		container->setSize((int) w, (int) h);
		//game->resize(container, (int) w, (int) h);
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeDone(JNIEnv* env) {
	if (arklog != NULL) {
		arklog->i("native close");
		container->close();
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativePause(JNIEnv* env,  jclass cls) {
	// stop...
	if (arklog != NULL) {
		arklog->i("native pause start");
		game->pause();
		arklog->i("native pause end");
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResume(JNIEnv* env,  jclass cls) {
	// stop...
	if (arklog != NULL) {
		arklog->i("native resume start");
		game->resume();
		arklog->i("native resume end");
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeBackPressed(JNIEnv* env,  jclass cls) {
	// stop...
	if (container != NULL && s_initted) {
		Input* i = ARK2D::getInput();
		i->pressKey(Input::ANDROID_BACK);
		i->releaseKey(Input::ANDROID_BACK);
	}
}
void fillRect(int x, int y, int w, int h) {
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);

	float m_raw_vertices[] = {
		0,	0,	0,  // tl
		w,	0,	0,	// tr
		0,	h,	0,  // bl
		w,	h,	0	// br
	};
	glTranslatef(x, y, 0);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertexPointer(3, GL_FLOAT, 0, m_raw_vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glTranslatef(-x, -y, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

}

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeRender(JNIEnv* env, jclass cls) {

	// TODO: frame limiting, input handling?
	if (arklog != NULL) {
		//arklog->i("native update");
		timer->tick();
		game->preUpdate(container, timer);
		game->update(container, timer);
		game->postUpdate(container, timer);
		ARK2D::getInput()->clearKeyPressedRecord();

		//arklog->i("native render");
		//fillRect(100,100,10,10);
		RendererStats::reset();
		game->preRender(container, r);
		game->render(container, r);
		game->postRender(container, r);
		if (container->isShowingFPS()) { container->renderFPS(); }

		//fillRect(200,200,10,10);
	}
}

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchDown(JNIEnv* env, jclass thiz, jint x, jint y) {
	if(container != NULL && s_initted) {
		float thisx = (float) x;
		float thisy = (float) y;
		thisx -= container->getTranslateX();
		thisy -= container->getTranslateY();

		thisx /= container->getScale();
		thisy /= container->getScale();

		
		/*string logstr = "touch-down: ";
		logstr += Cast::toString<int>((int) x);
		logstr += ",";
		logstr += Cast::toString<int>((int) y);
		logstr += " -- ";
		logstr += Cast::toString<int>((int) thisx);
		logstr += ",";
		logstr += Cast::toString<int>((int) thisy);
		arklog->i(logstr);*/



		Input* i = ARK2D::getInput();
		i->mouse_x = (int) thisx;
		i->mouse_y = (int) thisy;
		i->pressKey(Input::MOUSE_BUTTON_LEFT);
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchMove(JNIEnv* env, jclass thiz, jint x, jint y) {
	if(container != NULL && s_initted) {
		float thisx = (float) x;
		float thisy = (float) y;
		
		thisx -= container->getTranslateX();
		thisy -= container->getTranslateY();

		thisx /= container->getScale();
		thisy /= container->getScale();

		

		/*string logstr = "touch-move: ";
		logstr += Cast::toString<int>((int) x);
		logstr += ",";
		logstr += Cast::toString<int>((int) y);
		logstr += " -- ";
		logstr += Cast::toString<int>((int) thisx);
		logstr += ",";
		logstr += Cast::toString<int>((int) thisy);
		arklog->i(logstr);*/

		Input* i = ARK2D::getInput();
		game->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
		i->mouse_x = (int) thisx;
		i->mouse_y = (int) thisy;
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchUp(JNIEnv* env, jclass thiz, jint x, jint y) {
	if(container != NULL && s_initted) {
		float thisx = (float) x;
		float thisy = (float) y;
		thisx -= container->getTranslateX();
		thisy -= container->getTranslateY();

		thisx /= container->getScale();
		thisy /= container->getScale();

		

		/*string logstr = "touch-up: ";
		logstr += Cast::toString<int>((int) x);
		logstr += ",";
		logstr += Cast::toString<int>((int) y);
		logstr += " -- ";
		logstr += Cast::toString<int>((int) thisx);
		logstr += ",";
		logstr += Cast::toString<int>((int) thisy);
		arklog->i(logstr);*/

		Input* i = ARK2D::getInput();
		i->mouse_x = (int) thisx;
		i->mouse_y = (int) thisy;
		i->releaseKey(Input::MOUSE_BUTTON_LEFT);
	}
}

string getCharacter(jstring ch);
string getCharacter(jstring ch) {
	const char* keyCharacter;
	jboolean isCopy;
	keyCharacter = s_env->GetStringUTFChars(ch, &isCopy);
	return string(keyCharacter);
}

static int android_key_table[] = {
	/* 00 */ 0,
	/* 01 */ 0,
	/* 02 */ 0,
	/* 03 */ 0,
	/* 04 */ Input::ANDROID_BACK,
	/* 05 */ 0,
	/* 06 */ 0,
	/* 07 */ Input::KEY_0,
	/* 08 */ Input::KEY_1,
	/* 09 */ Input::KEY_2,
	/* 10 */ Input::KEY_3,
	/* 11 */ Input::KEY_4,
	/* 12 */ Input::KEY_5,
	/* 13 */ Input::KEY_6,
	/* 14 */ Input::KEY_7,
	/* 15 */ Input::KEY_8,
	/* 16 */ Input::KEY_9,
	/* 17 */ Input::KEY_NUMPAD_MULTIPLY,
	/* 18 */ Input::KEY_HASH,
	/* 19 */ Input::KEY_UP,
	/* 20 */ Input::KEY_DOWN,
	/* 21 */ Input::KEY_LEFT,
	/* 22 */ Input::KEY_RIGHT,
	/* 23 */ 0,
	/* 24 */ 0,
	/* 25 */ 0,
	/* 26 */ 0,
	/* 27 */ 0,
	/* 28 */ 0,
	/* 29 */ Input::KEY_A,
	/* 30 */ Input::KEY_B,
	/* 31 */ Input::KEY_C,
	/* 32 */ Input::KEY_D,
	/* 33 */ Input::KEY_E,
	/* 34 */ Input::KEY_F,
	/* 35 */ Input::KEY_G,
	/* 36 */ Input::KEY_H,
	/* 37 */ Input::KEY_I,
	/* 38 */ Input::KEY_J,
	/* 39 */ Input::KEY_K,
	/* 40 */ Input::KEY_L,
	/* 41 */ Input::KEY_M,
	/* 42 */ Input::KEY_N,
	/* 43 */ Input::KEY_O,
	/* 44 */ Input::KEY_P,
	/* 45 */ Input::KEY_Q,
	/* 46 */ Input::KEY_R,
	/* 47 */ Input::KEY_S,
	/* 48 */ Input::KEY_T,
	/* 49 */ Input::KEY_U,
	/* 50 */ Input::KEY_V,
	/* 51 */ Input::KEY_W,
	/* 52 */ Input::KEY_X,
	/* 53 */ Input::KEY_Y,
	/* 54 */ Input::KEY_Z,
	/* 55 */ Input::KEY_COMMA,
	/* 56 */ Input::KEY_PERIOD,
	/* 57 */ 0,
	/* 58 */ 0,
	/* 59 */ Input::KEY_LSHIFT,
	/* 60 */ 0,
	/* 61 */ 0,
	/* 62 */ Input::KEY_SPACE,
	/* 63 */ 0,
	/* 64 */ 0,
	/* 65 */ 0,
	/* 66 */ Input::KEY_ENTER,
	/* 67 */ Input::KEY_BACKSPACE,
	/* 68 */ 0,
	/* 69 */ Input::KEY_HYPHEN,
	/* 70 */ Input::KEY_EQUALS,
	/* 71 */ Input::KEY_LEFT_SQUARE_BRACKET,
	/* 72 */ Input::KEY_RIGHT_SQUARE_BRACKET,
	/* 73 */ Input::KEY_BACK_SLASH,
	/* 74 */ Input::KEY_SEMICOLON,
	/* 75 */ Input::KEY_APOSTROPHE,
	/* 75 */ Input::KEY_FORWARD_SLASH,
	/* 77 */ Input::KEY_AT,
	/* 78 */ 0,
	/* 79 */ 0,
	/* 80 */ 0,
	/* 81 */ Input::KEY_NUMPAD_ADD,
	/* 82 */ Input::ANDROID_MENU,
	/* 83 */ 0,
	/* 84 */ Input::ANDROID_SEARCH
};

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeKeyDown(JNIEnv* env, jclass thiz, jint keyCode, jstring ch) { 
	if(container != NULL) {
		ARK2D::getLog()->i("key down: ");
		
		string str = getCharacter(ch);
		ARK2D::getLog()->i(str);

		if (keyCode < 0 || keyCode > 84) {
			ARK2D::getLog()->e("invalid key. english only supported. ");
			return;
		}

		container->m_platformSpecific.m_pluggable->m_keyChar = str;
		ARK2D::getInput()->pressKey(android_key_table[keyCode]);
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeKeyUp(JNIEnv* env, jclass thiz, jint keyCode, jstring ch) { 
	if(container != NULL) {
		ARK2D::getLog()->i("key up: ");
		//ARK2D::getLog()->i(keyCode);

		string str = getCharacter(ch);
		ARK2D::getLog()->i(str);

		if (keyCode < 0 || keyCode > 84) {
			ARK2D::getLog()->e("invalid key. english only supported. ");
			return;
		}

		ARK2D::getInput()->releaseKey(android_key_table[keyCode]);
		container->m_platformSpecific.m_pluggable->m_keyChar = "";
	}
}

string MyAndroidPluggable::urlRequest(string url) {
	ARK2D::getLog()->i("starting url request");

	jstring jstr = s_env->NewStringUTF(url.c_str());
	ARK2D::getLog()->i("herp");
	jclass clazz = s_env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	ARK2D::getLog()->i("derp");
	jmethodID messageMe = s_env->GetStaticMethodID(clazz, "urlRequest", "(Ljava/lang/String;)Ljava/lang/String;"); // Get the method that you want to call
	ARK2D::getLog()->i("gerp");
    jobject result = s_env->CallStaticObjectMethod(clazz, messageMe, jstr); // Call the method on the object
	ARK2D::getLog()->i("zerp");
    const char* str = s_env->GetStringUTFChars((jstring) result, NULL);
    string returnStr(str);
    s_env->ReleaseStringUTFChars(jstr, str); 
	ARK2D::getLog()->i(returnStr);
    ARK2D::getLog()->i("done url request");
     
    return returnStr;
}
void MyAndroidPluggable::openBrowserToUrl(string url) {
	ARK2D::getLog()->i("opening browser to url");

	jstring jstr = s_env->NewStringUTF(url.c_str());
	//ARK2D::getLog()->i("herp");
	jclass clazz = s_env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	//ARK2D::getLog()->i("derp");
	jmethodID messageMe = s_env->GetStaticMethodID(clazz, "openBrowserToUrl", "(Ljava/lang/String;)V"); // Get the method that you want to call
	//ARK2D::getLog()->i("gerp");
    jobject result = s_env->CallStaticObjectMethod(clazz, messageMe, jstr); // Call the method on the object
	//ARK2D::getLog()->i("zerp");
   // const char* str = s_env->GetStringUTFChars((jstring) result, NULL);
    //string returnStr(str);
    //s_env->ReleaseStringUTFChars(jstr); 
	//ARK2D::getLog()->i(returnStr);
    ARK2D::getLog()->i("done opening browser to url");
     
    
}
void MyAndroidPluggable::openSoftwareKeyboard() {
	ARK2D::getLog()->i("Opening software keyboard");

	jclass clazz = s_env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");

	ARK2D::getLog()->i("herp");
	jmethodID messageMe = s_env->GetStaticMethodID(clazz, "openSoftwareKeyboard", "()V"); // Get the method that you want to call
	ARK2D::getLog()->i("derp");
   	s_env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
    
    ARK2D::getLog()->i("Opened software keyboard");
}
void MyAndroidPluggable::closeSoftwareKeyboard() { 

}






