/*
 * RSSL.h
 *
 *  Created on: 23 Aug 2011
 *      Author: Ashley
 */

#ifndef RSSL_H_
#define RSSL_H_

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

namespace ARK {
	namespace Util {

		class RSSLException {
			private:
				string m_message;
			public:
				RSSLException(string s);
				string getMessage();
				static void parseException(string s);
		};

		class RSSLArgument {
			public:
				string name;
				unsigned int type;

				string valStr;
				int valInt;
				float valFloat;

				string asString();
				int asInteger();
				float asFloat();
		};

		class RSSLFunction {
			friend class RSSL;
			private:
				vector<RSSLArgument*> m_args;
			public:
				string name;
				bool multiline;
				void* callback;
				void addArgument(string name, unsigned int t);
				RSSLArgument* getArgument(int i);
				RSSLArgument* getArgument(string s);
		};

		/*!
		 * \brief Really Simple Scripting Language
		 *
		 * Hook JavaScript like text to functions in your code.
		 * It's quite verbose and length to implement but does the job without the
		 * overhead of a JavaScript engine such as Rhino or V8.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class RSSL {
			friend class RSSLFunction;
			private:
				vector<RSSLFunction*> m_functions;
			public:
				static const unsigned int TYPE_STRING = 0;
				static const unsigned int TYPE_INT = 1;
				static const unsigned int TYPE_FLOAT = 2;

				RSSL();
				void addFunction(RSSLFunction* f);
				void parse(string s);
				virtual ~RSSL();
		};

	}
}

#endif /* RSSL_H_ */
