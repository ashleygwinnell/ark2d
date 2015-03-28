
#include "User.h"
#include "../Includes.h"
#include "../Util/Cast.h"

namespace ARK {
	namespace GJ {

		User::User(): PropertyItem() {

		}
		string User::getName() {
			return m_properties["name"];
		}
		string User::getToken() {
			return m_properties["token"];
		}
		unsigned int User::getType() {
			return Cast::fromString<unsigned int>(m_properties["type"]);
		}
		unsigned int User::getStatus() {
			return Cast::fromString<unsigned int>(m_properties["status"]);
		}
		User::~User()  {
			 
		}

	}
}
