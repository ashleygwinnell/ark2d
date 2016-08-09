#if defined(_WIN32) || defined(_XBOX_ONE) || defined(ARK2D_WINDOWS)
	#ifndef ANGELSCRIPT_DLL_LIBRARY_IMPORT // abc.dll source code will define this macro before including this header
		#define ANGELSCRIPT_DLL_LIBRARY_IMPORT
	#endif
#endif

#include "../../vendor/angelscript/angelscript.h"
#include "../../vendor/angelscript/add_on/scriptstdstring/scriptstdstring.h"
#include "../../vendor/angelscript/add_on/scriptarray/scriptarray.h"
#include "../../vendor/angelscript/add_on/scriptbuilder/scriptbuilder.h"
