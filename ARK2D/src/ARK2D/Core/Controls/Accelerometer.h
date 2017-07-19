/*
 * Accelerometer.h
 *
 *  Created on: 10-Jul-2017
 *      Author: Ashley
 */

#ifndef ARK2D_CORE_CONTROLS_ACCELEROMETER_H_
#define ARK2D_CORE_CONTROLS_ACCELEROMETER_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"
#include "../Geometry/Vector3.h"

#include <string>
#include <set>
#include <map>
#include <vector>
using namespace std;

#ifdef ARK2D_IPHONE
	#import <UIKit/UIKit.h>
	#import <CoreMotion/CoreMotion.h>
#endif

namespace ARK {
    namespace Core {
        namespace Controls {
        	class ARK2D_API Accelerometer {
        		public:
        			#ifdef ARK2D_IPHONE
        				CMMotionManager* m_motionManager;
        			#endif

                    bool m_enabled;
        			Vector3<float>* m_acceleration;
        			Vector3<float>* m_accelerationMax;

        		private:
        			Accelerometer();

        		public:
        			void setEnabled(bool b);
                    bool isEnabled();
                    inline Vector3<float>* acceleration() {
                        return m_acceleration;
                    }

        			static Accelerometer* s_instance;
        			static Accelerometer* getInstance();

        	};
        }
    }
}


#endif
