
#include "Trophy.h"
//#include "../Includes.h"
#include "../Core/Cast.h"

namespace ARK {
	namespace GJ {

		Trophy::Trophy(): PropertyItem() {

		}
		string Trophy::getName() {
			return m_properties["name"];
		}
		unsigned int Trophy::getDifficulty() {
			return Cast::fromString<unsigned int>(m_properties["difficulty"]);
		}
		Trophy::~Trophy() {

		} 
	} 
}
