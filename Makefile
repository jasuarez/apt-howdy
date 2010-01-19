BIN		= $(DESTDIR)/usr/bin
APT-CONF	= $(DESTDIR)/etc/apt/apt.conf.d

install:
	install -d $(BIN) $(APT-CONF)
	install apt-howdy $(BIN)
	install -m644 apt-howdy-hook $(APT-CONF)/20apt-howdy
