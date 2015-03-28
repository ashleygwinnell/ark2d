
/*
 * MidiUtil.h
 *
 *  Created on: 19 Jan 2014
 *      Author: Ashley
 */

#ifndef MIDIUTIL_H_
#define MIDIUTIL_H_

#include <iostream>
#include <fstream>
#include <string>
#include "../Includes.h"

using namespace std;

#if defined(ARK2D_MACINTOSH) 
	#import <CoreAudio/CoreAudio.h>
	#import <CoreAudioKit/CoreAudioKit.h>
	#import <CoreMIDI/CoreMIDI.h>
#endif

namespace ARK {
	namespace Util {
	
		class ARK2D_API MidiUtil {
			public:
				
				void printDevices();

		};
	}
}

#endif /* MIDIUTIL_H_ */
