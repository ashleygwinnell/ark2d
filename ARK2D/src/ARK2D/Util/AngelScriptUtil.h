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

namespace ARK {
	namespace Util {

        class ARK2D_API ScriptComponent : public ARK::Core::GameComponent {
			public:
				string m_path;
				string m_classname;
				ScriptComponent(string path);
				virtual void init();
				virtual void update();
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
