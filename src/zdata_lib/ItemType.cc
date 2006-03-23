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
 * @file ItemType.cc
 * @brief An implimentation file for the ItemType class.
 *
 * This type was developed to provide an object to represent a generic
 * item. It is designed to be used by the ZaurusSync application as well as
 * its plugins so there is a common generic object. It should be used as a
 * base class for other types of items.
 */

#include "ItemType.hh"

/**
 * Construct a default ItemType object.
 *
 * Construct a default ItemType object with all the basic initialization.
 */
ItemType::ItemType(void) {
    // Initialize the sync id to zero (an invalid value). Count starts at 1
    // NOT zero.
    syncId = 0;

    // Set the time variables to zero.
    createdTime = 0;
    modifiedTime = 0;

    // Set the attribute to NULL.
    attribute = '\0';
}

/**
 * Destruct the ItemType object.
 *
 * Destruct the ItemType object by deallocating any dynamically allocated
 * memory. 
 */
ItemType::~ItemType(void) {

}

/**
 * Set the attribute.
 *
 * Set the attribute of the item.
 * @param value The value to set the attribute to.
 */
void ItemType::SetAttribute(unsigned char value) {
    attribute = value;
}

/**
 * Get the attribute.
 *
 * Get the attribute of the item this object represents.
 * @return The value of the attribute of this object.
 */
unsigned char ItemType::GetAttribute(void) const {
    return attribute;
}

/**
 * Set the created time.
 *
 * Set the time that the item this object represents was created.
 * @param epochSecs Number of seconds since Epoch, (00:00:00 UTC, Jan 1, 1970)
 */
void ItemType::SetCreatedTime(time_t epochSecs) {
    createdTime = epochSecs;
}

/**
 * Get the created time.
 *
 * Obtain the time that the item this object represents was created.
 * @return Time Created, as Number of seconds since Epoch.
 */
time_t ItemType::GetCreatedTime(void) const {
    return modifiedTime;
}

/**
 * Set the modified time.
 *
 * Set the time that the item this object represents was last modified.
 * @param epochSecs Number of seconds since Epoch, (00:00:00 UTC, Jan 1, 1970)
 */
void ItemType::SetModifiedTime(time_t epochSecs) {
    modifiedTime = epochSecs;
}

/**
 * Get the modified time.
 *
 * Obtain the time that the item this object represents was last modified.
 * @return Time Last Modified, as Number of seconds since Epoch.
 */
time_t ItemType::GetModifiedTime(void) const {
    return modifiedTime;
}

/**
 * Set the Sync ID.
 *
 * Set the items synchronization ID.
 * @param value The value to set the synchronization ID to.
 */
void ItemType::SetSyncID(unsigned long int value) {
    syncId = value;
}

/**
 * Get the Sync ID.
 *
 * Obtain the items synchronization ID.
 * @return The value of the items synchronization ID.
 */
unsigned long int ItemType::GetSyncID(void) const {
    return syncId;
}

/**
 * Set the App ID.
 *
 * Set the items unique ID that was given to it by the Desktop PIM
 * application.
 * @param value The value to set the app ID to.
 */
void ItemType::SetAppID(std::string value) {
    appId = value;
}

/**
 * Get the App ID.
 *
 * Obtain the items ID that was given to it by the Desktop PIM application.
 * @return The value of the items app ID.
 */
std::string ItemType::GetAppID(void) const {
    return appId;
}
