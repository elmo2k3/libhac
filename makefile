#CC = /usr/bin/i586-mingw32msvc-gcc
#LDFLAGS = -lws2_32
CFLAGS=-fPIC

libhac.so: libhac.o
	$(CC) -shared -fPIC libhac.o -o libhac.so $(LDFLAGS)

clean:
	rm *.o libhac.so

install: libhac.so
	cp libhac.so /usr/lib/
	cp libhac.h /usr/include/libhac/
