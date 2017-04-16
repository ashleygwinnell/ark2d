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
Scene* scene = NULL;
Log* arklog = NULL;
JNIEnv* s_env = NULL;
JavaVM* s_jvm = NULL;
static jclass s_gameClass = NULL;

bool s_initted = false;


void checkExceptions(JNIEnv* env, bool dolog) {
	if (dolog) { arklog->i("checking exceptions"); }
	if (env->ExceptionCheck()) {
		//std::cout << "exception occured" << std::endl;
		arklog->e("exception occured");

		jthrowable e = env->ExceptionOccurred();

		char buf[1024];
		//strnset(buf, 0, 1024);
		memset(buf, 0, 1024);

		// have to clear the exception before JNI will work again.
		env->ExceptionClear();

		jclass eclass = env->GetObjectClass(e);

		jmethodID mid = env->GetMethodID(eclass, "toString", "()Ljava/lang/String;");

		jstring jErrorMsg = (jstring) env->CallObjectMethod(e, mid);
		const char* cErrorMsg = env->GetStringUTFChars(jErrorMsg, NULL);

		strcpy(buf, cErrorMsg);

		env->ReleaseStringUTFChars(jErrorMsg, cErrorMsg);

		// you can do anything you want with error text in buf now
		//std::cout << buf << std::endl;
		arklog->e(buf);
	}
}
void checkExceptions(JNIEnv* env) {
	checkExceptions(env, false);
}

void s_thisInitGamepads();

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeInit(JNIEnv* env, jclass cls, jstring apkPath, jstring externalDataPath) {
	__android_log_print(ANDROID_LOG_INFO, "%GAME_CLASS_NAME%Activity", "native init");

	// setting some vars
	s_env = env;

	// set apk name
	const char* apkstr;
	jboolean isCopy;
	apkstr = env->GetStringUTFChars(apkPath, &isCopy);
	Resource::apkZipName = string(apkstr);

	// init game stuff

	game = new %GAME_CLASS_NAME%("%GAME_CLASS_NAME%");
	container = new GameContainer(*game, %GAME_WIDTH%, %GAME_HEIGHT%, 32, true);
	r = ARK2D::getRenderer();
	timer = container->getTimer();
	arklog = ARK2D::getLog();
	//arklog->setFilter(ARK::Util::Log::TYPE_ERROR);
	arklog->i("native init (ark2d) done");

	// settings moar vars...
	s_env->GetJavaVM(&container->m_platformSpecific.m_jvm);
	s_env->GetJavaVM(&s_jvm);

	// global references
	jclass local_clazz = s_env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	if (local_clazz == NULL) {
		arklog->e("could not find class 'org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity'.");
	}
	s_gameClass = (jclass) env->NewGlobalRef(local_clazz);

	// set external data path
	const char* externalDataStr;
	jboolean isCopy2;
	externalDataStr = env->GetStringUTFChars(externalDataPath, &isCopy2);
	container->m_platformSpecific.m_externalDataStr = string(externalDataStr);

	// random init.
	arklog->i("seed random");
	Random::init();

	// populate the gamepads.
	arklog->i("Initialising Gamepads... ");
	s_thisInitGamepads();

	scene = container->scene;

	// init pluggable?
	//delete container->m_platformSpecific.m_pluggable;
	container->setClearColor(Color::darker_grey);
	container->m_platformSpecific.m_pluggable = new MyAndroidPluggable();

	// init opengl
	container->m_platformSpecific.initGL("%GAME_CLEAR_COLOR%", %GAME_WIDTH%, %GAME_HEIGHT%);
	container->m_platformSpecific.initGL2D(%GAME_WIDTH%, %GAME_HEIGHT%);

	ARK2D::getRenderer()->init();

	arklog->i("init fonts");
	Renderer::setInterpolation(Renderer::INTERPOLATION_NEAREST);
	ARK::Core::Font::BMFont* fnt = Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); //new BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
	fnt->scale(0.5f);
	r->setDefaultFont(fnt);
	r->setFont(fnt);
	// TODO: init default font.


 	arklog->i("init openal");
	bool b = container->m_platformSpecific.initOpenAL();
	if (!b) {
		arklog->i("openal init failed");
	}

	arklog->i("Initialising Log");
	arklog->init();

	arklog->i("Initialising Localisations");
	container->initLocalisation();

	arklog->i("init game class");
	game->init(container);
	arklog->i("game class initialised!");

	s_initted = true;
}



JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResize(JNIEnv* env, jclass  thiz, jint w, jint h) {
	//__android_log_print(ANDROID_LOG_INFO, "%GAME_CLASS_NAME%Activity", "resize w=%d h=%d", w, h);
	if (container != NULL) {

		// have we just come from a resume?
		arklog->i("game class nativeResize!");
		if (GameContainerPlatform::s_gamePaused == true)
		{
			// do this after the context is created otherwise it won't work.
			arklog->i("game class nativeResize - game was paused and now it is not.");
			Renderer* r = ARK2D::getRenderer();
			r->init();
			FBOStore::getInstance()->reloadFBOs();
			TextureStore::getInstance()->reloadTextures();
			//ShaderStore::getInstance()->reloadShaders();

			GameContainerPlatform::s_gamePaused = false;
		} //else {

			//game->resize(container, (int) w, (int) h);
			//container->setSize((int) w, (int) h);
			//GameContainerPlatform::s_nativeResizing = true;
			container->setSize((int) w, (int) h, true);
			//GameContainerPlatform::s_nativeResizing = false;
		//}


		if (GameContainerPlatform::s_gamePaused == true)
		{
			GameContainerPlatform::s_gamePaused = false;
		}
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
		arklog->i("native pause");
		if (!GameContainerPlatform::s_gamePaused) {
			arklog->i("native pause start");
			game->pause();
			GameContainerPlatform::s_gamePaused = true;
			arklog->i("native pause end");
		} else {
			arklog->i("native pause: game paused when already paused?");
		}
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResume(JNIEnv* env, jclass cls) {
	// stop...
	if (arklog != NULL) {
		arklog->i("native resume");

		if (GameContainerPlatform::s_gamePaused == true) {
			arklog->i("native resume start");
			game->resume();
			arklog->i("native resume end");
			//GameContainerPlatform::s_gamePaused = false;
		} else {
			arklog->i("native resume: game resumed when already running?");
		}
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeBackPressed(JNIEnv* env, jclass cls) {
	// stop...
	if (container != NULL && s_initted) {
		Input* i = ARK2D::getInput();
		i->pressKey(Input::ANDROID_BACK);
		i->releaseKey(Input::ANDROID_BACK);
	}
}


JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeCallbackById(JNIEnv* env, jclass cls, jint id) {
	if (arklog != NULL) {
		arklog->i("native callback by id");
		Callbacks::invoke((unsigned int) id);
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeCallbackByIdIntParam(JNIEnv* env, jclass cls, jint id, jint para) {
	if (arklog != NULL) {
		arklog->i("native callback by id (int param) ");
		Callbacks::invoke((unsigned int) id, (unsigned int) para);
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeCallbackByIdStringParam(JNIEnv* env, jclass cls, jint id, jstring javaString) {
	if (arklog != NULL) {

		arklog->i("native callback by id (string param) pre ");
		const char* nativeString = env->GetStringUTFChars(javaString, 0);
		string nativeStringString = string(nativeString);
		arklog->i("native callback by id (string param) ");
		Callbacks::invoke((unsigned int) id, nativeStringString);

		env->ReleaseStringUTFChars(javaString, nativeString);

	}
}

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeErrorDialog(JNIEnv* env, jclass cls, jstring err) {
	if (container != NULL && arklog != NULL && s_initted) {

		const char* cString;
		jboolean isCopy;
		cString = env->GetStringUTFChars(err, &isCopy);
		ErrorDialog::createAndShow(string(cString));
	}
}

/*void fillRect(int x, int y, int w, int h) {
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

}*/

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeRender(JNIEnv* env, jclass cls) {

	// TODO: frame limiting, input handling?
	if (arklog != NULL) {
		//arklog->i("native update");
		timer->tick();

		vector<Gamepad*>* gamepads = container->getGamepads();
		for (unsigned int i = 0; i < gamepads->size(); ++i) {
			gamepads->at(i)->update();
		}

		//game->preUpdate(container, timer);
		//game->update(container, timer);
		//game->postUpdate(container, timer);
		ARK2D::getInput()->update();
		scene->update();
		ARK2D::getInput()->clearKeyPressedRecord();

		for (unsigned int i = 0; i < gamepads->size(); i++) {
			gamepads->at(i)->clearButtonPressedRecord();
		}
		ARK2D::getInput()->endFrame();

		//arklog->i("native render");
		//fillRect(100,100,10,10);
		RendererStats::reset();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//game->preRender(container, r);
		//game->render(container, r);
		//ARK2D::getLog()->render(container, r);
		//game->postRender(container, r);
		//container->preRender();
		scene->render();
		//container->postRender();
		//if (container->isShowingFPS()) { container->renderFPS(); }


		//ARK2D::getRenderer()->finish();
		//ARK2D::getRenderer()->flush();
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



		string logstr = "touch-move: ";
		logstr += Cast::toString<int>((int) x);
		logstr += ",";
		logstr += Cast::toString<int>((int) y);
		logstr += " -- ";
		logstr += Cast::toString<int>((int) thisx);
		logstr += ",";
		logstr += Cast::toString<int>((int) thisy);
		arklog->i(logstr);

		Input* i = ARK2D::getInput();
		ARK2D::getScene()->mouseMoved((int) thisx, (int) thisy, i->mouse_x, i->mouse_y);
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



		string logstr = "touch-up: ";
		logstr += Cast::toString<int>((int) x);
		logstr += ",";
		logstr += Cast::toString<int>((int) y);
		logstr += " -- ";
		logstr += Cast::toString<int>((int) thisx);
		logstr += ",";
		logstr += Cast::toString<int>((int) thisy);
		arklog->i(logstr);

		Input* i = ARK2D::getInput();
		i->mouse_x = (int) thisx;
		i->mouse_y = (int) thisy;
		i->releaseKey(Input::MOUSE_BUTTON_LEFT);
	}
}

string getCharacter(JNIEnv* env, jstring ch);
string getCharacter(JNIEnv* env, jstring ch) {
	//if (env == NULL) {
	//	env = s_env;
	//}

	const char* keyCharacter;
	jboolean isCopy;
	keyCharacter = env->GetStringUTFChars(ch, &isCopy);
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
		//s_env = env;
		ARK2D::getLog()->i("key down: ");

		string str = getCharacter(env, ch);
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
		//s_env = env;
		ARK2D::getLog()->i("key up: ");

		string str = getCharacter(env, ch);
		ARK2D::getLog()->i(str);

		if (keyCode < 0 || keyCode > 84) {
			ARK2D::getLog()->e("invalid key. english only supported. ");
			return;
		}

		ARK2D::getInput()->releaseKey(android_key_table[keyCode]);
		container->m_platformSpecific.m_pluggable->m_keyChar = "";
	}
}

// GAMEPAD BITS
JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAdd(JNIEnv* env, jclass thiz, jint id, jstring padname) {
	if(container != NULL) {
		//s_env = env;






		int cid = (int) id;
		string cname = getCharacter(env, padname);
		GameContainerPlatform* platform = &ARK2D::getContainer()->m_platformSpecific;

		Gamepad* gamepad = new Gamepad();
		gamepad->id = platform->m_gamepadId++;
		gamepad->deviceId = cid; //platform->m_gamepadId++;
		gamepad->initted = true;
		gamepad->vendorId = cid; //IOHIDDeviceGetVendorID(device);
		gamepad->productId = cid; //IOHIDDeviceGetProductID(device);
		gamepad->name = cname.c_str();

		gamepad->numButtons = 0;
		gamepad->numAxes = 0;

		ARK2D::getLog()->i(StringUtil::append("Gamepad added: ", cid));
		ARK2D::getLog()->i(StringUtil::append("Gamepad name: ", gamepad->name));

		int dpad_bs[13];
		dpad_bs[0]  = Gamepad::DPAD_UP;
		dpad_bs[1]  = Gamepad::DPAD_DOWN;
		dpad_bs[2]  = Gamepad::DPAD_LEFT;
		dpad_bs[3]  = Gamepad::DPAD_RIGHT;
		dpad_bs[4]  = Gamepad::BUTTON_A;
		dpad_bs[5]  = Gamepad::BUTTON_B;
		dpad_bs[6]  = Gamepad::BUTTON_X;
		dpad_bs[7]  = Gamepad::BUTTON_Y;
		dpad_bs[8]  = Gamepad::BUTTON_LBUMPER;
		dpad_bs[9]  = Gamepad::BUTTON_RBUMPER;
		dpad_bs[10] = Gamepad::BUTTON_L3;
		dpad_bs[11] = Gamepad::BUTTON_R3;
		dpad_bs[12] = Gamepad::BUTTON_START;

		for(unsigned int i = 0; i < 13; i++) {
			ARK2D::getLog()->i(StringUtil::append("Gamepad adding button: ", dpad_bs[i]));
			GamepadButton* dpad_button = new GamepadButton();
			//dpad_button->id = Gamepad::convertButtonToId(gamepad, dpad_bs[i]);
			dpad_button->id = dpad_bs[i];
			dpad_button->down = false;
			gamepad->buttons.push_back(dpad_button);
			gamepad->numButtons++;
		}

		ARK2D::getContainer()->getGamepads()->push_back(gamepad);

	}
}


JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadRemove(JNIEnv* env, jclass thiz, jint id) {
	if (container != NULL) {
		ARK2D::getLog()->i("Remove a gamepad... ");

		unsigned int numDevices = container->getGamepads()->size();
		for (unsigned int deviceIndex = 0; deviceIndex < numDevices; deviceIndex++)
		{
			Gamepad* pad = container->getGamepads()->at(deviceIndex);
			if (pad->deviceId == id) {
				ARK2D::getLog()->i(StringUtil::append("Remove gamepad: ", pad->id));
				container->getGamepads()->erase(container->getGamepads()->begin() + deviceIndex);
				return;
			}
		}
	}
}

JNIEXPORT bool JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadExists(JNIEnv* env, jclass thiz, jint id) {
	if(container != NULL) {
		//s_env = env;
		//ARK2D::getLog()->i("gamepad exists?: ");
		//ARK2D::getLog()->i(Cast::toString<int>((int) id));

		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				ARK2D::getLog()->i(StringUtil::append("Gamepad exists: ", (int) id));
				return true;
			}
		}
		ARK2D::getLog()->i(StringUtil::append("Gamepad does not exist: ", (int) id));
	}

	return false;
}

JNIEXPORT int JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadNumAxes(JNIEnv* env, jclass thiz, jint id) {
	if(container != NULL)
	{
		Gamepad* thisGamepad = NULL;
		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				thisGamepad = gamepads->at(i);
				break;
			}
		}
		if (thisGamepad != NULL) {
			return (int) thisGamepad->numAxes;
		}
		ARK2D::getLog()->i(StringUtil::append("Gamepad does not exist: ", (int) id));
	}
	return 0;
}



JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAddAxis(JNIEnv* env, jclass thiz, jint id, jint index, jint axis) {
	if(container != NULL) {
		//s_env = env;

		string gamepadStr = "Add axis ";
		gamepadStr += Cast::toString<int>((int) axis);
		gamepadStr += " to gamepad id: ";
		gamepadStr += Cast::toString<int>((int) id);
		gamepadStr += " at index: ";
		gamepadStr += Cast::toString<int>((int) index);
		ARK2D::getLog()->i(gamepadStr);

		Gamepad* thisGamepad = NULL;
		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				thisGamepad = gamepads->at(i);
				break;
			}
		}

		if (thisGamepad != NULL) {
			GamepadAxis* gaxis = new GamepadAxis();
			gaxis->id = thisGamepad->numAxes;
			gaxis->axisId = axis;
			gaxis->value = 0.0f;
			thisGamepad->axes.push_back(gaxis);
			thisGamepad->numAxes++;
		}
	}
}
JNIEXPORT int JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAxisAtIndex(JNIEnv* env, jclass thiz, jint id, jint index) {
	if (container != NULL)
	{
		Gamepad* thisGamepad = NULL;
		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				thisGamepad = gamepads->at(i);
				break;
			}
		}
		return (int) thisGamepad->axes.at((int) index)->axisId;
	}
	return 0;
}

JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadAxisChanged(JNIEnv* env, jclass thiz, jint id, jint axis, jfloat val) {
	if (container != NULL)
	{
		Gamepad* thisGamepad = NULL;
		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				thisGamepad = gamepads->at(i);
				break;
			}
		}

		for(unsigned int i = 0; i < thisGamepad->axes.size(); i++) {
			if (thisGamepad->axes.at(i)->axisId == (unsigned int) axis) {

				//ARK2D::getLog()->i(StringUtil::append("gamepad axis change 1: ", id));
				//ARK2D::getLog()->i(StringUtil::append("gamepad axis change 2: ", axis));
				//ARK2D::getLog()->i(StringUtil::appendf("gamepad axis change 3: ", val));

				thisGamepad->axes.at(i)->value = (float) val;
				return;
			}
		}
	}
}

void addButtonToGamepad(Gamepad* gamepad, unsigned int button);
void addButtonToGamepad(Gamepad* gamepad, unsigned int id) {
	GamepadButton* bu = new GamepadButton();
	bu->id = id;
	bu->down = false;
	gamepad->buttons.push_back(bu);
	gamepad->numButtons += 1;
}
JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadKeyDown(JNIEnv* env, jclass thiz, jint id, jint buttonid) {
	if(container != NULL && s_initted) {
		//s_env = env;
		ARK2D::getLog()->i("gamepad button down 1: ");
		ARK2D::getLog()->i(Cast::toString<int>((int) id));
		ARK2D::getLog()->i("gamepad button down 2: ");
		ARK2D::getLog()->i(Cast::toString<int>((int) buttonid));

		Gamepad* thisGamepad = NULL;
		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				thisGamepad = gamepads->at(i);
				break;
			}
		}

		if (thisGamepad != NULL) {

			// convert OuyaController.DPAD_UP (19) into Gamepad::DPAD_UP (103)
			unsigned int actualButton = Gamepad::convertButtonToId(thisGamepad,  (unsigned int) buttonid );
			/*bool buttonExists = thisGamepad->hasButton((unsigned int) actualButton);
			if (!buttonExists && (
					actualButton == Gamepad::DPAD_UP ||
					actualButton == Gamepad::DPAD_DOWN ||
					actualButton == Gamepad::DPAD_LEFT ||
					actualButton == Gamepad::DPAD_RIGHT
				)) {
				addButtonToGamepad(thisGamepad, buttonid);
			}
			ARK2D::getLog()->i(thisGamepad->toString());*/


			thisGamepad->pressButton(actualButton);
		}
	}
}
JNIEXPORT void JNICALL Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeGamepadKeyUp(JNIEnv* env, jclass thiz, jint id, jint buttonid) {
	if(container != NULL && s_initted) {
		//s_env = env;
		//ARK2D::getLog()->i("gamepad button up 1: ");
		//ARK2D::getLog()->i(Cast::toString<int>((int) id));
		//ARK2D::getLog()->i("gamepad button up 2: ");
		//ARK2D::getLog()->i(Cast::toString<int>((int) buttonid));

		Gamepad* thisGamepad = NULL;
		vector<Gamepad* >* gamepads = ARK2D::getContainer()->getGamepads();
		for(unsigned int i = 0; i < gamepads->size(); i++) {
			if (gamepads->at(i)->deviceId == (int) id) {
				thisGamepad = gamepads->at(i);
				break;
			}
		}

		if (thisGamepad != NULL) {
			//if (!thisGamepad->hasButton((unsigned int) buttonid)) {
			//	addButtonToGamepad(thisGamepad, (unsigned int) buttonid);
			//}
			unsigned int actualButton = Gamepad::convertButtonToId(thisGamepad, (unsigned int) buttonid );
			thisGamepad->releaseButton(actualButton);
		}
	}
}


