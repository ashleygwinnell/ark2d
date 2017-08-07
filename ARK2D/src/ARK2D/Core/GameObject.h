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
#include "SceneGraph/Scene.h"
#include <string>
#include <vector>
//#ifdef ARK2D_MACINTOSH
 //   #include <tr1/unordered_map>
  //  using namespace tr1;
//#else
    #include <unordered_map>
	#include <typeinfo>
//#endif

using namespace std;


namespace ARK {
	namespace Core {

		class ARK2D_API GameComponent {
			public:
				vector<const std::type_info*> dependencies;
				GameObject* gameObject;
				GameComponent();
				virtual void update();
				virtual void render();
		};

		/*!
		 * \brief A default game object for use in Vector and Pool Containers.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API GameObject : public SceneNode {
			protected:
				unordered_map<const std::type_info*, GameComponent*> components;
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

				// Override SceneNode
				virtual void update();
				virtual void render();

				virtual string toString();

				template <typename T>
				T* getComponent() {
					if (components.count(&typeid(T)) != 0) {
						return static_cast<T*>(components[&typeid(T)]);
					} else {
						return NULL;
					}
				}

				template <typename T>
				void addComponent(T* component) {
					components[&typeid(*component)] = component;
					component->gameObject = this;
				}

				virtual ~GameObject();

		};
	}
}

#endif /* ARKGAMEOBJECT_H_ */
