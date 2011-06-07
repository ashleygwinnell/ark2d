#include "JSON_Worker.h"
#include "JSON_Defs.h"
#include "Debug.h"

/*
    These are helper functions, which do string and character manipulation and translation
    This fetches array and nodes and does the translations for utf, octal, hex, and escaping and all other escaped characters
    It goes both to and from escaped characters
*/

//#ifdef _LINUX_
#ifdef ARK2D_UBUNTU_LINUX
    #ifdef DEBUG
        #include <cstring> //for strlen
    #endif
    #include <cstdlib> //for malloc
#endif

#ifdef DEBUG
	extern ErrorCallback Error;
#endif

#define QUOTECASE()\
    case TEXT('\"'):\
	   while (*(++p) != TEXT('\"')){\
		  libJSON_ASSERT_SAFE(*p, TEXT("Null terminator inside of a quotation"), return json_string::npos;);\
	   }\
	   break;

#ifdef DEBUG
    #define NULLCASE(error)\
	   case TEXT('\0'):\
		  libJSON_FAIL_SAFE(error, return json_string::npos;);\
		  break;
#else
    #define NULLCASE(error)
#endif

#define BRACKET(left, right)\
    case left: {\
         size_t brac = 1;\
         while (brac){\
            switch (*(++p)){\
                case right:\
                   --brac;\
                   break;\
                case left:\
                   ++brac;\
                   break;\
                QUOTECASE()\
                NULLCASE(TEXT("Null terminator inside of a bracket"))\
            }\
         }\
         break;}\
    case right:\
        return json_string::npos;

size_t Worker::FindNextRelevant(json_char ch, const json_string & value_t, const size_t pos){
    const json_char * start = value_t.c_str();
    const json_char * p = start + pos; //start at the correct offset
    do {
	   if (*p == ch) return p - start;
	   switch (*p){
		  BRACKET(TEXT('['), TEXT(']'))
		  BRACKET(TEXT('{'), TEXT('}'))
		  QUOTECASE()
	   }
    } while(*(++p));
    return json_string::npos;
}

inline void SingleLineComment(const json_char * & p){
    while((*(++p)) && (*p != TEXT('\n')));
}

