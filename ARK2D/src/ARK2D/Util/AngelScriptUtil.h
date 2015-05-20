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

namespace ARK {
	namespace Util {

		class ARK2D_API AngelScriptUtil {
			public:
		        static asIScriptEngine* s_engine;
				static asIScriptEngine* getEngine();
				static asIScriptEngine* restart();
		};

	}
}

#endif