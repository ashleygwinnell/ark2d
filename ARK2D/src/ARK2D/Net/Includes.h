// Common/Net.h
// Date: 28 March 2015
#ifndef COMMON_NET_INCLUDES_H_
#define COMMON_NET_INCLUDES_H_

	// Flascc
	#if defined(__AVM2__) || defined(ARK2D_FLASCC)

	// Emscripten
	#elif defined(ARK2D_EMSCRIPTEN_JS)

		#define ARK2D_API

		#include <iostream>
		#include <sys/socket.h>
		#include <netinet/in.h>
		#include <fcntl.h>

	// Android
	// #elif defined(ARK2D_ANDROID)

	// iOS
	// #elif defined(ARK2D_IPHONE)

	// WP8
	#elif defined(ARK2D_WINDOWS_PHONE_8)

 	// Windows
	#elif defined(_WIN32) || defined(ARK2D_WINDOWS)

		#include <winsock2.h>
		//#pragma comment( lib, "wsock32.lib" )
		#pragma comment(lib, "ws2_32.lib")

		#if defined(ARK2D_WINDOWS_DLL) || defined(ARK2D_XBOXONE_DLL)
			#define ARK2D_API __declspec( dllexport )
		#else
			#define ARK2D_API __declspec( dllimport )
		#endif

	// OS X & Linux & Android.
	#elif defined(ARK2D_MACINTOSH) || defined(__linux__) || defined(ARK2D_UBUNTU_LINUX) || defined(ARK2D_IPHONE) || defined(ARK2D_ANDROID)

		#define ARK2D_API

		#include <sys/socket.h>
		#include <netinet/in.h>
		#include <fcntl.h>

	#endif

	// Common
	#include <assert.h>
	#include <vector>
	#include <map>
	#include <stack>
	#include <list>
	#include <algorithm>
	#include <functional>

#endif // COMMON_NET_INCLUDES_H_

