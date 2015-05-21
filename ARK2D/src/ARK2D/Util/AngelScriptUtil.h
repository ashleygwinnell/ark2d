/*
 * AngelScriptUtil.h
 *
 *  Created on: 19 May 2015
 *      Author: Ashley
 */

#ifndef ARK_UTIL_ANGELSCRIPTUTIL_H_
#define ARK_UTIL_ANGELSCRIPTUTIL_H_

#include "../vendor/angelscript/angelscript.h"
#include "../vendor/angelscript/add_on/scriptstdstring/scriptstdstring.h"

#include "../Includes.h"
#include "../Namespaces.h"

#include "../UI/ErrorDialog.h"

namespace ARK {
	namespace Util {

		class ARK2D_API AngelScriptUtil {
			public:
		        static asIScriptEngine* s_engine;
				static asIScriptEngine* getEngine();
				static asIScriptEngine* restart(); 
				

				// Format the details of the script exception into a human readable text
				static std::string getExceptionInfo(asIScriptContext* ctx, bool showStack = false);

			public:
				static void assertInternal(string file, signed int line, signed int r);
				static void exceptionCheckInternal(string file, signed int line, asIScriptContext* ctx, asIScriptFunction* func, signed int r);
				
		};

	}
}

#define AngelScriptUtil_assert(r) AngelScriptUtil::assertInternal(__FILE__, __LINE__, r);
#define AngelScriptUtil_execeptionCheck(ctx, func, r) AngelScriptUtil::exceptionCheckInternal(__FILE__, __LINE__, ctx, func, r);

#endif