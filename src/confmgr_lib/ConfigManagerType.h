/*
 * Copyright 2004 Andrew De Ponte
 * 
 * This file is part of zsrep.
 * 
 * zsrep is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * zsrep is distributed in the hopes that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with zsrep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 * Copyright 2004 Andrew De Ponte.
 *
 * This file is part of oam_player.
 *
 * oam_player is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * oam_player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with oam_player; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
*  \file ConfigManagerType.h
 * \brief A specifications file for the ConfigManager type.
 * \author Andrew De Ponte
 *
 * This type is developed to be a nice interface to handle general config
 * files. It contains capabilities to read in config files of a specific
 * format as well as write config files of this format. It also contains an
 * interface to allow a client program to access the any data read in from the
 * config.
 */

#ifndef CONFIGMANAGERTYPE_H
#define CONFIGMANAGERTYPE_H

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "DebugControl.h"

// Here I define a type for the Items which are found in such configs. This
// is specifically the type that allows for the construction of a double
// linked list.
struct ConfigItemType {
    char value[256];
    char title[256];
    struct ConfigItemType *pNextConfigItem;
    struct ConfigItemType *pPrevConfigItem;
};

/**
 * \class ConfigManagerType
 * \brief A type representing an generic configuration file.
 *
 * The ConfigManagerType is a type that represents an generic configuration
 * file. It provides all the common actions that would be able to be taken on
 * a configuration file. Such actions include reading, writing, and
 * editing. It supports accessing and setting existing or non existing data.
 */
class ConfigManagerType {
 public:
    ConfigManagerType(void);
    ~ConfigManagerType(void);

    int Open(char *pConfigPath);
    int Save(char *pSavePath);
    int SetValue(char *pTitle, char *pValue);
    int GetValue(char *pTitle, char *pValue, int maxlen);
    int DeleteItem(char *pTitle);

 private:
    int AddItem(char *pTitle, char *pValue);
    struct ConfigItemType *GetFirstItem(void);
    struct ConfigItemType *GetLastItem(void);

    // Points to the first PlaylistItem of the playlist.
    struct ConfigItemType *pFirstConfigItem;

    // Points to the last PlaylistItem of the playlist.
    struct ConfigItemType *pLastConfigItem;
};

#endif
