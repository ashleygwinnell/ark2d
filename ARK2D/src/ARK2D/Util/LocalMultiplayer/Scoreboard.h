/*
 * Scoreboard.h
 *
 *  Created on: 17 Oct 2017
 *      Author: Ashley
 */

#ifndef ARK_UTIL_LOCALMULTIPLAYER_SCOREBOARD_H_
#define ARK_UTIL_LOCALMULTIPLAYER_SCOREBOARD_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"

using namespace std;

namespace ARK {
	namespace Util {
		namespace LocalMultiplayer {

			class ARK2D_API ScoreboardScoreChange {
				public:	
					signed int* scores;
					unsigned int m_teams;
				public:
					ScoreboardScoreChange(int maxTeams);
					void setAll(signed int val);
					~ScoreboardScoreChange();
            };


			class ARK2D_API Scoreboard {
				public:
					signed int* m_scores;
					signed int* m_scoresAtStartOfRound;

					unsigned int m_teams;
					signed int m_maxScore;
				public:
					Scoreboard(int maxTeams, int maxScore);
					void reset();
					void setMaxScore(int maxScore);
					void nextRound();
					bool isWon();
					void addScoreByChanges(ScoreboardScoreChange* sc);
					void addScoreForTeam(unsigned int team, signed int score);
					signed int getScoreForTeam(unsigned int team);
					signed int getScoreForTeamAtStartOfRound(unsigned int team);
					signed int getTopScoreAtStartOfRound();
					bool hasScoreChanges();
					~Scoreboard();
			};
		}
	}
}
#endif
