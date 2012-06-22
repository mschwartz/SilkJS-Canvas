# Change this to be the name of your module
MODULE=Canvas

UNAME := $(shell uname -s)

ifeq ($(UNAME),Darwin)
    MAKEFILE=Makefile.osx
else
    MAKEFILE=Makefile
endif

all:
	cd src && make -f$(MAKEFILE)
	cp src/cairo_module.so lib/

clean:
	rm -f src/*.so src/*.o lib/*.so

realclean:
	cd src && make -f$(MAKEFILE) realclean

install:
	mkdir -p /usr/local/silkjs/contrib/$(MODULE)
	cp -rp index.js lib /usr/local/silkjs/contrib/$(MODULE)

uninstall:
	cd src && make -f$(MAKEFILE) uninstall