JNIEnv* s_getCurrentEnv(bool dolog) {
	JNIEnv* env = 0;

	jint getEnvSuccess = s_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (getEnvSuccess == JNI_OK) {
		if (dolog) { ARK2D::getLog()->v("jni: thread already attached."); }
	} else if (getEnvSuccess == JNI_EDETACHED) {
		if (dolog) { ARK2D::getLog()->v("jni: thread not attached. attaching..."); }
		jint attachSuccess = s_jvm->AttachCurrentThread(&env, NULL);
		if (attachSuccess == 0) {
			if (dolog) { ARK2D::getLog()->v("jni: attached."); }
		} else {
			if (dolog) { ARK2D::getLog()->e("jni: could not attach current thread... maybe it's already attached! "); }
		}
	} else if (getEnvSuccess == JNI_EVERSION) {
		if (dolog) { ARK2D::getLog()->v("jni: version not supported."); }
	}
	return env;
}
JNIEnv* s_getCurrentEnv() {
	return s_getCurrentEnv(true);
}

void s_thisInitGamepads() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%View");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "initGamepads", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}

string MyAndroidPluggable::urlRequestThreaded(string url) {
	ARK2D::getLog()->i("jni: starting threaded url request");

	JNIEnv* env = s_getCurrentEnv();
	string s = urlRequest(env, url);

	//s_jvm->DetachCurrentThread();
	return s;
}

