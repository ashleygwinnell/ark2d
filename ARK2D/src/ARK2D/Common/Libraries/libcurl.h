#if (defined(ARK2D_WINDOWS) || defined(ARK2D_WINDOWS_VS) || defined(ARK2D_WINDOWS_STORE))
	#include "../../vendor/libcurl-windows/curl.h"
#else
	#include "../../vendor/libcurl/curl.h"
#endif
