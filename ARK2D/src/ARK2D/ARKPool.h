/*
 * ARKPool.h
 *
 *  Created on: Dec 15, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARKPOOL_H_
#define ARKPOOL_H_

#include "ARKVector.h"
#include "ARKGameObject.h"
#include "Util/MathUtil.h"

using namespace std;

template <class T=ARKGameObject*>
class ARKPool {
	private:
		ARKVector<T> m_inactive;
		ARKVector<T> m_active;

	public:
		ARKPool(): m_inactive(), m_active() {
			m_inactive.setUsingList(true);
			m_active.setUsingList(true);
		}
		void add(T object) {
			add(object, false);
		}
		void add(T object, bool active) {
			if (active) {
				m_active.add(object);
			} else {
				m_inactive.add(object);
			}
		}
		ARKVector<T>* getActive() {
			return &m_active;
		}
		ARKVector<T>* getInactive() {
			return &m_inactive;
		}
		void setUsingList(bool b) {
			m_active.setUsingList(b);
			m_inactive.setUsingList(b);
		}
		bool isUsingList() {
			return m_active.isUsingList();
		}
		T get() {
			try {
				T obj = m_inactive.pop();
				m_active.add(obj);
				return obj;
			} catch(exception& e) {
				throw e;
			}
			return NULL;
		}
		void prune(T obj) {
			obj->setPendingRemoval(false);
			obj->onPrune();
			m_active.remove(obj);
			m_inactive.add(obj);
		}
		void prune(ARKVectorIterator<T>* it, T obj) {
			obj->setPendingRemoval(false);
			obj->onPrune();
			m_inactive.add(obj);
			it->remove();
		}
		void pruneAll() {
			ARKVectorIterator<T>* it = m_active.iterator();
			while (it->hasNext()) {
				T obj = it->next();
				if (obj == NULL) { ErrorDialog::createAndShow("This is bad, very bad."); }
				if (obj->isPendingRemoval()) {

					prune(it, obj);
					/*obj->setPendingRemoval(false);
					obj->onPrune();
					m_inactive.add(obj);
					it->remove();*/
				}
			}
			/*for (unsigned int i = 0; i < m_active.size(); i++) {
				T obj = m_active.get(i);
				if (obj->isPendingRemoval()) {
					prune(obj);
					i = 0;
				}
			}*/
		}
		unsigned int size() {
			return m_active.size() + m_inactive.size();
		}
		void updateAll(GameContainer* container, GameTimer* timer) {
			m_active.updateAll(container, timer);
		}
		void renderAll(GameContainer* container, Graphics* graphics) {
			m_active.renderAll(container, graphics);
		}
		virtual ~ARKPool() {

		}
};


#endif /* ARKPOOL_H_ */