string MyAndroidPluggable::urlRequest(JNIEnv* env, string url) {
	ARK2D::getLog()->i("starting url request");

	if (env == NULL) {
		ARK2D::getLog()->i("setting env to s_env");
		//env = s_env;
		env = s_getCurrentEnv();
		checkExceptions(env, false);
	}

	jstring jstr = env->NewStringUTF(url.c_str());
	//ARK2D::getLog()->i("herp");
	//jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	//ARK2D::getLog()->i("derp");
	checkExceptions(env);
	//ARK2D::getLog()->i("lerp");
	if(s_gameClass == NULL) {
		ARK2D::getLog()->e("global game class is null");
		return "";
	}
	//ARK2D::getLog()->i("ferp");
	jmethodID messageMe = env->GetStaticMethodID(s_gameClass, "urlRequest", "(Ljava/lang/String;)Ljava/lang/String;"); // Get the method that you want to call
	//ARK2D::getLog()->i("gerp");
    jobject result = env->CallStaticObjectMethod(s_gameClass, messageMe, jstr); // Call the method on the object
	//ARK2D::getLog()->i("zerp");
    const char* str = env->GetStringUTFChars((jstring) result, NULL);
    if (str == NULL) {
    	ARK2D::getLog()->i("OUT OF MEMORY");
    	return string("");
    }
   /* char* copy = new char[ strlen(str) + 1];
    strncpy(tmp_filename, _filename, len);
 	tmp_filename[len] = '\0';
    returnStr += str; */
    string returnStr(str);
    env->ReleaseStringUTFChars(jstr, str);
	//ARK2D::getLog()->v(returnStr);
    //ARK2D::getLog()->v("done url request");

    checkExceptions(env);

    return returnStr;
}
bool MyAndroidPluggable::isNetworkAvailable() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "isNetworkAvailable", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}

void MyAndroidPluggable::openBrowserToUrl(string url) {
	JNIEnv* env = s_getCurrentEnv();

	ARK2D::getLog()->i("opening browser to url");

	jstring jstr = env->NewStringUTF(url.c_str());
	//ARK2D::getLog()->i("herp");
	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	//ARK2D::getLog()->i("derp");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "openBrowserToUrl", "(Ljava/lang/String;)V"); // Get the method that you want to call
	//ARK2D::getLog()->i("gerp");
    /*jobject result =*/ env->CallStaticVoidMethod(clazz, messageMe, jstr); // Call the method on the object
	//ARK2D::getLog()->i("zerp");
   // const char* str = env->GetStringUTFChars((jstring) result, NULL);
    //string returnStr(str);
    //env->ReleaseStringUTFChars(jstr);
	//ARK2D::getLog()->i(returnStr);
    ARK2D::getLog()->i("done opening browser to url");
}

