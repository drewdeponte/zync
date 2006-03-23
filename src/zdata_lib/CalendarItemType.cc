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
 * @file CalendarItemType.cc
 * @brief An implimentation file for the CalendarItemType class.
 *
 * This type was developed to provide an object to represent a Calendar
 * item. It is designed to be used by the ZaurusSync application as well as
 * its plugins so there is a common Calendar item object.
 */

#include "CalendarItemType.hh"

/**
 * Construct a default CalendarItemType object.
 *
 * Construct a default CalendarItemType object with all the basic
 * initialization.
 */
CalendarItemType::CalendarItemType(void) {

}

/**
 * Destruct the CalendarItemType object.
 *
 * Destruct the CalendarItemType object by deallocating any dynamically
 * allocated memory. 
 */
CalendarItemType::~CalendarItemType(void) {

}

/**
 * Set the category.
 *
 * Set the category of the item.
 * @param value The value to set the category to.
 */
void CalendarItemType::SetCategory(std::string value) {
    category = value;
}

/**
 * Get the category.
 *
 * Get the category of the item this object represents.
 * @return A string containing the category.
 */
std::string CalendarItemType::GetCategory(void) const {
    return category;
}

/**
 * Set the description.
 *
 * Set the description of the item.
 * @param value The value to set the description to.
 */
void CalendarItemType::SetDescription(std::string value) {
    description = value;
}

/**
 * Get the description.
 *
 * Get the description of the item this object represents.
 * @return A string containing the description.
 */
std::string CalendarItemType::GetDescription(void) const {
    return description;
}

/**
 * Set the location.
 *
 * Set the location of the item.
 * @param value The value to set the location to.
 */
void CalendarItemType::SetLocation(std::string value) {
    location = value;
}

/**
 * Get the location.
 *
 * Get the location of the item this object represents.
 * @return A string containing the location.
 */
std::string CalendarItemType::GetLocation(void) const {
    return location;
}

/**
 * Set the notes.
 *
 * Set the notes of the item.
 * @param value The value to set the notes to.
 */
void CalendarItemType::SetNotes(std::string value) {
    notes = value;
}

/**
 * Get the notes.
 *
 * Get the notes of the item this object represents.
 * @return A string containing the notes.
 */
std::string CalendarItemType::GetNotes(void) const {
    return notes;
}

/**
 * Set the Start Time.
 *
 * Set the date and time that this item should be/was started.
 * @param epochSecs Time, as number of seconds since Epoch, (Jan 1, 1970).
 */
void CalendarItemType::SetStartTime(time_t epochSecs) {
    startTime = epochSecs;
}

/**
 * Get the Start Time.
 *
 * Obtain the date and time that the item this object represents should be/was
 * started.
 * @return Date and Time Started, as number of seconds since Epoch.
 */
time_t CalendarItemType::GetStartTime(void) const {
    return startTime;
}

/**
 * Set the End Time.
 *
 * Set the date and time that this item is/was ended.
 * @param epochSecs Time, as number of seconds since Epoch, (Jan 1, 1970).
 */
void CalendarItemType::SetEndTime(time_t epochSecs) {
    endTime = epochSecs;
}

/**
 * Get the End Time.
 *
 * Obtain the date that the item this object represents is/was ended.
 * @return Date and Time Ended, an number of seconds since Epoch.
 */
time_t CalendarItemType::GetEndTime(void) const {
    return endTime;
}

/**
 * Set the Schedule Type.
 *
 * Set the Schedule Type of the item which this object represents. Acceptable
 * values for the schedule type are 0 (Normal) and 1 (All Day).
 * @param value The value to set the schedule type to, either 0 or 1.
 */
void CalendarItemType::SetScheduleType(unsigned char value) {
    scheduleType = value;
}

/**
 * Get the Schedule Type.
 *
 * Obtain the schedule type of the item which this object represents.
 * @return A value representing a specific schedule type.
 * @retval 0 Normal schedule type.
 * @retval 1 All Day schedule type.
 */
unsigned char CalendarItemType::GetScheduleType(void) const {
    return scheduleType;
}

