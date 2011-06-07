#ifndef libJSON_DEFS_H
#define libJSON_DEFS_H

#define JSON_NULL '\0'
#define JSON_STRING '\1'
#define JSON_NUMBER '\2'
#define JSON_BOOL '\3'
#define JSON_ARRAY '\4'
#define JSON_NODE '\5'
#define JSON_COMMENT '\6'

#include "JSON_Strings.h"

#ifdef JSON_DLL
    #define JSONNODE void*

    typedef JSONNODE (*JSON_Parse_t)(const json_char *);
    typedef void (*JSON_Delete_t)(const JSONNODE);
    typedef void (*JSON_Preparse_t)(const JSONNODE);

    typedef JSONNODE (*JSON_GetNode_t)(const JSONNODE, const json_char *);
    typedef JSONNODE (*JSON_NodeAt_t)(const JSONNODE, unsigned int);
    typedef unsigned int (*JSON_NodeSize_t)(const JSONNODE);
    typedef JSONNODE (*JSON_PopNode_t)(const JSONNODE, const json_char *);
    typedef JSONNODE (*JSON_PopNodeAt_t)(const JSONNODE, unsigned int);

    typedef json_char (*JSON_NodeType_t)(const JSONNODE);

    typedef const json_char * (*JSON_NodeName_t)(const JSONNODE);
    typedef const json_char * (*JSON_NodeAsString_t)(const JSONNODE);
    typedef int (*JSON_NodeAsInt_t)(const JSONNODE);
    typedef bool (*JSON_NodeAsBool_t)(const JSONNODE);
    typedef double (*JSON_NodeAsFloat_t)(const JSONNODE);

    typedef JSONNODE (*JSON_NewNode_t)(const json_char *, const json_char *);
    typedef JSONNODE (*JSON_AddNewChild_t)(const JSONNODE, const json_char *, const json_char *);
    typedef void (*JSON_AddChild_t)(const JSONNODE, const JSONNODE);
    typedef JSONNODE (*JSON_NewStringNode_t)(const json_char *, const json_char *);
    typedef JSONNODE (*JSON_AddNewStringChild_t)(const JSONNODE, const json_char *, const json_char *);
    typedef JSONNODE (*JSON_Duplicate_t)(const JSONNODE);

    typedef void (*JSON_SetValue_t)(const JSONNODE, const json_char *);
    typedef void (*JSON_SetStringValue_t)(const JSONNODE, const json_char *);

    typedef void (*JSON_AddComment_t)(const JSONNODE, const json_char *);
    typedef json_char *(*JSON_Write_t)(const JSONNODE);
    typedef json_char *(*JSON_WriteFormatted_t)(const JSONNODE);
    typedef void (*JSON_Free_t)(json_char * str);

    typedef json_char *(*JSON_Dump_t)(const JSONNODE);
    typedef unsigned int (*JSON_GetMemoryUsage_t)(const JSONNODE);
#else
    #include "JSONNode.h"
    #define JSONNODE JSONNode
#endif

//If the JSON engine is compiled without the DEBUG definition, there are no callback functions
typedef void (*JSON_ErrorCallback)(const json_char *);
typedef void (*JSON_RegisterErrorCallback_t)(JSON_ErrorCallback);

#endif
