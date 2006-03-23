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
 * @file ZaurusType.hh
 * @brief A specifications file for an object representing a Zaurus.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to represent a Zaurus. This
 * data type was created to provide a common interface that would obfuscate
 * the Zaurus Synchronization Protocol Implementation.
 */

#ifndef ZAURUSTYPE_H
#define ZAURUSTYPE_H

// Include the common data format for Todos from the zdata library.
#include <zdata_lib/TodoItemType.hh>
#include <zdata_lib/CalendarItemType.hh>
#include <zdata_lib/AddrBookItemType.hh>

// Memory Comparison, Settings, etc. Includes
#include <string>
#include <iostream>

// Network Related Includes
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <zmsg.h>

#include "CardParamInfoType.hh"

// The zaurus syncing softwares receiving port.
#define ZRECVPORT 4245

// Define all the different item parameter type identifiers
#define DATA_ID_BIT 0x06
#define DATA_ID_TIME 0x04
#define DATA_ID_ULONG 0x12
#define DATA_ID_BARRAY 0x0c
#define DATA_ID_UTF8 0x11
#define DATA_ID_UCHAR 0x0b
#define DATA_ID_WORD 0x08

// Define all the different synchronization types
#define SYNC_TODO 0x06
#define SYNC_CALENDAR 0x01
#define SYNC_ADDRESSBOOK 0x07

/**
 * @class ZaurusType
 * @brief A type representing a Zaurus.
 *
 * The ZaurusType is a class which represents a Zaurus. It is designed to
 * provide a common interface that obfuscates the Zaurus Synchronization
 * Protocol Implementation. It does not completely obfuscate the protocol due
 * to the fact that order of function calls matters.
 */
class ZaurusType {
public:
    ZaurusType(void);
    ~ZaurusType(void);

    int ListenConnection(void);
    void SetSyncType(const unsigned char type);
    bool RequiresPassword(void);
    int AuthenticatePassword(std::string passwd);
    time_t GetLastTimeSynced(void);

    int GetAllTodoSyncItems(TodoItemType::List &newItemList,
        TodoItemType::List &modItemList, SyncIDListType &delItemIdList);
    int GetAllCalendarSyncItems(CalendarItemType::List &newItemList,
        CalendarItemType::List &modItemList, SyncIDListType &delItemIdList);
    TodoItemType::List AddTodoItems(TodoItemType::List todoItems);
    CalendarItemType::List AddCalendarItems(CalendarItemType::List calItems);
    int ModTodoItems(TodoItemType::List todoItems);
    int ModCalendarItems(CalendarItemType::List calItems);
    int DelTodoItems(SyncIDListType todoItemIDs);
    int DelCalendarItems(SyncIDListType calItemIDs);

    int RequiresFullSync(void) const;
    void TerminateSync(void);
    int FinishSync(void);
    int ResetSyncLog(void);
    int SendRSSMsg(void);
    int SetNextSyncAnch(void);
    int ObtainParamInfo(void);
private:
    int InitiateSync(void);
    int ObtainDeviceInfo(void);
    int ObtainSyncLog(const unsigned char syncType);
    int ObtainLastSyncAnch(void);
    int ObtainSyncIDLists(const unsigned char type);

    TodoItemType GetTodoItem(unsigned char type, unsigned long int syncID);
    CalendarItemType GetCalendarItem(unsigned char type,
        unsigned long int syncID);
    int ModifyTodoItem(unsigned char type, TodoItemType todoItem);
    int ModifyCalendarItem(unsigned char type, CalendarItemType calItem);
    TodoItemType AddTodoItem(unsigned char type, TodoItemType todoItem);
    CalendarItemType AddCalendarItem(unsigned char type,
        CalendarItemType calItem);
    int DeleteItem(unsigned char type, unsigned long int syncID);
    int StateSyncDone(const unsigned char type);

    int GetTodoItemParam(const TodoItemType &item,
        RDWMessageType *const pRDWMsg,
        const CardParamInfoType &paramInfo);
    int SetTodoItemParam(TodoItemType &item,
        ADRMessageType *const pADRMsg, const CardParamInfoType &paramInfo);
    int GetCalendarItemParam(const CalendarItemType &item,
        RDWMessageType *const pRDWMsg, const CardParamInfoType &paramInfo);
    int SetCalendarItemParam(CalendarItemType &item,
        ADRMessageType *const pADRMsg, const CardParamInfoType &paramInfo);

    void PrintCardParams(void);

    // These variables are used to store the Device Information.
    std::string model;
    std::string language;
    unsigned char authState;

    SyncIDListType newSyncIDList;
    SyncIDListType modSyncIDList;
    SyncIDListType delSyncIDList;
    bool obtainedSyncIDLists;

    // This variable is used to store the last time synced anchor after it has
    // been obtained from the Zaurus.
    time_t lastTimeSynced;

    // This is a variable used to keep track of the socket/file descriptor for
    // the connection to the Zaurus.
    int connfd;

    // This variable is used to store the type of synchronization. This
    // variable should be set before any of the protocol functions are called.
    unsigned char syncType;

    // The following three variables are used to flag if full syncs are
    // needed for each of the pim applications.
    int reqTodoFullSync;
    int reqCalendarFullSync;
    int reqAddressBookFullSync;

    CardParamInfoType::List paramInfoList;
};

#endif
