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
Graphics* g = NULL;
GameTimer* timer = NULL;
%GAME_CLASS_NAME%* game = NULL;
ARKLog* arklog = NULL;

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeInit(JNIEnv* env, jclass cls, jstring apkPath, jstring externalDataPath) {
	//__android_log_print(ANDROID_LOG_INFO, "%GAME_CLASS_NAME%Activity", "native init");

	// set apk name
	const char* apkstr;
	jboolean isCopy;
	apkstr = env->GetStringUTFChars(apkPath, &isCopy);
	ARK::Resource::apkZipName = string(apkstr);

	// init game stuff
	game = new %GAME_CLASS_NAME%("%GAME_CLASS_NAME%");
	container = new GameContainer(*game, %GAME_WIDTH%, %GAME_HEIGHT%, 32, false);
	g = ARK2D::getGraphics();
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

	// init opengl
	container->m_platformSpecific.initGL("%GAME_CLEAR_COLOR%", %GAME_WIDTH%, %GAME_HEIGHT%);
	container->m_platformSpecific.initGL2D(%GAME_WIDTH%, %GAME_HEIGHT%);


	arklog->i("init fonts");
	ARK::Font::BMFont* fnt = ARK::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); //new BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
	g->setDefaultFont(fnt);
	g->setFont(fnt);
	// TODO: init default font.

	arklog->i("init openal");
	// TODO: init openal
	bool b = container->m_platformSpecific.initOpenAL();
	if (!b) {
		arklog->i("openal init failed");
		exit(0);
	}

	arklog->i("init game class");
	game->init(container);
	arklog->i("game class initialised!");
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
		arklog->i("native pause");
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeResume(JNIEnv* env,  jclass cls) {
	// stop...
	if (arklog != NULL) {
		arklog->i("native resume");
	}
}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeBackPressed(JNIEnv* env,  jclass cls) {
	// stop...
	if (container != NULL) {
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


		//arklog->i("native render");
		//fillRect(100,100,10,10);
		game->preRender(container, g);
		game->render(container, g);
		game->postRender(container, g);
		if (container->isShowingFPS()) { container->renderFPS(); }

		//fillRect(200,200,10,10);
	}
}

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchDown(JNIEnv* env, jclass thiz, jint x, jint y) {
	if(container != NULL) {
		float thisx = (float) x;
		float thisy = (float) y;
		thisx /= container->getScale();
		thisy /= container->getScale();

		thisx -= container->getTranslateX();
		thisy -= container->getTranslateY();

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
	if(container != NULL) {
		float thisx = (float) x;
		float thisy = (float) y;
		thisx /= container->getScale();
		thisy /= container->getScale();

		thisx -= container->getTranslateX();
		thisy -= container->getTranslateY();

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
	if(container != NULL) {
		float thisx = (float) x;
		float thisy = (float) y;
		thisx /= container->getScale();
		thisy /= container->getScale();

		thisx -= container->getTranslateX();
		thisy -= container->getTranslateY();

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
