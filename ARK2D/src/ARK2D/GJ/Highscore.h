/*
 * Highscore.h
 *
 *  Created on: Jun 26, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK_GJ_HIGHSCORE_H_
#define ARK_GJ_HIGHSCORE_H_

#include "PropertyItem.h"

namespace ARK {
	namespace GJ {

		class Highscore : public PropertyItem {
			
			public:
				static const unsigned int TYPE_GUEST = 0;
				static const unsigned int TYPE_USER = 1;				

			public:
				Highscore();
				unsigned int getType() const;
				unsigned int getTime() const;
				string getScoreString() const;
				signed int getScoreValue() const; 
				string getExtraData() const;
				unsigned int getUserId() const;
				string getName() const; 
				virtual ~Highscore();
		};
	}
}

#endif /* ARK_GJ_HIGHSCORE_H_ */

