/*
 * Scoreboard.cpp
 *
 *  Created on: 17 Oct 2017
 *      Author: Ashley
 */

#include "Scoreboard.h"

namespace ARK {
	namespace Util {
		namespace LocalMultiplayer {

			ScoreboardScoreChange::ScoreboardScoreChange(int maxTeams):
				m_teams(maxTeams) 
				{
				scores = (signed int*) malloc(sizeof(signed int) * m_teams);
				setAll(0);
			}
			void ScoreboardScoreChange::setAll(signed int val) {
				for(unsigned int i = 0; i < m_teams; ++i) {
					scores[i] = val;
				}	
			}
			ScoreboardScoreChange::~ScoreboardScoreChange() {
				free(scores);
				scores = NULL;
			}

			Scoreboard::Scoreboard(int maxTeams, int maxScore):
				m_scores(NULL),
				m_scoresAtStartOfRound(NULL),
				m_teams(maxTeams),
				m_maxScore(maxScore)
				{
				m_scores = (signed int*) malloc(sizeof(signed int) * m_teams);
				m_scoresAtStartOfRound = (signed int*) malloc(sizeof(signed int) * m_teams);
				reset();
			}
			void Scoreboard::reset() {
				for(unsigned int i = 0; i < m_teams; ++i) {
					m_scores[i] = 0;
					m_scoresAtStartOfRound[i] = 0;
				}
			}
			void Scoreboard::setMaxScore(int maxScore) {
				m_maxScore = maxScore;
			}

			void Scoreboard::nextRound() {
				for(unsigned int i = 0; i < m_teams; ++i) {
					m_scoresAtStartOfRound[i] = m_scores[i];
				}
			}
			bool Scoreboard::isWon() {
				for(unsigned int i = 0; i < m_teams; ++i) {
					if (m_scores[i] >= m_maxScore) { 
						return true;
					}
				} 
				return false;
			}

			void Scoreboard::addScoreByChanges(ScoreboardScoreChange* sc) {
				for(unsigned int i = 0; i < m_teams; ++i) {
					m_scores[i] += sc->scores[i]; 
					if (m_scores[i] < 0) { 
						m_scores[i] = 0; 
					}
				}
			}

			void Scoreboard::addScoreForTeam(unsigned int team, signed int score) {
				m_scores[team] += score;
			}
			signed int Scoreboard::getScoreForTeam(unsigned int team) {
				return m_scores[team];
			}
			signed int Scoreboard::getScoreForTeamAtStartOfRound(unsigned int team) {
				return m_scoresAtStartOfRound[team];
			}
			signed int Scoreboard::getTopScoreAtStartOfRound() {
				signed int top = 0;
				for(unsigned int i = 0; i < m_teams; ++i) {
					if (m_scores[i] > top) {
						top = m_scores[i];
					}
				}
				return top;
			}
			bool Scoreboard::hasScoreChanges() {
				for(unsigned int i = 0; i < m_teams; ++i) {
					if (m_scores[i] != m_scoresAtStartOfRound[i]) {
						return true;
					}
				}
				return false;
			}
			Scoreboard::~Scoreboard() {
				free(m_scores);
				m_scores = NULL;
				
				free(m_scoresAtStartOfRound);
				m_scoresAtStartOfRound = NULL;
			}
		}
	}
}
