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

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeInit(JNIEnv* env, jclass cls, jstring apkPath) {
	//__android_log_print(ANDROID_LOG_INFO, "%GAME_CLASS_NAME%Activity", "native init");

	// set apk name
	const char* apkstr;
	jboolean isCopy;
	apkstr = env->GetStringUTFChars(apkPath, &isCopy);
	ARK::Resource::apkZipName = string(apkstr);

	game = new %GAME_CLASS_NAME%("%GAME_CLASS_NAME%");
	container = new GameContainer(*game, 800,600,32,false);
	g = ARK2D::getGraphics();
	timer = container->getTimer();
	arklog = ARK2D::getLog();
	arklog->i("native init (ark2d) done");

	arklog->i("seed random");
	MathUtil::seedRandom();

	arklog->i("init opengl");
	// TODO: init opengl
	glViewport(0, 0, 800, 600);
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT );

	glShadeModel(GL_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


	//enable 2d?
	glMatrixMode(GL_PROJECTION) ;
	glPushMatrix();
	glLoadIdentity();

	glOrthof(0, 800, 600, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	arklog->i("init fonts");
	BMFont* fnt = ARK::Resource::get("ark2d/fonts/default.fnt")->asFont()->asBMFont(); //new BMFont("ark2d/fonts/default.fnt", "ark2d/fonts/default.png");
	g->setDefaultFont(fnt);
	g->setFont(fnt);
	// TODO: init default font.

	arklog->i("init openal");
	// TODO: init openal

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
		arklog->i("native update");
		timer->tick();
		game->update(container, timer);


		arklog->i("native render");
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		//fillRect(100,100,10,10);
		game->render(container, g);
		//fillRect(200,200,10,10);
	}
}

JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchDown(JNIEnv* env, jclass thiz, jint x, jint y) {

}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchMove(JNIEnv* env, jclass thiz, jint x, jint y) {

}
JNIEXPORT void Java_org_%COMPANY_NAME%_%GAME_SHORT_NAME%_%GAME_CLASS_NAME%Renderer_nativeTouchUp(JNIEnv* env, jclass thiz, jint x, jint y) {

}
