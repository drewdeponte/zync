SUBDIRS = zmsg_lib zdata_lib confmgr_lib zync_bin

all install clean:
	for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir $@ ; done