/*
 * Accelerometer.cpp
 *
 *  Created on: 11-Jul-2017
 *      Author: Ashley
 */


#include "Accelerometer.h"

namespace ARK {
	namespace Core {
		namespace Controls {

			Accelerometer* Accelerometer::s_instance = NULL;
			Accelerometer* Accelerometer::getInstance() {
				if (s_instance == NULL) {
					s_instance = new Accelerometer();
				}
				return s_instance;
			}

			Accelerometer::Accelerometer(){
                m_acceleration = new Vector3<float>(0.0f, 0.0f, 1.0f);
                m_accelerationMax = new Vector3<float>(1.0f, 1.0f, 1.0f);
                
				#ifdef ARK2D_IPHONE
					m_enabled = true;
					m_motionManager = [[CMMotionManager alloc] init];
					m_motionManager.accelerometerUpdateInterval = 0.05; // 50fps
					m_motionManager.gyroUpdateInterval = 0.05;

					[m_motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue]
						withHandler:^(CMAccelerometerData *accelerometerData, NSError* error) {

							//[self outputAccelerationData:accelerometerData.acceleration];
							CMAcceleration acc = accelerometerData.acceleration;
                            m_acceleration->set(acc.x, acc.y, acc.z);
							
                            if (error){
								NSLog(@"%@",error);
							}
					}];
					[m_motionManager startGyroUpdatesToQueue:[NSOperationQueue currentQueue]
						withHandler:^(CMGyroData* gyroData, NSError* error) {
							//[self outputRotationData: gyroData.rotationRate];
							CMRotationRate rate = gyroData.rotationRate;
							//NSLog(@"%@", rate);
					}];
				#endif
			}

			void Accelerometer::setEnabled(bool b) {
				m_enabled = b;
			}
            bool Accelerometer::isEnabled() {
                return m_enabled;
            }

		}
	}
}
