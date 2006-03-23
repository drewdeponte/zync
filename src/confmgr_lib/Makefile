#
# Copyright 2003, 2004 Andrew De Ponte.
#
# This file is part of zsrep.
#
# zsrep is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# any later version.
#
# zsrep is distributed in the hopes that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Generel Public License for details.
#
# You should have received a copy of the GNU General Public License
# along with zsrep; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA, 02111-1307 USA
#

CONFMGR_OBJ = ConfigManagerType.o
CONFMGR_SRC = ConfigManagerType.cc

# The directory where the library should be installed. Note: This needs to
# have a trailing / for it to be a fully qualified path and work properly.
DEST_LIB_DIRECTORY = /usr/local/lib/

# This is the libraries interface number, this only changes if the
# interface changes.
LIBCONFMGR_INTERFACE_NUM = 0
# This is the libraries output file name.
LIBCONFMGR_OUT_FILENAME = libconfmgr.so
# This is the name of the symlink to the file which should be created by
# ldconfig.
LIBCONFMGR_SONAME = $(LIBCONFMGR_OUT_FILENAME).$(LIBCONFMGR_INTERFACE_NUM)
# This is the libraries minor number
LIBCONFMGR_MIN_NUM = 0
# This is the libaries release number
LIBCONFMGR_REL_NUM = 0
# This is the actual name of the file
LIBCONFMGR_REALNAME = $(LIBCONFMGR_SONAME).$(LIBCONFMGR_MIN_NUM).$(LIBCONFMGR_REL_NUM)
# A series of all the object files used to create the ZMSG library.
LIBCONFMGR_OBJS = $(CONFMGR_OBJ)

# Remove command
RM = rm -rf

# The compiler and its options.
COMPILER = g++
# The compile only (do not link) flag.
COMPILE_FLAG = -c
# Position Independent Code Flag.
PIC_FLAG = -fPIC
# The warnings control flag.
WARNING_FLAG = -Wall
# The flag used to specify the file name to use for output.
OUTPUT_FLAG = -o
# The flag used to specify the SONAME of a library when creating
# a shared library.
SONAME_FLAG = -shared -Wl,-soname,

#####################################################################
# No user configuration should occur below this line.
#####################################################################

all : $(LIBCONFMGR_REALNAME)

# Create the shared library.
$(LIBCONFMGR_REALNAME) : $(LIBCONFMGR_OBJS)
	$(COMPILER) $(SONAME_FLAG)$(LIBCONFMGR_SONAME) $(OUTPUT_FLAG) $(LIBCONFMGR_REALNAME) $(LIBCONFMGR_OBJS)
	ln -sf $(LIBCONFMGR_REALNAME) $(LIBCONFMGR_SONAME)
	ln -sf $(LIBCONFMGR_REALNAME) $(LIBCONFMGR_OUT_FILENAME)

# Here we create the object file.
$(CONFMGR_OBJ) : $(CONFMGR_SRC)
	$(COMPILER) $(PIC_FLAG) $(WARNING_FLAG) $(COMPILE_FLAG) $(CONFMGR_SRC)

# Here we install the shared library into the proper directory.
install : $(LIBCONFMGR_REALNAME)
	cp $(LIBCONFMGR_REALNAME) $(DEST_LIB_DIRECTORY)
	chown root:root $(DEST_LIB_DIRECTORY)$(LIBCONFMGR_REALNAME)
	chmod 755 $(DEST_LIB_DIRECTORY)$(LIBCONFMGR_REALNAME)
	ln -sf $(DEST_LIB_DIRECTORY)$(LIBCONFMGR_REALNAME) $(DEST_LIB_DIRECTORY)$(LIBCONFMGR_SONAME)
	ln -sf $(DEST_LIB_DIRECTORY)$(LIBCONFMGR_REALNAME) $(DEST_LIB_DIRECTORY)$(LIBCONFMGR_OUT_FILENAME)
	mkdir -p /usr/local/include/confmgr
	cp ConfigManagerType.h /usr/local/include/confmgr/
	cp DebugControl.h /usr/local/include/confmgr/
	/sbin/ldconfig

# Here we get rid of the files that we created.
clean :
	$(RM) $(LIBCONFMGR_REALNAME) $(LIBCONFMGR_OBJS) $(LIBCONFMGR_OUT_FILENAME) $(LIBCONFMGR_SONAME)
