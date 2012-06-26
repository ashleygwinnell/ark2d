# Building libcurl for multiple architectures..
	
---

### Build x86_64
	./configure 
	make
	cp lib/.libs/libcurl.a libcurl-x86_64.a

### Build i386
	./configure CFLAGS="-arch i386";make clean;make
	cp lib/.libs/libcurl.a libcurl-i386.a

### Build ARM6
	./configure --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2  CFLAGS="-arch armv6 -pipe -mdynamic-no-pic -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=3.2 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/usr/include/libxml2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk" CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/cpp AR=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar LDFLAGS="-arch armv6 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk -miphoneos-version-min=3.2"
	make clean;make
	cp lib/.libs/libcurl.a libcurl-arm6.a

### Build ARM7
	./configure --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2  CFLAGS="-arch armv7 -pipe -mdynamic-no-pic -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=3.2 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/usr/include/libxml2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk" CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/cpp AR=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar LDFLAGS="-arch armv7 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk -miphoneos-version-min=3.2"
	make clean;make
	cp lib/.libs/libcurl.a libcurl-arm7.a

### Build Universal
	lipo -create -output libcurl.a libcurl-i386.a libcurl-x86_64.a libcurl-arm6.a libcurl-arm7.a

### Check Architecture
	lipo -info libcurl.a


