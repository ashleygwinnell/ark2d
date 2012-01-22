/*
 * ARKVector.h
 *
 *  Created on: 21 May 2011
 *      Author: Ashley
 */

#ifndef ARKVECTOR_H_
#define ARKVECTOR_H_

#include <vector>
#include <list>
#include <iterator>
#include "ARKGameObject.h"
#include "ARKException.h"
#include "Util/MathUtil.h"

using namespace std;

template <class T=ARKGameObject*>
class ARKVectorIterator;

template <class T=ARKGameObject*>
class ARKVector {
	friend class ARKVectorIterator<T>;
	private:
		vector<T> vec;
		list<T> lst;
		bool usingList;

		ARKVectorIterator<T>* it;

	public:
		ARKVector(): vec(), lst(), usingList(false), it(NULL) {

		}
		void setUsingList(bool b) {
			usingList = b;
		}
		bool isUsingList() {
			return usingList;
		}
		void add(T o) {
			if (!usingList) {
				vec.push_back(o);
				return;
			}
			lst.push_back(o);
		}
		T get(unsigned int i) {
			if (!usingList) {
				return vec.at(i);
			}
			typename list<T>::iterator it = lst.begin();
			advance(it, i);
			return *it;
		}
		T pop() {
			if (!usingList) {
				if (vec.size() == 0) {
					throw exception();
				}

				int end = vec.size()-1;
				T endObj = vec.at(end);
				vec.pop_back();
				return endObj;
			}

			if (lst.size() == 0) {
				throw exception();
			}
			int end = lst.size()-1;
			//T endObj = lst.begin() + end;
			typename list<T>::iterator it = lst.begin();
			advance(it, end);
			T endObj = (*it);

			lst.pop_back();
			return endObj;
		}
		T random(unsigned int i) {
			if (!usingList) {
				return vec.at(MathUtil::randBetween(0, vec.size()));
			}
			return lst.begin() + MathUtil::randBetween(0, lst.size());
		}
		void* getData() {
			if (!usingList) {
				return (void*) vec;
			}
			return (void*) lst;
		}
		void setData(void* data) {
			if (!usingList) {
				vec = data;
				return;
			}
			lst = data;
		}
		ARKVectorIterator<T>* iterator() {
			if (it == NULL) {
				it = new ARKVectorIterator<T>(this);
			}
			it->reset();
			return it;
		}

		unsigned int size() {
			if (!usingList) {
				return vec.size();
			}
			return lst.size();
		}
		void remove(unsigned int i) {
			if (!usingList) {
				vec.erase(vec.begin() + i);
				return;
			}
			typename list<T>::iterator it = lst.begin();
			advance(it, i);
			lst.erase(it);
		}
		void remove(T o) {
			if (!usingList) {
				typename vector<T>::iterator it = vec.begin();
				while (it != vec.end()) {
					T obj = (*it);
					if (obj == o) {
						it = vec.erase(it);
						break;
					} else {
						++it;
					}
				}
				return;
			}

			typename list<T>::iterator it = lst.begin();
			while (it != lst.end()) {
				T obj = (*it);
				if (obj == o) {
					it = lst.erase(it);
					break;
				} else {
					++it;
				}
			}
		}
		void removeAll() {
			clear();
		}
		void pruneAll() { // only use with ARKGameObject
			if (!usingList) {
				typename vector<T>::iterator it = vec.begin();
				while (it != vec.end()) {
					T obj = (*it);
					if (obj->isPendingRemoval()) {
						obj->onPrune();
						it = vec.erase(it);
						//break;
					} else {
						++it;
					}
				}
				return;
			}
			typename list<T>::iterator it = lst.begin();
			while (it != lst.end()) {
				T obj = (*it);
				if (obj->isPendingRemoval()) {
					obj->onPrune();
					it = lst.erase(it);
					//break;
				} else {
					++it;
				}
			}
		}

		void clear() {
			if (!usingList) {
				vec.clear();
				return;
			}
			lst.clear();
		}

