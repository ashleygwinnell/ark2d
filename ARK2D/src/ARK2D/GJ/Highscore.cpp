
#include "Highscore.h"
#include "../../ARK.h"

namespace ARK {
	namespace GJ {

		Highscore::Highscore(): PropertyItem() {

		}
		unsigned int Highscore::getType() const { 
			return Cast::fromString<unsigned int>(m_properties.find("type")->second);
		}
		unsigned int Highscore::getTime() const {
			return Cast::fromString<unsigned int>(m_properties.find("time")->second);
		}
		string Highscore::getScoreString() const {
			return m_properties.find("score")->second;
		} 
		signed int Highscore::getScoreValue() const {
			return Cast::fromString<signed int>(m_properties.find("sort")->second);
		}
		string Highscore::getExtraData() const { 
			return m_properties.find("extra_data")->second; 
		}
		unsigned int Highscore::getUserId() const {
			return Cast::fromString<unsigned int>(m_properties.find("user_id")->second);
		} 
		string Highscore::getName() const {
			return m_properties.find("name")->second;  
		}
		Highscore::~Highscore() { 
			m_properties.clear();
		}

	}
}
