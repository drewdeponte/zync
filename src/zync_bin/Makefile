#
# Copyright 2004, 2005 Andrew De Ponte.
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

Z_SYNCER_BIN = zync
Z_SYNCER_OBJ = zync.o
Z_SYNCER_SRC = zync.cc

ZAURUSTYPE_OBJ = ZaurusType.o
ZAURUSTYPE_SRC = ZaurusType.cc

CARDPARAMINFO_OBJ = CardParamInfoType.o
CARDPARAMINFO_SRC = CardParamInfoType.cc

Z_SYNCER_OBJS = $(Z_SYNCER_OBJ) $(CARDPARAMINFO_OBJ) $(ZAURUSTYPE_OBJ)

# This is the flag and named used to link to the zmsg library.
ZSYNCER_LIB_FLAG = -L../zmsg_lib -L../zdata_lib -L../confmgr_lib -lzmsg -lzdata -lconfmgr -ldl -lpthread
ZSYNCER_LIB_INC = -I../zmsg_lib -I../ -I../confmgr_lib

# Remove command
RM = rm -rf

# The compiler and its options.
COMPILER = g++
# The compile only (do not link) flag.
COMPILE_FLAG = -c
# Enable debug code within compile.
DEBUG_FLAG = -g
# The warnings control flag.
WARNING_FLAG = -Wall
# The flag used to specify the file name to use for output.
OUTPUT_FLAG = -o

#####################################################################
# No user configuration should occur below this line.
#####################################################################

all : $(Z_SYNCER_BIN)

# Here we link the z_sync_server object file into an executable.
$(Z_SYNCER_BIN) : $(Z_SYNCER_OBJS)
	$(COMPILER) $(OUTPUT_FLAG) $(Z_SYNCER_BIN) $(Z_SYNCER_OBJS) $(ZSYNCER_LIB_FLAG)

# Here we create the z_sync_server object file.
$(Z_SYNCER_OBJ) : $(Z_SYNCER_SRC)
	$(COMPILER) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(ZSYNCER_LIB_INC) $(Z_SYNCER_SRC)

$(ZAURUSTYPE_OBJ) : $(ZAURUSTYPE_SRC)
	$(COMPILER) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(ZSYNCER_LIB_INC) $(ZAURUSTYPE_SRC)

$(CARDPARAMINFO_OBJ) : $(CARDPARAMINFO_SRC)
	$(COMPILER) $(WARNING_FLAG) $(DEBUG_FLAG) $(COMPILE_FLAG) $(CARDPARAMINFO_SRC)


install :
	install $(Z_SYNCER_BIN) /usr/local/bin/
	mkdir -p /usr/local/lib/zync/plugins/todo
	mkdir -p /usr/local/lib/zync/plugins/calendar
	mkdir -p /usr/local/lib/zync/plugins/address
	mkdir -p /usr/local/include/zync
	cp TodoPluginType.hh /usr/local/include/zync/
	cp AddrBookPluginType.hh /usr/local/include/zync/
	cp CalendarPluginType.hh /usr/local/include/zync/

# Here we get rid of the files that we created.
clean :
	$(RM) $(Z_SYNCER_OBJS) $(Z_SYNCER_BIN)
