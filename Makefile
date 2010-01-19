LIB-BIN		= $(DESTDIR)/usr/lib
APT-CONF	= $(DESTDIR)/etc/apt/apt.conf.d

all:

clean:

install:
	install -d $(LIB-BIN) $(APT-CONF)
	install apt-howdy $(LIB-BIN)
	install -m644 apt-howdy-hook $(APT-CONF)/20apt-howdy

uninstall:
	-rm -f $(APT-CONF)/20apt-howdy
	-rm -f $(LIB-BIN)/apt-howdy