/**
 * Set the Alarm state.
 *
 * Set the Alarm state of the item which this object represents. Acceptable
 * values for the alarm state are 0 (On) and 1 (Off).
 * @param value The value to set the alarm to, either 0 or 1.
 */
void CalendarItemType::SetAlarm(unsigned char value) {
    alarm = value;
}

/**
 * Get the Alarm state.
 *
 * Obtain the Alarm state of the item which this object represents.
 * @return A value representing a specific alarm state.
 * @retval 0 Alarm is On.
 * @retval 1 Alarm is Off.
 */
unsigned char CalendarItemType::GetAlarm(void) const {
    return alarm;
}

/**
 * Set the Alarm Setting.
 *
 * Set the Alarm Setting of the item which this object represents. Acceptable
 * values for the alarm setting are 0 (Silent) and 1 (Load).
 * @param value The value to set the alarm setting to, either 0 or 1.
 */
void CalendarItemType::SetAlarmSetting(unsigned char value) {
    alarmSetting = value;
}

/**
 * Get the Alarm Setting.
 *
 * Obtain the Alarm setting of the item which this object represents.
 * @return A value representing a specific alarm setting.
 * @retval 0 Alarm is Silent.
 * @retval 1 Alarm is Load.
 */
unsigned char CalendarItemType::GetAlarmSetting(void) const {
    return alarmSetting;
}

/**
 * Set the Alarm Time.
 *
 * Set the Alarm Time of the item which this object represents.
 * @param value The value to set the alarm time to in minutes.
 */
void CalendarItemType::SetAlarmTime(unsigned short int value) {
    alarmTime = value;
}

/**
 * Get the Alarm Time.
 *
 * Obtain the Alarm Time of the item which this object represents.
 * @return A value representing the alarm time in minutes.
 */
unsigned short int CalendarItemType::GetAlarmTime(void) const {
    return alarmTime;
}

/**
 * Set the Repeat Type.
 *
 * Set the Repeat Type of the item which this object represents. Acceptable
 * repeat type values are 0 (Daily), 1 (Weekly), 2 (Monthly Day), 3 (Monthly
 * Date), 4 (Yearly), 0xff (Non-Repeat).
 * @param value The value to set the repeat type to.
 */
void CalendarItemType::SetRepeatType(unsigned char value) {
    repeatType = value;
}

/**
 * Get the Repeat Type.
 *
 * Obtain the Repeat Type of the item which this object represents.
 * @return A value representing a the object's repeat type.
 * @retval 0 Repeat Daily.
 * @retval 1 Repeat Weekly.
 * @retval 2 Repeat Day Every Month.
 * @retval 3 Repeat Date Every Month.
 * @retval 4 Repeat Yearly.
 * @retval 255 Non-Repeating.
 */
unsigned char CalendarItemType::GetRepeatType(void) const {
    return repeatType;
}

/**
 * Set the Repeat Period.
 *
 * Set the Repeat Period of the item this object represents.
 * @param value The value to set the repeat period to.
 */
void CalendarItemType::SetRepeatPeriod(unsigned short int value) {
    repeatPeriod = value;
}

/**
 * Get the Repeat Period.
 *
 * Obtain the Repeat Period of the item this object represents.
 * @return The repeat period of the item.
 */
unsigned short int CalendarItemType::GetRepeatPeriod(void) const {
    return repeatPeriod;
}

/**
 * Set the Repeat Position.
 *
 * Set the Repeat Position of the item this object represents.
 * @param value The value to set the repeat position to.
 */
void CalendarItemType::SetRepeatPosition(unsigned short int value) {
    repeatPosition = value;
}

/**
 * Get the Repeat Position.
 *
 * Obtain the Repeat Position of the item this object represents.
 * @return The repeat position of the item.
 */
unsigned short int CalendarItemType::GetRepeatPosition(void) const {
    return repeatPosition;
}

/**
 * Set the Repeat Date.
 *
 * Set the Repeat Date of the item this object represents. Set the value by
 * bitwise anding the acceptable values. Acceptable values are 0x01 (Monday),
 * 0x02 (Tuesday), 0x04 (Wednesday), 0x08 (Thursday), 0x10 (Friday), 0x20
 * (Saturday), 0x30 (Sunday) and any combination of this bitwise anded
 * together.
 * @param value The value to set the repeat date to.
 */
