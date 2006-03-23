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

ZDATA_OBJ = zdata.o
ZDATA_SRC = zdata.cc
ITEMTYPE_SRC = ItemType.cc
ITEMTYPE_OBJ = ItemType.o
TODOITEMTYPE_SRC = TodoItemType.cc
TODOITEMTYPE_OBJ = TodoItemType.o
ADDRBOOKITEMTYPE_OBJ = AddrBookItemType.o
ADDRBOOKITEMTYPE_SRC = AddrBookItemType.cc
CALENDARITEMTYPE_OBJ = CalendarItemType.o
CALENDARITEMTYPE_SRC = CalendarItemType.cc

# The directory where the library should be installed. Note: This needs to
# have a trailing / for it to be a fully qualified path and work properly.
DEST_LIB_DIRECTORY = /usr/local/lib/

# This is the libraries interface number, this only changes if the
# interface changes.
LIBZDATA_INTERFACE_NUM = 0
# This is the libraries output file name.
LIBZDATA_OUT_FILENAME = libzdata.so
# This is the name of the symlink to the file which should be created by
# ldconfig.
LIBZDATA_SONAME = $(LIBZDATA_OUT_FILENAME).$(LIBZDATA_INTERFACE_NUM)
# This is the libraries minor number
LIBZDATA_MIN_NUM = 0
# This is the libaries release number
LIBZDATA_REL_NUM = 0
# This is the actual name of the file
LIBZDATA_REALNAME = $(LIBZDATA_SONAME).$(LIBZDATA_MIN_NUM).$(LIBZDATA_REL_NUM)
# A series of all the object files used to create the ZMSG library.
LIBZDATA_OBJS = $(ZDATA_OBJ) $(ITEMTYPE_OBJ) $(TODOITEMTYPE_OBJ) $(ADDRBOOKITEMTYPE_OBJ) $(CALENDARITEMTYPE_OBJ)

# Remove command
RM = rm -rf

# The compiler and its options.
COMPILER = g++
# The compile only (do not link) flag.
COMPILE_FLAG = -c
# Position Independent Code Flag.
PIC_FLAG = -fPIC
# Enable debug code within compile.
DEBUG_FLAG = -g
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

all : $(LIBZDATA_REALNAME)

# Create the shared library.
$(LIBZDATA_REALNAME) : $(LIBZDATA_OBJS)
	$(COMPILER) $(DEBUG_FLAG) $(SONAME_FLAG)$(LIBZDATA_SONAME) $(OUTPUT_FLAG) $(LIBZDATA_REALNAME) $(LIBZDATA_OBJS)
	ln -sf $(LIBZDATA_REALNAME) $(LIBZDATA_SONAME)
	ln -sf $(LIBZDATA_REALNAME) $(LIBZDATA_OUT_FILENAME)

# Here we create the zdata shared object file.
$(ZDATA_OBJ) : $(ZDATA_SRC)
	$(COMPILER) $(PIC_FLAG) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(ZDATA_SRC)

$(ITEMTYPE_OBJ) : $(ITEMTYPE_SRC)
	$(COMPILER) $(PIC_FLAG) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(ITEMTYPE_SRC)

$(TODOITEMTYPE_OBJ) : $(TODOITEMTYPE_SRC)
	$(COMPILER) $(PIC_FLAG) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(TODOITEMTYPE_SRC)

$(ADDRBOOKITEMTYPE_OBJ) : $(ADDRBOOKITEMTYPE_SRC)
	$(COMPILER) $(PIC_FLAG) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(ADDRBOOKITEMTYPE_SRC)

$(CALENDARITEMTYPE_OBJ) : $(CALENDARITEMTYPE_SRC)
	$(COMPILER) $(PIC_FLAG) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(CALENDARITEMTYPE_SRC)


# Here we install the shared library into the proper directory.
install : $(LIBZDATA_REALNAME)
	cp $(LIBZDATA_REALNAME) $(DEST_LIB_DIRECTORY)
	chown root:root $(DEST_LIB_DIRECTORY)$(LIBZDATA_REALNAME)
	chmod 755 $(DEST_LIB_DIRECTORY)$(LIBZDATA_REALNAME)
	ln -sf $(DEST_LIB_DIRECTORY)$(LIBZDATA_REALNAME) $(DEST_LIB_DIRECTORY)$(LIBZDATA_SONAME)
	ln -sf $(DEST_LIB_DIRECTORY)$(LIBZDATA_REALNAME) $(DEST_LIB_DIRECTORY)$(LIBZDATA_OUT_FILENAME)
	mkdir -p /usr/local/include/zdata_lib
	cp ItemType.hh /usr/local/include/zdata_lib/
	cp TodoItemType.hh /usr/local/include/zdata_lib/
	cp AddrBookItemType.hh /usr/local/include/zdata_lib/
	cp CalendarItemType.hh /usr/local/include/zdata_lib/
	cp zdata.hh /usr/local/include/zdata_lib/
	/sbin/ldconfig

# Here we get rid of the files that we created.
clean :
	$(RM) $(LIBZDATA_REALNAME) $(LIBZDATA_OBJS) $(LIBZDATA_OUT_FILENAME) $(LIBZDATA_SONAME)
