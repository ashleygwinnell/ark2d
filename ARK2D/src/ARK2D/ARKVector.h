/*
 * ARKVector.h
 *
 *  Created on: 21 May 2011
 *      Author: Ashley
 */

#ifndef ARKVECTOR_H_
#define ARKVECTOR_H_

#include <vector>
#include <iterator>
#include "ARKGameObject.h"

using namespace std;

template <class T=ARKGameObject*>
class ARKVector {
	private:
		vector<T> vec;

	public:
		ARKVector(): vec() {

		}
		void add(T o) {
			vec.push_back(o);
		}
		T get(unsigned int i) {
			return vec.at(i);
		}
		T random(unsigned int i) {
			return vec.at(MathUtil::randBetween(0, vec.size()));
		}
		vector<T> getData() {
			return vec;
		}
		void setData(vector<T> data) {
			vec = data;
		}
		unsigned int size() {
			return vec.size();
		}
		void remove(unsigned int i) {
			vec.erase(vec.begin() + i);
		}
		void remove(T o) {
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
		}
		void removeAll() {
			clear();
		}
		void clear() {
			vec.clear();
		}
		void updateAll(GameContainer* container, GameTimer* timer) {
			for(unsigned int i = 0; i < vec.size(); i++) {
				T obj = vec.at(i);
				if (obj == NULL) { continue; }
				obj->update(container, timer);
			}
		}
		void renderAll(GameContainer* container, Graphics* g) {
			for(unsigned int i = 0; i < vec.size(); i++) {
				T obj = vec.at(i);
				if (obj == NULL) { continue; }
				obj->render(container, g);
			}
		}
		~ARKVector() {

		}
};

#endif /* ARKVECTOR_H_ */
