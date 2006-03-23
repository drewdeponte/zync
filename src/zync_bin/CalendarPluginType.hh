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
 * @file CalendarPluginType.hh
 * @brief A specifications file for a base class for Calendar Plugins.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to provide a base class used as
 * a template for Calendar plugins for zync.
 */

#ifndef CALENDARPLUGINTYPE_H
#define CALENDARPLUGINTYPE_H

#include <time.h>
#include <string>
#include <zdata_lib/CalendarItemType.hh>

/**
 * @class CalendarPluginType
 * @brief A type existing as a template for Calendar Plugins.
 *
 * The CalendarPluginType is a class which exists to be the base class of
 * actual Calendar plugins. One might say it is a rigid template which is
 * required to be followed when creating plugins. It specifies all the
 * required member functions.
 */
class CalendarPluginType {
public:
    /**
     * Destruct the CalendarPluginType object.
     *
     * Desctuct the CalendarPluginType object by freeing any dynamically
     * allocated memory.
     */
    virtual ~CalendarPluginType(void) { };

    /**
     * Initialize the CalendarPluginType object.
     *
     * Initialize the CalendarPluginType object in any way needed before the
     * normal synchronization based member functions start getting called.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int Initialize(void) = 0;

    /**
     * Clean up the CalendarPluginType object.
     *
     * Clean up the CalendarPluginType object in any way needed after the
     * normal synchronization based member functions have been called.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int CleanUp(void) = 0;

    /**
     * Get all Calendar Items.
     *
     * Obtain all the Calendar items that exist inside the object which
     * this plugin represents.
     * @return A list of all the Calendar Items.
     */
    virtual CalendarItemType::List GetAllCalendarItems(void) = 0;

    /**
     * Get the new Calendar Items.
     *
     * Obtain the Calendar items that were either created after the last
     * synchronization.
     * @param lastTimeSynced The last time synchronized represented as number
     * of seconds since epoch.
     * @return A list of the new Calendar Items.
     */
    virtual CalendarItemType::List GetNewCalendarItems(time_t lastTimeSynced) = 0;

    /**
     * Get the modified Calendar Items.
     *
     * Obtain the Calendar items that were modified after the last
     * synchronization.
     * @param lastTimeSynced The last time synchronized represented as number
     * of seconds since epoch.
     * @return A list of modified Calendar Items.
     */
    virtual CalendarItemType::List GetModCalendarItems(time_t lastTimeSynced) = 0;

    /**
     * Get the deleted Calendar Items.
     *
     * Obtain the IDs of the Calendar items that were deleted after the
     * last synchronizaiton.
     * @param lastTimeSynced The last time synchronized represented as number
     * of seconds since epoch.
     * @return A list of sync IDs of deleted Calendar Items.
     */
    virtual SyncIDListType GetDelCalendarItemIDs(time_t lastTimeSynced) = 0;

    /**
     * Add the Calendar Items.
     *
     * Add the Calendar items within the passed list to the component
     * represented by the plugin.
     * @param calendarItems A list of Calendar Items to add to the plugin
     * component.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int AddCalendarItems(CalendarItemType::List calendarItems) = 0;

    /**
     * Modify the Calendar Items.
     *
     * Modify the Calendar items within the passed list to the component
     * represented by the plugin.
     * @param calendarItems A list of Calendar Items to delete from the
     * plugin component.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int ModCalendarItems(CalendarItemType::List calendarItems) = 0;

    /**
     * Delete the Calendar Items.
     *
     * Delete the Calendar items that have the sync IDs contained in the
     * passed list from the component represented by the plugin.
     * @param syncIDList A list of sync IDs of the items to delete from the
     * plugin component.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int DelCalendarItems(SyncIDListType syncIDList) = 0;

    /**
     * Map the Item IDs
     *
     * Map the Item IDs between the Zaurus and the Desktop PIM application.
     * @param calendarItems A list of items which need their IDs mapped.
     * @return An integer representing success (zero) or failure (non-zero).
     */
    virtual int MapItemIDs(CalendarItemType::List calendarItems) = 0;

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

typedef CalendarPluginType* (*create_cal_t)();
typedef void (*destroy_cal_t)(CalendarPluginType*);

/**
 * Create CalendarPluginType instance.
 *
 * Creates an instance of the CalendarPluginType class. This function exists so
 * that the C++ class object can be obtained from the library
 * dynamically. This only has to exist because of name mangeling.
 * @return A pointer to the instance of the Calendar plugin.
 */
extern "C" CalendarPluginType *createCalendarPlugin(void);

/**
 * Destroy CalendarPluginType instance.
 *
 * Destroys the instance pointed to by the pointer passed over. This function
 * exists so the C++ class object can be destroyed after loading it from the
 * library dynamically. This only has to exist because of name mangeling.
 * @param pPlugin A pointer to an object created with the create function.
 */ 
extern "C" void destroyCalendarPlugin(CalendarPluginType *pCalendarPlugin);

#endif
