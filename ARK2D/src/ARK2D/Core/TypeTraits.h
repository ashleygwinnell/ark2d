#ifndef ARK_CORE_TYPETRAITS_H_
#define ARK_CORE_TYPETRAITS_H_


#include "GameObject.h"
#include "Math/MathUtil.h"
#include "../Namespaces.h"

#ifdef ARK2D_IPHONE
#include <type_traits>
#endif

using namespace std;

#if !defined(ARK2D_WINDOWS_VS) && !defined(ARK2D_EMSCRIPTEN_JS) && !defined(ARK2D_IPHONE)

	template <typename T>
	struct remove_const
	{
	    typedef T type;
	};

	#if !(__cplusplus > 199711L)

	    template <typename T>
	    struct remove_const<const T>
	    {
	        typedef T type;
	    };

	    template <typename T>
	    struct remove_volatile
	    {
	        typedef T type;
	    };

	    template <typename T>
	    struct remove_volatile<volatile T>
	    {
	        typedef T type;
	    };

	    template <typename T>
	    struct remove_cv : remove_const<typename remove_volatile<T>::type> {};


	    template<typename T>
	    struct remove_pointer
	    {
	        typedef T type;
	    };

	    template<typename T>
	    struct remove_pointer<T*>
	    {
	        typedef T type;
	    };

	#endif

	//
	//struct this_sort_function
	//{
	//		template<typename T>

	//};

#endif

template <typename T>
struct is_unqualified_pointer
{
    enum { value = false };
};

template <typename T>
struct is_unqualified_pointer<T*>
{
    enum { value = true };
};

template <typename T>
struct is_pointer_type : is_unqualified_pointer<typename remove_cv<T>::type> {};

template <typename T>
bool is_pointer(const T&)
{
    return is_pointer_type<T>::value;
}



#endif

