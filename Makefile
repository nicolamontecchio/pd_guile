all : pdexternal minitest

clean :
	rm -f *.o sampleplayer~.pd_darwin

pdexternal :
	cc -O3 `pkg-config guile-2.0 --libs` `pkg-config guile-2.0 --cflags` -shared -undefined dynamic_lookup -o guile.pd_darwin guilepdext.c

minitest :
	cc -g `pkg-config guile-2.0 --libs` `pkg-config guile-2.0 --cflags` -o test minitest.c
