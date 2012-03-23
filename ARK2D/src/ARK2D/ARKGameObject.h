/*
 * ARKGameObject.h
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#ifndef ARKGAMEOBJECT_H_
#define ARKGAMEOBJECT_H_

class GameContainer;
class GameTimer;
class Graphics;

/*!
 * \brief A default game object for use in Vector and Pool Containers.
 * @todo Rename to GameObject.
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class ARKGameObject {
	protected:
		bool m_pendingRemoval;
		int m_renderingLayer;

	public:
		ARKGameObject();

		virtual bool isPendingRemoval();
		virtual void setPendingRemoval(bool b);
		virtual void onPrune(); /* This even fires when removed from an ARK::Util::Container::Vector and when made inactive in a ARKPool */

		virtual int getRenderingLayer();
		virtual void setRenderingLayer(int i);

		virtual void update(GameContainer* container, GameTimer* timer) = 0;
		virtual void render(GameContainer* container, Graphics* g) = 0;

		virtual ~ARKGameObject();
};

#endif /* ARKGAMEOBJECT_H_ */
