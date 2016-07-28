#ifndef TILEDMAP_PROPERTY_H_
#define TILEDMAP_PROPERTY_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "../Core/ToString.h"

#include <string>
using namespace std;

namespace ARK {
	namespace Tiled {

		/*!
		 * \brief A key-value pair on a Tiled Map.
		 *
		 * @todo Deprecate this for a normal tuple.
		 *
		 * @warning You should never need to create this manually.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API TiledMapProperty : public ARK::Core::ToString {
			public:
				TiledMapProperty();
				TiledMapProperty(const std::string& name, const std::string& value);

				const string getName() const;
				const string getValue() const;
				signed int getValueAsInt() const;
				float getValueAsFloat() const;

				void setName(const string& s);
				void setValue(const string& s);

				string toString();
				const string operator+=(std::string f);

				virtual ~TiledMapProperty();

			private:
				string m_name;
				string m_value;
		};

	}
}

#endif
