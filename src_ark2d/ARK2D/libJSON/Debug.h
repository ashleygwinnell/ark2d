#ifndef libJSON_DEBUG_H
#define libJSON_DEBUG_H

#ifdef DEBUG
    #ifdef JSON_SAFE
        #define libJSON_ASSERT_SAFE(condition, message, failcode)\
            if (!(condition)){ if (Error){Error(message);} failcode }
        #define libJSON_FAIL_SAFE(message, failcode)\
            { if (Error){ Error(message); } failcode }
    #else
        #define libJSON_ASSERT_SAFE(condition, message, failcode)\
            if ((!(condition)) && (Error)) Error(message);
        #define libJSON_FAIL_SAFE(message, failcode)\
            if (Error) Error(message);
    #endif

    #define libJSON_ASSERT(condition, message)\
	    if ((!(condition)) && (Error)) Error(message);
    #define libJSON_FAIL(message)\
        if (Error) Error(message);

    #include "JSON_Strings.h"

	extern "C"{
		typedef void (*ErrorCallback)(const json_char *);
	}
#else
    #ifdef JSON_SAFE
        #define libJSON_ASSERT_SAFE(condition, message, failcode)\
            if (!(condition)){ failcode; }
        #define libJSON_FAIL_SAFE(message, failcode)\
            {failcode}
    #else
        #define libJSON_ASSERT_SAFE(condition, message, failcode)
        #define libJSON_FAIL_SAFE(message, failcode)
    #endif

    #define libJSON_ASSERT(condition, message)
    #define libJSON_FAIL(message)
#endif

#endif
