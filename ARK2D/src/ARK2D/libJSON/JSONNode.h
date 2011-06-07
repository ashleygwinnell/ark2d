#ifndef libJSON_NODE_H
#define libJSON_NODE_H

#include <string>
#include "JSON_Strings.h"
#include "JSONChildren.h"

#define foreach(iterator, container)\
    for (typeof(container.begin()) iterator = container.begin(), iterator##_end = container.end();\
         iterator != iterator##_end;\
         ++iterator)

using namespace std;

union value_union_t {
    bool _bool;
    #ifdef JSON_LESS_MEMORY
        float _number;
        size_t lastreal: 20;
    #else
        double _number;
        size_t lastreal;  //This is used to determine the last real entry in the Children, because it's possible to have comments at the end, we don't want commas before them
    #endif
};

class JSONNode {
    public:
	   JSONNode(const json_string & name_t, const json_string & value_t);
	   JSONNode(void){};  //Used ONLY for duplicating
	   JSONNode(JSONNode * parent);
	   ~JSONNode(void);
	   void SetMyValue(const json_string & value_t);
	   void CloneTo(JSONNode * node);
	   json_string Write(unsigned int indent, bool & commaafter);  //pass 0xFFFFFFFF to do no formatting
	   void Add(JSONNode * child);

        #ifdef JSON_LESS_MEMORY
            char type : 4;
        #else
            char type;  //an enumerated type that tells the engine how to treat it
        #endif

       #ifndef JSON_PREPARSE
            void Fetch(void);
            bool Fetched;  //if the value has been parsed yet or not
       #endif

	   #ifndef JSON_DLL  //These are the only methods you should actually use
            #ifndef JSON_PREPARSE
                void Preparse(void);
            #endif

		  JSONNode * GetNode(const json_string & name);
		  JSONNode * GetNode_NoCase(const json_string & name);
		  JSONNode * NodeAt(unsigned int position);
		  unsigned int NodeSize(void);
		  JSONNode * PopNode(const json_string & name);
		  JSONNode * PopNode_NoCase(const json_string & name);
		  JSONNode * PopNodeAt(unsigned int position);

		  char NodeType(void);
		  const json_string & NodeName(void);
		  const json_string & NodeAsString(void);
		  int NodeAsInt(void);
		  bool NodeAsBool(void);
		  #ifdef JSON_LESS_MEMORY
            float NodeAsFloat(void);
          #else
            double NodeAsFloat(void);
          #endif

		  JSONNode * AddNewChild(const json_string & name, const json_string & value);
		  JSONNode * AddNewStringChild(const json_string & name, const json_string & value);
		  JSONNode * Duplicate(void);

		  void AddChild(const JSONNode * node);
		  void SetValue(const json_string & value);
		  void SetStringValue(const json_string & value);

		  void AddComment(const json_string & comment);
		  json_string Write(unsigned int indent = 0xFFFFFFFF);
		  json_string WriteFormatted(void);

            #ifdef DEBUG
                json_string Dump(void);
            #endif
	   #endif

	   #ifdef JSON_SAFE
            void Nullify(void);
	   #endif

	   #ifdef DEBUG
            unsigned int GetMemoryUsage(void);
            json_string MemDump(unsigned int indent = 0);
	   #endif


       //This is the name of the node, the encoded flag tells the writer whether or not to re-escape it when it gets written
	   json_string _name;  bool name_encoded;

	   //This is the json_string that is the value of the node, this might be a json_string, a number, or even a set of nodes or array that hasn't been parsed yet
	   json_string _string;  bool string_encoded;

       //internal structure changes depending on type
	   value_union_t _value;

        jsonChildren Children;
    private:
        void FetchString(void);
        void FetchNode(void);
        void FetchArray(void);
        void FetchNumber(void);

	   json_string WriteChildren(unsigned int indent);
	   json_string WriteName(bool formatted) const;
	   json_string WriteComment(unsigned int indent) const;
};

#endif
