# Building libfreetype for iOS.
	
_** http://librocket.com/wiki/documentation/BuildingFreeTypeForiOS **_

---

### Build i386
	./configure CFLAGS="-arch i386"
	make
	cp objs/.libs/libfreetype.a libfreetype-i386.a

### Build x86_64
	./configure CFLAGS="-arch x86_64";make clean;make
	cp objs/.libs/libfreetype.a libfreetype-x86_64.a

### Build ARM6
	./configure --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2  CFLAGS="-arch armv6 -pipe -mdynamic-no-pic -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=3.2 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/usr/include/libxml2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk" CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/cpp AR=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar LDFLAGS="-arch armv6 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk -miphoneos-version-min=3.2"
	make clean;make
	cp objs/.libs/libfreetype.a libfreetype-arm6.a

### Build ARM7
	./configure --prefix=/usr/local/iphone --host=arm-apple-darwin --enable-static=yes --enable-shared=no CC=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2  CFLAGS="-arch armv7 -pipe -mdynamic-no-pic -std=c99 -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=3.2 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/usr/include/libxml2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk" CPP=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/cpp AR=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/ar LDFLAGS="-arch armv7 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk -miphoneos-version-min=3.2"
	make clean;make
	cp objs/.libs/libfreetype.a libfreetype-arm7.a

### Build Universal
	lipo -create -output libfreetype.a libfreetype-i386.a libfreetype-x86_64 libfreetype-arm6.a libfreetype-arm7.a

### Check Architecture
	lipo -info libfreetype.a