		/**
		 * only use with ARKGameObject
		 * careful not to prune in any update functions!!
		 */
		void updateAll(GameContainer* container, GameTimer* timer) {
			if (!usingList) {
				for(unsigned int i = 0; i < vec.size(); i++) {
					T obj = vec.at(i);
					//if (obj == NULL) { continue; }
					obj->update(container, timer);
				}
				return;
			}

			typename list<T>::iterator it = lst.begin();
			while (it != lst.end()) {
				T obj = (*it);
				//if (obj == NULL) { continue; }
				obj->update(container, timer);
				++it;
			}
			/*for(unsigned int i = 0; i < lst.size(); i++) {
				typename list<T>::iterator it = lst.begin();
				advance(it, i);
				T obj = *it;
				if (obj == NULL) { continue; }
				obj->update(container, timer);
			}*/
		}
		/**
		 * only use with ARKGameObject
		 * careful not to prune in any update functions!!
		 */
		void renderAll(GameContainer* container, Graphics* g) {
			if (!usingList) {
				for(unsigned int i = 0; i < vec.size(); i++) {
					T obj = vec.at(i);
					//if (obj == NULL) { continue; }
					obj->render(container, g);
				}
				return;
			}
			typename list<T>::iterator it = lst.begin();
			while (it != lst.end()) {
				T obj = (*it);
				//if (obj == NULL) { continue; }
				obj->render(container, g);
				++it;
			}
			/*for(unsigned int i = 0; i < lst.size(); i++) {
				typename list<T>::iterator it = lst.begin();
				advance(it, i);
				T obj = *it;
				if (obj == NULL) { continue; }
				obj->render(container, g);
			}*/
		}
		string toString() {
			string s = "[";
				if (!usingList) {
					for(unsigned int i = 0; i < vec.size(); i++) {
						T obj = vec.at(i);
						if (obj == NULL) { continue; }
						s += obj->toString();
						if (i != vec.size() - 1) {
							s += ",";
						}
					}
				} else {
					for(unsigned int i = 0; i < lst.size(); i++) {
						typename list<T>::iterator it = lst.begin();
						advance(it, i);
						T obj = *it;
						if (obj == NULL) { continue; }
						s += obj->toString();
						if (i != lst.size() - 1) {
							s += ",";
						}
					}
				}
			s += "]";
			return s;
		}
		~ARKVector() {

		}
};

template <class T>
class ARKVectorIterator {
	private:
		ARKVector<T>* parent;
		unsigned int index;
		typename list<T>::iterator iterator; // Used only for lists.
	public:
		ARKVectorIterator(ARKVector<T>* p): parent(p), index(0), iterator(NULL) {

		}
		T next() {
			if (parent->isUsingList()) {
				if (hasNext()) {
					T obj = (*iterator);
					++iterator;
					return obj;
				}
				ErrorDialog::createAndShow("Out of items in Iterator (lst) - returning NULL.");
				return NULL;
			}

			if (hasNext()) {
				T obj = parent->get(index);
				++index;
				return obj;
			}
			ErrorDialog::createAndShow("Out of items in Iterator (vec) - returning NULL.");
			return NULL;
		}
		ARKVector<T>* getVector() {
			return parent;
		}
		bool hasNext() {
			signed int top = parent->size();
			if (top == 0) { return false; }

			if (parent->isUsingList()) {
				if (iterator != parent->lst.end()) {
					return true;
				}
				return false;
			}

			signed int thisIndex = index;
			if (thisIndex >= top) {
				return false;
			}
			return true;
		}
		/**
		 * -1 because .next() forwards to the next element which is not normally the one we are trying to remove.
		 */
		void remove() {
			if (parent->isUsingList()) {
				iterator--;
				iterator = parent->lst.erase(iterator);
				return;
			}

			parent->remove(index-1);
		}
		void reset() {
			if (parent->isUsingList()) {
				iterator = parent->lst.begin();
				return;
			}

			index = 0;
		}
		~ARKVectorIterator() {

		}
};

#endif /* ARKVECTOR_H_ */