void CalendarItemType::SetRepeatDate(unsigned char value) {
    repeatDate = value;
}

/**
 * Get the Repeat Date.
 *
 * Obtain the Repeat Date of the item this object represents. The value can
 * represent a single day or a number of days. Multiple days are represented
 * by bitwise anding the values which represent the single days.
 * @return A value representing a single or combination of days (bitwise anded
 * values).
 * @retval 0x01 Monday.
 * @retval 0x02 Tuesday.
 * @retval 0x04 Wednesday.
 * @retval 0x08 Thursday.
 * @retval 0x10 Friday.
 * @retval 0x20 Saturday.
 * @retval 0x30 Sunday.
 */
unsigned char CalendarItemType::GetRepeatDate(void) const {
    return repeatDate;
}

/**
 * Set the Repeat End Date Setting.
 *
 * Set the Repeat End Date Setting of the item which this object
 * represents. Acceptable values are 0 (Not Set), and 1 (Set).
 * @param value The value to set the repeat end date setting to.
 */
void CalendarItemType::SetRepeatEndDateSetting(unsigned char value) {
    repeatEndDateSetting = value;
}

/**
 * Get the Repeat End Date Setting.
 *
 * Obtain the Repeat End Date Setting of the item which this object
 * represents.
 * @return A value representing a the object's repeat end date setting.
 * @retval 0 Not Set.
 * @retval 1 Set.
 */
unsigned char CalendarItemType::GetRepeatEndDateSetting(void) const {
    return repeatEndDateSetting;
}

/**
 * Set the Repeat End Date.
 *
 * Set the Repeat End Date and time of this item.
 * @param epochSecs Repeat End Date, as number of seconds since Epoch, (Jan 1,
 * 1970).
 */
void CalendarItemType::SetRepeatEndDate(time_t epochSecs) {
    repeatEndDate = epochSecs;
}

/**
 * Get the Repeat End Date.
 *
 * Obtain the repeat end date and time of the item this object represents.
 * @return Repeat End Date and Time, as number of seconds since Epoch.
 */
time_t CalendarItemType::GetRepeatEndDate(void) const {
    return repeatEndDate;
}

/**
 * Set the All Day Start Date.
 *
 * Set the All Day Start Date and time of this item.
 * @param epochSecs All Day Start Date, as number of seconds since Epoch, (Jan
 * 1, 1970).
 */
void CalendarItemType::SetAllDayStartDate(time_t epochSecs) {
    allDayStartDate = epochSecs;
}

/**
 * Get the All Day Start Date.
 *
 * Obtain the All Day Start Date and time of the item this object represents.
 * @return All Day Start Date, as number of seconds since Epoch.
 */
time_t CalendarItemType::GetAllDayStartDate(void) const {
    return allDayStartDate;
}

/**
 * Set the All Day End Date.
 *
 * Set the All Day End Date and time of this item.
 * @param epochSecs All Day End Date, as number of seconds since Epoch, (Jan
 * 1, 1970).
 */
void CalendarItemType::SetAllDayEndDate(time_t epochSecs) {
    allDayEndDate = epochSecs;
}

/**
 * Get the All Day End Date.
 *
 * Obtain the All Day End Date and time of the item this object represents.
 * @return All Day End Date, as number of seconds since Epoch.
 */
time_t CalendarItemType::GetAllDayEndDate(void) const {
    return allDayEndDate;
}

/**
 * Set the Multiple Days Flag.
 *
 * Set the Multiple Days Flag of the item which this object
 * represents. Acceptable values are 0 (Same Day), and 1 (Multiple Days).
 * @param value The value to set the Multiple Days Flag to.
 */
void CalendarItemType::SetMultipleDaysFlag(unsigned char value) {
    multipleDaysFlag = value;
}

/**
 * Get the Multiple Days Flag.
 *
 * Obtain the Multiple Days Flag of the item which this object represents.
 * @return A value representing a the object's multiple days flag.
 * @retval 0 Same Day.
 * @retval 1 Multiple Days.
 */
unsigned char CalendarItemType::GetMultipleDaysFlag(void) const {
    return multipleDaysFlag;
}
