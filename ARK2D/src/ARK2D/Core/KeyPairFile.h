/*
 * KeyPairFile.h
 *
 *  Created on: 8 Nov 2013
 *      Author: Ashley
 */

#ifndef ARK_CORE_KEYPAIRFILE_H_
#define ARK_CORE_KEYPAIRFILE_H_

#include "../Namespaces.h"
#include "../Common/DLL.h"

#include "Cast.h"
#include "Resource.h"
#include "Util/StringUtil.h"
#include "Util/SystemUtil.h"

#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

namespace ARK {
	namespace Core {

		/*!
		 * \brief Locally stored highscores. Use Resource class to load and save these.
		 *
		 * They are stored as key pair, duh!
		 *
		 * @see Resource
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API KeyPairFile : public ARK::Core::Resource {
			public:
				string m_filename;
				void* m_data;
				map<string, string> m_map;
				map<string, string> m_mapUnsaved; // keep a map of unsaved variables so we can
												  // clear them separately instead of reloading
												  // the file again.

			public:
				KeyPairFile(string filename);
				KeyPairFile(string filename, void* data);

				void read(string data);
				bool save();
				void clear();
				void clearUnsaved();

				void add(string key, string val);
				void add(string key, bool val);
				void add(string key, unsigned int val);
				void add(string key, float val);
				void set(string key, string val);
				void set(string key, bool val);
				void set(string key, unsigned int val);
				void set(string key, float val);

				bool getBoolean(string key);
				bool getBoolean(string key, bool defaultValue);

				unsigned int getInteger(string key);
				unsigned int getInteger(string key, unsigned int defaultValue);

				float getFloat(string key);
				float getFloat(string key, float defaultValue);

				string getString(string key);
				string getString(string key, string defaultValue);

				string getFilename();
				string toString();

				virtual ~KeyPairFile();

			private:
				void parse();

		};
	}
}

#endif /* KEYPAIRFILE_H_ */
