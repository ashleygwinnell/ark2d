/*
 * Recipe.h
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#ifndef RECIPE_H_
#define RECIPE_H_

#include "ARK.h"
#include "GameObject.h"
#include "FallingPoint.h"
#include <map>
#include <string>
using namespace std;

class DefaultGame;

class Recipe : public GameObject {
	public:
		string m_name;
		map<unsigned int, unsigned int> ingredients; // Ingredient type id to amount
		ARKVector<FallingPoint*> fallingPoints; // points where things fall

		Recipe();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		virtual ~Recipe();
};

#endif /* RECIPE_H_ */
