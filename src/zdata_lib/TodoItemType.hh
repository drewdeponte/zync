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
 * @file TodoItemType.hh
 * @brief A specifications file for an object representing a Todo item.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to represent a Todo item. This
 * data type was created to provide a common data type between the plugins and
 * the ZaurusSyncer application.
 */

#ifndef TODOITEMTYPE_H
#define TODOITEMTYPE_H

#include "ItemType.hh"

#include <time.h>

#include <string>
#include <list>

/**
 * @class TodoItemType
 * @brief A type representing a Todo item.
 *
 * The TodoItemType is a class which represents a Todo item. It is designed to
 * be used between the ZaurusSync application and its plugins so that there is
 * a common object for Todo items.
 */
class TodoItemType : public ItemType {
 public:
    // Define a type that is a list of TodoItemTypes.
    typedef std::list<TodoItemType> List;

    TodoItemType(void);
    ~TodoItemType(void);

    // Access functions for the below data members.
    void SetCategory(std::string value);
    std::string GetCategory(void) const;

    void SetStartDate(time_t epochSecs);
    time_t GetStartDate(void) const;

    void SetDueDate(time_t epochSecs);
    time_t GetDueDate(void) const;

    void SetCompletedDate(time_t epochSecs);
    time_t GetCompletedDate(void) const;

    void SetProgressStatus(unsigned char value);
    unsigned char GetProgressStatus(void) const;

    void SetPriority(unsigned char value);
    unsigned char GetPriority(void) const;

    void SetDescription(std::string value);
    std::string GetDescription(void) const;

    void SetNotes(std::string value);
    std::string GetNotes(void) const;

    // Non Access Functions.

 private:
    std::string category;
    time_t startDate;
    time_t dueDate;
    time_t completedDate;
    unsigned char progressStatus;
    unsigned char priority;
    std::string description;
    std::string notes;
};

#endif
