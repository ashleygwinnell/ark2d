#if defined (ARK2D_WINDOWS_PHONE_8)
//	typedef jmp_buf* (*png_set_longjmp_fnPtr)(png_structp png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size);
//	png_set_longjmp_fnPtr mypng_set_longjmp_fnPtr = 0;
	#include "../../vendor/lpng1251-wp8/png.h"
#else
	#include "../../vendor/lpng151/png.h"
#endif
