LIB-BIN		= $(DESTDIR)/usr/lib

all: apt-howdy-check

apt-howdy-check: apt-howdy-check.c
	gcc apt-howdy-check.c -o apt-howdy-check `pkg-config --cflags --libs alarm`

clean:
	-rm apt-howdy-check
install:
	install -d $(LIB-BIN)
	install apt-howdy $(LIB-BIN)
	install apt-howdy-check $(LIB-BIN)

uninstall:
	-rm -f $(LIB-BIN)/apt-howdy
	-rm -f $(LIB-BIN)/apt-howdy-check

