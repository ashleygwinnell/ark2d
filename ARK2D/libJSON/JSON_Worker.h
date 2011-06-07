#ifndef libJSON_WORKER_H
#define libJSON_WORKER_H

#include <string>
#include "JSONNode.h"

using namespace std;

class Worker {
    public:
	   static void DoArray(JSONNode * parent, const json_string & value_t);
	   static void DoNode(JSONNode * parent, const json_string & value_t);
	   #if defined DEBUG || defined JSON_SAFE
            static json_char * RemoveWhiteSpace(const json_string & value_t, json_char & last);
       #else
            static json_char * RemoveWhiteSpace(const json_string & value_t);
       #endif
	   static json_string FixString(const json_string & value_t, bool & flag);
	   static json_string UnfixString(const json_string & value_t, bool flag);
    private:
       static json_char Hex(const json_char * & pos);
	   static unsigned json_char UTF8(const json_char * & pos);
	   static json_string toUTF8(unsigned json_char p);
	   #ifdef JSON_UNICODE
            static json_string UTF(const json_char * & pos);
            static json_string toSurrogatePair(unsigned json_char pos);
       #endif
	   static void SpecialChar(const json_char * & pos, json_string & res);
	   static size_t FindNextRelevant(json_char ch, const json_string & value_t, const size_t pos);
};

#endif
