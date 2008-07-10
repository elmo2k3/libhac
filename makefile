libhac.so: libhac.o
	$(CC) -shared libhac.o -o libhac.so

clean:
	rm *.o libhac.so
