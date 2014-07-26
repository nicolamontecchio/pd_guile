all : pdexternal

clean :
	rm -f *.o sampleplayer~.pd_darwin

pdexternal :
	clang -O3 -I/usr/local/include/guile/2.0 -shared -undefined dynamic_lookup -o guile.pd_darwin guilepdext.c


# probably useful for later:
# > pkg-config guile-2.0 --libs
# -L/usr/local/Cellar/guile/2.0.11/lib -L/usr/local/Cellar/bdw-gc/7.2e/lib -lguile-2.0 -lgc
