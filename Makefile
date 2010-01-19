BIN		= $(DESTDIR)/usr/bin
APT-CONF	= $(DESTDIR)/etc/apt/apt.conf.d

all:

install:
	install -d $(BIN) $(APT-CONF)
	install apt-howdy $(BIN)
	install -m644 apt-howdy-hook $(APT-CONF)/20apt-howdy

uninstall:
	-rm -f $(APT-CONF)/20apt-howdy
	-rm -f $(BIN)/apt-howdy