void MyAndroidPluggable::openGalleryToImageUrl(string url) {
	JNIEnv* env = s_getCurrentEnv();

	ARK2D::getLog()->i("opening gallery to image");

	jstring jstr = env->NewStringUTF(url.c_str());
	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "openGalleryToImageUrl", "(Ljava/lang/String;)V"); // Get the method that you want to call
	/*jobject result =*/ env->CallStaticVoidMethod(clazz, messageMe, jstr); // Call the method on the object

	ARK2D::getLog()->i("done opening gallery to image");
}

void MyAndroidPluggable::openGooglePlayStore(string packageName) {
	JNIEnv* env = s_getCurrentEnv();

	ARK2D::getLog()->i("opening google play store");

	jstring jstr = env->NewStringUTF(packageName.c_str());
	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "openGooglePlayStore", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, jstr); // Call the method on the object
	ARK2D::getLog()->i("done opening google play store");
}
void MyAndroidPluggable::openErrorDialog(string message) {
	JNIEnv* env = s_getCurrentEnv();

	ARK2D::getLog()->i("opening error dialog");

	jstring jstr = env->NewStringUTF(message.c_str());
	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "openErrorDialog", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, jstr); // Call the method on the object
	ARK2D::getLog()->i("done opening error dialog");
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

void MyAndroidPluggable::openInputDialog(unsigned int jniCallbackId, string title, string defaultStr) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	ARK2D::getLog()->i("Opening input dialog");
	jint jCallbackId = (jint) jniCallbackId;
	jstring jTitle = env->NewStringUTF(title.c_str());
	jstring jDefaultStr = env->NewStringUTF(defaultStr.c_str());

	ARK2D::getLog()->i("getting method id");
	jmethodID messageMe = env->GetStaticMethodID(s_gameClass, "openInputDialog", "(ILjava/lang/String;Ljava/lang/String;)V"); // Get the method that you want to call

	ARK2D::getLog()->i("got method. calling method.");
    env->CallStaticVoidMethod(s_gameClass, messageMe, jCallbackId, jTitle, jDefaultStr); // Call the method on the object

	ARK2D::getLog()->i("done opening input dialog");
	checkExceptions(env);
}
string MyAndroidPluggable::getInputDialogText() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	ARK2D::getLog()->i("getting input dialog text");
	jmethodID messageMe = env->GetStaticMethodID(s_gameClass, "getInputDialogText", "()Ljava/lang/String;"); // Get the method that you want to call

    ARK2D::getLog()->i("got method id. calling method.");
    jobject result = env->CallStaticObjectMethod(s_gameClass, messageMe);

    ARK2D::getLog()->i("called method. getting response");
    const char* str = env->GetStringUTFChars((jstring) result, NULL);
    if (str == NULL) {
    	ARK2D::getLog()->i("OUT OF MEMORY");
    	return string("");
    }
    checkExceptions(env);

    string returnStr(str);
    //env->ReleaseStringUTFChars(str);
	ARK2D::getLog()->i(returnStr);
    ARK2D::getLog()->i("done opening input dialog");
    return returnStr;
}

void MyAndroidPluggable::openAlertDialog(string title, string message) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	ARK2D::getLog()->i("Opening alert dialog");
	jstring jTitle = env->NewStringUTF(title.c_str());
	jstring jMessage = env->NewStringUTF(message.c_str());

	ARK2D::getLog()->i("getting method id");
	jmethodID messageMe = env->GetStaticMethodID(s_gameClass, "openAlertDialog", "(Ljava/lang/String;Ljava/lang/String;)V");

	ARK2D::getLog()->i("got method. calling method.");
    env->CallStaticVoidMethod(s_gameClass, messageMe, jTitle, jMessage);

	ARK2D::getLog()->i("done opening alert dialog");
	checkExceptions(env);
}


string MyAndroidPluggable::getISO6391Language() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	ARK2D::getLog()->i("getting ios language");
	jmethodID messageMe = env->GetStaticMethodID(s_gameClass, "getISO6391Language", "()Ljava/lang/String;"); // Get the method that you want to call

    ARK2D::getLog()->i("got method id. calling method.");
    jobject result = env->CallStaticObjectMethod(s_gameClass, messageMe);

    ARK2D::getLog()->i("called method. getting response");
    const char* str = env->GetStringUTFChars((jstring) result, NULL);
    if (str == NULL) {
    	ARK2D::getLog()->i("OUT OF MEMORY");
    	return string("");
    }
    checkExceptions(env);

    string returnStr(str);
    //env->ReleaseStringUTFChars(str);
	ARK2D::getLog()->i(returnStr);

    return returnStr;
}

bool MyAndroidPluggable::vibrator_hasVibrator() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "util_vibrator_hasVibrator", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}
void MyAndroidPluggable::vibrator_vibrate(int millis) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jint millis_int = (jint) millis;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "util_vibrator_vibrate", "(I)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, millis_int); // Call the method on the object
}
void MyAndroidPluggable::vibrator_cancel() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "util_vibrator_cancel", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}

