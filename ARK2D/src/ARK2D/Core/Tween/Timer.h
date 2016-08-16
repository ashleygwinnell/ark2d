/*
 * Thread.h
 *
 *  Created on: 30 Mar 2011
 *      Author: Ashley
 */

#ifndef ARK_CORE_TWEEN_TIMER_H_
#define ARK_CORE_TWEEN_TIMER_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"

#include <string>
using std::string;

namespace ARK {
    namespace Core {
        namespace Tween {

            class ARK2D_API Timer {
                public:
                    float m_timer;
                    float m_duration;

                public:
                    Timer(float duration);
                    void limit();
                    void limit(int i);
                    void setTime(float time);
                    float getTime();
                    void setDuration(float duration);
                    float getDuration();
                    float percentage();
                    bool update();
                    bool update(float modifier);
                    void loop();
                    void reset();
                    string toString();
                    virtual ~Timer();

            };
        }
	}
}

#endif /* ARK_TIMER_H_ */
