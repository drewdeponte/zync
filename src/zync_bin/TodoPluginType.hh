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
 * @file TodoPluginType.hh
 * @brief A specifications file for a base class for TodoPlugins.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to provide a base class used as
 * a template for Todo plugins for the ZaurusSync application.
 */

#ifndef TODOPLUGINTYPE_H
#define TODOPLUGINTYPE_H

#include <time.h>
#include <string>
#include <zdata_lib/TodoItemType.hh>

/**
 * @class TodoPluginType
 * @brief A type existing as a template for TodoPlugins.
 *
 * The TodoPluginType is a class which exists to be the base class of actual
 * Todo plugins. One might say it is a rigid template which is required to be
 * followed when creating plugins. It specifies all the required member
 * functions.
 */
class TodoPluginType {
public:
    /**
     * Destruct the TodoPluginType object.
     *
     * Desctuct the TodoPluginType object by freeing any dynamically allocated
     * memory.
     */
    virtual ~TodoPluginType(void) { };

    /**
     * Initialize the TodoPluginType object.
     *
     * Initialize the TodoPluginType object in any way needed before the
     * normal synchronization based member functions start getting called.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int Initialize(void) = 0;

    /**
     * Clean up the TodoPluginType object.
     *
     * Clean up the TodoPluginType object in any way needed after the normal
     * synchronization based member functions have been called.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int CleanUp(void) = 0;

    /**
     * Get all Todo Items.
     *
     * Obtan all the Todo items that exist inside the object which this plugin
     * represents.
     * @return A list of all the Todo Items.
     */
    virtual TodoItemType::List GetAllTodoItems(void) = 0;

    /**
     * Get the new Todo Items.
     *
     * Obtain the Todo items that were either created after the last
     * synchronization the last synchronization.
     * @param lastTimeSynced The last time synchronized represented as number
     * of seconds since epoch.
     * @return A list of the new Todo Items.
     */
    virtual TodoItemType::List GetNewTodoItems(time_t lastTimeSynced) = 0;

    /**
     * Get the modified Todo Items.
     *
     * Obtain the Todo items that were modified after the last
     * synchronization.
     * @param lastTimeSynced The last time synchronized represented as number
     * of seconds since epoch.
     * @return A list of modified Todo Items.
     */
    virtual TodoItemType::List GetModTodoItems(time_t lastTimeSynced) = 0;

    /**
     * Get the deleted Todo Items.
     *
     * Obtain the IDs of the Todo items that were deleted after the last
     * synchronizaiton.
     * @param lastTimeSynced The last time synchronized represented as number
     * of seconds since epoch.
     * @return A list of sync IDs of deleted Todo Items.
     */
    virtual SyncIDListType GetDelTodoItemIDs(time_t lastTimeSynced) = 0;

    /**
     * Add the Todo Items.
     *
     * Add the Todo items within the passed list to the component represented
     * by the plugin.
     * @param todoItems A list of Todo Items to add to the plugin component.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int AddTodoItems(TodoItemType::List todoItems) = 0;

    /**
     * Modify the Todo Items.
     *
     * Modify the Todo items within the passed list to the component
     * represented by the plugin.
     * @param todoItems A list of todo Items to delete from the plugin
     * component.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int ModTodoItems(TodoItemType::List todoItems) = 0;

    /**
     * Delete the Todo Items.
     *
     * Delete the Todo items that have the sync IDs contained in the passed
     * list from the component represented by the plugin.
     * @param syncIDList A list of sync IDs of the items to delete from the
     * plugin component.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int DelTodoItems(SyncIDListType syncIDList) = 0;

    /**
     * Map the Item IDs
     *
     * Map the Item IDs between the Zaurus and the Desktop PIM application.
     * @param todoItems A list of items which need their IDs mapped.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int MapItemIDs(TodoItemType::List todoItems) = 0;

    /**
     * Get plugin description.
     *
     * Get the description of the plugin.
     * @return The plugin's description.
     */
    virtual std::string GetPluginDescription(void) const = 0;

    /**
     * Get plugin name.
     *
     * Get the name of the plugin.
     * @return The plugin's name.
     */
    virtual std::string GetPluginName(void) const = 0;

    /**
     * Get the plugin author.
     *
     * Get the author of the plugin.
     * @return The plugin's author.
     */
    virtual std::string GetPluginAuthor(void) const = 0;

    /**
     * Get the plugin version.
     *
     * Get the version of the plugin.
     * @return The plugin's version.
     */
    virtual std::string GetPluginVersion(void) const = 0;
};

typedef TodoPluginType* (*create_todo_t)();
typedef void (*destroy_todo_t)(TodoPluginType*);

/**
 * Create TodoPluginType instance.
 *
 * Creates an instance of the TodoPluginType class. This function exists so
 * that the C++ class object can be obtained from the library
 * dynamically. This only has to exist because of name mangeling.
 * @return A pointer to the instance of the Todo plugin.
 */
extern "C" TodoPluginType *createTodoPlugin(void);

/**
 * Destroy TodoPluginType instance.
 *
 * Destroys the instance pointed to by the pointer passed over. This function
 * exists so the C++ class object can be destroyed after loading it from the
 * library dynamically. This only has to exst because of name mangeling.
 * @param pPlugin A pointer to an object created with the create function.
 */ 
extern "C" void destroyTodoPlugin(TodoPluginType *pTodoPlugin);

#endif
