#ifndef ARK2D_RAPIDXML_UTILS_HPP_INCLUDED____
#define ARK2D_RAPIDXML_UTILS_HPP_INCLUDED____

#pragma once

#include "rapidxml.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

namespace rapidxml
{
    template<class Str=std::string, class Ch=char>
	class rapidxml_myutil { 
		public:

		// Rapid XML util
		static Str rapidXmlUtil_value(rapidxml::xml_node<Ch>* node) {
			Str val = node->value();
			if (val.length() == 0) {
				if (node->first_node() != NULL) {
					val = node->first_node()->value();
				}
			}
			return val;
		}
		static unsigned int rapidXmlUtil_countChildren(rapidxml::xml_node<Ch>* node, Str childname) {
			unsigned int i = 0;
		    rapidxml::xml_node<Ch>* child = 0;
			for (child = node->first_node(childname.c_str());
				child != NULL;
				child = child->next_sibling(childname.c_str())) {
				i++;
			}
			return i;
		}

	};
}

#endif