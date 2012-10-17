/*
 * SquaresInTransition.cpp
 *
 *  Created on: 13 Oct 2012
 *      Author: Ashley
 */

#include "SquaresInTransition.h"
#include "../../Includes.h"

namespace ARK {
	namespace State {
		namespace Transition {

			SquaresInTransition::SquaresInTransition(float duration, float squareSize):
				Transition(),
				m_timer(0.0f),
				m_duration(duration),
				m_squareSize(squareSize),
				m_squares(),
				m_squaresFilled(),
				m_squaresEmptied(0),
				m_squaresTotal(0),
				m_color(NULL)
				{
 				m_color = const_cast<Color*>(&Color::black);
			}
			SquaresInTransition::SquaresInTransition(float duration, float squareSize, Color* c):
				Transition(),
				m_timer(0.0f),
				m_duration(duration), 
				m_squareSize(squareSize),
				m_squares(),
				m_squaresFilled(),
				m_squaresEmptied(0),
				m_squaresTotal(0),
				m_color(c)
				{
		
			}
			void SquaresInTransition::init(GameContainer* container, StateBasedGame* game, GameState* from, GameState* to) {
				Transition::init(container, game, from, to);

				m_timer = 0.0f;

				int squaresX = float(container->getDynamicWidth()) / m_squareSize;
				int squaresY = float(container->getDynamicHeight()) / m_squareSize;

				for(int i = 0; i < squaresX; i++) {
					for(int j = 0; j < squaresY; j++) {
						m_squares.push_back(true);
					}
				}

				for(unsigned int i = 0; i < m_squares.size(); i++) {
					m_squaresFilled.push_back(i);
				}
				std::random_shuffle(m_squaresFilled.begin(), m_squaresFilled.end());

				m_squaresEmptied = 0;
				m_squaresTotal = squaresX * squaresY;
				
			}
			void SquaresInTransition::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) 
			{
				m_timer += timer->getDelta();
				if (m_timer > m_duration) {
					m_timer = m_duration;
				}

				while (m_timer / m_duration > float(m_squaresEmptied) / float(m_squaresTotal)) 
				{
					int index = m_squaresFilled.back();
					m_squaresFilled.pop_back();
					m_squares[index] = false;
					m_squaresEmptied++;
				}
 
			}
			void SquaresInTransition::preRender(GameContainer* container, StateBasedGame* game, Renderer* g) {

			}
			void SquaresInTransition::postRender(GameContainer* container, StateBasedGame* game, Renderer* g) 
			{
				int squaresX = float(container->getDynamicWidth()) / m_squareSize;
				int squaresY = float(container->getDynamicHeight()) / m_squareSize;

				g->setDrawColorf(m_color->getRedf(), m_color->getGreenf(), m_color->getBluef(), m_color->getAlphaf());

				for(signed int x = 0; x < squaresX; x++) 
				{
					for(signed int y = 0; y < squaresY; y++) 
					{
						unsigned int gid = (y * squaresX) + x;
						if (m_squares.at(gid)) {
							g->fillRect(x*m_squareSize, y*m_squareSize, m_squareSize, m_squareSize);
						}
					}
					
				}

				
			}
			bool SquaresInTransition::isComplete() {
				if (m_timer >= m_duration) {
					return true;
				}
				return false;
			}

			SquaresInTransition::~SquaresInTransition() {

			}
		}
	}
}
