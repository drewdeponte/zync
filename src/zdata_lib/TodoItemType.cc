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
 * @file TodoItemType.cc
 * @brief An implimentation file for the TodoItemType class.
 *
 * This type was developed to provide an object to represent a Todo item. It
 * is designed to be used by the ZaurusSync application as well as its plugins
 * so there is a common Todo object.
 */

#include "TodoItemType.hh"

/**
 * Construct a default TodoItemType object.
 *
 * Construct a default TodoItemType object with all the basic initialization.
 */
TodoItemType::TodoItemType(void) {

}

/**
 * Destruct the TodoItemType object.
 *
 * Destruct the TodoItemType object by deallocating any dynamically allocated
 * memory. 
 */
TodoItemType::~TodoItemType(void) {

}

/**
 * Set the category.
 *
 * Set the category of the item.
 * @param value The value to set the category to.
 */
void TodoItemType::SetCategory(std::string value) {
    category = value;
}

/**
 * Get the category.
 *
 * Get the category of the item this object represents.
 * @return A string containing the category.
 */
std::string TodoItemType::GetCategory(void) const {
    return category;
}

/**
 * Set the Start Date.
 *
 * Set the date that this item should be/was started. If a start does not
 * exist the value should be set to zero.
 * @param epochSecs Date, as number of seconds since Epoch, (Jan 1, 1970).
 */
void TodoItemType::SetStartDate(time_t epochSecs) {
    startDate = epochSecs;
}

/**
 * Get the Start Date.
 *
 * Obtain the date that the item this object represents should be/was started.
 * @return Date Started, an number of seconds since Epoch.
 */
time_t TodoItemType::GetStartDate(void) const {
    return startDate;
}

/**
 * Set the Due Date.
 *
 * Set the date that this item is/was due. If a due date does not exist the
 * value should be set to zero.
 * @param epochSecs Date, as number of seconds since Epoch, (Jan 1, 1970).
 */
void TodoItemType::SetDueDate(time_t epochSecs) {
    dueDate = epochSecs;
}

/**
 * Get the Due Date.
 *
 * Obtain the date that the item this object represents is/was due.
 * @return Date Due, an number of seconds since Epoch.
 */
time_t TodoItemType::GetDueDate(void) const {
    return dueDate;
}

/**
 * Set the Completed Date.
 *
 * Set the date that this item was completed. If a completed date does not
 * exist the value should be set to zero.
 * @param epochSecs Date, as number of seconds since Epoch, (Jan 1, 1970).
 */
void TodoItemType::SetCompletedDate(time_t epochSecs) {
    completedDate = epochSecs;
}

/**
 * Get the Completed Date.
 *
 * Obtain the date that the item this object represents was completed.
 * @return Date Completed, an number of seconds since Epoch.
 */
time_t TodoItemType::GetCompletedDate(void) const {
    return completedDate;
}

/**
 * Set the Progress Status.
 *
 * Set the progress status of the item this object represents.
 * @param value Value to set status to, completed (zero), incomplete (one).
 */
void TodoItemType::SetProgressStatus(unsigned char value) {
    if (value) {
	progressStatus = 1;
    } else {
	progressStatus = 0;
    }
}

/**
 * Get the Progress Status.
 *
 * Obtain the progress status of the item this object represents.
 * @return The progress status as a value, completed (zero), incomplete (one).
 * @retval 0 The Todo item has been marked as being completed.
 * @retval 1 The Todo item has NOT been marked as being completed.
 */
unsigned char TodoItemType::GetProgressStatus(void) const {
    return progressStatus;
}

/**
 * Set the Priority.
 *
 * Set the priority of the item this object represents. Note: There is no
 * bounds checking so if the priority is set beyond the described bounds the
 * effects are unknown.
 * @param value Value to set priority to, very high (one) - very low (five).
 */
void TodoItemType::SetPriority(unsigned char value) {
    priority = value;
}

/**
 * Get the Priority.
 *
 * Obtain the priority of the item this object represents.
 * @return The priority as a value, very high (one) - very low (five).
 * @retval 1 The priority is Very High.
 * @retval 2 The priority is High.
 * @retval 3 The priority is Normal.
 * @retval 4 The priority is Low.
 * @retval 5 The priority is Very Low.
 */
unsigned char TodoItemType::GetPriority(void) const {
    return priority;
}

/**
 * Set the description.
 *
 * Set the description of the item.
 * @param value The value to set the description to.
 */
void TodoItemType::SetDescription(std::string value) {
    description = value;
}

/**
 * Get the description.
 *
 * Get the description of the item this object represents.
 * @return A string containing the description.
 */
std::string TodoItemType::GetDescription(void) const {
    return description;
}

/**
 * Set the notes.
 *
 * Set the notes of the item.
 * @param value The value to set the notes to.
 */
void TodoItemType::SetNotes(std::string value) {
    notes = value;
}

/**
 * Get the notes.
 *
 * Get the notes of the item this object represents.
 * @return A string containing the notes.
 */
std::string TodoItemType::GetNotes(void) const {
    return notes;
}
