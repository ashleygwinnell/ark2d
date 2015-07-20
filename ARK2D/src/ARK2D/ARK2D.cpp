/*
 * ARK2D.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#include "ARK2D.h"
#include "Util/Log.h"
#include "Core/GameContainer.h"
#include "Util/FileUtil.h"
#include "Util/Strings.h"

#if defined( ARK2D_MISSING_C99_FUNCTIONS ) && defined( ARK2D_MISSING_C99_FUNCTIONDEFINITIONS )
 	double round( double number ) { return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5); }
	double log2( double n ) { return log( n ) / log( 2 ); }
#endif

GameContainer* ARK2D::s_container = 0;
Game* ARK2D::s_game = 0; 
Renderer* ARK2D::s_graphics = 0; 
Input* ARK2D::s_input = 0;  
Log* ARK2D::s_log = 0;
Strings* ARK2D::s_strings = 0;
bool ARK2D::s_debug = false;
bool ARK2D::s_expo = false;
bool ARK2D::s_steam = false;
vector<string> ARK2D::s_commandLineParameters;

void ARK2D::main(int args, char** argv) {
	// add to command line parameters somewhere. 
	// do things with these cmd params.
	string args_str = Cast::toString<int>(args);
	 
	// no arguments were passed
	ARK2D::getLog()->i("ARK2D main");
	ARK2D::getLog()->i(StringUtil::append("Command line parameters: ", args_str));
	if (args >= 2) 
    {
        for (int i = 1; i < args; ++i) {
        	char* param = argv[i];
			ARK2D::getLog()->i(param);
        	s_commandLineParameters.push_back( string(param) );
        }
    }


    if (hasCommandLineParameter("-verbose")) {
    	ARK2D::getLog()->i("Setting verbose logging.");
        ARK2D::getLog()->setFilter(Log::TYPE_ALL);
	}

	//if (hasCommandLineParameter("-dumplog")) {
	//	ARK2D::getLog()->i("Setting dumping log to file.");
	//}

}

GameContainer* ARK2D::getContainer() {
	return s_container;
} 

Game* ARK2D::getGame() {  
	return s_game;
}

Renderer* ARK2D::getRenderer() {
	return s_graphics;
}

Input* ARK2D::getInput() {
	return s_input;
}

GameTimer* ARK2D::getTimer() {
	return s_container->getTimer();
}
 
Log* ARK2D::getLog() {
	if (s_log == NULL) { 
		s_log = Log::getInstance();
	} 
	return s_log;
}
Strings* ARK2D::getStrings() {
	if (s_strings == NULL) {
        s_strings = ARK::Util::Strings::getInstance();
	}
	return s_strings;
}

bool ARK2D::isDebug() {
	return s_debug;
}
void ARK2D::setDebug(bool b) {
	s_debug = b;
}

bool ARK2D::isExpoMode() {
	return s_expo; 
}
void ARK2D::setExpoMode(bool b) {
	s_expo = b;
}
		
bool ARK2D::isSteam() {
	return s_steam;
}
void ARK2D::setSteam(bool b) {
	s_steam = b;
}

vector<string>* ARK2D::getCommandLineParameters() {
	return &s_commandLineParameters;
}
bool ARK2D::hasCommandLineParameter(string key) {
	for(unsigned int i = 0; i < s_commandLineParameters.size(); ++i) {
		if (s_commandLineParameters[i] == key) {
			return true;
		} else if (s_commandLineParameters[i].find("=") != string::npos) {
			vector<string> spl = StringUtil::split(s_commandLineParameters[i], "=");
			if (spl[0] == key) {
				return true;
			}
		}
	}
	return false;
}
string ARK2D::getCommandLineParameter(string key, string def) {
	for(unsigned int i = 0; i < s_commandLineParameters.size(); ++i) {
		if (s_commandLineParameters[i] == key) {
			return key;
		} else if (s_commandLineParameters[i].find("=") != string::npos) {
			vector<string> spl = StringUtil::split(s_commandLineParameters[i], "=");
			if (spl[0] == key) {
				return spl[1];
			}
		}
	}
	return def;
}
void ARK2D::printCommandLineParameters() {
	for(unsigned int i = 0; i < s_commandLineParameters.size(); ++i) {
		ARK2D::getLog()->i(s_commandLineParameters[i]);
	}
}

unsigned int ARK2D::getPlatform() {
	#if defined(ARK2D_FLASCC)
		return PLATFORM_BROWSER_FLASCC;
	#elif defined(ARK2D_EMSCRIPTEN_JS)
		return PLATFORM_BROWSER_JS;
	#elif defined(ARK2D_WINDOWS)
		return PLATFORM_WINDOWS;
	#elif defined(ARK2D_MACINTOSH)
		return PLATFORM_OSX;
	#elif defined(ARK2D_IPHONE)
		return PLATFORM_IPHONE;
	#elif defined(ARK2D_ANDROID)
		return PLATFORM_ANDROID;
	#elif defined(ARK2D_WINDOWS_PHONE_8)
		return PLATFORM_WINDOWS_PHONE_8;
	#elif defined(ARK2D_XBOXONE)
		return PLATFORM_XBOXONE;
	#elif defined(ARK2D_UBUNTU_LINUX)
		return PLATFORM_LINUX;
	#endif 

		
}
std::string ARK2D::getPlatformString() {
	#if defined(ARK2D_FLASCC)
		return "flascc";
	#elif defined(ARK2D_EMSCRIPTEN_JS)
		return "html5";
	#elif defined(ARK2D_WINDOWS)
		return "windows";
	#elif defined(ARK2D_MACINTOSH)
		return "osx";
	#elif defined(ARK2D_IPHONE)
		return "ios";
	#elif defined(ARK2D_ANDROID)
		return "android";
	#elif defined(ARK2D_WINDOWS_PHONE_8)
		return "wp8";
	#elif defined(ARK2D_XBOXONE)
		return "xbone";
	#elif defined(ARK2D_UBUNTU_LINUX)
		return "linux";
	#endif
}

bool ARK2D::is64BitArchitecture() {

	#ifdef ARK2D_WINDOWS

		bool ret = false;

		typedef bool (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, bool*);
		LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");

		if (fnIsWow64Process != NULL) {
			if (!fnIsWow64Process(GetCurrentProcess(), &ret)) {
				return false;
			}
		}
		return ret;

	#endif

	return false;
}

bool ARK2D::isBigEndian() {
	/*union {
		uint32_t i;
		char c[4];
	} bint = {0x01020304};

	return bint.c[0] == 1;*/
	static unsigned long x(1);
	static bool result(reinterpret_cast<unsigned char*>(&x)[0] == 0);
	return result;
}


