libhac.so: libhac.o
	$(CC) -shared libhac.o -o libhac.so

clean:
	rm *.o libhac.so

install: libhac.so
	cp libhac.so /usr/lib/
	cp libhac.h /usr/include/libhac/
