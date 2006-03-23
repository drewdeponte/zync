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
 * @file CalendarItemType.hh
 * @brief A specifications file for an object representing a Calendar item.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to represent a Calendar
 * item. This data type was created to provide a common data type between the
 * plugins and the ZaurusSyncer application.
 */

#ifndef CALENDARITEMTYPE_H
#define CALENDARITEMTYPE_H

#include "ItemType.hh"

#include <time.h>

#include <string>
#include <list>

/**
 * @class CalendarItemType
 * @brief A type representing a Calendar item.
 *
 * The CalendarItemType is a class which represents a Calendar item. It is
 * designed to be used between the ZaurusSync application and its plugins so
 * that there is a common object for Calendar items.
 */
class CalendarItemType : public ItemType {
 public:
    // Define a type that is a list of CalendarItemTypes.
    typedef std::list<CalendarItemType> List;

    CalendarItemType(void);
    ~CalendarItemType(void);

    // Access functions for the below data members.
    void SetCategory(std::string value);
    std::string GetCategory(void) const;

    void SetDescription(std::string value);
    std::string GetDescription(void) const;

    void SetLocation(std::string value);
    std::string GetLocation(void) const;

    void SetNotes(std::string value);
    std::string GetNotes(void) const;

    void SetStartTime(time_t epochSecs);
    time_t GetStartTime(void) const;

    void SetEndTime(time_t epochSecs);
    time_t GetEndTime(void) const;

    void SetScheduleType(unsigned char value);
    unsigned char GetScheduleType(void) const;

    void SetAlarm(unsigned char value);
    unsigned char GetAlarm(void) const;

    void SetAlarmSetting(unsigned char value);
    unsigned char GetAlarmSetting(void) const;

    void SetAlarmTime(unsigned short int mins);
    unsigned short int GetAlarmTime(void) const;

    void SetRepeatType(unsigned char value);
    unsigned char GetRepeatType(void) const;

    void SetRepeatPeriod(unsigned short int value);
    unsigned short int GetRepeatPeriod(void) const;

    void SetRepeatPosition(unsigned short int value);
    unsigned short int GetRepeatPosition(void) const;

    void SetRepeatDate(unsigned char value);
    unsigned char GetRepeatDate(void) const;

    void SetRepeatEndDateSetting(unsigned char value);
    unsigned char GetRepeatEndDateSetting(void) const;

    void SetRepeatEndDate(time_t epochSecs);
    time_t GetRepeatEndDate(void) const;

    void SetAllDayStartDate(time_t epochSecs);
    time_t GetAllDayStartDate(void) const;

    void SetAllDayEndDate(time_t epochSecs);
    time_t GetAllDayEndDate(void) const;

    void SetMultipleDaysFlag(unsigned char value);
    unsigned char GetMultipleDaysFlag(void) const;

    // Non Access Functions.

 private:
    std::string category;
    std::string description;
    std::string location;
    std::string notes;
    time_t startTime;
    time_t endTime;
    unsigned char scheduleType;
    unsigned char alarm;
    unsigned char alarmSetting;
    unsigned short int alarmTime;
    unsigned char repeatType;
    unsigned short int repeatPeriod;
    unsigned short int repeatPosition;
    unsigned char repeatDate;
    unsigned char repeatEndDateSetting;
    time_t repeatEndDate;
    time_t allDayStartDate;
    time_t allDayEndDate;
    unsigned char multipleDaysFlag;
};

#endif
