#ifndef JSON_STRING_CONV_H
#define JSON_STRING_CONV_H

#include <sstream>

template <class T>
json_string ToString(T number){
    std::stringstream out;
    out << number;
    #ifdef JSON_UNICODE
        string s(out.str());
        return json_string(s.begin(), s.end());
    #else
        return out.str();
    #endif
}

#endif
