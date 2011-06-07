// Mac OSX
//   ./upx "libJSON/Embedded Test Suite/Embedded Test Suite OSX Debug"
//   ./upx "libJSON/Embedded Test Suite/Embedded Test Suite OSX"

#ifndef libJSON_main_h
#define libJSON_main_h

#include "Debug.h"
#include "JSON_Defs.h"

/*
 *
 *  Created by Jonathan Wallace on 11/26/09.
 *  Copyright (c) 2009 Jonathan Wallace. All rights reserved.
 *  Free to use and modify for personal and commerical products under GPL Version 3
 *  Provided AS IS with no warranty or guarentee
 *  Please report bugs to ninja9578@yahoo.com
 *  Check included Open Document for documentation
 *
 */

#ifdef JSON_DLL
    //DLL interface
    extern "C" {
	   //These are for creating and deleting json document
	   JSONNODE JSON_Parse(const json_char * json);
	   void JSON_Delete(const JSONNODE node);
	   #ifndef JSON_PREPARSE
            void JSON_Preparse(const JSONNODE node);
       #endif

	   //These are for getting nodes out of that document
	   JSONNODE JSON_GetNode(const JSONNODE parent, const json_char * name);
	   JSONNODE JSON_GetNode_NoCase(const JSONNODE parent, const json_char * name);
	   JSONNODE JSON_NodeAt(const JSONNODE parent, unsigned int position);
	   unsigned int JSON_NodeSize(const JSONNODE node);
	   JSONNODE JSON_PopNode(const JSONNODE parent, const json_char * name);
	   JSONNODE JSON_PopNode_NoCase(const JSONNODE parent, const json_char * name);
	   JSONNODE JSON_PopNodeAt(const JSONNODE parent, unsigned int position);

	   //These are for inquiring about the json nodes
	   char JSON_NodeType(const JSONNODE node);
	   const json_char * JSON_NodeName(const JSONNODE node);
	   const json_char * JSON_NodeAsString(const JSONNODE node);
	   int JSON_NodeAsInt(const JSONNODE node);
	   bool JSON_NodeAsBool(const JSONNODE node);
	   double JSON_NodeAsFloat(const JSONNODE node);

	   //These are for adding nodes
	   JSONNODE JSON_NewNode(const json_char * name, const json_char * value);
	   JSONNODE JSON_AddNewChild(const JSONNODE parent, const json_char * name, const json_char * value);
	   JSONNODE JSON_NewStringNode(const json_char * name, const json_char * value);
	   JSONNODE JSON_AddNewStringChild(const JSONNODE parent, const json_char * name, const json_char * value);
	   JSONNODE JSON_Duplicate(const JSONNODE node);

	    //These are for manipulating nodes
	   void JSON_AddChild(const JSONNODE parent, const JSONNODE node);
	   void JSON_SetValue(const JSONNODE node, const json_char * value);
	   void JSON_SetStringValue(const JSONNODE node, const json_char * value);

	   //These are for going from nodes to strings
	   void JSON_AddComment(const JSONNODE parent, const json_char * comment);
	   json_char * JSON_Write(const JSONNODE node);
	   json_char * JSON_WriteFormatted(const JSONNODE node);
	   void JSON_Free(json_char * str);

	    #ifdef DEBUG
		    //Registering an error callback
		    void JSON_RegisterErrorCallback(ErrorCallback method);
            json_char * JSON_Dump(const JSONNODE root);
            unsigned int JSON_GetMemoryUsage(const JSONNODE root);
        #endif
    }
#else
    //Embedded helper class
    class libJSON {
        public:
            static JSONNode * Parse(const json_string & json);
            static JSONNode * NewNode(const json_string & name, const json_string & value);
            static JSONNode * NewStringNode(const json_string & name, const json_string & value);
            static void Delete(JSONNODE * node);
            #ifdef DEBUG
                static void RegisterErrorCallback(ErrorCallback method);
            #endif
    };
#endif

#endif
