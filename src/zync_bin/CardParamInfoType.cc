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
 * @file CardParamInfoType.cc
 * @brief A implementation file for an object to hold info for a card param.
 *
 * An implementation file for a class existing to provide an object to store
 * information about a card parameter.
 */

#include "CardParamInfoType.hh"

/**
 * Construct a defualt CardParamInfoType object.
 *
 * Construct a default, with nothing set, CardParamInfoType object.
 */
CardParamInfoType::CardParamInfoType(void) {

}

/**
 * Construct a CardParamInfoType object.
 *
 * Construct a CardParamInfoType object while specifying the abreviation,
 * description and type identifier.
 * @param abr The abreviation of the card parameter.
 * @param des The description of the card parameter.
 * @param typID The type ID of the card parameter.
 */
CardParamInfoType::CardParamInfoType(std::string abr, std::string des,
				     unsigned char typID) {
    SetAbrev(abr);
    SetDesc(des);
    SetTypeID(typID);
}

/**
 * Set the Abreviation.
 *
 * Set the Abreviation of the CardParamInfoType object.
 * @param value The value to set the abreviation to.
 */
void CardParamInfoType::SetAbrev(std::string value) {
    abrev = value;
}

/**
 * Get the Abreviation.
 *
 * Get the Abreviation of the CardParamInfoType object.
 * @return The abreviation of the CardParamInfoType object.
 */
std::string CardParamInfoType::GetAbrev() const {
    return abrev;
}

/**
 * Set the Description.
 *
 * Set the Description of the CardParamInfoType object.
 * @param value The value to set the description to.
 */
void CardParamInfoType::SetDesc(std::string value) {
    desc = value;
}

/**
 * Get the Description.
 *
 * Get the Description of the CardParamInfoType object.
 * @return The description of the CardParamInfoType object.
 */
std::string CardParamInfoType::GetDesc() const {
    return desc;
}

/**
 * Set the Type ID.
 *
 * Set the Type ID of the CardParamInfoType object.
 * @param value The value to set the type ID to.
 */
void CardParamInfoType::SetTypeID(unsigned char value) {
    typeID = value;
}

/**
 * Get the Type ID.
 *
 * Get the Type ID of the CardParamInfoType object.
 * @return The type ID of the CardParamInfoType object.
 */
unsigned char CardParamInfoType::GetTypeID() const {
    return typeID;
}
