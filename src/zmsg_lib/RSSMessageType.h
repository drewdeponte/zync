/*
 * Copyright 2005 Andrew De Ponte
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

/**
 * @file RSSMessageType.h
 * @brief A specifications file of a message of type RSS.
 * @author Andrew De Ponte
 *
 * A specifications for a class to represent a message that is of type RSS.
 */

#ifndef RSSMESSAGETYPE_H
#define RSSMESSAGETYPE_H

#include "DesktopMessageType.h"

/**
 * @class RSSMessageType
 * @brief A type representing messages of type RSS.
 *
 * The RSSMessageType is a class which represents messages of type RSS. It
 * handles all access to data contained in such messages.
 */
class RSSMessageType : public DesktopMessageType {
 public:
    RSSMessageType(void);
    RSSMessageType(const unsigned char type);
    ~RSSMessageType(void);

    void SetType(const unsigned char type);
 private:
};

#endif
