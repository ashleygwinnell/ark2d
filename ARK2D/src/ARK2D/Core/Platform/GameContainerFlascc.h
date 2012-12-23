/*
 * GameContainerFlascc.h
 *
 *  Created on: Dec 19, 2012
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERMAC_H_
#define GAMECONTAINERMAC_H_

#include "../../Namespaces.h"
#include "../../ARK2D.h"

	#ifdef ARK2D_FLASCC

		#include <string>
		using namespace std;

		#include <string>
		#include <stdlib.h>

		namespace ARK {
			namespace Core {

				class GameContainerPlatform {
					friend class GameContainer;
					public:
						GameContainer* m_container;
						
						static void* getARK2DResource(int resourceId, int resourceType);

					public:
					//private:

						void doTick();
						
						void doEvents();
						
						bool initOpenAL();
						bool deinitOpenAL();

						void setTitle(std::string title);
						string getResourcePath() const;
				};

			}
		}

	#endif

#endif /* GAMECONTAINERMAC_H_ */
