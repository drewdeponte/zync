/*
 * Copyright 2004, 2005 Andrew De Ponte
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
 * @file CardParamInfoType.hh
 * @brief A specifications file for an object to hold info for a card param.
 *
 * A specifications file for a class existing to provide an object to store
 * information about a card parameter.
 */

#ifndef CARDPARAMINFOTYPE_H
#define CARDPARAMINFOTYPE_H

#include <string>
#include <vector>

/**
 * @class CardParamInfoType
 * @brief A class to designed to encapsulate information used to a card param.
 *
 * This class is a class which was designed specifically to encapsulate
 * information used to describe a card parameter. This class was created so
 * that a list of such objects could be created, in turn defining a format for
 * the types of cards being transfering during what ever type of
 * synchronization is occurring.
 */
class CardParamInfoType {
public:
    // Define a type that is a lost of CardParamInfoTypes.
    typedef std::vector<CardParamInfoType> List;

    CardParamInfoType(void);
    CardParamInfoType(std::string abr, std::string des, unsigned char typID);

    void SetAbrev(std::string value);
    std::string GetAbrev() const;
    void SetDesc(std::string value);
    std::string GetDesc() const;
    void SetTypeID(unsigned char value);
    unsigned char GetTypeID() const;

private:
    std::string abrev;
    std::string desc;
    unsigned char typeID;
};

#endif
