/*
 * Recipe.cpp
 *
 *  Created on: 22 May 2011
 *      Author: Ashley
 */

#include "Recipe.h"
#include "DefaultGame.h"

Recipe::Recipe(): GameObject(), ingredients(), fallingPoints() {

}

void Recipe::update(GameContainer* container, GameTimer* timer) {
	fallingPoints.updateAll(container, timer);
}
void Recipe::render(GameContainer* container, Graphics* g) {
	DefaultGame* game = DefaultGame::getGame();


	fallingPoints.renderAll(container, g);

	g->setDrawColor(Color::black_50a);
	g->fillRoundedRect(10, 50, 100, 380, 10);
	g->setDrawColor(Color::white);

	ARK2D::getGraphics()->setFont(game->font_small);

	int y = 60;
	map<unsigned int, unsigned int>::iterator it = ingredients.begin();
	while (it != ingredients.end()) {
		if (it->second == 0) {
			++it; continue;
		}
		string s = StringUtil::prepend(" x ", it->second);
		s = StringUtil::append(s, game->state_game->ingredientNames[it->first]);
		g->drawString(s, 20, y);
		y += g->getFont()->getStringHeight(s);
		y += 5;
		Image* im = game->state_game->ingredientImages[it->first];
		im->draw(60 - int(im->getWidth()/2), y); // between 20x and 100 -- 60.
		y += im->getHeight();
		y += 5;
		++it;
	}




	ARK2D::getGraphics()->setFont(DefaultGame::getGame()->font);
}

Recipe::~Recipe() {

}
