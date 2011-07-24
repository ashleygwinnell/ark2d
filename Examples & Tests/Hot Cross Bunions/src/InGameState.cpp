/*
 * InGameState.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "ARK.h"
#include "InGameState.h"

#include "DefaultGame.h"

InGameState::InGameState():
	GameState(), ingredients(), recipes(),
	ingredientImages(), ingredientNames(), witchLines(), witchLineCurrent(0),
	player(NULL), bg(NULL),
	fadingText(), paused(false),
	witchWaitTimer(20.0f), witchDurationTimer(0.0f) {

}

unsigned int InGameState::id() {
	return 1;
}

void InGameState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}
void InGameState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void InGameState::init(GameContainer* container, StateBasedGame* game) {
	DefaultGame* g = DefaultGame::getGame();
	Image* sheet = g->sheet;
	SpriteSheetDescription* desc = g->desc;

	player = new Player();

	bg = sheet->getSubImage(desc->getItemByName("bg.png"))->getScaledCopy(8, 8);

	// igredientimages
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("flour.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("egg.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("bacon.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("beans.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("broomstick.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("cat.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("mushroom.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("pumpkin.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("rat.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("sausage.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("toast.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("water.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("fish.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("shoe.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("cheese.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("lice.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("cross.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("mud.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("potato.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("purple-syrup.png"))->getScaledCopy(4, 4));
	ingredientImages.push_back(sheet->getSubImage(desc->getItemByName("blood.png"))->getScaledCopy(4, 4));

	// ingredientnames
	ingredientNames.push_back("Flour");
	ingredientNames.push_back("Egg");
	ingredientNames.push_back("Bacon");
	ingredientNames.push_back("Beans");
	ingredientNames.push_back("Broomstick");
	ingredientNames.push_back("Cat");
	ingredientNames.push_back("Mushroom");
	ingredientNames.push_back("Pumpkin");
	ingredientNames.push_back("Rat");
	ingredientNames.push_back("Sausage");
	ingredientNames.push_back("Toast");
	ingredientNames.push_back("Water");
	ingredientNames.push_back("Fish");
	ingredientNames.push_back("Shoe");
	ingredientNames.push_back("Cheese");
	ingredientNames.push_back("Lice");
	ingredientNames.push_back("Cross");
	ingredientNames.push_back("Mud");
	ingredientNames.push_back("Potato");
	ingredientNames.push_back("Purple Syrup");
	ingredientNames.push_back("Blood");

	witchLines.push_back("WHERE'S MY DINNER?");
	witchLines.push_back("Y U NO MAKE FOOD?");
	witchLines.push_back("TOO MANY CAULDRONS SPOIL THE BROTH?");
	witchLines.push_back("YOU CAN'T COOK FOR SHIT!");
	witchLines.push_back("FIRE UP THE CAULDRONS!");
	witchLines.push_back("MAKE ME A SANDWICH!");
	witchLines.push_back("WHERE'S MY SANDWICH!");
	witchLines.push_back("NOM NOM NOM NOM NOM!");
	witchLines.push_back("UBER LEEK PACKS!");
	witchLines.push_back("FIZZY FEEL GOOD!");
	witchLines.push_back("KEEP IT UP! (THAT'S WHAT SHE SAID!)");
	witchLines.push_back("HELLO SEXY LADY... CURRY?");
	witchLines.push_back("YOU USELESS POT HEAD!");
	witchLines.push_back("TOO MANY FOOD!");
	witchLines.push_back("YOUR BITTER TEARS ARE DELICIOUS!");
	witchLines.push_back("THE CAKE IS NOT A LIE.");
	witchLines.push_back("NO SPAM!");
	witchLines.push_back("FASTER!");
	witchLines.push_back("PERISHABLE GOODS SHALL PERISH!");
	witchLines.push_back("AND ON THE 7TH DAY, YOU MADE ME FOOD. AND IT WAS GOOD.");
	witchLines.push_back("OH. THIS ONE IS LABELLED \"BEST BEFORE READING\"...");
	witchLines.push_back("BRING FROTH, THE FOODPOCALYPSE");
	witchLines.push_back("\"BEAUTY AND THE FEAST\". THIS CAN'T BE RIGHT...");
	witchLines.push_back("END OF LINE.");
	witchLines.push_back("LARGE BREAKFAST IS LARGE");
	witchLines.push_back("IF THE BILL IS LARGER THAN MY WARTS, I AIN'T TIPPIN'!");

	prepareRecipes();
}

void InGameState::prepareRecipes() {

	recipes.removeAll();

	// level 1
	Recipe* level1 = new Recipe();
		level1->m_name = "Toma-toe Soup";
		level1->ingredients[Ingredient::TYPE_WATER] = 2;
			FallingPoint* level1_point1 = new FallingPoint();
			level1_point1->setIndex(0);
			level1_point1->setTime(2.0f);
			level1_point1->add(Ingredient::TYPE_WATER, 100);
			level1->fallingPoints.add(level1_point1);
	recipes.add(level1);

	Recipe* level2 = new Recipe();
		level2->m_name = "Cheese Ghostie";
		level2->ingredients[Ingredient::TYPE_CHEESE] = 2;
		level2->ingredients[Ingredient::TYPE_TOAST] = 2;
			FallingPoint* level2_point1 = new FallingPoint();
				level2_point1->setIndex(0);
				level2_point1->setTime(2.0f);
				level2_point1->add(Ingredient::TYPE_CHEESE, 50);
				level2_point1->add(Ingredient::TYPE_RAT, 50);
				level2->fallingPoints.add(level2_point1);
			FallingPoint* level2_point2 = new FallingPoint();
				level2_point2->setIndex(1);
				level2_point2->setTime(2.0f);
				level2_point2->setTimer(1.0f);
				level2_point2->add(Ingredient::TYPE_TOAST, 33);
				level2_point2->add(Ingredient::TYPE_MUD, 33);
				level2_point2->add(Ingredient::TYPE_LICE, 33);
				level2->fallingPoints.add(level2_point2);
	recipes.add(level2);

	Recipe* level3 = new Recipe();
		level3->m_name = "Draccola";
		level3->ingredients[Ingredient::TYPE_WATER] = 2;
		level3->ingredients[Ingredient::TYPE_MUD] = 2;
			FallingPoint* level3_point1 = new FallingPoint();
				level3_point1->setIndex(0);
				level3_point1->setTime(2.0f);
				level3_point1->setTimer(1.0f);
				level3_point1->add(Ingredient::TYPE_WATER, 30);
				level3_point1->add(Ingredient::TYPE_CROSS, 30);
				level3_point1->add(Ingredient::TYPE_BEANS, 30);
				level3->fallingPoints.add(level3_point1);
			FallingPoint* level3_point2 = new FallingPoint();
				level3_point2->setIndex(1);
				level3_point2->setTime(2.0f);
				level3_point2->add(Ingredient::TYPE_MUD, 30);
				level3_point2->add(Ingredient::TYPE_EGG, 30);
				level3_point2->add(Ingredient::TYPE_MUSHROOM, 30);
				level3->fallingPoints.add(level3_point2);
	recipes.add(level3);

	Recipe* level4 = new Recipe();
		level4->m_name = "Brushroom Stro-gone-off";
		level4->ingredients[Ingredient::TYPE_BROOMSTICK] = 2;
		level4->ingredients[Ingredient::TYPE_MUSHROOM] = 3;
			FallingPoint* level4_point1 = new FallingPoint();
				level4_point1->setIndex(0);
				level4_point1->setTime(1.8f);
				level4_point1->add(Ingredient::TYPE_BROOMSTICK, 10);
				level4_point1->add(Ingredient::TYPE_WATER, 30);
				level4_point1->add(Ingredient::TYPE_EGG, 30);
				level4->fallingPoints.add(level4_point1);
			FallingPoint* level4_point2 = new FallingPoint();
				level4_point2->setIndex(1);
				level4_point2->setTime(1.8f);
				level4_point2->setTimer(0.6f);
				level4_point2->add(Ingredient::TYPE_MUSHROOM, 33);
				level4_point2->add(Ingredient::TYPE_WATER, 33);
				level4_point2->add(Ingredient::TYPE_BACON, 33);
				level4->fallingPoints.add(level4_point2);
			FallingPoint* level4_point3 = new FallingPoint();
				level4_point3->setIndex(2);
				level4_point3->setTime(1.8f);
				level4_point3->setTimer(1.2f);
				level4_point3->add(Ingredient::TYPE_BROOMSTICK, 10);
				level4_point3->add(Ingredient::TYPE_FISH, 30);
				level4_point3->add(Ingredient::TYPE_TOAST, 30);
				level4->fallingPoints.add(level4_point3);
	recipes.add(level4);

	// TODO: start from here.
	Recipe* level5 = new Recipe();
		level5->m_name = "Dreadsticks";
		level5->ingredients[Ingredient::TYPE_FLOUR] = 8;
		level5->ingredients[Ingredient::TYPE_BROOMSTICK] = 1;
			FallingPoint* level5_point1 = new FallingPoint();
				level5_point1->setIndex(0);
				level5_point1->setTime(1.8f);
				level5_point1->add(Ingredient::TYPE_BROOMSTICK, 10);
				level5_point1->add(Ingredient::TYPE_BEANS, 45);
				level5_point1->add(Ingredient::TYPE_SAUSAGE, 45);
				level5->fallingPoints.add(level5_point1);
			FallingPoint* level5_point2 = new FallingPoint();
				level5_point2->setIndex(1);
				level5_point2->setTime(0.9f);
				level5_point2->add(Ingredient::TYPE_FLOUR, 10);
				level5->fallingPoints.add(level5_point2);
			FallingPoint* level5_point3 = new FallingPoint();
				level5_point3->setIndex(2);
				level5_point3->setTime(1.8f);
				level5_point3->setTimer(0.9f);
				level5_point3->add(Ingredient::TYPE_BROOMSTICK, 10);
				level5_point3->add(Ingredient::TYPE_MUD, 45);
				level5_point3->add(Ingredient::TYPE_FLOUR, 45);
				level5->fallingPoints.add(level5_point3);
	recipes.add(level5);

	Recipe* level6 = new Recipe();
		level6->m_name = "Bacon Sandwitches";
			FallingPoint* level6_point1 = new FallingPoint();
				level6_point1->setIndex(0);
				level6_point1->setTime(1.8f);
				level6_point1->setTimer(1.2f);
				level6_point1->add(Ingredient::TYPE_BACON, 20);
				level6_point1->add(Ingredient::TYPE_BEANS, 20);
				level6_point1->add(Ingredient::TYPE_EGG, 20);
				level6_point1->add(Ingredient::TYPE_SAUSAGE, 20);
				level6->fallingPoints.add(level6_point1);
			FallingPoint* level6_point2 = new FallingPoint();
				level6_point2->setIndex(1);
				level6_point2->setTime(1.8f);
				level6_point2->setTimer(0.6f);
				level6_point2->add(Ingredient::TYPE_TOAST, 20);
				level6_point2->add(Ingredient::TYPE_BEANS, 20);
				level6_point2->add(Ingredient::TYPE_EGG, 20);
				level6_point2->add(Ingredient::TYPE_SAUSAGE, 20);
				level6->fallingPoints.add(level6_point2);
			FallingPoint* level6_point3 = new FallingPoint();
				level6_point3->setIndex(2);
				level6_point3->setTime(1.8f);
				level6_point3->add(Ingredient::TYPE_BACON, 5);
				level6_point3->add(Ingredient::TYPE_TOAST, 5);
				level6_point3->add(Ingredient::TYPE_RAT, 10);
				level6_point3->add(Ingredient::TYPE_BEANS, 20);
				level6_point3->add(Ingredient::TYPE_EGG, 20);
				level6_point3->add(Ingredient::TYPE_SAUSAGE, 20);
				level6_point3->add(Ingredient::TYPE_SHOE, 20);
				level6->fallingPoints.add(level6_point3);
		level6->ingredients[Ingredient::TYPE_BACON] = 4;
		level6->ingredients[Ingredient::TYPE_TOAST] = 2;
	recipes.add(level6);

	Recipe* level7 = new Recipe();
		level7->m_name = "Lice Scream";
		level7->ingredients[Ingredient::TYPE_WATER] = 5;
		level7->ingredients[Ingredient::TYPE_LICE] = 20;
			FallingPoint* level7_point1 = new FallingPoint();
				level7_point1->setIndex(0);
				level7_point1->setTime(1.7f);
				level7_point1->add(Ingredient::TYPE_WATER, 10);
				level7_point1->add(Ingredient::TYPE_LICE, 50);
				level7_point1->add(Ingredient::TYPE_CHEESE, 10);
				level7->fallingPoints.add(level7_point1);
			FallingPoint* level7_point2 = new FallingPoint();
				level7_point2->setIndex(1);
				level7_point2->setTime(1.7f);
				level7_point2->setTimer(0.85f);
				level7_point2->add(Ingredient::TYPE_WATER, 10);
				level7_point2->add(Ingredient::TYPE_LICE, 50);
				level7_point2->add(Ingredient::TYPE_RAT, 10);
				level7->fallingPoints.add(level7_point2);
			FallingPoint* level7_point3 = new FallingPoint();
				level7_point3->setIndex(2);
				level7_point3->setTime(1.7f);
				level7_point3->setTimer(0.85f);
				level7_point3->add(Ingredient::TYPE_WATER, 10);
				level7_point3->add(Ingredient::TYPE_LICE, 50);
				level7_point3->add(Ingredient::TYPE_CAT, 10);
				level7->fallingPoints.add(level7_point3);
			FallingPoint* level7_point4 = new FallingPoint();
				level7_point4->setIndex(3);
				level7_point4->setTime(1.7f);
				level7_point4->add(Ingredient::TYPE_WATER, 10);
				level7_point4->add(Ingredient::TYPE_LICE, 50);
				level7_point4->add(Ingredient::TYPE_FISH, 10);
				level7->fallingPoints.add(level7_point4);
	recipes.add(level7);

	Recipe* level8 = new Recipe();
		level8->m_name = "Frankenfurters";
		level8->ingredients[Ingredient::TYPE_SAUSAGE] = 10;
		level8->ingredients[Ingredient::TYPE_BACON] = 5;
			FallingPoint* level8_point1 = new FallingPoint();
				level8_point1->setIndex(0);
				level8_point1->setTime(1.6f);
				level8_point1->setTimer(0.8f);
				level8_point1->add(Ingredient::TYPE_RAT, 100);
				level8->fallingPoints.add(level8_point1);
			FallingPoint* level8_point2 = new FallingPoint();
				level8_point2->setIndex(1);
				level8_point2->setTime(1.6f);
				level8_point2->add(Ingredient::TYPE_BACON, 50);
				level8_point2->add(Ingredient::TYPE_SAUSAGE, 100);
				level8->fallingPoints.add(level8_point2);
			FallingPoint* level8_point3 = new FallingPoint();
				level8_point3->setIndex(2);
				level8_point3->setTime(1.6f);
				level8_point3->add(Ingredient::TYPE_BACON, 50);
				level8_point3->add(Ingredient::TYPE_SAUSAGE, 100);
				level8->fallingPoints.add(level8_point3);
			FallingPoint* level8_point4 = new FallingPoint();
				level8_point4->setIndex(3);
				level8_point4->setTime(1.6f);
				level8_point4->setTimer(0.8f);
				level8_point4->add(Ingredient::TYPE_RAT, 100);
				level8->fallingPoints.add(level8_point4);
	recipes.add(level8);

	Recipe* level9 = new Recipe();
		level9->m_name = "Hairy pota-toes";
		level9->ingredients[Ingredient::TYPE_FLOUR] = 2;
		level9->ingredients[Ingredient::TYPE_CAT] = 1;
		level9->ingredients[Ingredient::TYPE_POTATO] = 4;
			FallingPoint* level9_point1 = new FallingPoint();
				level9_point1->setIndex(0);
				level9_point1->setTime(2.0f);
				level9_point1->add(Ingredient::TYPE_POTATO, 10);
				level9_point1->add(Ingredient::TYPE_EGG, 40);
				level9_point1->add(Ingredient::TYPE_CAT, 5);
				level9->fallingPoints.add(level9_point1);
			FallingPoint* level9_point2 = new FallingPoint();
				level9_point2->setIndex(1);
				level9_point2->setTime(2.0f);
				level9_point2->setTimer(0.375f);
				level9_point2->add(Ingredient::TYPE_POTATO, 10);
				level9_point2->add(Ingredient::TYPE_FLOUR, 20);
				level9_point2->add(Ingredient::TYPE_BEANS, 80);
				level9->fallingPoints.add(level9_point2);
			FallingPoint* level9_point3 = new FallingPoint();
				level9_point3->setIndex(2);
				level9_point3->setTime(2.0f);
				level9_point3->setTimer(0.75f);
				level9_point3->add(Ingredient::TYPE_POTATO, 10);
				level9_point3->add(Ingredient::TYPE_FLOUR, 20);
				level9_point3->add(Ingredient::TYPE_CHEESE, 80);
				level9->fallingPoints.add(level9_point3);
			FallingPoint* level9_point4 = new FallingPoint();
				level9_point4->setIndex(3);
				level9_point4->setTime(2.0f);
				level9_point4->setTimer(1.125f);
				level9_point4->add(Ingredient::TYPE_POTATO, 10);
				level9_point4->add(Ingredient::TYPE_BACON, 40);
				level9_point4->add(Ingredient::TYPE_CAT, 5);
				level9->fallingPoints.add(level9_point4);
	recipes.add(level9);

	Recipe* level10 = new Recipe();
		level10->m_name = "Coughee";
		level10->ingredients[Ingredient::TYPE_MUD] = 6;
		level10->ingredients[Ingredient::TYPE_WATER] = 3;
		level10->ingredients[Ingredient::TYPE_PURPLE_SYRUP] = 1;
			FallingPoint* level10_point1 = new FallingPoint();
				level10_point1->setIndex(0);
				level10_point1->setTime(1.4f);
				level10_point1->add(Ingredient::TYPE_MUD, 20);
				level10_point1->add(Ingredient::TYPE_MUSHROOM, 40);
				level10_point1->add(Ingredient::TYPE_BROOMSTICK, 40);
				level10->fallingPoints.add(level10_point1);
			FallingPoint* level10_point2 = new FallingPoint();
				level10_point2->setIndex(1);
				level10_point2->setTime(1.4f);
				level10_point2->setTimer(0.46f);
				level10_point2->add(Ingredient::TYPE_WATER, 10);
				level10_point2->add(Ingredient::TYPE_FISH, 45);
				level10_point2->add(Ingredient::TYPE_FLOUR, 45);
				level10->fallingPoints.add(level10_point2);
			FallingPoint* level10_point3 = new FallingPoint();
				level10_point3->setIndex(2);
				level10_point3->setTime(2.8f);
				level10_point3->setTimer(0.0f);
				level10_point3->add(Ingredient::TYPE_PURPLE_SYRUP, 20);
				level10_point3->add(Ingredient::TYPE_LICE, 80);
				level10->fallingPoints.add(level10_point3);
			FallingPoint* level10_point4 = new FallingPoint();
				level10_point4->setIndex(3);
				level10_point4->setTime(1.4f);
				level10_point4->setTimer(0.46f);
				level10_point4->add(Ingredient::TYPE_WATER, 10);
				level10_point4->add(Ingredient::TYPE_FISH, 45);
				level10_point4->add(Ingredient::TYPE_FLOUR, 45);
				level10->fallingPoints.add(level10_point4);
			FallingPoint* level10_point5 = new FallingPoint();
				level10_point5->setIndex(4);
				level10_point5->setTime(1.4f);
				level10_point5->add(Ingredient::TYPE_MUD, 20);
				level10_point5->add(Ingredient::TYPE_MUSHROOM, 40);
				level10_point5->add(Ingredient::TYPE_BROOMSTICK, 40);
				level10->fallingPoints.add(level10_point5);
	recipes.add(level10);

	Recipe* level11 = new Recipe();
		level11->m_name = "Boil-ed Eggs";
		level11->ingredients[Ingredient::TYPE_EGG] = 10;
		level11->ingredients[Ingredient::TYPE_WATER] = 5;
		level11->ingredients[Ingredient::TYPE_LICE] = 5;
			FallingPoint* level11_point1 = new FallingPoint();
				level11_point1->setIndex(0);
				level11_point1->setTime(1.3f);
				level11_point1->setTimer(1.3f);
				level11_point1->add(Ingredient::TYPE_EGG, 30);
				level11_point1->add(Ingredient::TYPE_TOAST, 10);
				level11_point1->add(Ingredient::TYPE_BACON, 10);
				level11_point1->add(Ingredient::TYPE_SAUSAGE, 10);
				level11->fallingPoints.add(level11_point1);
			FallingPoint* level11_point2 = new FallingPoint();
				level11_point2->setIndex(1);
				level11_point2->setTime(1.3f);
				level11_point2->setTimer(0.65f);
				level11_point2->add(Ingredient::TYPE_FLOUR, 10);
				level11_point2->add(Ingredient::TYPE_RAT, 10);
				level11_point2->add(Ingredient::TYPE_CAT, 10);
				level11_point2->add(Ingredient::TYPE_MUSHROOM, 10);
				level11->fallingPoints.add(level11_point2);
			FallingPoint* level11_point3 = new FallingPoint();
				level11_point3->setIndex(2);
				level11_point3->setTime(1.3f);
				level11_point3->setTimer(1.3f);
				level11_point3->add(Ingredient::TYPE_BLOOD, 10);
				level11_point3->add(Ingredient::TYPE_MUD, 10);
				level11_point3->add(Ingredient::TYPE_PUMPKIN, 10);
				level11_point3->add(Ingredient::TYPE_FISH, 10);
				level11->fallingPoints.add(level11_point3);
			FallingPoint* level11_point4 = new FallingPoint();
				level11_point4->setIndex(3);
				level11_point4->setTime(1.3f);
				level11_point4->setTimer(0.65f);
				level11_point4->add(Ingredient::TYPE_WATER, 10);
				level11_point4->add(Ingredient::TYPE_SHOE, 10);
				level11_point4->add(Ingredient::TYPE_CHEESE, 10);
				level11_point4->add(Ingredient::TYPE_PURPLE_SYRUP, 10);
				level11->fallingPoints.add(level11_point4);
			FallingPoint* level11_point5 = new FallingPoint();
				level11_point5->setIndex(4);
				level11_point5->setTime(1.3f);
				level11_point5->setTimer(1.3f);
				level11_point5->add(Ingredient::TYPE_LICE, 10);
				level11_point5->add(Ingredient::TYPE_BEANS, 10);
				level11_point5->add(Ingredient::TYPE_CROSS, 10);
				level11_point5->add(Ingredient::TYPE_POTATO, 10);
				level11->fallingPoints.add(level11_point5);
	recipes.add(level11);

	Recipe* level12 = new Recipe();
		level12->m_name = "Shardines";
		level12->ingredients[Ingredient::TYPE_WATER] = 10;
		level12->ingredients[Ingredient::TYPE_FISH] = 10;
		level12->ingredients[Ingredient::TYPE_MUD] = 5;
			FallingPoint* level12_point1 = new FallingPoint();
				level12_point1->setIndex(0);
				level12_point1->setTime(1.3f);
				level12_point1->add(Ingredient::TYPE_WATER, 20);
				level12_point1->add(Ingredient::TYPE_SHOE, 20);
				level12_point1->add(Ingredient::TYPE_CROSS, 20);
				level12_point1->add(Ingredient::TYPE_BACON, 10);
				level12_point1->add(Ingredient::TYPE_BEANS, 5);
				level12->fallingPoints.add(level12_point1);
			FallingPoint* level12_point2 = new FallingPoint();
				level12_point2->setIndex(1);
				level12_point2->setTime(1.95f);
				level12_point2->add(Ingredient::TYPE_EGG, 20);
				level12_point2->add(Ingredient::TYPE_SHOE, 20);
				level12_point2->add(Ingredient::TYPE_CROSS, 20);
				level12_point2->add(Ingredient::TYPE_BACON, 10);
				level12_point2->add(Ingredient::TYPE_BEANS, 5);
				level12->fallingPoints.add(level12_point2);
			FallingPoint* level12_point3 = new FallingPoint();
				level12_point3->setIndex(2);
				level12_point3->setTime(1.3f);
				level12_point3->setTimer(1.3f);
				level12_point3->add(Ingredient::TYPE_FISH, 30);
				level12_point3->add(Ingredient::TYPE_SHOE, 20);
				level12_point3->add(Ingredient::TYPE_CROSS, 20);
				level12_point3->add(Ingredient::TYPE_BACON, 10);
				level12_point3->add(Ingredient::TYPE_BEANS, 5);
				level12->fallingPoints.add(level12_point3);
			FallingPoint* level12_point4 = new FallingPoint();
				level12_point4->setIndex(3);
				level12_point4->setTime(1.95f);
				level12_point4->add(Ingredient::TYPE_EGG, 20);
				level12_point4->add(Ingredient::TYPE_SHOE, 20);
				level12_point4->add(Ingredient::TYPE_CROSS, 20);
				level12_point4->add(Ingredient::TYPE_BACON, 10);
				level12_point4->add(Ingredient::TYPE_BEANS, 5);
				level12->fallingPoints.add(level12_point4);
			FallingPoint* level12_point5 = new FallingPoint();
				level12_point5->setIndex(4);
				level12_point5->setTime(1.3f);
				level12_point5->add(Ingredient::TYPE_MUD, 20);
				level12_point5->add(Ingredient::TYPE_SHOE, 20);
				level12_point5->add(Ingredient::TYPE_CROSS, 20);
				level12_point5->add(Ingredient::TYPE_BACON, 10);
				level12_point5->add(Ingredient::TYPE_BEANS, 5);
				level12->fallingPoints.add(level12_point5);
	recipes.add(level12);

	Recipe* level13 = new Recipe();
		level13->m_name = "Smelly Shoeshi";
		level13->ingredients[Ingredient::TYPE_FISH] = 2;
		level13->ingredients[Ingredient::TYPE_SHOE] = 10;
		level13->ingredients[Ingredient::TYPE_WATER] = 1;
			FallingPoint* level13_point1 = new FallingPoint();
				level13_point1->setIndex(0);
				level13_point1->setTime(1.8f);
				level13_point1->add(Ingredient::TYPE_FISH, 5);
				level13_point1->add(Ingredient::TYPE_SHOE, 5);
				level13_point1->add(Ingredient::TYPE_POTATO, 10);
				level13->fallingPoints.add(level13_point1);
			FallingPoint* level13_point2 = new FallingPoint();
				level13_point2->setIndex(1);
				level13_point2->setTime(1.8f);
				level13_point2->setTimer(1.35f);
				level13_point2->add(Ingredient::TYPE_WATER, 5);
				level13_point2->add(Ingredient::TYPE_SHOE, 5);
				level13_point2->add(Ingredient::TYPE_POTATO, 10);
				level13->fallingPoints.add(level13_point2);
			FallingPoint* level13_point3 = new FallingPoint();
				level13_point3->setIndex(2);
				level13_point3->setTime(1.8f);
				level13_point3->setTimer(0.9f);
				level13_point3->add(Ingredient::TYPE_FISH, 5);
				level13_point3->add(Ingredient::TYPE_SHOE, 5);
				level13_point3->add(Ingredient::TYPE_POTATO, 10);
				level13->fallingPoints.add(level13_point3);
			FallingPoint* level13_point4 = new FallingPoint();
				level13_point4->setIndex(3);
				level13_point4->setTime(1.8f);
				level13_point4->setTimer(1.35f);
				level13_point4->add(Ingredient::TYPE_WATER, 5);
				level13_point4->add(Ingredient::TYPE_SHOE, 5);
				level13_point4->add(Ingredient::TYPE_POTATO, 10);
				level13->fallingPoints.add(level13_point4);
			FallingPoint* level13_point5 = new FallingPoint();
				level13_point5->setIndex(4);
				level13_point5->setTime(1.8f);
				level13_point5->add(Ingredient::TYPE_FISH, 5);
				level13_point5->add(Ingredient::TYPE_SHOE, 5);
				level13_point5->add(Ingredient::TYPE_POTATO, 10);
				level13->fallingPoints.add(level13_point5);
	recipes.add(level13);

	Recipe* level14 = new Recipe();
		level14->m_name = "Mango Gutney";
		level14->ingredients[Ingredient::TYPE_WATER] = 5;
		level14->ingredients[Ingredient::TYPE_PUMPKIN] = 1;
		level14->ingredients[Ingredient::TYPE_POTATO] = 3;
		level14->ingredients[Ingredient::TYPE_BLOOD] = 5;
			FallingPoint* level14_point1 = new FallingPoint();
				level14_point1->setIndex(0);
				level14_point1->setTime(1.0f);
				level14_point1->add(Ingredient::TYPE_BLOOD, 5);
				level14_point1->add(Ingredient::TYPE_WATER, 10);
				level14_point1->add(Ingredient::TYPE_SHOE, 30);
				level14->fallingPoints.add(level14_point1);
			FallingPoint* level14_point2 = new FallingPoint();
				level14_point2->setIndex(1);
				level14_point2->setTime(1.0f);
				level14_point2->add(Ingredient::TYPE_BLOOD, 5);
				level14_point2->add(Ingredient::TYPE_POTATO, 5);
				level14_point2->add(Ingredient::TYPE_WATER, 10);
				level14_point2->add(Ingredient::TYPE_CROSS, 30);
				level14->fallingPoints.add(level14_point2);
			FallingPoint* level14_point3 = new FallingPoint();
				level14_point3->setIndex(2);
				level14_point3->setTime(2.0f);
				level14_point3->add(Ingredient::TYPE_POTATO, 10);
				level14_point3->add(Ingredient::TYPE_PUMPKIN, 40);
				level14->fallingPoints.add(level14_point3);
			FallingPoint* level14_point4 = new FallingPoint();
				level14_point4->setIndex(3);
				level14_point4->setTime(1.0f);
				level14_point4->add(Ingredient::TYPE_BLOOD, 5);
				level14_point4->add(Ingredient::TYPE_POTATO, 5);
				level14_point4->add(Ingredient::TYPE_WATER, 10);
				level14_point4->add(Ingredient::TYPE_CAT, 30);
				level14->fallingPoints.add(level14_point4);
			FallingPoint* level14_point5 = new FallingPoint();
				level14_point5->setIndex(4);
				level14_point5->setTime(1.0f);
				level14_point5->add(Ingredient::TYPE_BLOOD, 5);
				level14_point5->add(Ingredient::TYPE_MUD, 10);
				level14_point5->add(Ingredient::TYPE_FISH, 30);
				level14->fallingPoints.add(level14_point5);
	recipes.add(level14);

	Recipe* level15 = new Recipe();
		level15->m_name = "Hot-cross Bunions";
		level15->ingredients[Ingredient::TYPE_TOAST] = 5;
		level15->ingredients[Ingredient::TYPE_FLOUR] = 5;
		level15->ingredients[Ingredient::TYPE_CROSS] = 10;
			FallingPoint* level15_point1 = new FallingPoint();
				level15_point1->setIndex(0);
				level15_point1->setTime(2.0f);
				level15_point1->add(Ingredient::TYPE_CROSS, 10);
				level15_point1->add(Ingredient::TYPE_FLOUR, 5);
				level15_point1->add(Ingredient::TYPE_WATER, 10);
				level15_point1->add(Ingredient::TYPE_RAT, 20);
				level15->fallingPoints.add(level15_point1);
			FallingPoint* level15_point2 = new FallingPoint();
				level15_point2->setIndex(1);
				level15_point2->setTime(2.0f);
				level15_point2->setTimer(1.0f);
				level15_point2->add(Ingredient::TYPE_TOAST, 10);
				level15_point2->add(Ingredient::TYPE_FLOUR, 10);
				level15_point2->add(Ingredient::TYPE_WATER, 10);
				level15_point2->add(Ingredient::TYPE_RAT, 20);
				level15->fallingPoints.add(level15_point2);
			FallingPoint* level15_point3 = new FallingPoint();
				level15_point3->setIndex(2);
				level15_point3->setTime(1.0f);
				level15_point3->setTimer(1.0f);
				level15_point3->add(Ingredient::TYPE_CROSS, 10);
				level15_point3->add(Ingredient::TYPE_BLOOD, 10);
				level15_point3->add(Ingredient::TYPE_PURPLE_SYRUP, 10);
				level15_point3->add(Ingredient::TYPE_CAT, 20);
				level15->fallingPoints.add(level15_point3);
			FallingPoint* level15_point4 = new FallingPoint();
				level15_point4->setIndex(3);
				level15_point4->setTime(1.0f);
				level15_point4->setTimer(1.0f);
				level15_point4->add(Ingredient::TYPE_CROSS, 10);
				level15_point4->add(Ingredient::TYPE_BLOOD, 10);
				level15_point4->add(Ingredient::TYPE_PURPLE_SYRUP, 10);
				level15_point4->add(Ingredient::TYPE_CAT, 20);
				level15->fallingPoints.add(level15_point4);
			FallingPoint* level15_point5 = new FallingPoint();
				level15_point5->setIndex(4);
				level15_point5->setTime(2.0f);
				level15_point5->setTimer(1.0f);
				level15_point5->add(Ingredient::TYPE_TOAST, 10);
				level15_point5->add(Ingredient::TYPE_FLOUR, 10);
				level15_point5->add(Ingredient::TYPE_WATER, 10);
				level15_point5->add(Ingredient::TYPE_RAT, 20);
				level15->fallingPoints.add(level15_point5);
			FallingPoint* level15_point6 = new FallingPoint();
				level15_point6->setIndex(5);
				level15_point6->setTime(2.0f);
				level15_point6->add(Ingredient::TYPE_CROSS, 10);
				level15_point6->add(Ingredient::TYPE_FLOUR, 5);
				level15_point6->add(Ingredient::TYPE_WATER, 10);
				level15_point6->add(Ingredient::TYPE_RAT, 20);
				level15->fallingPoints.add(level15_point6);
	recipes.add(level15);

	Recipe* level16 = new Recipe();
		level16->m_name = "Cattenburg";
		level16->ingredients[Ingredient::TYPE_FLOUR] = 2;
		level16->ingredients[Ingredient::TYPE_CAT] = 6;
		level16->ingredients[Ingredient::TYPE_EGG] = 2;
			FallingPoint* level16_point1 = new FallingPoint();
				level16_point1->setIndex(0);
				level16_point1->setTime(1.8f);
				level16_point1->add(Ingredient::TYPE_CAT, 5);
				level16_point1->add(Ingredient::TYPE_EGG, 5);
				level16_point1->add(Ingredient::TYPE_BLOOD, 20);
				level16_point1->add(Ingredient::TYPE_FISH, 10);
				level16->fallingPoints.add(level16_point1);
			FallingPoint* level16_point2 = new FallingPoint();
				level16_point2->setIndex(1);
				level16_point2->setTime(1.8f);
				level16_point2->setTimer(0.9f);
				level16_point2->add(Ingredient::TYPE_CAT, 5);
				level16_point2->add(Ingredient::TYPE_FLOUR, 10);
				level16_point2->add(Ingredient::TYPE_BLOOD, 10);
				level16_point2->add(Ingredient::TYPE_FISH, 10);
				level16->fallingPoints.add(level16_point2);
			FallingPoint* level16_point3 = new FallingPoint();
				level16_point3->setIndex(2);
				level16_point3->setTime(1.8f);
				level16_point3->add(Ingredient::TYPE_RAT, 5);
				level16_point3->add(Ingredient::TYPE_TOAST, 10);
				level16_point3->add(Ingredient::TYPE_BEANS, 10);
				level16_point3->add(Ingredient::TYPE_BACON, 10);
				level16->fallingPoints.add(level16_point3);
			FallingPoint* level16_point4 = new FallingPoint();
				level16_point4->setIndex(3);
				level16_point4->setTime(1.8f);
				level16_point4->add(Ingredient::TYPE_RAT, 5);
				level16_point4->add(Ingredient::TYPE_TOAST, 10);
				level16_point4->add(Ingredient::TYPE_BEANS, 10);
				level16_point4->add(Ingredient::TYPE_SAUSAGE, 10);
				level16->fallingPoints.add(level16_point4);
			FallingPoint* level16_point5 = new FallingPoint();
				level16_point5->setIndex(4);
				level16_point5->setTime(1.8f);
				level16_point5->setTimer(0.9f);
				level16_point5->add(Ingredient::TYPE_CAT, 5);
				level16_point5->add(Ingredient::TYPE_FLOUR, 10);
				level16_point5->add(Ingredient::TYPE_BLOOD, 10);
				level16_point5->add(Ingredient::TYPE_FISH, 10);
				level16->fallingPoints.add(level16_point5);
			FallingPoint* level16_point6 = new FallingPoint();
				level16_point6->setIndex(5);
				level16_point6->setTime(1.8f);
				level16_point6->add(Ingredient::TYPE_CAT, 5);
				level16_point6->add(Ingredient::TYPE_EGG, 5);
				level16_point6->add(Ingredient::TYPE_BLOOD, 20);
				level16_point6->add(Ingredient::TYPE_FISH, 10);
				level16->fallingPoints.add(level16_point6);
	recipes.add(level16);

	Recipe* level17 = new Recipe();
		level17->m_name = "Wartcake";
		level17->ingredients[Ingredient::TYPE_FLOUR] = 3;
		level17->ingredients[Ingredient::TYPE_PUMPKIN] = 5;
		level17->ingredients[Ingredient::TYPE_WATER] = 5;
		level17->ingredients[Ingredient::TYPE_SHOE] = 3;
			FallingPoint* level17_point1 = new FallingPoint();
				level17_point1->setIndex(0);
				level17_point1->setTime(1.6f);
				level17_point1->add(Ingredient::TYPE_FLOUR, 20);
				level17_point1->add(Ingredient::TYPE_BEANS, 20);
				level17_point1->add(Ingredient::TYPE_BLOOD, 10);
				level17_point1->add(Ingredient::TYPE_CAT, 10);
				level17->fallingPoints.add(level17_point1);
			FallingPoint* level17_point2 = new FallingPoint();
				level17_point2->setIndex(1);
				level17_point2->setTime(1.6f);
				level17_point2->setTimer(0.8f);
				level17_point2->add(Ingredient::TYPE_PUMPKIN, 20);
				level17_point2->add(Ingredient::TYPE_SAUSAGE, 10);
				level17_point2->add(Ingredient::TYPE_TOAST, 10);
				level17_point2->add(Ingredient::TYPE_FISH, 10);
				level17->fallingPoints.add(level17_point2);
			FallingPoint* level17_point3 = new FallingPoint();
				level17_point3->setIndex(2);
				level17_point3->setTime(1.6f);
				level17_point3->add(Ingredient::TYPE_SHOE, 20);
				level17_point3->add(Ingredient::TYPE_WATER, 20);
				level17_point3->add(Ingredient::TYPE_MUD, 10);
				level17_point3->add(Ingredient::TYPE_BLOOD, 10);
				level17_point3->add(Ingredient::TYPE_PURPLE_SYRUP, 10);
				level17_point3->add(Ingredient::TYPE_EGG, 30);
				level17->fallingPoints.add(level17_point3);
			FallingPoint* level17_point4 = new FallingPoint();
				level17_point4->setIndex(3);
				level17_point4->setTime(1.6f);
				level17_point4->add(Ingredient::TYPE_SHOE, 20);
				level17_point4->add(Ingredient::TYPE_WATER, 20);
				level17_point4->add(Ingredient::TYPE_MUD, 10);
				level17_point4->add(Ingredient::TYPE_BLOOD, 10);
				level17_point4->add(Ingredient::TYPE_PURPLE_SYRUP, 10);
				level17_point4->add(Ingredient::TYPE_BACON, 30);
				level17->fallingPoints.add(level17_point4);
			FallingPoint* level17_point5 = new FallingPoint();
				level17_point5->setIndex(4);
				level17_point5->setTime(1.6f);
				level17_point5->setTimer(0.8f);
				level17_point5->add(Ingredient::TYPE_FLOUR, 20);
				level17_point5->add(Ingredient::TYPE_MUSHROOM, 10);
				level17_point5->add(Ingredient::TYPE_TOAST, 10);
				level17_point5->add(Ingredient::TYPE_LICE, 10);
				level17->fallingPoints.add(level17_point5);
			FallingPoint* level17_point6 = new FallingPoint();
				level17_point6->setIndex(5);
				level17_point6->setTime(1.6f);
				level17_point6->add(Ingredient::TYPE_PUMPKIN, 20);
				level17_point6->add(Ingredient::TYPE_BEANS, 20);
				level17_point6->add(Ingredient::TYPE_MUD, 10);
				level17_point6->add(Ingredient::TYPE_RAT, 10);
				level17->fallingPoints.add(level17_point6);
	recipes.add(level17);

	Recipe* level18 = new Recipe();
		level18->m_name = "Tombscones";
		level18->ingredients[Ingredient::TYPE_FLOUR] = 4;
		level18->ingredients[Ingredient::TYPE_PUMPKIN] = 4;
		level18->ingredients[Ingredient::TYPE_WATER] = 4;
		level18->ingredients[Ingredient::TYPE_SHOE] = 4;
			FallingPoint* level18_point1 = new FallingPoint();
				level18_point1->setIndex(0);
				level18_point1->setTime(1.5f);
				level18_point1->setTimer(0.75f);
				level18_point1->add(Ingredient::TYPE_FLOUR, 10);
				level18_point1->add(Ingredient::TYPE_TOAST, 20);
				level18_point1->add(Ingredient::TYPE_PUMPKIN, 10);
				level18_point1->add(Ingredient::TYPE_CHEESE, 10);
				level18->fallingPoints.add(level18_point1);
			FallingPoint* level18_point2 = new FallingPoint();
				level18_point2->setIndex(1);
				level18_point2->setTime(1.5f);
				level18_point2->add(Ingredient::TYPE_BACON, 20);
				level18_point2->add(Ingredient::TYPE_SHOE, 20);
				level18_point2->add(Ingredient::TYPE_BROOMSTICK, 10);
				level18_point2->add(Ingredient::TYPE_CHEESE, 10);
				level18->fallingPoints.add(level18_point2);
			FallingPoint* level18_point3 = new FallingPoint();
				level18_point3->setIndex(2);
				level18_point3->setTime(1.5f);
				level18_point3->add(Ingredient::TYPE_RAT, 10);
				level18_point3->add(Ingredient::TYPE_BLOOD, 20);
				level18_point3->add(Ingredient::TYPE_BROOMSTICK, 10);
				level18_point3->add(Ingredient::TYPE_CHEESE, 10);
				level18->fallingPoints.add(level18_point3);
			FallingPoint* level18_point4 = new FallingPoint();
				level18_point4->setIndex(3);
				level18_point4->setTime(1.5f);
				level18_point4->setTimer(0.75f);
				level18_point4->add(Ingredient::TYPE_WATER, 10);
				level18_point4->add(Ingredient::TYPE_MUD, 20);
				level18_point4->add(Ingredient::TYPE_BROOMSTICK, 10);
				level18_point4->add(Ingredient::TYPE_CHEESE, 10);
				level18->fallingPoints.add(level18_point4);
			FallingPoint* level18_point5 = new FallingPoint();
				level18_point5->setIndex(4);
				level18_point5->setTime(1.5f);
				level18_point5->setTimer(0.75f);
				level18_point5->add(Ingredient::TYPE_LICE, 20);
				level18_point5->add(Ingredient::TYPE_CROSS, 20);
				level18_point5->add(Ingredient::TYPE_BROOMSTICK, 10);
				level18_point5->add(Ingredient::TYPE_CHEESE, 10);
				level18_point5->add(Ingredient::TYPE_PUMPKIN, 10);
				level18->fallingPoints.add(level18_point5);
			FallingPoint* level18_point6 = new FallingPoint();
				level18_point6->setIndex(5);
				level18_point6->setTime(1.5f);
				level18_point6->add(Ingredient::TYPE_FISH, 20);
				level18_point6->add(Ingredient::TYPE_CROSS, 20);
				level18_point6->add(Ingredient::TYPE_BROOMSTICK, 10);
				level18_point6->add(Ingredient::TYPE_CHEESE, 10);
				level18_point6->add(Ingredient::TYPE_PUMPKIN, 10);
				level18->fallingPoints.add(level18_point6);
			FallingPoint* level18_point7 = new FallingPoint();
				level18_point7->setIndex(6);
				level18_point7->setTime(1.5f);
				level18_point7->add(Ingredient::TYPE_EGG, 10);
				level18_point7->add(Ingredient::TYPE_CROSS, 20);
				level18_point7->add(Ingredient::TYPE_BROOMSTICK, 10);
				level18_point7->add(Ingredient::TYPE_CHEESE, 10);
				level18->fallingPoints.add(level18_point7);
			FallingPoint* level18_point8 = new FallingPoint();
				level18_point8->setIndex(7);
				level18_point8->setTime(1.5f);
				level18_point8->setTimer(0.75f);
				level18_point8->add(Ingredient::TYPE_SAUSAGE, 20);
				level18_point8->add(Ingredient::TYPE_TOAST, 20);
				level18_point8->add(Ingredient::TYPE_PUMPKIN, 10);
				level18_point8->add(Ingredient::TYPE_CHEESE, 10);
				level18->fallingPoints.add(level18_point8);
	recipes.add(level18);

	Recipe* level19 = new Recipe();
		level19->m_name = "Atchoo-ewww Nut \"Chicken\"";
		level19->ingredients[Ingredient::TYPE_RAT] = 3;
		level19->ingredients[Ingredient::TYPE_MUSHROOM] = 2;
		level19->ingredients[Ingredient::TYPE_SHOE] = 1;
		level19->ingredients[Ingredient::TYPE_BROOMSTICK] = 2;
			FallingPoint* level19_point1 = new FallingPoint();
				level19_point1->setIndex(0);
				level19_point1->setTime(1.8f);
				level19_point1->add(Ingredient::TYPE_RAT, 20);
				level19_point1->add(Ingredient::TYPE_FLOUR, 10);
				level19_point1->add(Ingredient::TYPE_BACON, 10);
				level19_point1->add(Ingredient::TYPE_MUD, 60);
				level19->fallingPoints.add(level19_point1);
			FallingPoint* level19_point2 = new FallingPoint();
				level19_point2->setIndex(1);
				level19_point2->setTime(1.8f);
				level19_point2->setTimer(0.9f);
				level19_point2->add(Ingredient::TYPE_TOAST, 10);
				level19_point2->add(Ingredient::TYPE_LICE, 10);
				level19_point2->add(Ingredient::TYPE_BEANS, 10);
				level19_point2->add(Ingredient::TYPE_CHEESE, 60);
				level19->fallingPoints.add(level19_point2);
			FallingPoint* level19_point3 = new FallingPoint();
				level19_point3->setIndex(2);
				level19_point3->setTime(1.8f);
				level19_point3->setTimer(0.9f);
				level19_point3->add(Ingredient::TYPE_CAT, 10);
				level19_point3->add(Ingredient::TYPE_CHEESE, 10);
				level19_point3->add(Ingredient::TYPE_PUMPKIN, 10);
				level19_point3->add(Ingredient::TYPE_PURPLE_SYRUP, 60);
				level19->fallingPoints.add(level19_point3);
			FallingPoint* level19_point4 = new FallingPoint();
				level19_point4->setIndex(3);
				level19_point4->setTime(1.8f);
				level19_point4->add(Ingredient::TYPE_MUSHROOM, 20);
				level19_point4->add(Ingredient::TYPE_CROSS, 10);
				level19_point4->add(Ingredient::TYPE_TOAST, 10);
				level19_point4->add(Ingredient::TYPE_PUMPKIN, 60);
				level19->fallingPoints.add(level19_point4);
			FallingPoint* level19_point5 = new FallingPoint();
				level19_point5->setIndex(4);
				level19_point5->setTime(1.8f);
				level19_point5->add(Ingredient::TYPE_EGG, 10);
				level19_point5->add(Ingredient::TYPE_PURPLE_SYRUP, 10);
				level19_point5->add(Ingredient::TYPE_SAUSAGE, 10);
				level19_point5->add(Ingredient::TYPE_CAT, 60);
				level19->fallingPoints.add(level19_point5);
			FallingPoint* level19_point6 = new FallingPoint();
				level19_point6->setIndex(5);
				level19_point6->setTime(1.8f);
				level19_point6->setTimer(0.9f);
				level19_point6->add(Ingredient::TYPE_SHOE, 20);
				level19_point6->add(Ingredient::TYPE_MUD, 10);
				level19_point6->add(Ingredient::TYPE_FISH, 10);
				level19_point6->add(Ingredient::TYPE_BACON, 60);
				level19->fallingPoints.add(level19_point6);
			FallingPoint* level19_point7 = new FallingPoint();
				level19_point7->setIndex(6);
				level19_point7->setTime(1.8f);
				level19_point7->setTimer(0.9f);
				level19_point7->add(Ingredient::TYPE_FISH, 10);
				level19_point7->add(Ingredient::TYPE_BLOOD, 10);
				level19_point7->add(Ingredient::TYPE_WATER, 10);
				level19_point7->add(Ingredient::TYPE_SAUSAGE, 60);
				level19->fallingPoints.add(level19_point7);
			FallingPoint* level19_point8 = new FallingPoint();
				level19_point8->setIndex(7);
				level19_point8->setTime(1.8f);
				level19_point8->add(Ingredient::TYPE_BROOMSTICK, 20);
				level19_point8->add(Ingredient::TYPE_WATER, 10);
				level19_point8->add(Ingredient::TYPE_CAT, 10);
				level19_point8->add(Ingredient::TYPE_FLOUR, 60);
				level19->fallingPoints.add(level19_point8);
	recipes.add(level19);

	Recipe* level20 = new Recipe();
		level20->m_name = "Grief Burger with Fleas";
		level20->ingredients[Ingredient::TYPE_TOAST] = 4;
		level20->ingredients[Ingredient::TYPE_MUSHROOM] = 4;
		level20->ingredients[Ingredient::TYPE_RAT] = 1;
		level20->ingredients[Ingredient::TYPE_EGG] = 2;
		level20->ingredients[Ingredient::TYPE_LICE] = 10;
			FallingPoint* level20_point1 = new FallingPoint();
				level20_point1->setIndex(0);
				level20_point1->setTime(1.5f);
				level20_point1->setTimer(0.75f);
				level20_point1->add(Ingredient::TYPE_EGG, 10);
				level20_point1->add(Ingredient::TYPE_FLOUR, 20);
				level20_point1->add(Ingredient::TYPE_POTATO, 20);
				level20->fallingPoints.add(level20_point1);
			FallingPoint* level20_point2 = new FallingPoint();
				level20_point2->setIndex(1);
				level20_point2->setTime(1.5f);
				level20_point2->setTimer(0.75f);
				level20_point2->add(Ingredient::TYPE_LICE, 20);
				level20_point2->add(Ingredient::TYPE_FISH, 20);
				level20_point2->add(Ingredient::TYPE_BROOMSTICK, 20);
				level20_point2->add(Ingredient::TYPE_WATER, 20);
				level20->fallingPoints.add(level20_point2);
			FallingPoint* level20_point3 = new FallingPoint();
				level20_point3->setIndex(2);
				level20_point3->setTime(1.5f);
				level20_point3->add(Ingredient::TYPE_MUSHROOM, 10);
				level20_point3->add(Ingredient::TYPE_CAT, 20);
				level20_point3->add(Ingredient::TYPE_PURPLE_SYRUP, 20);
				level20->fallingPoints.add(level20_point3);
			FallingPoint* level20_point4 = new FallingPoint();
				level20_point4->setIndex(3);
				level20_point4->setTime(1.5f);
				level20_point4->add(Ingredient::TYPE_BACON, 20);
				level20_point4->add(Ingredient::TYPE_MUD, 20);
				level20_point4->add(Ingredient::TYPE_WATER, 20);
				level20->fallingPoints.add(level20_point4);
			FallingPoint* level20_point5 = new FallingPoint();
				level20_point5->setIndex(4);
				level20_point5->setTime(1.5f);
				level20_point5->setTimer(0.75f);
				level20_point5->add(Ingredient::TYPE_TOAST, 10);
				level20_point5->add(Ingredient::TYPE_CROSS, 20);
				level20_point5->add(Ingredient::TYPE_SHOE, 20);
				level20->fallingPoints.add(level20_point5);
			FallingPoint* level20_point6 = new FallingPoint();
				level20_point6->setIndex(5);
				level20_point6->setTime(1.5f);
				level20_point6->setTimer(0.75f);
				level20_point6->add(Ingredient::TYPE_RAT, 10);
				level20_point6->add(Ingredient::TYPE_SHOE, 20);
				level20_point6->add(Ingredient::TYPE_PUMPKIN, 20);
				level20->fallingPoints.add(level20_point6);
			FallingPoint* level20_point7 = new FallingPoint();
				level20_point7->setIndex(6);
				level20_point7->setTime(1.5f);
				level20_point7->add(Ingredient::TYPE_WATER, 20);
				level20_point7->add(Ingredient::TYPE_PUMPKIN, 20);
				level20_point7->add(Ingredient::TYPE_CROSS, 10);
				level20->fallingPoints.add(level20_point7);
			FallingPoint* level20_point8 = new FallingPoint();
				level20_point8->setIndex(7);
				level20_point8->setTime(1.5f);
				level20_point8->add(Ingredient::TYPE_SAUSAGE, 20);
				level20_point8->add(Ingredient::TYPE_PURPLE_SYRUP, 20);
				level20_point8->add(Ingredient::TYPE_CROSS, 10);
				level20->fallingPoints.add(level20_point8);
			FallingPoint* level20_point9 = new FallingPoint();
				level20_point9->setIndex(8);
				level20_point9->setTime(1.5f);
				level20_point9->setTimer(0.75f);
				level20_point9->add(Ingredient::TYPE_LICE, 20);
				level20_point9->add(Ingredient::TYPE_POTATO, 20);
				level20_point9->add(Ingredient::TYPE_WATER, 20);
				level20_point9->add(Ingredient::TYPE_BROOMSTICK, 20);
				level20->fallingPoints.add(level20_point9);
			FallingPoint* level20_point10 = new FallingPoint();
				level20_point10->setIndex(9);
				level20_point10->setTime(1.5f);
				level20_point10->setTimer(0.75f);
				level20_point10->add(Ingredient::TYPE_BLOOD, 20);
				level20_point10->add(Ingredient::TYPE_BEANS, 20);
				level20_point10->add(Ingredient::TYPE_SAUSAGE, 20);
				level20_point10->add(Ingredient::TYPE_CROSS, 20);
				level20->fallingPoints.add(level20_point10);
	recipes.add(level20);

	// level 21
	Recipe* level21 = new Recipe();
		level21->m_name = "Grief Burger with Fleas";
		level21->ingredients[Ingredient::TYPE_WATER] = 2;
			FallingPoint* level21_point1 = new FallingPoint();
			level21_point1->setIndex(0);
			level21_point1->setTime(10.0f);
			level21_point1->add(Ingredient::TYPE_WATER, 100);
			level21->fallingPoints.add(level21_point1);
	recipes.add(level21);
}
void InGameState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

	Input* i = ARK2D::getInput();
	//if (i->isKeyPressed(Input::KEY_ENTER) || i->isKeyPressed(Input::KEY_P)
	if (i->isKeyPressed(DefaultGame::KEY_ENTER)
			|| i->isKeyPressed(DefaultGame::KEY_BACK)
			|| (i->getGamepads().size() > 0) && i->isGamepadButtonPressed(Gamepad::BUTTON_BACK)) {
		paused = !paused;
	}

	if (paused) {
		return;
	}

	DefaultGame* g = DefaultGame::getGame();

	witchWaitTimer -= timer->getDelta();
	if (witchWaitTimer <= 0) {
		witchDurationTimer += timer->getDelta();
		if (witchDurationTimer >= 5.0f) {
			witchDurationTimer = 0.0f;
			witchWaitTimer = 20.0f;

			// make sure we have a new line each time.
			unsigned int witchOldLine = witchLineCurrent;
			while (witchOldLine == witchLineCurrent) {
				witchLineCurrent = MathUtil::randBetween(0, witchLines.size());
			}

		}
	}

	player->update(container, timer);

	ingredients.updateAll(container, timer);
	recipes.get(g->levelNumber)->update(container, timer);

	fadingText.updateAll(container, timer);

	// next level conditions
	bool done = true;
	Recipe* r = g->getCurrentRecipe();
	map<unsigned int, unsigned int>::iterator it = r->ingredients.begin();
	while (it != r->ingredients.end()) {
		if (it->second > 0) {
			done = false;
			break;
		}
		++it;
	}

	// death conditions
	if (g->contamination >= 100) {
		ARK2D::getInput()->clearKeyPressedRecord();

		g->snd_enter->play();

		SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
			const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
		);
		g->state_win->m_win = false;
		g->enterState(g->state_win, t, NULL);


	}

	if (done == true) {
		ingredients.removeAll();
		g->levelNumber += 1;
		g->contamination = 0;

		std::cout << "done level " << g->levelNumber << std::endl;
		std::cout << "new levelnumber is " << (g->levelNumber+1) << std::endl;

		if (g->levelNumber == 20) {
			g->levelNumber -= 1;
			ARK2D::getInput()->clearKeyPressedRecord();

			g->snd_enter->play();

			SlideRectanglesAcrossTransition* t = new SlideRectanglesAcrossTransition(
				const_cast<Color*>(&Color::darker_grey), 50, Constants::DIRECTION_LEFT, 1.0f
			);
			g->state_win->m_win = true;
			g->enterState(g->state_win, t, NULL);
		}
	}

}
void InGameState::render(GameContainer* container, StateBasedGame* game, Graphics* g) {
	//img_cauldron->drawCentered(int(container->getWidth()/2), int(container->getHeight()/2));

	DefaultGame* ga = DefaultGame::getGame();

	ARK2D::getGraphics()->setFont(ga->font);

	bg->draw(0, 0);
	player->render(container, g);
	ingredients.renderAll(container, g);

	if (witchDurationTimer) {
		ga->witch->draw(20, ORIGINAL_HEIGHT - 80 - ga->witch->getHeight());

		//ga->font_small->drawString(witchLines.at(witchLineCurrent), 100, 320);
		g->setFont(ga->font_small);
		g->drawStringWordWrap(witchLines.at(witchLineCurrent), 100, 320, 440, 10);

		g->setFont(ga->font);
	}

	string rec = "Recipe ";
	rec = StringUtil::append(rec, ga->levelNumber+1);
	rec = StringUtil::append(rec, ": ");
	rec = StringUtil::append(rec, ga->getCurrentRecipe()->m_name);
	g->drawString(rec, 10, 12);

	string s(StringUtil::append("Score: ", ga->score));
	g->drawString(s, 10, ORIGINAL_HEIGHT - 12 - g->getFont()->getStringHeight(s));

	string s2("Contamination: ");
	s2 = StringUtil::append(s2, ga->contamination);
	s2 = StringUtil::append(s2, "%");
	g->drawString(s2, ORIGINAL_WIDTH - 10 - g->getFont()->getStringWidth(s2), ORIGINAL_HEIGHT - 12 - g->getFont()->getStringHeight(s2));

	recipes.get(ga->levelNumber)->render(container, g);
	fadingText.renderAll(container, g);

	if (paused) {
		g->setDrawColor(Color::black_50a);
		g->fillRect(0, 0, ORIGINAL_WIDTH, ORIGINAL_HEIGHT);

		g->setDrawColor(Color::white);
		g->drawStringCenteredAt("PAUSED", int(ORIGINAL_WIDTH/2), int(ORIGINAL_HEIGHT/2));
	}
}

InGameState::~InGameState() {

}
