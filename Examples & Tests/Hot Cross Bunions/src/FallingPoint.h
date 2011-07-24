/*
 * FallingPoint.h
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#ifndef FALLINGPOINT_H_
#define FALLINGPOINT_H_

#include <vector>
#include "ARK.h"
#include "GameObject.h"
#include "Ingredient.h"

class DefaultGame;

using namespace std;

class FallingPointChance {
	public:
		unsigned int type;
		unsigned int weight;
		FallingPointChance(): type(Ingredient::TYPE_FLOUR), weight(1) {

		}

		~FallingPointChance() {

		}
};

class FallingPoint : public ARKGameObject {
	vector<FallingPointChance*> items;
	unsigned int index;
	unsigned int totalWeight;
	float m_timer;

	float m_time;



	public:
		FallingPoint();
		void setIndex(unsigned int i);
		void setTime(float i);
		void setTimer(float f);
		void add(FallingPointChance* c);
		void add(unsigned int type, unsigned int weight);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		virtual ~FallingPoint();
};

#endif /* FALLINGPOINT_H_ */
