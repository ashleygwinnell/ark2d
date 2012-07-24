
#include "Highscore.h"
#include "../../ARK.h"

namespace ARK {
	namespace GJ {

		Highscore::Highscore(): PropertyItem() {

		}
		unsigned int Highscore::getType() {
			return Cast::fromString<unsigned int>(m_properties["type"]);
		}
		unsigned int Highscore::getTime() {
			return Cast::fromString<unsigned int>(m_properties["time"]);
		}
		string Highscore::getScoreString() {
			return m_properties["score"];
		}
		signed int Highscore::getScoreValue() {
			return Cast::fromString<signed int>(m_properties["sort"]);
		}
		string Highscore::getExtraData() { 
			return m_properties["extra_data"]; 
		}
		unsigned int Highscore::getUserId() {
			return Cast::fromString<unsigned int>(m_properties["user_id"]);
		}
		Highscore::~Highscore() {
			m_properties.clear();
		}

	}
}
