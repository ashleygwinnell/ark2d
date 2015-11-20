
#include "AngelScriptUtil.h"

#include "../ARK2D.h"
#include "Log.h"
#include "KeyPairFile.h"
#include "../Core/GameContainer.h"
#include "../Font/BMFont.h"
#include "../Graphics/Image.h"
#include "../Graphics/Renderer.h"
#include "../Geometry/Rectangle.h"
#include "../Pathfinding/AStar.h"
#include "../Util/Containers/Pool.h"
#include "../Audio/Sound.h"

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

int AngelScriptUtil_IncludeCallback(const char* include, const char* from, CScriptBuilder* builder, void* userParam) {

	if (!Resource::exists(include)) {
		stringstream s; s << "Include file '" << include << "' does not exist.";
		ARK2D::getLog()->e(s.str());
		return -1;
	}

	String* s = Resource::get(include)->asString();
	if (s == NULL) { return -1; }

	string script = s->getc();
	int r = builder->AddSectionFromMemory(include, script.c_str());
	AngelScriptUtil_assert(r);

	delete s;
	s = NULL;

	return 1;
}


void AngelScriptUtil_Print(const string& s) {
	ARK2D::getLog()->i(s);
}
void AngelScriptUtil_Print_UINT(unsigned int i) {
	string s = Cast::toString<unsigned int >(i);
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

void AngelScriptUtil::functionCheckInternal(string file, signed int line, asIScriptFunction* func, string decl) {
	if( func == 0 )
	{
		// The function couldn't be found. Instruct the script writer to include the expected function in the script.
		stringstream s;
		s << "The script must have the function '" << decl << "'. Please add it and try again.";
		ErrorDialog::createAndShow(s.str());
		exit(0);
        return;
	}
} 

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

void AngelScriptUtil::compileAndRunOnce(string moduleName, string sourceFile, string functionDecl) 
{
	asIScriptEngine* engine = AngelScriptUtil::getEngine();
   	
	CScriptBuilder builder;
	int r = builder.StartNewModule(engine, moduleName.c_str());
	AngelScriptUtil_assert(r);
    
   	builder.SetIncludeCallback(AngelScriptUtil_IncludeCallback, NULL);

	string script = Resource::get(sourceFile)->asString()->getc();
	r = builder.AddSectionFromMemory(sourceFile.c_str(), script.c_str());
	AngelScriptUtil_assert(r);

	r = builder.BuildModule(); 
	AngelScriptUtil_assert(r);

	// Now we run it.
	// Find the function that is to be called. 
	asIScriptModule* mod = engine->GetModule(moduleName.c_str());
	asIScriptFunction* func = mod->GetFunctionByDecl(functionDecl.c_str());
	AngelScriptUtil_functionCheck(func, functionDecl);

	// Create our context, prepare it, and then execute
	asIScriptContext *ctx = engine->CreateContext();
	ctx->Prepare(func); 
	r = ctx->Execute();
	AngelScriptUtil_execeptionCheck(ctx, func, r);
	ctx->Release();

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
		r = s_engine->RegisterGlobalFunction("void print(uint val)", asFUNCTION(AngelScriptUtil_Print_UINT), asCALL_CDECL); assert( r >= 0 );

		// Resource loading
		r = s_engine->RegisterObjectType("Resource", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Resource@ opAssign(Resource@ &in)", asMETHOD(Resource, operator=), asCALL_THISCALL); assert( r >= 0 );

		// Image resource
		r = s_engine->RegisterObjectType("Image", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Image@ asImage()", asMETHOD(Resource, asImage), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "int getWidth()", asMETHOD(Image, getWidth), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "int getHeight()", asMETHOD(Image, getHeight), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "Image@ rotate(double)", asMETHODPR(Image, rotate, (double), SceneNode*), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "Image@ scale(float, float)", asMETHODPR(Image, scale, (float, float), SceneNode*), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "Image@ flip(bool, bool)", asMETHODPR(Image, flip, (bool, bool), Image*), asCALL_THISCALL); assert( r >= 0 );		
		r = s_engine->RegisterObjectMethod("Image", "void draw(float, float)", asMETHODPR(Image, draw, (float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "void drawCentered(float, float)", asMETHODPR(Image, drawCentered, (float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "void drawCenteredScaled(float, float, float, float)", asMETHODPR(Image, drawCenteredScaled, (float, float, float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Image", "Image@ opAssign(Image@ &in)", asMETHOD(Image, operator=), asCALL_THISCALL); assert( r >= 0 );

		// Spritesheet
		r = s_engine->RegisterObjectType("SpriteSheet", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "SpriteSheet@ asSpriteSheet()", asMETHOD(Resource, asSpriteSheetDescription), asCALL_THISCALL); assert( r >= 0 );
		// SpriteSheetStore::addSheet(SpriteSheetDescription* desc, Image* sheet);

		// Font/s
		r = s_engine->RegisterObjectType("Font", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Font@ asFont()", asMETHOD(Resource, asFont), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectType("BMFont", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Font", "BMFont@ asBMFont()", asMETHOD(ARK::Font::Font, asBMFont), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("BMFont", "uint getStringWidth(const string)", asMETHODPR(ARK::Font::BMFont, getStringWidth, (const string&) const, unsigned int), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("BMFont", "uint getStringHeight(const string)", asMETHODPR(ARK::Font::BMFont, getStringHeight, (const string&) const, unsigned int), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("BMFont", "uint getLineHeight()", asMETHOD(ARK::Font::BMFont, getLineHeight), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("BMFont", "void drawString(const string, float, float, int = -1, int = -1, float = 0.0f, float = 1.0f)", asMETHODPR(ARK::Font::BMFont, drawString, (const string, float, float, int, int, float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("BMFont", "Image@ getImage()", asMETHOD(ARK::Font::BMFont, getImage), asCALL_THISCALL); assert( r >= 0 );

		// Sound resource
		r = s_engine->RegisterObjectType("Sound", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "Sound@ asSound()", asMETHOD(Resource, asSound), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Sound", "void play()", asMETHODPR(Sound, play, (void), void), asCALL_THISCALL); assert( r >= 0 );

		// Input
		r = s_engine->RegisterObjectType("Input", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectType("Gamepad", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterGlobalProperty("uint KEY_LEFT", (void*) &Input::KEY_LEFT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_RIGHT", (void*) &Input::KEY_RIGHT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_UP", (void*) &Input::KEY_UP); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_DOWN", (void*) &Input::KEY_DOWN); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_A", (void*) &Input::KEY_A); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_B", (void*) &Input::KEY_B); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_C", (void*) &Input::KEY_C); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_D", (void*) &Input::KEY_D); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_E", (void*) &Input::KEY_E); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F", (void*) &Input::KEY_F); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_G", (void*) &Input::KEY_G); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_H", (void*) &Input::KEY_H); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_I", (void*) &Input::KEY_I); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_J", (void*) &Input::KEY_J); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_K", (void*) &Input::KEY_K); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_L", (void*) &Input::KEY_L); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_M", (void*) &Input::KEY_M); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_N", (void*) &Input::KEY_N); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_O", (void*) &Input::KEY_O); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_P", (void*) &Input::KEY_P); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_Q", (void*) &Input::KEY_Q); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_R", (void*) &Input::KEY_R); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_S", (void*) &Input::KEY_S); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_T", (void*) &Input::KEY_T); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_U", (void*) &Input::KEY_U); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_V", (void*) &Input::KEY_V); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_W", (void*) &Input::KEY_W); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_X", (void*) &Input::KEY_X); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_Y", (void*) &Input::KEY_Y); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_Z", (void*) &Input::KEY_Z); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_0", (void*) &Input::KEY_0); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_1", (void*) &Input::KEY_1); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_2", (void*) &Input::KEY_2); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_3", (void*) &Input::KEY_3); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_4", (void*) &Input::KEY_4); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_5", (void*) &Input::KEY_5); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_6", (void*) &Input::KEY_6); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_7", (void*) &Input::KEY_7); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_8", (void*) &Input::KEY_8); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_9", (void*) &Input::KEY_9); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F1", (void*) &Input::KEY_F1); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F2", (void*) &Input::KEY_F2); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F3", (void*) &Input::KEY_F3); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F4", (void*) &Input::KEY_F4); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F5", (void*) &Input::KEY_F5); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F6", (void*) &Input::KEY_F6); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F7", (void*) &Input::KEY_F7); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F8", (void*) &Input::KEY_F8); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F9", (void*) &Input::KEY_F9); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F10", (void*) &Input::KEY_F10); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F11", (void*) &Input::KEY_F11); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_F12", (void*) &Input::KEY_F12); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_LSHIFT", (void*) &Input::KEY_LSHIFT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_LCONTROL", (void*) &Input::KEY_LCONTROL); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_SPACE", (void*) &Input::KEY_SPACE); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_ENTER", (void*) &Input::KEY_ENTER); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_TAB", (void*) &Input::KEY_TAB); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_BACKSPACE", (void*) &Input::KEY_BACKSPACE); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint KEY_INSERT", (void*) &Input::KEY_INSERT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint MOUSE_BUTTON_LEFT", (void*) &Input::MOUSE_BUTTON_LEFT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint MOUSE_BUTTON_RIGHT", (void*) &Input::MOUSE_BUTTON_RIGHT); assert( r >= 0 );
        r = s_engine->RegisterGlobalProperty("uint MOBILE_BACK", (void*) &Input::MOBILE_BACK); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "bool isKeyDown(uint)", asMETHODPR(Input, isKeyDown, (unsigned int), bool), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Input", "bool isKeyPressed(uint)", asMETHODPR(Input, isKeyPressed, (unsigned int), bool), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Input", "bool isKeyReleased(uint)", asMETHODPR(Input, isKeyReleased, (unsigned int), bool), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "int getMouseX()", asMETHOD(Input, getMouseX), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "int getMouseY()", asMETHOD(Input, getMouseY), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "bool isAnyGamepadButtonPressed()", asMETHOD(Input, isAnyGamepadButtonPressed), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "int countGamepads()", asMETHOD(Input, countGamepads), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "Gamepad@ getGamepad(uint id)", asMETHOD(Input, getGamepad), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Input", "Gamepad@ getGamepadByIndex(uint id)", asMETHOD(Input, getGamepadByIndex), asCALL_THISCALL); assert( r >= 0 );

        // Gamepads
        r = s_engine->RegisterObjectMethod("Gamepad", "bool isButtonPressed(uint button)", asMETHOD(Gamepad, isButtonPressed), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Gamepad", "bool isButtonReleased(uint button)", asMETHOD(Gamepad, isButtonReleased), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Gamepad", "bool isButtonDown(uint button)", asMETHOD(Gamepad, isButtonDown), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Gamepad", "bool isAnyButtonPressed()", asMETHOD(Gamepad, isAnyButtonPressed), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Gamepad", "float getAxisValue()", asMETHOD(Gamepad, getAxisValue), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Gamepad", "string getName()", asMETHOD(Gamepad, getName), asCALL_THISCALL); assert( r >= 0 );


        // Timer
        r = s_engine->RegisterObjectType("Timer", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Timer", "float getDelta()", asMETHODPR(GameTimer, getDelta, (void) const, float), asCALL_THISCALL); assert( r >= 0 );
		

		// Renderer
		r = s_engine->RegisterObjectType("Renderer", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void pushMatrix()", asMETHOD(Renderer, pushMatrix), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void popMatrix()", asMETHOD(Renderer, popMatrix), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void translate(float x, float y, float z)", asMETHODPR(Renderer, translate, (float, float, float) const, void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void scale(float x, float y, float z)", asMETHODPR(Renderer, scale, (float, float ,float) const, void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void rotate(float degrees)", asMETHODPR(Renderer, rotate, (float) const, void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void fillRect(float x, float y, int width, int height)", asMETHOD(Renderer,fillRect), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void fillCircle(float x, float y, int radius, int points)", asMETHOD(Renderer,fillCircle), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void drawLine(float x1, float y1, float x2, float y2)", asMETHODPR(Renderer, drawLine, (float, float, float, float) const, void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void setDrawColor(float r, float g, float b, float a)", asMETHODPR(Renderer, setDrawColorf, (float, float, float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Renderer", "void setDrawColor(string hex)", asMETHODPR(Renderer, setDrawColor, (string), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterGlobalProperty("int ALIGN_LEFT", (void*) &ARK::Graphics::Renderer::ALIGN_LEFT); assert( r >= 0 );
		r = s_engine->RegisterGlobalProperty("int ALIGN_RIGHT", (void*) &Renderer::ALIGN_RIGHT); assert( r >= 0 );
		r = s_engine->RegisterGlobalProperty("int ALIGN_CENTER", (void*) &Renderer::ALIGN_CENTER); assert( r >= 0 );
		r = s_engine->RegisterGlobalProperty("int ALIGN_TOP", (void*) &Renderer::ALIGN_TOP); assert( r >= 0 );
		r = s_engine->RegisterGlobalProperty("int ALIGN_BOTTOM", (void*) &Renderer::ALIGN_BOTTOM); assert( r >= 0 );


		// Simple key/pair save data.
		r = s_engine->RegisterObjectType("KeyPairFile", 0, asOBJ_REF | asOBJ_NOCOUNT); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Resource", "KeyPairFile@ asKeyPairFile()", asMETHOD(Resource, asKeyPairFile), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void save()", asMETHOD(KeyPairFile, save), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void clear()", asMETHOD(KeyPairFile, clear), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void clearUnsaved()", asMETHOD(KeyPairFile, clearUnsaved), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void set(string key, string val)", asMETHODPR(KeyPairFile, set, (string, string), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void set(string key, bool val)", asMETHODPR(KeyPairFile, set, (string, bool), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void set(string key, uint val)", asMETHODPR(KeyPairFile, set, (string, unsigned int), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "void set(string key, float val)", asMETHODPR(KeyPairFile, set, (string, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "bool getBoolean(string key, bool defaultValue)", asMETHODPR(KeyPairFile, getBoolean, (string, bool), bool), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "uint getInteger(string key, uint defaultValue)", asMETHODPR(KeyPairFile, getInteger, (string, unsigned int), unsigned int), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "float getFloat(string key, float defaultValue)", asMETHODPR(KeyPairFile, getFloat, (string, float), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "string getString(string key, string defaultValue)", asMETHODPR(KeyPairFile, getString, (string, string), string), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("KeyPairFile", "string toString()", asMETHOD(KeyPairFile, toString), asCALL_THISCALL); assert( r >= 0 );

		// Geometry
		r = s_engine->RegisterObjectType("Shape<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);

		r = s_engine->RegisterObjectType("Rectangle", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
		r = s_engine->RegisterObjectMethod("Rectangle", "void set(float x, float y, float width, float height)", asMETHODPR(Rectangle, set, (float, float, float, float), void), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getMinX()", asMETHODPR(Rectangle, getMinX, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getMaxX()", asMETHODPR(Rectangle, getMaxX, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getCenterX()", asMETHODPR(Rectangle, getCenterX, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getMinY()", asMETHODPR(Rectangle, getMinY, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getMaxY()", asMETHODPR(Rectangle, getMaxY, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getCenterY()", asMETHODPR(Rectangle, getCenterY, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getWidth()", asMETHODPR(Rectangle, getWidth, (void), float), asCALL_THISCALL); assert( r >= 0 );
		r = s_engine->RegisterObjectMethod("Rectangle", "float getHeight()", asMETHODPR(Rectangle, getHeight, (void), float), asCALL_THISCALL); assert( r >= 0 );

		r = s_engine->RegisterObjectType("Vector2<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
		r = s_engine->RegisterObjectMethod("Vector2<T>", "void set(float x, float y)", asMETHODPR(Vector2<float>, set, (float, float), void), asCALL_THISCALL); assert( r >= 0 );

		// Maths
		r = s_engine->RegisterObjectType("MathUtil", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
        r = s_engine->RegisterGlobalFunction("void MathUtil_moveAngle(Vector2<float>@ vector, double angle, float dist)", asFUNCTIONPR(MathUtil::moveAnglef, (Vector2<float>*, double, float), void), asCALL_CDECL); assert(r >= 0);
        r = s_engine->RegisterGlobalFunction("double MathUtil_distance(float ox, float oy, float x, float y)", asFUNCTIONPR(MathUtil::distance, (float, float, float, float), double), asCALL_CDECL); assert(r >= 0);
        r = s_engine->RegisterGlobalFunction("double MathUtil_angle(float ox, float oy, float x, float y)", asFUNCTIONPR(MathUtil::anglef, (float, float, float, float), double), asCALL_CDECL); assert(r >= 0);

        // Pathfinding
        r = s_engine->RegisterObjectType("AStarNode", 0, asOBJ_REF | asOBJ_NOCOUNT); assert(r >= 0);
        r = s_engine->RegisterObjectMethod("AStarNode", "void reset()", asMETHODPR(AStarNode, reset, (void), void), asCALL_THISCALL); assert( r >= 0 );

		// Pools
		r = s_engine->RegisterObjectType("Pool<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
		r = s_engine->RegisterObjectType("PoolIterator<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
        //r = s_engine->RegisterObjectMethod("Pool<T>", "const PoolIterator<T> iterator()", asMETHODPR(Pool<>, newiteratorref, (void), PoolIterator<>), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Pool<T>", "PoolIterator<T>@ useIterator()", asMETHOD(Pool<>, iterator), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Pool<T>", "PoolIterator<T>@ newIterator()", asMETHOD(Pool<>, newiterator), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("PoolIterator<T>", "bool hasNext()", asMETHODPR(PoolIterator<>, hasNext, (void), bool), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("PoolIterator<T>", "T@ next()", asMETHOD(PoolIterator<>, next), asCALL_THISCALL); assert( r >= 0 );

        r = s_engine->RegisterObjectType("Vector<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
        r = s_engine->RegisterObjectType("VectorIterator<class T>", 0, asOBJ_REF | asOBJ_NOCOUNT | asOBJ_TEMPLATE); assert(r >= 0);
        r = s_engine->RegisterObjectMethod("Vector<T>", "VectorIterator<T>@ useIterator()", asMETHOD(Vector<>, iterator), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("Vector<T>", "VectorIterator<T>@ newIterator()", asMETHOD(Vector<>, newiterator), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("VectorIterator<T>", "bool hasNext()", asMETHODPR(VectorIterator<>, hasNext, (void), bool), asCALL_THISCALL); assert( r >= 0 );
        r = s_engine->RegisterObjectMethod("VectorIterator<T>", "T@ next()", asMETHOD(VectorIterator<>, next), asCALL_THISCALL); assert( r >= 0 );
		//r = s_engine->RegisterObjectBehaviour("Pool<T>", asBEHAVE_FACTORY, "Pool<T>@ f(int&in)", asFUNCTIONPR(myTemplateFactory, (asIObjectType*), Pool*), asCALL_CDECL); assert( r >= 0 );

		// Globals
		r = s_engine->RegisterGlobalFunction("Resource@ getResource(const string)", asFUNCTIONPR(Resource::get, (string), Resource*), asCALL_CDECL); assert(r >= 0);
		r = s_engine->RegisterGlobalFunction("Input@ getInput()", asFUNCTIONPR(ARK2D::getInput, (void), Input*), asCALL_CDECL); assert( r >= 0 );
		r = s_engine->RegisterGlobalFunction("void error(const string)", asFUNCTIONPR(ErrorDialog::createAndShow, (string), void), asCALL_CDECL); assert( r >= 0 );
		 
	}
	return s_engine;

}