all : pdexternal

clean :
	rm -f *.o sampleplayer~.pd_darwin

pdexternal :
	clang -O3 -arch i386 -arch x86_64 -shared -undefined dynamic_lookup -o guile.pd_darwin guilepdext.c
