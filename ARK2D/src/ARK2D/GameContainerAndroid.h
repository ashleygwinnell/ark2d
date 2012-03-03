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
		string m_externalDataStr;

	public:
		GameContainerPlatform();
		void setTitle(string title);
		void initGL(string clearColorStr, int w, int h);
		void initGL2D(int w, int h);
		bool initOpenAL();
		bool deinitOpenAL();
		string getResourcePath() const;
		string getExternalResourcePath() const;
		virtual ~GameContainerPlatform();

		static void* getARK2DResource(int resourceId, int resourceType);
};

#endif /* GAMECONTAINERANDROID_H_ */
