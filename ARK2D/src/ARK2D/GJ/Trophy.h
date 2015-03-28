/*
 * Trophy.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_TROPHY_H_
#define ARK_GJ_TROPHY_H_

#include "PropertyItem.h"

namespace ARK {
	namespace GJ {

		class ARK2D_API Trophy : public PropertyItem {
			public:
				static const unsigned int DIFFICULTY_BRONZE = 0;
				static const unsigned int DIFFICULTY_SILVER = 1;
				static const unsigned int DIFFICULTY_GOLD = 2;
				static const unsigned int DIFFICULTY_PLATINUM = 3;

				static const unsigned int ACHIEVED_FALSE = 0;
				static const unsigned int ACHIEVED_TRUE = 1;
				static const unsigned int ACHIEVED_EMPTY = 2;

			public:
				Trophy();
				string getName();
				unsigned int getDifficulty();
				virtual ~Trophy();
		};

	}
}

#endif /* ARK_GJ_TROPHY_H_ */
