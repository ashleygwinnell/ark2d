/*
 * ARKGameObject.h
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#ifndef ARKGAMEOBJECT_H_
#define ARKGAMEOBJECT_H_

//#include "../Includes.h"
#include "../Namespaces.h"
#include "../Common/DLL.h"
#include <string>
using namespace std;

namespace ARK {
	namespace Core {

		/*!
		 * \brief A default game object for use in Vector and Pool Containers.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API GameObject {
			protected:
				bool m_pendingRemoval;
				int m_renderingLayer;

			public:
				GameObject();

				virtual bool isPendingRemoval();
				virtual void setPendingRemoval(bool b);
				virtual void onPrune(); /* This even fires when removed from an ARK::Util::Container::Vector and when made inactive in a ARKPool */

				virtual int getRenderingLayer();
				virtual void setRenderingLayer(int i);

				virtual void update(GameContainer* container, GameTimer* timer);
				virtual void render(GameContainer* container, Renderer* g);

				virtual string toString();

				virtual ~GameObject();

		};
	}
}

#endif /* ARKGAMEOBJECT_H_ */
