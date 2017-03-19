// Common/DLL.h
// Date: 24 May 2015
#ifndef ARK_COMMON_DLL_INCLUDES_H_
#define ARK_COMMON_DLL_INCLUDES_H_

	// This file primarily exists because of Windows's ridiculous need
	// to include __declspec( dllexport/dllimport ) before every class definition.

	#if (defined(__AVM2__) || \
         defined(ARK2D_EMSCRIPTEN_JS) || \
		 defined(ARK2D_ANDROID) || \
		 defined(ARK2D_IPHONE) || \
		 defined(ARK2D_MACINTOSH) || \
		 defined(__linux__) || \
		 (defined(_WIN32) && defined(__GNUC__)) \
		 )
		#define ARK2D_API
	#elif defined(ARK2D_WINDOWS_PHONE_8)
		#ifdef ARK2D_WINDOWS_DLL
			#define ARK2D_API __declspec( dllexport )
		#else
			#define ARK2D_API __declspec( dllimport )
		#endif
	#elif (defined(_WIN32) && (defined(_MSC_FULL_VER) || defined(_MSC_VER))) || defined(ARK2D_XBOXONE)
		#if defined(ARK2D_WINDOWS_DLL) || defined(ARK2D_XBOXONE_DLL)
			#define ARK2D_API __declspec( dllexport )
			#define ANGELSCRIPT_EXPORT
		#else
			#define ARK2D_API __declspec( dllimport )
			#define ANGELSCRIPT_DLL_LIBRARY_IMPORT
		#endif

		#if defined(ARK2D_WINDOWS_VS)
			#ifndef Assert
				#if defined( ARK2D_DEBUG )
					#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
				#else
					#define Assert(b)
				#endif //DEBUG || _DEBUG
			#endif
		#endif

		#ifdef ARK2D_WINDOWS_STORE
			#include <pch.h>
			#undef min
			#undef max
			//#define sprintf sprintf_s
		#endif

		#ifndef SAFE_RELEASE
		#define SAFE_RELEASE(x) \
		   if(x != NULL)        \
		   {                    \
		      x->Release();     \
		      x = NULL;         \
		   }
		#endif

		#define NOMINMAX
	#endif


#endif