void MyAndroidPluggable::ga_sendSocial(string network, string action, string targeturl) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring network_jstr = env->NewStringUTF(network.c_str());
	jstring action_jstr = env->NewStringUTF(action.c_str());
	jstring targeturl_jstr = env->NewStringUTF(targeturl.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ga_sendSocial", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, network_jstr, action_jstr, targeturl_jstr); // Call the method on the object

}
void MyAndroidPluggable::ga_sendEvent(string category, string action, string label, long opt_value) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring category_jstr = env->NewStringUTF(category.c_str());
	jstring action_jstr = env->NewStringUTF(action.c_str());
	jstring label_jstr = env->NewStringUTF(label.c_str());

	jlong optValue_jlong = (jlong) opt_value;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ga_sendEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, category_jstr, action_jstr, label_jstr, optValue_jlong); // Call the method on the object
}
void MyAndroidPluggable::ga_sendTiming(long loadTime, string category, string name, string label) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring category_jstr = env->NewStringUTF(category.c_str());
	jstring name_jstr = env->NewStringUTF(name.c_str());
	jstring label_jstr = env->NewStringUTF(label.c_str());

	jlong loadTime_jlong = (jlong) loadTime;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ga_sendTiming", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, loadTime_jlong, category_jstr, name_jstr, label_jstr); // Call the method on the object
}


void MyAndroidPluggable::share_googleplus(string text) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring shareText_jstr = env->NewStringUTF(text.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "share_googleplus", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, shareText_jstr); // Call the method on the object
}
void MyAndroidPluggable::thread_start(unsigned int thread_id) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jint jintCallbackId = (jint) Callbacks::CALLBACK_ANDROID_THREAD_START;
	jint jintThreadId = (jint) thread_id;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "thread_start", "(II)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, jintCallbackId, jintThreadId); // Call the method on the object
}

void MyAndroidPluggable::container_close() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "container_close", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}

bool MyAndroidPluggable::ouya_isOuya() {
	JNIEnv* env = s_getCurrentEnv(false);
	checkExceptions(env, false);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ouya_isOuya", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}

void MyAndroidPluggable::ouya_requestPurchase(string name)
{
	JNIEnv* env = s_getCurrentEnv();

	ARK2D::getLog()->i("ouya_requestPurchase start");

	jstring jstr = env->NewStringUTF(name.c_str());
	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ouya_requestPurchaseByString", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, jstr); // Call the method on the object

	ARK2D::getLog()->i("ouya_requestPurchase finished");
}

string MyAndroidPluggable::ouya_getUsername() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	ARK2D::getLog()->i("getting ouya username");
	jmethodID messageMe = env->GetStaticMethodID(s_gameClass, "ouya_getUsername", "()Ljava/lang/String;"); // Get the method that you want to call

    ARK2D::getLog()->i("got method id. calling method.");
    jobject result = env->CallStaticObjectMethod(s_gameClass, messageMe);

    ARK2D::getLog()->i("called method. getting response");
    const char* str = env->GetStringUTFChars((jstring) result, NULL);
    if (str == NULL) {
    	ARK2D::getLog()->i("OUT OF MEMORY");
    	return string("");
    }
    checkExceptions(env);

    string returnStr(str);
    //env->ReleaseStringUTFChars(str);
	ARK2D::getLog()->i(returnStr);
    ARK2D::getLog()->i("done getting ouya username");
    return returnStr;
}

bool MyAndroidPluggable::firetv_isAmazonFireTV() {
	JNIEnv* env = s_getCurrentEnv(false);
	checkExceptions(env, false);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "firetv_isAmazonFireTV", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}

string MyAndroidPluggable::firetv_getUsername() {
	return "";
}
void MyAndroidPluggable::firetv_viewAchievements() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "firetv_viewAchievements", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}
void MyAndroidPluggable::firetv_unlockAchievement(string id) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring achievementId_jstr = env->NewStringUTF(id.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "firetv_unlockAchievement", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, achievementId_jstr); // Call the method on the object
}
void MyAndroidPluggable::firetv_viewScores() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "firetv_viewScores", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}
void MyAndroidPluggable::firetv_viewScores(string id) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring leaderboardId_jstr = env->NewStringUTF(id.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "firetv_viewScores", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, leaderboardId_jstr); // Call the method on the object
}
void MyAndroidPluggable::firetv_submitScore(string id, int score) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring leaderboardId_jstr = env->NewStringUTF(id.c_str());
	jint leaderboardScore_jint = (jint) score;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "firetv_submitScore", "(Ljava/lang/String;I)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, leaderboardId_jstr, leaderboardScore_jint); // Call the method on the object
}



