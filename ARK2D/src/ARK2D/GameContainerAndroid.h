/*
 * GameContainerAndroid.h
 *
 *  Created on: Feb 13, 2012
 *      Author: ashleygwinnell
 */

#ifndef GAMECONTAINERANDROID_H_
#define GAMECONTAINERANDROID_H_

#include "ARK2D.h"

class GameContainer;
using namespace std;

class GameContainerPlatform {
	friend class GameContainer;

	public:
		GameContainer* m_container;

	public:
		GameContainerPlatform();
		void setTitle(string title);
		bool initOpenAL();
		bool deinitOpenAL();
		string getResourcePath() const;
		virtual ~GameContainerPlatform();

		static void* getARK2DResource(int resourceId, int resourceType);
};

#endif /* GAMECONTAINERANDROID_H_ */
