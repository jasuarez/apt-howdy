LIB-BIN		= $(DESTDIR)/usr/libexec

all:

clean:

install:
	install -d $(LIB-BIN)
	install apt-howdy $(LIB-BIN)
	install apt-howdy-check $(LIB-BIN)

uninstall:
	-rm -f $(LIB-BIN)/apt-howdy
	-rm -f $(LIB-BIN)/apt-howdy-check

