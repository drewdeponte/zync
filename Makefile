SUBDIRS = src man conf

all clean:
	for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir $@ ; done

install:
	for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir $@ ; done

	mkdir -p /usr/local/share/zync
	cp COPYING /usr/local/share/zync/
	cp INSTALL /usr/local/share/zync/
	cp AUTHORS /usr/local/share/zync/
	cp README /usr/local/share/zync/
	cp ChangeLog /usr/local/share/zync/