/*
 * Ingredient.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef CORAL_H_
#define CORAL_H_

#include "ARK.h"
#include "GameObject.h"

class DefaultGame;

class Ingredient : public GameObject {
	public:

		static const unsigned int TYPE_FLOUR = 0;
		static const unsigned int TYPE_EGG = 1;
		static const unsigned int TYPE_BACON = 2;
		static const unsigned int TYPE_BEANS = 3;
		static const unsigned int TYPE_BROOMSTICK = 4;
		static const unsigned int TYPE_CAT = 5;
		static const unsigned int TYPE_MUSHROOM = 6;
		static const unsigned int TYPE_PUMPKIN = 7;
		static const unsigned int TYPE_RAT = 8;
		static const unsigned int TYPE_SAUSAGE = 9;
		static const unsigned int TYPE_TOAST = 10;
		static const unsigned int TYPE_WATER = 11;
		static const unsigned int TYPE_FISH = 12;
		static const unsigned int TYPE_SHOE = 13;
		static const unsigned int TYPE_CHEESE = 14;
		static const unsigned int TYPE_LICE = 15;
		static const unsigned int TYPE_CROSS = 16;
		static const unsigned int TYPE_MUD = 17;
		static const unsigned int TYPE_POTATO = 18;
		static const unsigned int TYPE_PURPLE_SYRUP = 19;
		static const unsigned int TYPE_BLOOD = 20;


		//static const unsigned int TYPE_ = 0;

		Ingredient();
		void setType(unsigned int i);
		unsigned int getType();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Graphics* g);
		virtual ~Ingredient();

	private:
		DefaultGame* m_game;
		Image* m_image;
		int m_type;

	public:

};

#endif /* CORAL_H_ */
