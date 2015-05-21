
#include "AngelScriptUtil.h"

#include "../ARK2D.h"
#include "Log.h"
#include "../Core/GameContainer.h"
#include "../Util/Containers/Pool.h"

asIScriptEngine* AngelScriptUtil::s_engine = NULL;

void AngelScriptUtil_MessageCallback(const asSMessageInfo* msg, void *param) {
	//ARK2D::getLog()->e(msg->message);
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	 

	stringstream text;
	text << msg->section << "(" << msg->row << ", " << msg->col << " : " << type << " : " << msg->message;
	ARK2D::getLog()->i(text.str());
	if (msg->type == asMSGTYPE_ERROR) {
		ErrorDialog::createAndShow(text.str());
	}
	//printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
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

//Resource* Resource_Get_Generic(asIScriptGeneric* gen)
//{
//	string s = gen->GetArgTypeId(0);
//	return Resource::get("");
//}

template <class T>
class MyTestClass {
	public:
		MyTestClass() {

		}
		void hello() {

		}
		~MyTestClass() {

		}
};

void AngelScriptUtil::exceptionCheckInternal(string file, signed int line, asIScriptContext* ctx, asIScriptFunction* func, signed int r) {
	if (r != asEXECUTION_FINISHED) 
	{
		// The execution didn't complete as expected. Determine what happened.
		if (r == asEXECUTION_EXCEPTION) 
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			ErrorDialog::createAndShow(StringUtil::append("An AngelScript exception occurred. Please correct the code and try again: ", ctx->GetExceptionString()));
			ErrorDialog::createAndShow(AngelScriptUtil::getExceptionInfo(ctx, true));
		}
		else {
			ErrorDialog::createAndShow(StringUtil::append("An AngelScript exception occured, but we don't know why.", r));
		}
	}
} 

void AngelScriptUtil::assertInternal(string file, signed int line, signed int r) {
	if (r < 0) {
		stringstream out;
		out << "assert failed: " << r << ". line: " << line << ". file: " << file;
		ErrorDialog::createAndShow(out.str());
		//assert(false);
		exit(0);
	}
}

string AngelScriptUtil::getExceptionInfo(asIScriptContext *ctx, bool showStack)
{
	if( ctx->GetState() != asEXECUTION_EXCEPTION ) return "";

	stringstream text;

	const asIScriptFunction *function = ctx->GetExceptionFunction();
	text << "func: " << function->GetDeclaration() << "\n";
	text << "modl: " << function->GetModuleName() << "\n";
	text << "sect: " << function->GetScriptSectionName() << "\n";
	text << "line: " << ctx->GetExceptionLineNumber() << "\n";
	text << "desc: " << ctx->GetExceptionString() << "\n";

	if( showStack )
	{
		text << "--- call stack ---\n";
		for( asUINT n = 1; n < ctx->GetCallstackSize(); n++ )
		{
			function = ctx->GetFunction(n);
			if( function )
			{
				if( function->GetFuncType() == asFUNC_SCRIPT )
				{
					text << function->GetScriptSectionName() << " (" << ctx->GetLineNumber(n) << "): " << function->GetDeclaration() << "\n";
				}
				else
				{
					// The context is being reused by the application for a nested call
					text << "{...application...}: " << function->GetDeclaration() << "\n";
				}
			}
			else
			{
				// The context is being reused by the script engine for a nested call
				text << "{...script engine...}\n";
			}
		}
	}

	return text.str();
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

		// Pools
		r = s_engine->RegisterObjectType("Pool<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
		r = s_engine->RegisterObjectType("PoolIterator<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
        r = s_engine->RegisterObjectMethod("Pool<T>", "PoolIterator<T>@ iterator()", asMETHOD(Pool<const char*>, newiteratorref), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("PoolIterator<T>", "T@ next()", asMETHOD(PoolIterator<const char*>, next), asCALL_THISCALL); assert( r >= 0 );

		//r = s_engine->RegisterObjectBehaviour("Pool<T>", asBEHAVE_FACTORY, "Pool<T>@ f(int&in)", asFUNCTIONPR(myTemplateFactory, (asIObjectType*), Pool*), asCALL_CDECL); assert( r >= 0 );

		// Globals
		r = s_engine->RegisterGlobalFunction("Resource@ getResource(const string)", asFUNCTIONPR(Resource::get, (string), Resource*), asCALL_CDECL); assert(r >= 0);
		r = s_engine->RegisterGlobalFunction("Input@ getInput()", asFUNCTIONPR(ARK2D::getInput, (void), Input*), asCALL_CDECL); assert( r >= 0 );
		r = s_engine->RegisterGlobalFunction("void error(const string)", asFUNCTIONPR(ErrorDialog::createAndShow, (string), void), asCALL_CDECL); assert( r >= 0 );
		 
	}
	return s_engine;

}