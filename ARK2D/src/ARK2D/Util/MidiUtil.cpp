/*
 * MidiUtil.cpp
 *
 *  Created on: 19 Jan 2014
 *      Author: Ashley
 */

#include "MidiUtil.h"

#include "../Includes.h"

#include <stdio.h>
#include "../ARK2D.h"
#include "StringUtil.h"
#include "Log.h"
#include "../Core/GameContainer.h"
#include "../Core/String.h"
 


namespace ARK {
	namespace Util {
 
		void MidiUtil::printDevices() {
			#if defined(ARK2D_MACINTOSH) 

				ItemCount numOfDevices = MIDIGetNumberOfDevices();
			    
				for (int i = 0; i < numOfDevices; i++) {
					MIDIDeviceRef midiDevice = MIDIGetDevice(i);
					NSDictionary* midiProperties;
			        
					CFDictionaryRef midiPropertiesCF = (__bridge CFDictionaryRef )midiProperties;
					MIDIObjectGetProperties(midiDevice, (CFPropertyListRef* )&midiPropertiesCF, YES);
					
					midiProperties = CFBridgingRelease(midiPropertiesCF);
					NSLog(@"Midi properties: %d \n %@", i, midiProperties);
				}
			#else 
				ARK2D::getLog()->w("MidiUtil::printDevices not available on this platform.");
			#endif 
		}

	}
}