void MyAndroidPluggable::googleplaygameservices_signIn() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_signIn", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}
void MyAndroidPluggable::googleplaygameservices_signOut() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_signOut", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}
bool MyAndroidPluggable::googleplaygameservices_isSignedIn() {
	JNIEnv* env = s_getCurrentEnv(false);
	checkExceptions(env, false);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_isSignedIn", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}
bool MyAndroidPluggable::googleplaygameservices_isSigningIn() {
	JNIEnv* env = s_getCurrentEnv(false);
	checkExceptions(env, false);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_isSigningIn", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}
void MyAndroidPluggable::googleplaygameservices_viewAchievements() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_viewAchievements", "()V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe); // Call the method on the object
}
void MyAndroidPluggable::googleplaygameservices_unlockAchievement(string id) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring achievementId_jstr = env->NewStringUTF(id.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_unlockAchievement", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, achievementId_jstr); // Call the method on the object
}
void MyAndroidPluggable::googleplaygameservices_viewScores(string id) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring leaderboardId_jstr = env->NewStringUTF(id.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_viewScores", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, leaderboardId_jstr); // Call the method on the object
}
void MyAndroidPluggable::googleplaygameservices_submitScore(string id, int score) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring leaderboardId_jstr = env->NewStringUTF(id.c_str());
	jint leaderboardScore_jint = (jint) score;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_submitScore", "(Ljava/lang/String;I)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, leaderboardId_jstr, leaderboardScore_jint); // Call the method on the object
}

void MyAndroidPluggable::googleplaygameservices_savedGamesSelect(bool allowAdd, bool allowDelete) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jboolean bAllowAdd = (jboolean) allowAdd;
	jboolean bAllowDelete = (jboolean) allowDelete;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_savedGamesSelect", "(ZZ)V");
	env->CallStaticVoidMethod(clazz, messageMe, bAllowAdd, bAllowDelete);
}
void MyAndroidPluggable::googleplaygameservices_savedGamesLoad(string name) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring jstrName = env->NewStringUTF(name.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_savedGamesLoad", "(Ljava/lang/String;)V");
	env->CallStaticVoidMethod(clazz, messageMe, jstrName);
}
void MyAndroidPluggable::googleplaygameservices_savedGamesUpdate(string name, string data, bool createIfMissing) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring jstrName = env->NewStringUTF(name.c_str());
	jstring jstrData = env->NewStringUTF(data.c_str());
	jboolean bCreateIfMissing = (jboolean) createIfMissing;

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_savedGamesUpdate", "(Ljava/lang/String;Ljava/lang/String;Z)V");
	env->CallStaticVoidMethod(clazz, messageMe, jstrName, jstrData, bCreateIfMissing);
}

/*bool MyAndroidPluggable::googleplaygameservices_isConnected() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_isConnected", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}
bool MyAndroidPluggable::googleplaygameservices_isConnecting() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaygameservices_isConnecting", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}*/

bool MyAndroidPluggable::googleplaybilling_isSetup() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaybilling_isSetup", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret ) {
		return true;
	}
	return false;
}

void MyAndroidPluggable::googleplaybilling_startPurchase(string id, int referenceNumber, string extraToken) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring itemId_jstr = env->NewStringUTF(id.c_str());
	jint referenceNumber_jint = (jint) referenceNumber;
	jstring extraT_jstr = env->NewStringUTF(extraToken.c_str());


	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaybilling_startPurchase", "(Ljava/lang/String;ILjava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, itemId_jstr, referenceNumber_jint, extraT_jstr); // Call the method on the object
}
bool MyAndroidPluggable::googleplaybilling_hasPurchase(string sku) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring itemId_jstr = env->NewStringUTF(sku.c_str());


	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "googleplaybilling_hasPurchase", "(Ljava/lang/String;)Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe, itemId_jstr); // Call the method on the object
	if (ret) {
		return true;
	}
	return false;
}
void MyAndroidPluggable::googleplaybilling_consumePurchase(string sku) {

}
void MyAndroidPluggable::googleplaybilling_queryPurchases() {

}

bool MyAndroidPluggable::ironsource_isRewardedAdAvailable() {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ironsource_isRewardedAdAvailable", "()Z"); // Get the method that you want to call
	jboolean ret = env->CallStaticBooleanMethod(clazz, messageMe); // Call the method on the object
	if (ret ) {
		return true;
	}
	return false;
}
void MyAndroidPluggable::ironsource_showRewardedAd(string placement) {
	JNIEnv* env = s_getCurrentEnv();
	checkExceptions(env);

	jstring placement_jstr = env->NewStringUTF(placement.c_str());

	jclass clazz = env->FindClass("org/%COMPANY_NAME%/%GAME_SHORT_NAME%/%GAME_CLASS_NAME%Activity");
	jmethodID messageMe = env->GetStaticMethodID(clazz, "ironsource_showRewardedAd", "(Ljava/lang/String;)V"); // Get the method that you want to call
	env->CallStaticVoidMethod(clazz, messageMe, placement_jstr); // Call the method on the object
}