#if defined DEBUG || defined JSON_SAFE
json_char * Worker::RemoveWhiteSpace(const json_string & value_t, json_char & last){
#else
json_char * Worker::RemoveWhiteSpace(const json_string & value_t){
#endif
    json_char * result;
    json_char * runner = result = (json_char*)malloc(value_t.length() * sizeof(json_char));  //dealing with raw memory is faster than adding to a json_string
    libJSON_ASSERT(result, TEXT("Out of memory"));
    const json_char * p = value_t.c_str();
    while(*p){
	   switch(*p){
		  case TEXT(' '):   //defined as white space
		  case TEXT('\t'):  //defined as white space
		  case TEXT('\n'):  //defined as white space
		  case TEXT('\r'):  //defined as white space
			 break;
          case TEXT('/'):  //a C comment
                if (*(++p) == TEXT('*')){  //a multiline comment
                    while ((*(++p) != TEXT('*')) || (*(p + 1) != TEXT('/'))){
					   libJSON_ASSERT_SAFE(*p, TEXT("Null terminator inside of a multiline quote"), goto endofloop;);
				    }
				    ++p;
				    break;
                }
                //Should be a single line C comment, so let it fall through to use the bash comment stripper
                libJSON_ASSERT_SAFE(*p == TEXT('/'), TEXT("stray / character, not quoted, or a comment"), goto endofloop;);
		  case TEXT('#'):  //a bash comment
                SingleLineComment(p);
                break;
		  case TEXT('\"'):  //a quote
			 *runner++ = TEXT('\"');
			 while(*(++p) != TEXT('\"')){  //find the end of the quotation, as white space is preserved within it
				libJSON_ASSERT(*p, TEXT("Null terminator inside of a quotation"));
				switch(*p){
				    case TEXT('\\'):
					   *runner++ = TEXT('\\');
					   *runner++ = (*++p == TEXT('\"')) ? TEXT('\1') : *p;  //an escaped quote will reak havoc will all of my searching functions, so change it into an illegal character in JSON for convertion later on
					   break;
				    default:
					   *runner++ = *p;
					   break;
				}
			 }
			 //no break, let it fall through so that the trailing quote gets added
		  default:
			 libJSON_ASSERT_SAFE((unsigned json_char)*p >= 32, TEXT("Invalid JSON character detected (lo)"), goto endofloop;);
			 libJSON_ASSERT_SAFE((unsigned json_char)*p <= 126, TEXT("Invalid JSON character detected (hi)"), goto endofloop;);
			 *runner++ = *p;
			 break;
	   }
	   ++p;
    }
#ifdef JSON_SAFE
endofloop:
#endif
    #if defined DEBUG || defined JSON_SAFE
        last = *(runner - 1);
    #endif
    *runner = TEXT('\0');
    return result;
}

/*
    These three functions analyze json_string literals and convert them into std::strings
    This includes dealing with special characters and utf characters
*/
#ifdef JSON_UNICODE
    inline unsigned json_char SurrogatePair(const unsigned json_char hi, const unsigned json_char lo){
        libJSON_ASSERT(sizeof(unsigned int) == 4, TEXT("size of unsigned int is not 32-bit"));
        libJSON_ASSERT(sizeof(unsigned json_char) == 4, TEXT("size of json_char is not 32-bit"));
        return (((hi << 10) & 0x1FFC00) + 0x10000) | lo & 0x3FF;
    }

    json_string Worker::UTF(const json_char * & pos){
        json_string result;
        unsigned json_char first = UTF8(pos);
        if ((*(pos + 1) == '\\') && (*(pos + 2) == 'u')){
            pos += 2;
            unsigned json_char second = UTF8(pos);
            //surrogate pair, not two characters
            if ((first > 0xD800) && (first < 0xDBFF) && (second > 0xDC00) && (second < 0xDFFF)){
                result += SurrogatePair(first, second);
            } else {
                result += first;
                result += second;
            }
        } else {
            result += first;
        }
        libJSON_ASSERT(!result.empty(), TEXT("missing case, somehow UTF returned empty"));
        return result;
    }
#endif

unsigned json_char Worker::UTF8(const json_char * & pos){
    #ifdef JSON_UNICODE
        ++pos;
        unsigned json_char temp = Hex(pos) << 8;
        ++pos;
        return temp | Hex(pos);
    #else
        libJSON_ASSERT(*(pos + 1) == TEXT('0'), TEXT("wide utf character (hihi)"));
        libJSON_ASSERT(*(pos + 2) == TEXT('0'), TEXT("wide utf character (hilo)"));
        pos += 3;
        return Hex(pos);
    #endif
}

json_char Worker::Hex(const json_char * & pos){
    /*
	   takes the numeric value of the next two characters and convert them
	   \u0058 becomes 0x58

	   In case of \u, it's SpecialChar's responsibility to move past the first two chars
	   as this method is also used for \x
    */
    //First character
    unsigned json_char hi = *pos++ - 48;
    if (hi > 48){  //A-F don't immediately follow 0-9, so have to pull them down a little
	   hi -= 39;
    } else if (hi > 9){  //neither do a-f
	   hi -= 7;
    }
    //second character
    unsigned json_char lo = *pos - 48;
    if (lo > 48){  //A-F don't immediately follow 0-9, so have to pull them down a little
	   lo -= 39;
    } else if (lo > 9){  //neither do a-f
	   lo -= 7;
    }
    //combine them
    return (json_char)((hi << 4) | lo);
}

inline json_char FromOctal(const json_char * & str){
    libJSON_ASSERT(json_strlen(str) > 3, TEXT("Octal will go out of bounds"));
    const unsigned json_char top = ((unsigned json_char)(*(str++) - 48));
    const unsigned json_char middle = (unsigned json_char)(*(str++) - 48);
    const unsigned json_char bottom = (unsigned json_char)(*str - 48);
    return (json_char)((top << 6) | (middle << 3) | bottom);
}

void Worker::SpecialChar(const json_char * & pos, json_string & res){
    /*
	   Since JSON uses forward slash escaping for special characters within strings, I have to
	   convert these escaped characters into C characters
    */
    switch(*pos){
	   case TEXT('\1'):  //quote character (altered by RemoveWhiteSpace)
		  res += TEXT('\"');
		  break;
	   case TEXT('t'):	//tab character
		  res += TEXT('\t');
		  break;
	   case TEXT('n'):	//newline character
		  res += TEXT('\n');
		  break;
	   case TEXT('r'):	//return character
		  res += TEXT('\r');
		  break;
	   case TEXT('\\'):	//backslash
		  res += TEXT('\\');
		  break;
	   case TEXT('/'):	//forward slash
		  res += TEXT('/');
		  break;
	   case TEXT('b'):	//backspace
		  res += TEXT('\b');
		  break;
	   case TEXT('f'):	//formfeed
		  res += TEXT('\f');
		  break;
	   case TEXT('v'):	//vertical tab
		  res += TEXT('\v');
		  break;
	   case TEXT('\''):	//apostrophe
		  res += TEXT('\'');
		  break;
	   case TEXT('x'):   //hexidecimal ascii code
		  res += Hex(++pos);
		  break;
	   case TEXT('u'):	//utf character
          #ifdef JSON_UNICODE
                res += UTF(pos);
          #else
                res += UTF8(pos);
          #endif
		  break;

        //octal encoding
        case TEXT('1'):
        case TEXT('2'):
        case TEXT('3'):
        case TEXT('4'):
        case TEXT('5'):
        case TEXT('6'):
        case TEXT('7'):
        case TEXT('0'):
            res += FromOctal(pos);
            break;

	   #ifdef DEBUG
		  default:
			 libJSON_FAIL(TEXT("Unsupported escaped character"));
	   #endif
    }
}

json_string Worker::FixString(const json_string & value_t, bool & flag){
    /*
	   Do things like unescaping
    */
    flag = false;
    json_string res;
    res.reserve(value_t.length());	 //since it goes one character at a time, want to reserve it first so that it doens't have to reallocating
    const json_char * p = value_t.c_str();
    while(*p){
	   switch (*p){
		  case TEXT('\\'):
             flag = true;
			 SpecialChar(++p, res);
			 break;
		  default:
			 res += *p;
			 break;
	   }
	   ++p;
    }
    return res;
}

#ifdef JSON_UNICODE
    json_string Worker::toSurrogatePair(unsigned json_char C){
        libJSON_ASSERT(sizeof(unsigned int) == 4, TEXT("size of unsigned int is not 32-bit"));
        libJSON_ASSERT(sizeof(unsigned short) == 2, TEXT("size of unsigned short is not 16-bit"));
        libJSON_ASSERT(sizeof(unsigned json_char) == 4, TEXT("json_char is not 32-bit"));
        //Compute the high surrogate
        const unsigned int U = (C >> 16) & 31;
        unsigned short HiSurrogate = 0xD800 | (((unsigned short)U - 1) << 6) | ((unsigned short)C) >> 10;

        //compute the low surrogate
        unsigned short LoSurrogate = (unsigned short) (0xDC00 | ((unsigned short)C) & 1023);

        json_string res;
        res += toUTF8(HiSurrogate);
        res += toUTF8(LoSurrogate);
        return res;
    }
#endif

json_string Worker::toUTF8(unsigned json_char p){
    #ifdef JSON_UNICODE
        if (p > 0xFFFF) return toSurrogatePair(p);
    #endif
    json_string res(TEXT("\\u"));
    #ifdef JSON_UNICODE
        unsigned json_char hihi = ((p & 0xF000) >> 12) + 48;
        if (hihi > 57) hihi += 7; //A-F don't immediately follow 0-9, so have to further adjust those
        unsigned json_char hilo = ((p & 0x0F00) >> 8) + 48;
        if (hilo > 57) hilo += 7; //A-F don't immediately follow 0-9, so have to further adjust those
        res += hihi;
        res += hilo;
        unsigned json_char hi = ((p & 0x00F0) >> 4) + 48;
    #else
        res += TEXT("00");
        unsigned json_char hi = (p >> 4) + 48;
    #endif
    //convert the character to be escaped into two digits between 0 and 15
    if (hi > 57) hi += 7; //A-F don't immediately follow 0-9, so have to further adjust those
    unsigned json_char lo = (p & 0x000F) + 48;
    if (lo > 57) lo += 7; //A-F don't immediately follow 0-9, so have to further adjust those
    res += hi;
    res += lo;
    return res;
}

json_string Worker::UnfixString(const json_string & value_t, bool flag){
    if (!flag) return value_t;
    /*
	   Re-escapes a json_string so that it can be written out into a JSON file
    */
    json_string res;
    res.reserve(value_t.length());  //since it goes one character at a time, want to reserve it first so that it doens't have to reallocating
    const json_char * p = value_t.c_str();
    while(*p){
	   switch(*p){
		  case TEXT('\"'):  //quote character
			 res += TEXT("\\\"");
			 break;
		  case TEXT('\t'):	//tab character
			 res += TEXT("\\t");
			 break;
		  case TEXT('\n'):	//newline character
			 res += TEXT("\\n");
			 break;
		  case TEXT('\r'):	//return character
			 res += TEXT("\\r");
			 break;
		  case TEXT('\\'):	//backslash
			 res += TEXT("\\\\");
			 break;
		  case TEXT('/'):	//forward slash
			 res += TEXT("\\/");
			 break;
		  case TEXT('\b'):	//backspace
			 res += TEXT("\\b");
			 break;
		  case TEXT('\f'):	//formfeed
			 res += TEXT("\\f");
			 break;
		  case TEXT('\v'):	//vertical tab
			 res += TEXT("\\v");
			 break;
		  case TEXT('\''):	//apostrophe
			 res += TEXT("\\\'");
			 break;
		  default:
			 if (((unsigned json_char)(*p) < 32) || ((unsigned json_char)(*p) > 126)){
			     res += toUTF8((unsigned json_char)(*p));
			 } else {
				res += *p;
			 }
			 break;
	   }
	   ++p;
    }
    return res;
}

//Create a childnode
inline void NewNode(JSONNode * parent, const json_string & name, const json_string & value){
    parent -> Children.push_back(new JSONNode(name, value));	    //attach it to the parent node
    //Don't increment the lastreal, that is done after they are all complete
}

//Create a subarray
void Worker::DoArray(JSONNode * parent, const json_string & value_t){
    /*
	   This takes an array and creates nodes out of them
    */
    libJSON_ASSERT(!value_t.empty(), TEXT("DoArray is empty"));
    libJSON_ASSERT_SAFE(value_t[0] == TEXT('['), TEXT("DoArray is not an array"), parent -> Nullify(); return;);
    const size_t len = value_t.length();
    if (len <= 2) return;  // just a [] (blank array)

    //Not sure what's in the array, so we have to use commas
    size_t starting = 1;  //ignore the [
    size_t ending = FindNextRelevant(TEXT(','), value_t, 1);
    while (ending != json_string::npos){
        #ifdef JSON_SAFE
            json_string newValue = value_t.substr(starting, ending - starting);
            libJSON_ASSERT_SAFE(FindNextRelevant(TEXT(':'), newValue, 0) == json_string::npos, TEXT("Key/Value pairs are not allowed in arrays"), parent -> Nullify(); return;);
            NewNode(parent, TEXT(""), newValue);
        #else
            NewNode(parent, TEXT(""), value_t.substr(starting, ending - starting));
        #endif
	   starting = ending + 1;
	   ending = FindNextRelevant(TEXT(','), value_t, starting);
    }
    //since the last one will not find the comma, we have to add it here, but ignore the final ]

    #ifdef JSON_SAFE
        json_string newValue = value_t.substr(starting, len - starting - 1);
        libJSON_ASSERT_SAFE(FindNextRelevant(TEXT(':'), newValue, 0) == json_string::npos, TEXT("Key/Value pairs are not allowed in arrays"), parent -> Nullify(); return;);
        NewNode(parent, TEXT(""), newValue);
    #else
        NewNode(parent, TEXT(""), value_t.substr(starting, len - starting - 1));
    #endif
    parent -> Children.shrink();
}


//Create all child nodes
void Worker::DoNode(JSONNode * parent, const json_string & value_t){
    /*
	   This take a node and creates its members and such
    */
    libJSON_ASSERT(!value_t.empty(), TEXT("DoNode is empty"));
    libJSON_ASSERT_SAFE(value_t[0] == TEXT('{'), TEXT("DoNode is not an node"), parent -> Nullify(); return;);
    const size_t len = value_t.length();
    if (len <= 2) return;  // just a {} (blank node)

    size_t name_starting = 1;  //ignore the {
    size_t name_ending = FindNextRelevant(TEXT(':'), value_t, 1);  //find where the name ends
    libJSON_ASSERT_SAFE(name_ending != json_string::npos, TEXT("Missing :"), parent -> Nullify(); return;);
    json_string name = value_t.substr(name_starting + 1, name_ending - 3);	  //pull the name out
    size_t value_ending = FindNextRelevant(TEXT(','), value_t, name_ending);  //find the end of the value
    while (value_ending != json_string::npos){
	   NewNode(parent, name, value_t.substr(name_ending + 1, value_ending - name_ending - 1));
	   name_starting = value_ending + 1;
	   name_ending = FindNextRelevant(TEXT(':'), value_t, name_starting);
	   libJSON_ASSERT_SAFE(name_ending != json_string::npos, TEXT("Missing :"), parent -> Nullify(); return;);
	   name = value_t.substr(name_starting + 1, name_ending - name_starting - 2);
	   value_ending = FindNextRelevant(TEXT(','), value_t, name_ending);
    }
    //since the last one will not find the comma, we have to add it here
    NewNode(parent, name, value_t.substr(name_ending + 1, len - name_ending - 2));
    parent -> Children.shrink();
}
