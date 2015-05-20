
#include "AngelScriptUtil.h"

#include "../ARK2D.h"
#include "Log.h"
#include "../Core/GameContainer.h"

asIScriptEngine* AngelScriptUtil::s_engine = NULL;

void AngelScriptUtil_MessageCallback(const asSMessageInfo* msg, void *param) {
	//ARK2D::getLog()->e(msg->message);
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);

}
void AngelScriptUtil_Print(const string& s) {
	ARK2D::getLog()->i(s);
}

asIScriptEngine* AngelScriptUtil::restart() {
	if (s_engine != NULL) {
		s_engine->ShutDownAndRelease();
		s_engine = NULL;
	}
	return getEngine();
}
asIScriptEngine* AngelScriptUtil::getEngine() {
	if (s_engine == NULL)
	{
		// Print out what options we have compiled AngelScript under. Useful for debugging.
		const char* options = asGetLibraryOptions();
		ARK2D::getLog()->e(options);

		// Create the script engine
		s_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

		// Set the message callback to receive information on errors in human readable form.
		int r = s_engine->SetMessageCallback(asFUNCTION(AngelScriptUtil_MessageCallback), 0, asCALL_CDECL); assert( r >= 0 );

		// AngelScript doesn't have a built-in string type, as there is no definite standard
		// string type for C++ applications. Every developer is free to register it's own string type.
		// The SDK do however provide a standard add-on for registering a string type, so it's not
		// necessary to implement the registration yourself if you don't want to.
		RegisterStdString(s_engine);

		// Logging
		r = s_engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(AngelScriptUtil_Print), asCALL_CDECL); assert( r >= 0 );

		// Resource loading
		r = s_engine->RegisterObjectType("Resource", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Resource@ opAssign(Resource@ &in)", asMETHOD(Resource, operator=), asCALL_THISCALL); assert( r >= 0 );

		// Image resource
		r = s_engine->RegisterObjectType("Image", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Image@ asImage()", asMETHOD(Resource, asImage), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "Image@ opAssign(Image@ &in)", asMETHOD(Image, operator=), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "void draw(float, float)", asMETHODPR(Image, draw, (float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "void drawCenteredScaled(float, float, float, float)", asMETHODPR(Image, drawCenteredScaled, (float, float, float, float), void), asCALL_THISCALL); assert( r >= 0 );

		// Sound resource
		r = s_engine->RegisterObjectType("Sound", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Sound@ asSound()", asMETHOD(Resource, asSound), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Sound", "void play()", asMETHODPR(Sound, play, (void), void), asCALL_THISCALL); assert( r >= 0 );

		// Input
		r = s_engine->RegisterObjectType("Input", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Input", "bool isKeyDown(uint)", asMETHODPR(Input, isKeyDown, (unsigned int), bool), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Input", "bool isKeyPressed(uint)", asMETHODPR(Input, isKeyPressed, (unsigned int), bool), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Input", "bool isKeyReleased(uint)", asMETHODPR(Input, isKeyReleased, (unsigned int), bool), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_LEFT", (void*) &Input::KEY_LEFT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_RIGHT", (void*) &Input::KEY_RIGHT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_UP", (void*) &Input::KEY_UP); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_DOWN", (void*) &Input::KEY_DOWN); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_SPACE", (void*) &Input::KEY_SPACE); assert( r >= 0 );

        // Timer
        r = s_engine->RegisterObjectType("Timer", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Timer", "float getDelta()", asMETHODPR(GameTimer, getDelta, (void) const, float), asCALL_THISCALL); assert( r >= 0 );
		

		// Renderer
		r = s_engine->RegisterObjectType("Renderer", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void fillRect(float x, float y, int width, int height)", asMETHOD(Renderer,fillRect), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void fillCircle(float x, float y, int radius, int points)", asMETHOD(Renderer,fillCircle), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void setDrawColorf(float r, float g, float b, float a)", asMETHOD(Renderer, setDrawColorf), asCALL_THISCALL); assert( r >= 0 );

		// Globals
		r = s_engine->RegisterGlobalFunction("Resource@ get(const string &in)", asFUNCTIONPR(Resource::get, (string), Resource*), asCALL_CDECL); assert( r >= 0 );
		r = s_engine->RegisterGlobalFunction("Input@ getInput()", asFUNCTIONPR(ARK2D::getInput, (void), Input*), asCALL_CDECL); assert( r >= 0 );
		r = s_engine->RegisterGlobalFunction("void error(const string &in)", asFUNCTIONPR(ErrorDialog::createAndShow, (string), void), asCALL_CDECL); assert( r >= 0 );

	}
	return s_engine;

}