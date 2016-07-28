/*
 * FlowControl.h
 *
 *  Created on: 28 March 2015
 *      Author: Ashley
 */

#ifndef ARK_NET_FLOWCONTROL_H_
#define ARK_NET_FLOWCONTROL_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

namespace ARK {
	namespace Net {

		class ARK2D_API FlowControl
		{
			private:
				enum Mode {
					Good,
					Bad
				};

				Mode mode;
				float penalty_time;
				float good_conditions_time;
				float penalty_reduction_accumulator;

			public:

				FlowControl();
				void reset();
				void update(float deltaTime, float rtt);
				float getSendRate();

		};
	}
}

#endif /* ARK_NET_FLOWCONTROL_H_ */
