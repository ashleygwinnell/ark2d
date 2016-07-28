/*
 * RSSL.cpp
 *
 *  Created on: 23 Aug 2011
 *      Author: Ashley
 */

#include "RSSL.h"
#include "../Core/String.h"
#include "StringUtil.h"
#include "Cast.h"
#include "ErrorDialog.h"

namespace ARK {
	namespace Util {

		RSSLException::RSSLException(string s): m_message(s) {

		}
		string RSSLException::getMessage() {
			return m_message;
		}
		void RSSLException::parseException(string s) {
		#ifdef EXCEPTIONS_AVAILABLE
			throw new RSSLException(StringUtil::append("Error parsing RSSL: ", s));
		#else
			ErrorDialog::createAndShow(StringUtil::append("Error parsing RSSL: ", s));
			exit(0);
		#endif
		}

		string RSSLArgument::asString() {
			return valStr;
		}
		int RSSLArgument::asInteger() {
			return valInt;
		}
		float RSSLArgument::asFloat() {
			return valFloat;
		}

		void RSSLFunction::addArgument(string name, unsigned int t) {
			RSSLArgument* arg = new RSSLArgument();
			arg->name = name;
			arg->type = t;
			m_args.push_back(arg);
		}
		RSSLArgument* RSSLFunction::getArgument(int i) {
			return m_args.at(i);
		}
		RSSLArgument* RSSLFunction::getArgument(string s) {
			for(unsigned int i = 0; i < m_args.size(); i++) {
				if (m_args.at(i)->name == s) {
					return m_args.at(i);
				}
			}
			return NULL;
		}

		RSSL::RSSL(): m_functions() {

		}

		void RSSL::addFunction(RSSLFunction* f) {
			m_functions.push_back(f);
		}

		void RSSL::parse(string s)
		{
			//string response = "";
			//bool err = false;

			stringbuf* sb = new stringbuf();
			sb->str(s);

			istream stream(sb);
			string line;

			int lineNumber = 1;
			while(!stream.eof()) {
				stringstream lineStream;
				std::getline(stream, line);
				if (line.length() == 0) { continue; }

				String arkLine(line);
				if (arkLine.contains("#")) {
					arkLine = arkLine.split("#")[0];
				}

				vector<String> parts = arkLine.split("(");
				if (parts.size() <= 1 || arkLine.countOccurrences(")") != 1) {
					string s = "syntax error at line " + Cast::toString<int>(lineNumber) + "\r\nexpected something in the format `command_name(arguments)`.";
					RSSLException::parseException(s);

					return;
				}
				String commandName = parts[0];
				String argString = parts[1];
				if (argString.length()==0 || argString.lastChar() != ")") {
					string s = "unexpected line end. it should be ')' character.";
					RSSLException::parseException(s);

					return;
				}
				argString = argString.substring(0, argString.length()-1);
				argString = StringUtil::trimret(argString.get(), " ");
				// param 1: game mode ( normal, surrounded, etc ) (unquoted string)
				// param 2: time (int)

				for(unsigned int i = 0; i < m_functions.size(); i++)
				{
					RSSLFunction* f = m_functions.at(i);
					if (commandName.equals(f->name))
					{
						vector<String> args = argString.split(",");

						// if ((args.size() == 1 && args[0].length() == 0) || args.size() != 2) {
						if (args.size() != f->m_args.size()) {
							string s = "wrong args passed. expected xxx,xxx. \r\n";
							RSSLException::parseException(s);

							return;
						} else {
							bool blankargument = false;
							for(unsigned int i = 0; i < args.size(); i++) { // make sure we trim all args.
								args[i] = StringUtil::trimret(args[i].get(), " ");
								if (args[i].length() == 0) {
									blankargument = true;
									break;
								}
							}
							if (blankargument) {
								string s = "syntax error. one or more arguments to " + commandName.get() + " were blank. \r\n";
								RSSLException::parseException(s);

								return;
							}

							// assign args here.
							for(unsigned int i = 0; i < args.size(); i++) {
								if (f->m_args.at(i)->type == RSSL::TYPE_STRING) {
									f->m_args.at(i)->valStr = args.at(i).get();
								} else if (f->m_args.at(i)->type == RSSL::TYPE_INT) {
									f->m_args.at(i)->valInt = args.at(i).getAsInt();
								} else if (f->m_args.at(i)->type == RSSL::TYPE_FLOAT) {
									f->m_args.at(i)->valFloat = args.at(i).getAsFloat();
								}

							}
							//String gameMode = StringUtil::trimret(args[0].get(), " ");
							//String time = StringUtil::trimret(args[1].get(), " ");

							void (*pt)(RSSLFunction*) = (void(*)(RSSLFunction*)) f->callback;
							pt(f);

							break;
						}

					}
					if (i == m_functions.size()-1) {
						string s = "unknown command: " + commandName.get();
						RSSLException::parseException(s);
					}
				}
				lineNumber += 1;

				/*if (commandName.equals("game_mode")) {


				}

				// param 1: boss_id (int)
				// param 2: time (int)
				else if (commandName.equals("start_boss")) {

				}

				// param 1: pixels (int)
				// param 2: time (int)
				else if (commandName.equals("camera_shake")) {

				}

				// param 1: number of lines to read (int)
				// param 2: time (int)
				else if (commandName.equals("modal_message")) {

				}

				// param 1: spawn at x coordinate (int)
				// param 2: time
				else if (commandName.equals("spawn_trinket")) {

				} else {

				}*/

			}

			//if (err == false) {
			//	RSSLException::exception(StringUtil::append("Error parsing RSSL: " + response));
			//}
		}

		RSSL::~RSSL() {

		}

	}
}
