#ifndef TILEDMAP_PROPERTY_H_
#define TILEDMAP_PROPERTY_H_

#include <vector>
#include <string>

#include "../ToString.h"

using namespace std;

class TiledMapProperty : public ARK::ToString {
	public:
		TiledMapProperty();
		TiledMapProperty(const std::string& name, const std::string& value);

		const string getName() const;
		const string getValue() const;

		void setName(const string& s);
		void setValue(const string& s);

		string toString();
		const string operator+=(std::string f);

		virtual ~TiledMapProperty();

	private:
		string m_name;
		string m_value;
};

#endif
