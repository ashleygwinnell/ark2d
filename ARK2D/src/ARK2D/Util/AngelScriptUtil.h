/*
 * AngelScriptUtil.h
 *
 *  Created on: 19 May 2015
 *      Author: Ashley
 */

#ifndef ARK_UTIL_ANGELSCRIPTUTIL_H_
#define ARK_UTIL_ANGELSCRIPTUTIL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"
#include "../Common/Libraries/angelscript.h"

#include "../Core/GameObject.h"
#include "../Core/Controls/ErrorDialog.h"

#ifdef ARK2D_EMSCRIPTEN_JS
 	#include <emscripten.h>
	extern "C" void EMSCRIPTEN_KEEPALIVE emscripten_html5helper_addHttpGetVar(char* key, char* value);
	extern "C" void EMSCRIPTEN_KEEPALIVE AngelScriptUtil_MessageCallback(const asSMessageInfo* msg, void *param);
#endif

namespace ARK {
	namespace Util {

		class ARK2D_API HTML5Helper {
			public:
				static map<string, string>* s_vars;

				static string getElementByIdContents(string id);
				static void setElementByIdContents(string id, string contents);
				static map<string, string>* getHTTPVars();

				static bool hasHTTPVar(string key);
				static string getHTTPVar(string key);
		};

        class ARK2D_API ScriptComponent : public ARK::Core::GameComponent {
			public:
				string m_path;
				string m_classname;
				ScriptComponent(string path);
				virtual void init();
				virtual void update();
		};


		class ARK2D_API AngelScriptGame {
			public:
				static AngelScriptGame* createFromFile(string scriptFile);
				static AngelScriptGame* createFromFile(string scriptFile, string moduleName);
				static AngelScriptGame* createFromString(string contents);
				static AngelScriptGame* createFromString(string contents, string moduleName);

			public:
				string m_module;
				string m_file;
			public:
				AngelScriptGame();
				void init();
				void update();
				void render();

			private:
				void _initFromFile();
				void _initFromMemory(string s);
		};

		class ARK2D_API AngelScriptUtil {
			public:
		        static asIScriptEngine* s_engine;
				static asIScriptEngine* getEngine();
				static asIScriptEngine* restart();
				static void compileAndRunOnce(string moduleName, string sourceFile, string functionDecl);


				// Format the details of the script exception into a human readable text
				static std::string getExceptionInfo(asIScriptContext* ctx, bool showStack = false);

			public:
				static void assertInternal(string file, signed int line, signed int r);
				static void functionCheckInternal(string file, signed int line, asIScriptFunction* func, string decl);
				static void exceptionCheckInternal(string file, signed int line, asIScriptContext* ctx, asIScriptFunction* func, signed int r);


		};

	}
}

#define AngelScriptUtil_assert(r) AngelScriptUtil::assertInternal(__FILE__, __LINE__, r);
#define AngelScriptUtil_functionCheck(func, s) AngelScriptUtil::functionCheckInternal(__FILE__, __LINE__, func, s);
#define AngelScriptUtil_execeptionCheck(ctx, func, r) AngelScriptUtil::exceptionCheckInternal(__FILE__, __LINE__, ctx, func, r);
ARK2D_API int AngelScriptUtil_IncludeCallback(const char* include, const char* from, CScriptBuilder* builder, void* userParam);

#endif
