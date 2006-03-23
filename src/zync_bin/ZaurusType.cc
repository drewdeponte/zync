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
 * @file ZaurusType.cc
 * @brief An implimentation file for the ZaurusType class.
 *
 * This type was developed to provide an object to represent a Zaurus. It is
 * designed to provide a common interface that obfuscates the Zaurus
 * Synchronization Protocol Implementation. 
 */

#include "ZaurusType.hh"

/**
 * Construct a default Zaurus object.
 *
 * Construct a default Zaurus object with all the basic initialization.
 */
ZaurusType::ZaurusType(void) {
    // Initialize the obtainedSyncIDLists flag to a state of false.
    obtainedSyncIDLists = false;

    // Initialize the requires full sync flag to a state of full sync not
    // required.
    reqTodoFullSync = 0;
    reqCalendarFullSync = 0;
    reqAddressBookFullSync = 0;

    // Set the synchronization type to To-Do for default since it is only one
    // that is implimented at this point.
    syncType = 0x06;
}

/**
 * Destruct the ZaurusType object.
 *
 * Destruct the ZaurusType object by deallocating any dynamically allocated
 * memory.
 */
ZaurusType::~ZaurusType(void) {

}

/**
 * Listen for incoming synchronization connections.
 *
 * Create a new socket, bind it for listening for connections for
 * synchronization (probably from a Zaurus), the wait for a connection.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully accepted a connection.
 * @retval 1 Failed to create socket.
 * @retval 2 Failed to bind the socket to address.
 * @retval 3 Failed to put socket into listen mode.
 * @retval 4 Failed to accept connection.
 * @retval 5 Failed to convert connecting socket address to a string.
 */
int ZaurusType::ListenConnection(void) {
    int listenfd;
    int reuse_set_flag = 1;
    struct sockaddr_in servaddr;
    struct sockaddr_in clntaddr;
    char source_addr[16];
    socklen_t len;
    int retval;

    // Attepmt to create a socket to be used to allow the Synchronization
    // Client running on the Zaurus to connect to the Desktop Synchronization
    // Server (this object).
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
	perror("ListenConnection");
	return 1;
    }

    // Here, I set a socket option so that if the application crashes the
    // socket is not blocked by the TIME_WAIT state.
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse_set_flag,
	       sizeof(reuse_set_flag));

    // Specify the addresses that I am going to allow to listen for
    // connections on. In this case I want to allow connections from any IP
    // address attempting to connect to port 4245 (ZRECVPORT) because I do not
    // want to limit the users configurations for synchoronization.
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(ZRECVPORT);

    // Since I have defined the address (or rather acceptance rules) for the
    // listening socket, I bind the address to the socket.
    retval = bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (retval == -1) {
        perror("ListenConnection");
        return 2;
    }

    // Now that the socket is prepared to listen for connections I attempt to
    // put the socket into listening mode. I only want one connection in the
    // queue at a time.
    retval = listen(listenfd, 1);
    if (retval == -1) {
        perror("ListenConnection");
        return 3;
    }

    // Here, I block until a connection is made to the listening socket.
    memset(&clntaddr, 0, sizeof(clntaddr));
    len = sizeof(clntaddr);
    connfd = accept(listenfd, (struct sockaddr *) &clntaddr, &len);
    if (connfd == -1) {
        perror("ListenConnection");
        return 4;
    }

    // Here, I print a message showing the socket that just connected.
    if (inet_ntop(AF_INET, &clntaddr.sin_addr, source_addr, 16) == NULL) {
        perror("ListenConnection");
        return 5;
    }
//    printf("Received connection from %s, port %d.\n", source_addr,
//	   ntohs(clntaddr.sin_port));

    // Return in success.
    return 0;
}

/**
 * Set the type of synchronization.
 *
 * Set the type of synchronization that will be performed. This should be
 * called before any of the protocol functions are called.
 * @param type This is the type of synchronization that will be
 * performed. Acceptable values and their meanings are To-Do: 0x06, Calendar:
 * 0x01, and Address Book: 0x07.
 */
void ZaurusType::SetSyncType(const unsigned char type) {
    syncType = type;
}

/**
 * Determine if a password is required.
 *
 * Determine if the Zaurus requires a password for synchronization.
 * @return A boolean value representing true (yes) or false (no).
 * @retval true The Zaurus does require a password for synchronization.
 * @retval false The Zaurus does NOT require a password for synchronization.
 */
bool ZaurusType::RequiresPassword(void) {
    int retval;

    // The first thing that happends in either a "Normal Sync" or a "Full
    // Sync" is that an RAY message is sent and an AAY message is
    // received. This is just used to start the synchronization process.
    if ((retval = InitiateSync()) != 0) {
	std::cerr << "Failed to Initiate the Sync process (" << retval
		  << ").\n";
	exit(1);
    }
    
    // The second thing that happends in either a "Normal Sync" or a "Full
    // Sync" is that an RIG message is sent and an AIG message is
    // received. This is a request and answer for device information (Model,
    // Language, Authentication State).
    if ((retval = ObtainDeviceInfo()) != 0) {
	std::cerr << "Failed to Obtain Device Info (" << retval << ").\n";
	exit(1);
    }
    

    // Check the value of the Authentication State received with the call to
    // the ObtainDeviceInfo function to see if a password is required.
    if ((authState == 0x0b) || (authState == 0x07))
	return true;
    else
	return false;
}

/**
 * Authenticate the password.
 *
 * Authenticate the given password with the Zaurus.
 * @param passwd The password to send to the Zaurus for authentication.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully authenticated to the Zaurus.
 * @retval 1 Failed to authenticate the password.
 */
int ZaurusType::AuthenticatePassword(std::string passwd) {
    int retval;
    MessageType msg;
    bool authenticated = false;
    int retryCount = 0;

    while ((!authenticated) && (retryCount < 3)) {

	if (RecvRqst(connfd) != 0) {
	    printf("ERROR: Failed to receive a req.\n");
	    exit(12);
	}

	SendRRL(connfd, (char *)passwd.c_str());
	if (RecvAck(connfd) != 0) {
	    printf("ERROR: Failed to receiving an ack for RRL.\n");
	    exit(12);
	}

	SendRqst(connfd);

	// Receive message, success AEX, failure 96 18
	retval = RecvMessage(connfd, &msg);
	if (retval != 0) {
	    if (retval == 8) {
		SendRqst(connfd);

		// Here I recv the ANG message.
		RecvMessage(connfd, &msg);
		SendAck(connfd);

		retryCount++;
	    }
	} else {
	    SendAck(connfd);
	    authenticated = true;
	}
    }

    if (authenticated) {
	return 0;
    } else {
	FinishSync();
	return 1;
    }
}

/**
 * Get the Last Time Synchronized.
 *
 * Obtain the last time synchronized from the Zaurus.
 * @return The last time synchronized as seconds since Epoch.
 */
time_t ZaurusType::GetLastTimeSynced(void) {
    int retval;

    if ((retval = ObtainDeviceInfo()) != 0) {
	std::cerr << "Failed to Obtain Device Info (" << retval << ").\n";
	exit(1);
    }

    if ((retval = ObtainSyncLog(syncType)) != 0) {
	std::cerr << "Failed to Obtain Sync Log (" << retval << ").\n";
	exit(1);
    }

    if ((retval = ObtainLastSyncAnch()) != 0) {
	std::cerr << "Failed to Obtain Last Sync Anchor (" << retval << ").\n";
	exit(1);
    }

    return lastTimeSynced;
}

/**
 * Obtain all the Todo sync items.
 *
 * Obtain all the new, modified, and deleted Todo sync items.
 * @param newItemList A reference to a list to hold the Todo items that are
 * new to the Zaurus. 
 * @param modItemList A reference to a list to hold the Todo items that have
 * been modified from the Zaurus.
 * @param delItemIdList A reference to a list to hold sync IDs of the Todo
 * items that have been deleted from the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the Todo sync items.
 * @retval 1 Failed to set the next sync anchor.
 */
int ZaurusType::GetAllTodoSyncItems(TodoItemType::List &newItemList,
				    TodoItemType::List &modItemList,
				    SyncIDListType &delItemIdList) {

    SyncIDListType::iterator syncIDIter;
    unsigned long int curSyncID;
    TodoItemType todoItem;

    if (!obtainedSyncIDLists)
	ObtainSyncIDLists(syncType);

    std::cout << "OBTAINED SYNC ID LIST.\n";

    // Loop through the newSyncIDList and obtain the data for each of the sync
    // IDs and store the data in the newItemList refrenced list.
    for (syncIDIter = newSyncIDList.begin(); syncIDIter != newSyncIDList.end();
	 syncIDIter++) {
	curSyncID = *(syncIDIter);
	std::cout << "GETTING DATA FOR " << curSyncID << std::endl;

	todoItem = GetTodoItem(syncType, curSyncID);

	std::cout << "GOT DATA FOR " << curSyncID << std::endl;
	newItemList.push_front(todoItem);
    }

    std::cout << "FINISHED LOOPIND NEW ITEMS\n";

    // Loop through the modSyncIDList and obtain the data for each of the sync
    // IDs and store the data in the modItemList referenced list.
    for (syncIDIter = modSyncIDList.begin(); syncIDIter != modSyncIDList.end();
	 syncIDIter++) {
	curSyncID = *(syncIDIter);

	todoItem = GetTodoItem(syncType, curSyncID);
	modItemList.push_front(todoItem);
    }

    // Set the delItemIdList equal to the list of sync ids of the deleted
    // items.
    delItemIdList = delSyncIDList;

    return 0;
}

/**
 * Obtain all the Calendar sync items.
 *
 * Obtain all the new, modified, and deleted Calendar sync items.
 * @param newItemList A reference to a list to hold the Calendar items that
 * are new to the Zaurus. 
 * @param modItemList A reference to a list to hold the Calendar items that
 * have been modified from the Zaurus.
 * @param delItemIdList A reference to a list to hold sync IDs of the Calendar
 * items that have been deleted from the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the Todo sync items.
 * @retval 1 Failed to set the next sync anchor.
 */
int ZaurusType::GetAllCalendarSyncItems(CalendarItemType::List &newItemList,
					CalendarItemType::List &modItemList,
					SyncIDListType &delItemIdList) {
    SyncIDListType::iterator syncIDIter;
    unsigned long int curSyncID;
    CalendarItemType calItem;

    if (!obtainedSyncIDLists)
	ObtainSyncIDLists(syncType);

    // Loop through the newSyncIDList and obtain the data for each of the sync
    // IDs and store the data in the newItemList refrenced list.
    for (syncIDIter = newSyncIDList.begin(); syncIDIter != newSyncIDList.end();
	 syncIDIter++) {
	curSyncID = *(syncIDIter);

	calItem = GetCalendarItem(syncType, curSyncID);
	newItemList.push_front(calItem);
    }

    // Loop through the modSyncIDList and obtain the data for each of the sync
    // IDs and store the data in the modItemList referenced list.
    for (syncIDIter = modSyncIDList.begin(); syncIDIter != modSyncIDList.end();
	 syncIDIter++) {
	curSyncID = *(syncIDIter);

	calItem = GetCalendarItem(syncType, curSyncID);
	modItemList.push_front(calItem);
    }

    // Set the delItemIdList equal to the list of sync ids of the deleted
    // items.
    delItemIdList = delSyncIDList;

    return 0;
}

/**
 * Add the Todo items.
 *
 * Add the Todo items to the Zaurus with the data in the items in the
 * passed list.
 * @param todoItems The list of items to add and their data.
 * @return A list of To-Do items which need their IDs mapped.
 */
TodoItemType::List ZaurusType::AddTodoItems(TodoItemType::List todoItems) {
    std::cout << "Entered the AddTodoItems() function.\n";
    TodoItemType::List::iterator pTodoItem;
    TodoItemType todoItem;
    TodoItemType::List mapIdList;
    std::cout << "Created function scoped variables.\n";

    // Here, I iterate through the list of items that should be added and
    // add them all.
    for (pTodoItem = todoItems.begin(); pTodoItem != todoItems.end();
	 pTodoItem++) {
	std::cout << "Began iteration.\n";
	todoItem = *(pTodoItem);
	std::cout << "Set the Current Todo Item.\n";

	std::cout << "About to add, " << todoItem.GetDescription();
	std::cout << std::endl;

	mapIdList.push_front(AddTodoItem(syncType, todoItem));

	std::cout << "Pushed item ID onto mapIdList.\n";

	std::cout << "Ended iteration.\n";
    }

    return mapIdList;
}

/**
 * Add the Calendar items.
 *
 * Add the Calendar items to the Zaurus with the data in the items in the
 * passed list.
 * @param calItems The list of items to add and their data.
 * @return A list of Calendar items which need their IDs mapped.
 */
CalendarItemType::List ZaurusType::AddCalendarItems(CalendarItemType::List calItems) {
    CalendarItemType::List::iterator pCalItem;
    CalendarItemType calItem;
    CalendarItemType::List mapIdList;

    // Here, I iterate through the list of items that should be added and
    // add them all.
    for (pCalItem = calItems.begin(); pCalItem != calItems.end();
	 pCalItem++) {
	calItem = *(pCalItem);

	mapIdList.push_front(AddCalendarItem(syncType, calItem));
    }

    return mapIdList;
}

/**
 * Modify the Todo items.
 *
 * Modify the Todo items on the Zaurus with the data in the items in the
 * passed list.
 * @param todoItems The list of items to modify and their data.
 * @return The number of items failed, hence a value of zero is success.
 * @retval 0 Successfully modified all items contained in the passed list.
 */
int ZaurusType::ModTodoItems(TodoItemType::List todoItems) {
    TodoItemType::List::iterator pTodoItem;
    TodoItemType todoItem;

    int retval = 0;

    // Here, I iterate through the list of items that should be modified and
    // modify them all.
    for (pTodoItem = todoItems.begin(); pTodoItem != todoItems.end();
	 pTodoItem++) {
	todoItem = *(pTodoItem);

	if (ModifyTodoItem(syncType, todoItem) != 0)
	    retval++;
    }

    return retval;
}

/**
 * Modify the Calendar items.
 *
 * Modify the Calendar items on the Zaurus with the data in the items in the
 * passed list.
 * @param calItems The list of items to modify and their data.
 * @return The number of items failed, hence a value of zero is success.
 * @retval 0 Successfully modified all items contained in the passed list.
 */
int ZaurusType::ModCalendarItems(CalendarItemType::List calItems) {
    CalendarItemType::List::iterator pCalItem;
    CalendarItemType calItem;

    int retval = 0;

    // Here, I iterate through the list of items that should be modified and
    // modify them all.
    for (pCalItem = calItems.begin(); pCalItem != calItems.end();
	 pCalItem++) {
	calItem = *(pCalItem);

	if (ModifyCalendarItem(syncType, calItem) != 0)
	    retval++;
    }

    return retval;
}

/**
 * Delete the Todo items.
 *
 * Delete the Todo items that have sync IDs contained in the passed list.
 * @param todoItemIDs The Todo Item IDs of the items to remove.
 * @return The number of items failed, hence a value of zero is success.
 * @retval 0 Successfully removed all items contained in the passed list.
 */
int ZaurusType::DelTodoItems(SyncIDListType todoItemIDs) {
    SyncIDListType::iterator it;
    int retval;
    int numDelsFailed = 0;

    for (it = todoItemIDs.begin(); it != todoItemIDs.end(); it++) {
	retval = DeleteItem(syncType, *(it));
	if (retval != 0) {
	    numDelsFailed++;
	}
    }

    return numDelsFailed;
}

/**
 * Delete the Calendar items.
 *
 * Delete the Calendar items that have sync IDs contained in the passed list.
 * @param calItemIDs The Calendar Item IDs of the items to remove.
 * @return The number of items failed, hence a value of zero is success.
 * @retval 0 Successfully removed all items contained in the passed list.
 */
int ZaurusType::DelCalendarItems(SyncIDListType calItemIDs) {
    SyncIDListType::iterator it;
    int retval;
    int numDelsFailed = 0;

    for (it = calItemIDs.begin(); it != calItemIDs.end(); it++) {
	retval = DeleteItem(syncType, *(it));
	if (retval != 0) {
	    numDelsFailed++;
	}
    }

    return numDelsFailed;
}

/**
 * Determine if a full sync is required.
 *
 * Determine if the Zaurus requires a full synchronization for the
 * specified synchronization type..
 * @return An integer value representing true (yes) or false (no).
 * @retval 1 The Zaurus does require a full synchronization.
 * @retval 0 The Zaurus does NOT require a full synchronization.
 * @retval -1 Error couldn't determine.
 */
int ZaurusType::RequiresFullSync(void) const {
    if (syncType == SYNC_TODO) {
        return reqTodoFullSync;
    } else if (syncType == SYNC_CALENDAR) {
        return reqCalendarFullSync;
    } else if (syncType == SYNC_ADDRESSBOOK) {
        return reqAddressBookFullSync;
    } else {
        return -1;
    }
}

/**
 * Terminates the synchronization.
 *
 * Terminates the synchronization process and closes connection.
 */
void ZaurusType::TerminateSync(void) {
    int retval;

    if ((retval = StateSyncDone(syncType)) != 0) {
	std::cerr << "Failed to State Sync Done (" << retval << ").\n";
	exit(1);
    }


    if ((retval = ObtainDeviceInfo()) != 0) {
	std::cerr << "Failed to Obtain Device Info (" << retval << ").\n";
	exit(1);
    }

    if ((retval = FinishSync()) != 0) {
	std::cerr << "Failed to Finish Sync (" << retval << ").\n";
	exit(1);
    }
}

/**
 * Finish the synchronization.
 *
 * The FinishSync function finishes the synchronization by doing any garbage
 * clean up and closing the socket.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully set the next sync anchor.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 4 Failed to receive an AEX message.
 * @retval 5 Failed to receive a second request.
 * @retavl 6 Failed in reading garbage data.
 */ 
int ZaurusType::FinishSync(void) {
    char garbageBuff[256];
    MessageType msg;
    int bytesRead;

    if (RecvRqst(connfd) != 0)
	return 1;

    SendRQT(connfd);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg) != 0) {
	return 4;
    }

    SendAck(connfd);

    if (RecvRqst(connfd) != 0)
	return 5;

    // In this section I have it perform the appropriate connection
    // termination so that the socket is not left in connection wait state.
    bytesRead = read(connfd, garbageBuff, 256);
    while (bytesRead > 0) {
	bytesRead = read(connfd, garbageBuff, 256);
    }

    if (bytesRead == 0) {
	close(connfd);
    } else if (bytesRead == -1) {
	return 6;
    }

    return 0;
}

/**
 * Reset the Synchronization Log.
 *
 * Reset the Synchronization Log to a proper value. This is used to set the
 * log file state so that zync knows if it has been synced before or not. Such
 * is used in the case when a Full Sync is needed.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully reset the synchronization log.
 */
int ZaurusType::ResetSyncLog(void) {
    MessageType msg;
    MessageType msgA;
    int retval;

    // First i send an empty RMS message to notify the Zaurus that I am going
    // to reset the sync log.
    if (RecvRqst(connfd) != 0) {
        printf("ERROR: Failed to receive a request.\n");
        exit(12);
    }

    SendEmptyRMS(connfd);

    if (RecvAck(connfd) != 0) {
        printf("ERROR: Failed to receive an RTG ack.\n");
        exit(12);
    }

    SendRqst(connfd);

    if (RecvAbrt(connfd) != 0) {
        printf("ERROR: Failed to received an ABRT.\n");
    }

    SendRqst(connfd);

    retval = RecvMessage(connfd, &msg);
    if (retval != 0) {
        printf("ERROR: Failed to receive ANG message (%d).\n",
	    retval);
    }

    SendAck(connfd);

    // Then I send the Zaurus the new content of the sync log. The Zaurus
    // should take this content and store it for later return when I obtain
    // the synchronization log.
    if (RecvRqst(connfd) != 0) {
        printf("ERROR: Failed to received an RQST.\n");
    }

    SendRMS(connfd);

    if (RecvAck(connfd) != 0) {
        printf("ERROR: Failed to receive an RTG ack.\n");
        exit(12);
    }

    SendRqst(connfd);

    retval = RecvMessage(connfd, &msgA);
    if (retval != 0) {
        printf("ERROR: Failed to receive AEX message (%d).\n",
	        retval);
    }

    SendAck(connfd);

    printf("FINISED RESET SYNC LOG FUNCTION.\n");

    return 0;
}

int ZaurusType::SendRSSMsg(void) {
    int retval;
    MessageType msg;

    if (RecvRqst(connfd) != 0) {
	printf("ERROR: Failed to receive a request.\n");
	exit(12);
    }

    SendRSS(connfd, syncType);

    if (RecvAck(connfd) != 0) {
	printf("ERROR: Failed to receive an RTS ack.\n");
	exit(12);
    }

    SendRqst(connfd);
    
    retval = RecvMessage(connfd, &msg);
    if (retval != 0) {
	printf("ERROR: Failed to receive AEX message (%d).\n", retval);
    }

    SendAck(connfd);

    printf("Finished with SendRSSMsg().\n");

    return 0;
}

/**
 * Set the Next Synchronization Anchor.
 *
 * The SetNextSyncAnchor sets the synchronization anchor on the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully set the next sync anchor.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 4 Failed to receive an AEX message.
 */ 
int ZaurusType::SetNextSyncAnch(void) {
    MessageType msg;

    printf("Entered SetNextSynchAnch() --------\n");

    if (RecvRqst(connfd) != 0)
        return 1;

    printf("Received Requesti() ----------------\n");

    SendRTS(connfd);

    printf("Seeeeeeent RTS message.\n");

    if (RecvAck(connfd) != 0)
        return 2;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg) != 0) {
        return 4;
    }

    SendAck(connfd);

    printf("Finished SetNextSyncAnch().\n");

    return 0;
}

/**
 * Obtain Parameter Information.
 *
 * Obtain the Parameter Information associated with a given type of
 * synchronization. This information is used to construct a format for which
 * data is exchanged between the Zaurus and the Desktop.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the parameter information.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 3 Failed to allocate memory for the ADI message.
 * @retval 4 Failed to receive an ADI message.
 * @retval 5 Failed to obtain parameter abreviation from the ADI message.
 * @retval 6 Failed to obtain parameter description from the ADI message.
 */
int ZaurusType::ObtainParamInfo(void) {
    ADIMessageType *pADIMsg;
    RDIMessageType rdiMsg(syncType);
    unsigned short int i;
    CardParamInfoType cardParamInfo;
    char buff[256];
    int retval;

    if (RecvRqst(connfd) != 0)
	return 1;

    SendMessage(connfd, (MessageType *)&rdiMsg);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    pADIMsg = new ADIMessageType;
    if (!pADIMsg)
	return 3;

    if (RecvMessage(connfd, pADIMsg) != 0)
	return 4;

    for (i = 0; i < pADIMsg->GetNumParams(); i++) {

	// Obtain all the data for the given parameter and create a card
	// parameter information object to append to the list of parameter
	// information items.
	retval = pADIMsg->GetParamAbrev(buff, 256, i);
	if (retval != 0) {
	    return 5;
	}
	cardParamInfo.SetAbrev(std::string(buff));
	cardParamInfo.SetTypeID(pADIMsg->GetParamTypeID(i));
	retval = pADIMsg->GetParamDesc(buff, 256, i);
	if (retval != 0) {
	    return 6;
	}
	cardParamInfo.SetDesc(std::string(buff));

	// Append the object to the end of the list.
	paramInfoList.push_back(cardParamInfo);
    }

    delete pADIMsg;

    SendAck(connfd);

    PrintCardParams();

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Private Member Function Section Lies Below
/////////////////////////////////////////////////////////////////////////////

/**
 * Initiate Synchronization.
 *
 * The InitiateSync function starts the synchronization process. This is the
 * first step in the synchronization process.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully initiated the synchronization process.
 * @retval 2 Failed to receive an ack message.
 * @retval 3 Failed to allocate memory to store AAY message.
 * @retval 4 Failed to receive an AAY message.
 */
int ZaurusType::InitiateSync(void) {
    SendRAY(connfd);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    AAYMessageType *pAAYMsg = new AAYMessageType;
    if (!pAAYMsg)
	return 3;

    if (RecvMessage(connfd, pAAYMsg) != 0) {
	delete pAAYMsg;
	return 4;
    }
    delete pAAYMsg;

    SendAck(connfd);

    return 0;
}

/**
 * Obtain Device Information.
 *
 * The ObtainDeviceInfo function obtains all the device info (Model, Language,
 * Authentication State, etc). The device information is stored within private
 * member variables for later access.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained device info.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 3 Failed to allocate memory to store AIG message.
 * @retval 4 Failed to receive an AIG message.
 */
int ZaurusType::ObtainDeviceInfo(void) {
    char buff[256];

    if (RecvRqst(connfd) != 0)
	return 1;

    SendRIG(connfd);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    AIGMessageType *pAIGMsg = new AIGMessageType;
    if (!pAIGMsg)
	return 3;

    if (RecvMessage(connfd, pAIGMsg) != 0) {
	delete pAIGMsg;
	return 4;
    }

    // Store the obtained device info.
    pAIGMsg->GetModel(buff, 256);
    model.assign(buff);

    pAIGMsg->GetModel(buff, 256);
    language.assign(buff);

    authState = pAIGMsg->GetAuthState();

    delete pAIGMsg;

    SendAck(connfd);

    return 0;
}

/**
 * Obtain Sync Log.
 *
 * The ObtainSyncLog function obtains the Synchronization Log from the
 * Zaurus. The state of the log is stored within a private member variable for
 * later use.
 * @param syncType The identifier specifying the type of synchronization.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained sync log.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 3 Failed to allocate memory to store AMG message.
 * @retval 4 Failed to receive an AMG message.
 */ 
int ZaurusType::ObtainSyncLog(const unsigned char syncType) {
    if (RecvRqst(connfd) != 0)
	return 1;

    SendRMG(connfd, syncType);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    AMGMessageType *pAMGMsg = new AMGMessageType;
    if (!pAMGMsg)
	return 3;

    if (RecvMessage(connfd, pAMGMsg) != 0) {
	delete pAMGMsg;
	return 4;
    }

    SendAck(connfd);

    if (syncType == SYNC_TODO) {
        if (pAMGMsg->IsEmptyTodo())
            reqTodoFullSync = 1;
    } else if (syncType == SYNC_CALENDAR) {
        if (pAMGMsg->IsEmptyCalendar())
            reqCalendarFullSync = 1;
    } else if (syncType == SYNC_ADDRESSBOOK) {
        if (pAMGMsg->IsEmptyAddressBook())
            reqAddressBookFullSync = 1;
    }

    delete pAMGMsg;

    return 0;
}

/**
 * Obtain Last Synchronization Anchor.
 *
 * The ObtainLastSyncAnch function obtains the Last Synchronization Anchor
 * from the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully obtained the last sync anchor.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 3 Failed to allocate memory to store ATG message.
 * @retval 4 Failed to receive an ATG message.
 */ 
int ZaurusType::ObtainLastSyncAnch(void) {
    char buff[256];
    char tmpBuff[256];
    struct tm tmpTime;

    if (RecvRqst(connfd) != 0)
	return 1;

    SendRTG(connfd);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    ATGMessageType *pATGMsg = new ATGMessageType;
    if (!pATGMsg)
	return 3;

    if(RecvMessage(connfd, pATGMsg) != 0) {
	delete pATGMsg;
	return 4;
    }

    SendAck(connfd);

    pATGMsg->GetTimestamp(buff, 256);

    // Here I should step through the content of the TimeStamp and convert it
    // to integer values using atoi(). Using the converted items I should fill
    // a tm struct and pass it to the mktime() function to produce an
    // equivalent time_t value. At which point I should store it in a private
    // member variable of type time_t named lastTimeSynced.
    // Get and set the year.
    memcpy(tmpBuff, buff, 4);
    tmpBuff[4] = '\0';
    tmpTime.tm_year = (atoi(tmpBuff) - 1900);

    // Get and set the month.
    memcpy(tmpBuff, (buff + 4), 2);
    tmpBuff[2] = '\0';
    tmpTime.tm_mon = (atoi(tmpBuff) - 1);

    // Get and set the day of the month.
    memcpy(tmpBuff, (buff + 4 + 2), 2);
    tmpBuff[2] = '\0';
    tmpTime.tm_mday = (atoi(tmpBuff));

    // Get and set the hour
    memcpy(tmpBuff, (buff + 4 + 2 + 2), 2);
    tmpBuff[2] = '\0';
    tmpTime.tm_hour = (atoi(tmpBuff));

    // Get and set the minutes
    memcpy(tmpBuff, (buff + 4 + 2 + 2 + 2), 2);
    tmpBuff[2] = '\0';
    tmpTime.tm_min = (atoi(tmpBuff));

    // Get and set the seconds
    memcpy(tmpBuff, (buff + 4 + 2 + 2 + 2 + 2), 2);
    tmpBuff[2] = '\0';
    tmpTime.tm_sec = (atoi(tmpBuff));

    // The following are calculated and set by mktime(): tm_wday, tm_yday

    tmpTime.tm_isdst = -1;

    lastTimeSynced = mktime(&tmpTime);

    delete pATGMsg;

    return 0;
}

/**
 * Obtain the lists of Sync IDs of new, modified, and deleted items.
 *
 * Obtain the lists of Sync IDs of new, modified, and deleted items from the
 * Zaurus and store them in private member variables of the class.
 * @param type An identifier representing the type of synchronization.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully set the next sync anchor.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 3 Failed to allocate memory for the ASY message.
 * @retval 4 Failed to receive an ASY message.
 */
int ZaurusType::ObtainSyncIDLists(const unsigned char type) {
    SyncIDListType syncIDList;
    unsigned short int syncIDCnt;
    unsigned short int numSyncIDs;
    ASYMessageType *pASYMsg;

    if (RecvRqst(connfd) != 0)
	return 1;

    SendRSY(connfd, type);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    pASYMsg = new ASYMessageType;
    if (!pASYMsg)
	return 3;

    if (RecvMessage(connfd, pASYMsg) != 0)
	return 4;

    numSyncIDs = pASYMsg->GetNumNewSyncIDs();
    std::cout << "num New Sync IDs: " << numSyncIDs << ".\n";
    for (syncIDCnt = 0; syncIDCnt < numSyncIDs; syncIDCnt++)
	newSyncIDList.push_front(pASYMsg->GetNewSyncID(syncIDCnt));

    numSyncIDs = pASYMsg->GetNumModSyncIDs();
    std::cout << "num Mod Sync IDs: " << numSyncIDs << ".\n";    
    for (syncIDCnt = 0; syncIDCnt < numSyncIDs; syncIDCnt++)
	modSyncIDList.push_front(pASYMsg->GetModSyncID(syncIDCnt));

    numSyncIDs = pASYMsg->GetNumDelSyncIDs();
    std::cout << "num Del Sync IDs: " << numSyncIDs << ".\n";
    for (syncIDCnt = 0; syncIDCnt < numSyncIDs; syncIDCnt++)
	delSyncIDList.push_front(pASYMsg->GetDelSyncID(syncIDCnt));

    delete pASYMsg;

    SendAck(connfd);

    obtainedSyncIDLists = true;

    return 0;
}

/**
 * Get Todo Item.
 *
 * Obtain a Todo item from the Zaurus given the type of sync and the items
 * synchronization id.
 * @param type An identifier representing the type of sync.
 * @param syncID The sync ID of the item to retreive from the Zaurus.
 * @return A TodoItemType object containing the requested items data.
 */
TodoItemType ZaurusType::GetTodoItem(unsigned char type,
				     unsigned long int syncID) {
    ADRMessageType *pADRMsg;
    TodoItemType todoItem;
    CardParamInfoType::List::iterator iter;

    if (RecvRqst(connfd) != 0)
	return todoItem;

    // Here, I send the RDR to request the data content of an item given the
    // type (Todo, Calendar, etc) and the synchronization ID (unique ID) of
    // the item.
    SendRDR(connfd, type, syncID);

    if (RecvAck(connfd) != 0)
	return todoItem;

    SendRqst(connfd);

    // Here, I dynamically allocate memory for an ADR message and recv a
    // message, storing it in the ADR message (the response to the RDR).
    pADRMsg = new ADRMessageType;
    if (!pADRMsg)
	return todoItem;

    if (RecvMessage(connfd, pADRMsg) != 0)
	return todoItem;

    // Load the Content of the message, if this is not done then
    // SetTodoItemParam() will cause a segfault because it will be trying to
    // access a pointer that is set to NULL.
    pADRMsg->LoadContent();

    // Here I parse the message content to obtain the item data within so that
    // I may create a Calendar item object with the proper data.
    for (iter = paramInfoList.begin(); iter != paramInfoList.end(); ++iter) {
	SetTodoItemParam(todoItem, pADRMsg, (*iter));
    }

    delete pADRMsg;

    SendAck(connfd);

    return todoItem;
}

/**
 * Get Calendar Item.
 *
 * Obtain a Calendar item from the Zaurus given the type of sync and the items
 * synchronization id.
 * @param type An identifier representing the type of sync.
 * @param syncID The sync ID of the item to retreive from the Zaurus.
 * @return A CalendarItemType object containing the requested items data.
 */
CalendarItemType ZaurusType::GetCalendarItem(unsigned char type,
					     unsigned long int syncID) {
    ADRMessageType *pADRMsg;
    CalendarItemType calItem;
    CardParamInfoType::List::iterator iter;

    if (RecvRqst(connfd) != 0)
	return calItem;

    // Here, I send the RDR to request the data content of an item given the
    // type (Todo, Calendar, etc) and the synchronization ID (unique ID) of
    // the item.
    SendRDR(connfd, type, syncID);

    if (RecvAck(connfd) != 0)
	return calItem;

    SendRqst(connfd);

    // Here, I dynamically allocate memory for an ADR message and recv a
    // message, storing it in the ADR message (the response to the RDR).
    pADRMsg = new ADRMessageType;
    if (!pADRMsg)
	return calItem;

    if (RecvMessage(connfd, pADRMsg) != 0)
	return calItem;

    // Load the Content of the message, if this is not done then
    // SetCalendarItemParam() will cause a segfault because it will be trying
    // to access a pointer that is set to NULL.
    pADRMsg->LoadContent();

    // Here I parse the message content to obtain the item data within so that
    // I may create a Calendar item object with the proper data.
    for (iter = paramInfoList.begin(); iter != paramInfoList.end(); ++iter) {
	SetCalendarItemParam(calItem, pADRMsg, (*iter));
    }

    delete pADRMsg;

    SendAck(connfd);

    return calItem;
}

/**
 * Modify Todo Item.
 *
 * Modify a Todo item on the Zaurus given the type of sync and the items data.
 * @param type An identifier representing the type of sync. The To-Do item
 * must have the same Sync ID as the item it's data should modify.
 * @param todoItem The todoItem and its data to update on the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully, requested item be deleted.
 * @retval 1 Failed to receive a message request.
 * @retval 2 Failed to receive a message acknowledgment.
 * @retval 3 Failed to allocate memory for building message content.
 * @retval 4 Failed to receive a ADW message in response.
 */
int ZaurusType::ModifyTodoItem(unsigned char type, TodoItemType todoItem) {
    RDWMessageType RDWMsg;
    MessageType msg;
    CardParamInfoType::List::iterator iter;
    
    // Initialize the RDW Message object to that of the Modification variation
    // of the message.
    if (RDWMsg.InitAsMod(type, todoItem.GetSyncID()))
	return 1;

    // Iterate through the parameter list and build the RDW message.
    iter = paramInfoList.begin();
    for (iter += 4; iter != paramInfoList.end(); ++iter) {
	if (GetTodoItemParam(todoItem, &RDWMsg, (*iter)))
	    return 2;
    }

    // Commit the RDW message content so that it the building is finished and
    // the message is put into a more usable state.
    if (RDWMsg.CommitContent())
	return 3;

    // Now that it has been packed into the RDWMsg the message is now ready to
    // be sent. However I must handle the rest of the protocol before I can
    // send the message.
    if (RecvRqst(connfd))
	return 4;

    SendMessage(connfd, &RDWMsg);

    if (RecvAck(connfd))
	return 5;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg))
	return 6;

    SendAck(connfd);

    return 0;
}

/**
 * Modify Calendar Item.
 *
 * Modify a Calendar item on the Zaurus given the type of synchronization and
 * the item data that should be modified.
 * @param type An identifier representing the type of sync. The item
 * must have the same Sync ID as the item it's data should modify.
 * @param calItem The calItem and its data to update on the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully, requested item be deleted.
 * @retval 1 Failed to Initialize RDWMessageType object as Mod variation.
 * @retval 2 Failed to Get a calender item parameter and append it.
 * @retval 3 Failed to Commit the build RDWMessageType objects content.
 * @retval 4 Failed to receive a message request.
 * @retval 5 Failed to receive a message acknowledgment.
 * @retval 6 Failed to allocate memory for building message content.
 */
int ZaurusType::ModifyCalendarItem(unsigned char type,
				   CalendarItemType calItem) {
    RDWMessageType RDWMsg;
    MessageType msg;
    CardParamInfoType::List::iterator iter;
    
    // Initialize the RDW Message object to that of the Modification variation
    // of the message.
    if (RDWMsg.InitAsMod(type, calItem.GetSyncID()))
	return 1;

    // Iterate through the parameter list and build the RDW message.
    iter = paramInfoList.begin();
    for (iter += 4; iter != paramInfoList.end(); ++iter) {
	if (GetCalendarItemParam(calItem, &RDWMsg, (*iter)))
	    return 2;
    }

    // Commit the RDW message content so that it the building is finished and
    // the message is put into a more usable state.
    if (RDWMsg.CommitContent())
	return 3;

    // Now that it has been packed into the RDWMsg the message is now ready to
    // be sent. However I must handle the rest of the protocol before I can
    // send the message.
    if (RecvRqst(connfd))
	return 4;

    SendMessage(connfd, &RDWMsg);

    if (RecvAck(connfd))
	return 5;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg))
	return 6;

    SendAck(connfd);

    return 0;
}

/**
 * Add Todo Item.
 *
 * Add a Todo item to the Zaurus given the type of sync and the items data.
 * @param type An identifier representing the type of sync.
 * @param todoItem The todoItem and its data to update on the Zaurus.
 * @return The To-Do item that was just added with its sync ID now set.
 */
TodoItemType ZaurusType::AddTodoItem(unsigned char type,
				     TodoItemType todoItem) {
    TodoItemType errTodo;
    RDWMessageType obtIdMsg;
    ADWMessageType idIsMsg;
    CardParamInfoType::List::iterator iter;
    unsigned long int syncId;

    // Since adding an item to the Zaurus is a two step event I perform the
    // first step here, which is basically to request that the Zaurus allocate
    // space for a new item and have it send back the synchronization ID of
    // the allocated space so that in the second step I can send the data to
    // the Zaurus.

    // Initialize the obtIdMsg object to the Obtain Sync ID variation of the
    // RDWMessageType object.
    if (obtIdMsg.InitAsObt(type))
	return errTodo;

    // Build the message content. In this case all the obtain ID variation of
    // the RDW message contains is the ATTR attribute of the message. Since
    // the ATTR attribute (parameter) of the is always the first attribute in
    // the parameter list I just set the iterator to the beginning of the list.
    iter = paramInfoList.begin();
    if (GetTodoItemParam(todoItem, &obtIdMsg, (*iter)))
	return errTodo;

    // Commit the RDW message content so that it the building is finished and
    // the message is put into a more usable state.
    if (obtIdMsg.CommitContent())
	return errTodo;

    // At this point the message content has been built and packed into the
    // correct instance of the RDWMessageType class. Hence, I send the message
    // and attempt to get a ADW message back containing the sync ID of the
    // newly requested item.
    if (RecvRqst(connfd))
	return errTodo;

    SendMessage(connfd, &obtIdMsg);

    if (RecvAck(connfd))
	return errTodo;

    SendRqst(connfd);

    if (RecvMessage(connfd, &idIsMsg)) {
	return errTodo;
    }

    SendAck(connfd);

    // At this point I have requested and have obtained the message containing
    // the synchronization ID of the new item. Due to this I extract the
    // synchronization ID from the message and store it for later use when I
    // update tho item data.
    syncId = idIsMsg.GetSyncID();

    errTodo = todoItem;
    errTodo.SetSyncID(syncId);

    // Now that I have obtained the synchronization ID I want to use the
    // synchronization ID to perform the second phase of the event. In this
    // phase I build another variation of the RDW message, which is used to
    // send the data of a new item to the Zaurus, and attempt to receive
    // an ADW message back stating succes.
    RDWMessageType RDWMsg;
    MessageType msg;

    // Initialize the RDWMsg object to the New Item variation of the
    // RDWMessageType object.
    if (RDWMsg.InitAsNew(type))
	return errTodo;

    // Iterate through the parameter list and build the RDW message.
    for (iter = paramInfoList.begin(); iter != paramInfoList.end(); ++iter) {
	if (GetTodoItemParam(errTodo, &RDWMsg, (*iter)))
	    return errTodo;
    }

    // Commit the RDW message content so that it the building is finished and
    // the message is put into a more usable state.
    if (RDWMsg.CommitContent())
	return errTodo;
    
    // Now that it has been packed into the RDWMsg the message is now ready to
    // be sent. However I must handle the rest of the protocol before I can
    // send the message.
    if (RecvRqst(connfd) != 0)
	return errTodo;

    SendMessage(connfd, &RDWMsg);

    if (RecvAck(connfd) != 0)
	return errTodo;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg) != 0) {
	return errTodo;
    }

    SendAck(connfd);

    return errTodo;
}

/**
 * Add Calendar Item.
 *
 * Add a Calendar item to the Zaurus given the type of sync and the items data.
 * @param type An identifier representing the type of sync.
 * @param calItem The Calendar Item and its data to update on the Zaurus.
 * @return The calendar item that was just added with its sync ID now set.
 */
CalendarItemType ZaurusType::AddCalendarItem(unsigned char type,
					     CalendarItemType calItem) {
    CalendarItemType errCal;
    RDWMessageType obtIdMsg;
    ADWMessageType idIsMsg;
    CardParamInfoType::List::iterator iter;
    unsigned long int syncId;

    // Since adding an item to the Zaurus is a two step event I perform the
    // first step here, which is basically to request that the Zaurus allocate
    // space for a new item and have it send back the synchronization ID of
    // the allocated space so that in the second step I can send the data to
    // the Zaurus.

    // Initialize the obtIdMsg object to the Obtain Sync ID variation of the
    // RDWMessageType object.
    if (obtIdMsg.InitAsObt(type))
	return errCal;

    // Build the message content. In this case all the obtain ID variation of
    // the RDW message contains is the ATTR attribute of the message. Since
    // the ATTR attribute (parameter) of the is always the first attribute in
    // the parameter list I just set the iterator to the beginning of the list.
    iter = paramInfoList.begin();
    if (GetCalendarItemParam(calItem, &obtIdMsg, (*iter)))
	return errCal;

    // Commit the RDW message content so that it the building is finished and
    // the message is put into a more usable state.
    if (obtIdMsg.CommitContent())
	return errCal;

    // At this point the message content has been built and packed into the
    // correct instance of the RDWMessageType class. Hence, I send the message
    // and attempt to get a ADW message back containing the sync ID of the
    // newly requested item.
    if (RecvRqst(connfd))
	return errCal;

    SendMessage(connfd, &obtIdMsg);

    if (RecvAck(connfd))
	return errCal;

    SendRqst(connfd);

    if (RecvMessage(connfd, &idIsMsg)) {
	return errCal;
    }

    SendAck(connfd);

    // At this point I have requested and have obtained the message containing
    // the synchronization ID of the new item. Due to this I extract the
    // synchronization ID from the message and store it for later use when I
    // update tho item data.
    syncId = idIsMsg.GetSyncID();

    errCal = calItem;
    errCal.SetSyncID(syncId);

    // Now that I have obtained the synchronization ID I want to use the
    // synchronization ID to perform the second phase of the event. In this
    // phase I build another variation of the RDW message, which is used to
    // send the data of a new item to the Zaurus, and attempt to receive
    // an ADW message back stating succes.
    RDWMessageType RDWMsg;
    MessageType msg;

    // Initialize the RDWMsg object to the New Item variation of the
    // RDWMessageType object.
    if (RDWMsg.InitAsNew(type))
	return errCal;

    // Iterate through the parameter list and build the RDW message.
    for (iter = paramInfoList.begin(); iter != paramInfoList.end(); ++iter) {
	if (GetCalendarItemParam(calItem, &RDWMsg, (*iter)))
	    return errCal;
    }

    // Commit the RDW message content so that it the building is finished and
    // the message is put into a more usable state.
    if (RDWMsg.CommitContent())
	return errCal;
    
    // Now that it has been packed into the RDWMsg the message is now ready to
    // be sent. However I must handle the rest of the protocol before I can
    // send the message.
    if (RecvRqst(connfd) != 0)
	return errCal;

    SendMessage(connfd, &RDWMsg);

    if (RecvAck(connfd) != 0)
	return errCal;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg) != 0) {
	return errCal;
    }

    SendAck(connfd);

    return errCal;
}

/**
 * Delete Item.
 *
 * Delete an item from the Zaurus given the type of synchronization and the
 * synchronization ID of the item that should be removed.
 * @param type An identifier representing the type of sync.
 * @param syncID The synchronization ID of the item to delete from the Zaurus.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully, requested item be deleted.
 * @retval 1 Failed to receive a message request.
 * @retval 2 Failed to receive a message acknowledgment.
 * @retval 4 Failed to receive a AEX message in response.
 */
int ZaurusType::DeleteItem(unsigned char type, unsigned long int syncID) {
    MessageType msg;

    if (RecvRqst(connfd) != 0)
	return 1;

    SendRDD(connfd, type, syncID);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg) != 0) {
	return 4;
    }

    SendAck(connfd);

    return 0;
}

/**
 * State the sync is done.
 *
 * The State that the synchronization is done.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully set the next sync anchor.
 * @retval 1 Failed to receive a request.
 * @retval 2 Failed to receive an ack message.
 * @retval 4 Failed to receive an AEX message.
 */ 
int ZaurusType::StateSyncDone(const unsigned char type) {
    MessageType msg;

    if (RecvRqst(connfd) != 0)
	return 1;

    SendRDS(connfd, type);

    if (RecvAck(connfd) != 0)
	return 2;

    SendRqst(connfd);

    if (RecvMessage(connfd, &msg) != 0) {
	return 4;
    }

    SendAck(connfd);

    return 0;
}

/**
 * Get Todo Item Parameter.
 *
 * Get a Todo Item Parameter from a Todo object and append it to a
 * previously initialized RDWMessageType object.
 * @param item Reference to TodoItemType object to obtain the data from.
 * @param A pointer to the RDWMessageType object to append parameter to.
 * @param paramInfo A reference to an object containing the param info for the
 * item to write.
 * @return An integer representing success (zero) or failure (non-zero).
 */
int ZaurusType::GetTodoItemParam(const TodoItemType &item,
				 RDWMessageType *const pRDWMsg,
				 const CardParamInfoType &paramInfo) {
    std::string paramAbrev;
    unsigned char paramTypeID;

    paramAbrev = paramInfo.GetAbrev();
    paramTypeID = paramInfo.GetTypeID();

    switch (paramTypeID) {
	case DATA_ID_BIT:
	    if (paramAbrev == std::string("ATTR")) {
		if (pRDWMsg->AppendBit(item.GetAttribute()))
		    return 1;
	    }
	    break;

	case DATA_ID_TIME:
	    if (paramAbrev == std::string("CTTM")) {
		if (pRDWMsg->AppendTime(item.GetCreatedTime()))
		    return 2;
	    } else if (paramAbrev == std::string("MDTM")) {
		if (pRDWMsg->AppendTime(item.GetModifiedTime()))
		    return 2;
	    } else if (paramAbrev == std::string("ETDY")) {
		if (pRDWMsg->AppendTime(item.GetStartDate()))
		    return 2;
	    } else if (paramAbrev == std::string("LTDY")) {
		if (pRDWMsg->AppendTime(item.GetDueDate()))
		    return 2;
	    } else if (paramAbrev == std::string("FNDY")) {
		if (pRDWMsg->AppendTime(item.GetCompletedDate()))
		    return 2;
	    }
	    break;

	case DATA_ID_ULONG:
	    if (paramAbrev == std::string("SYID")) {
		if (pRDWMsg->AppendULong(item.GetSyncID()))
		    return 3;
	    }
	    break;

	case DATA_ID_BARRAY:
	    if (paramAbrev == std::string("CTGR")) {
		if (pRDWMsg->AppendBarray(item.GetCategory()))
		    return 4;
	    }
	    break;

	case DATA_ID_UTF8:
	    if (paramAbrev == std::string("TITL")) {
		if (pRDWMsg->AppendUTF8(item.GetDescription()))
		    return 5;
	    } else if (paramAbrev == std::string("MEM1")) {
		if (pRDWMsg->AppendUTF8(item.GetNotes()))
		    return 5;
	    }
	    break;

	case DATA_ID_UCHAR:
	    if (paramAbrev == std::string("MARK")) {
		if (pRDWMsg->AppendUChar(item.GetProgressStatus()))
		    return 6;
	    } else if (paramAbrev == std::string("PRTY")) {
		if (pRDWMsg->AppendUChar(item.GetPriority()))
		    return 6;
	    }
	    break;

	default:
	    break;
    }

    return 0;
}

/**
 * Set Todo Item Parameter.
 *
 * Set a Todo Item Parameter given a reference to the todo item, a
 * pointer to the ADR message to obtain the data from and a reference to the
 * parameter information.
 * @param item Reference to Todo item to set parameter in.
 * @param pADRMsg A pointer to the ADR message to obtain the data.
 * @param paramInfo CardParamInfoType object containing the parameter info.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully set the calendar item parameter.
 */
int ZaurusType::SetTodoItemParam(TodoItemType &item,
				 ADRMessageType *const pADRMsg,
				 const CardParamInfoType &paramInfo) {
    std::string paramAbrev;
    unsigned char paramTypeID;

    paramAbrev = paramInfo.GetAbrev();
    paramTypeID = paramInfo.GetTypeID();

    std::cout << "Entered SetTodoItemParm().\n";
    std::cout << "Attempting to Set " << paramAbrev << ".\n";

    switch (paramTypeID) {
	case DATA_ID_BIT:
	    if (paramAbrev == std::string("ATTR")) {
		std::cout << "About to set attribute.\n";
		unsigned char dataBit;
		dataBit = pADRMsg->GetBit();
		std::cout << "Obtained the bit.\n";
		item.SetAttribute(dataBit);
		std::cout << "Set attribute.\n";
	    }
	    break;

	case DATA_ID_TIME:
	    if (paramAbrev == std::string("CTTM")) {
		item.SetCreatedTime(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("MDTM")) {
		item.SetModifiedTime(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("ETDY")) {
		item.SetStartDate(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("LTDY")) {
		item.SetDueDate(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("FNDY")) {
		item.SetCompletedDate(pADRMsg->GetTime());
	    }
	    break;

	case DATA_ID_ULONG:
	    if (paramAbrev == std::string("SYID")) {
		item.SetSyncID(pADRMsg->GetULong());
	    }
	    break;

	case DATA_ID_BARRAY:
	    if (paramAbrev == std::string("CTGR")) {
		item.SetCategory(pADRMsg->GetBarray());
	    }
	    break;

	case DATA_ID_UTF8:
	    if (paramAbrev == std::string("TITL")) {
		item.SetDescription(pADRMsg->GetUTF8());
	    } else if (paramAbrev == std::string("MEM1")) {
		item.SetNotes(pADRMsg->GetUTF8());
	    }
	    break;

	case DATA_ID_UCHAR:
	    if (paramAbrev == std::string("MARK")) {
		item.SetProgressStatus(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("PRTY")) {
		item.SetPriority(pADRMsg->GetUChar());
	    }
	    break;

	default:
	    break;
    }

    std::cout << "Set param " << paramAbrev << ".\n";
    std::cout << "Exiting SetTodoItemParam().\n";

    return 0;
}

/**
 * Get Calendar Item Parameter.
 *
 * Get a Calendar Item Parameter from a Calendar object and append it to a
 * previously initialized RDWMessageType object.
 * @param item Reference to CalendarItemType object to obtain the data from.
 * @param A pointer to the RDWMessageType object to append parameter to.
 * @param paramInfo A reference to an object containing the param info for the
 * item to write.
 * @return An integer representing success (zero) or failure (non-zero).
 */
int ZaurusType::GetCalendarItemParam(const CalendarItemType &item,
				     RDWMessageType *const pRDWMsg,
				     const CardParamInfoType &paramInfo) {
    std::string paramAbrev;
    unsigned char paramTypeID;

    paramAbrev = paramInfo.GetAbrev();
    paramTypeID = paramInfo.GetTypeID();

    switch (paramTypeID) {
	case DATA_ID_BIT:
	    if (paramAbrev == std::string("ATTR")) {
		if (pRDWMsg->AppendBit(item.GetAttribute()))
		    return 1;
	    }
	    break;

	case DATA_ID_TIME:
	    if (paramAbrev == std::string("CTTM")) {
		if (pRDWMsg->AppendTime(item.GetCreatedTime()))
		    return 2;
	    } else if (paramAbrev == std::string("MDTM")) {
		if (pRDWMsg->AppendTime(item.GetModifiedTime()))
		    return 2;
	    } else if (paramAbrev == std::string("TLM1")) {
		if (pRDWMsg->AppendTime(item.GetStartTime()))
		    return 2;
	    } else if (paramAbrev == std::string("TLM2")) {
		if (pRDWMsg->AppendTime(item.GetEndTime()))
		    return 2;
	    } else if (paramAbrev == std::string("REDT")) {
		if (pRDWMsg->AppendTime(item.GetRepeatEndDate()))
		    return 2;
	    } else if (paramAbrev == std::string("ALSD")) {
		if (pRDWMsg->AppendTime(item.GetAllDayStartDate()))
		    return 2;
	    } else if (paramAbrev == std::string("ALED")) {
		if (pRDWMsg->AppendTime(item.GetAllDayEndDate()))
		    return 2;
	    }
	    break;

	case DATA_ID_ULONG:
	    if (paramAbrev == std::string("SYID")) {
		if (pRDWMsg->AppendULong(item.GetSyncID()))
		    return 3;
	    }
	    break;

	case DATA_ID_BARRAY:
	    if (paramAbrev == std::string("CTGR")) {
		if (pRDWMsg->AppendBarray(item.GetCategory()))
		    return 4;
	    }
	    break;

	case DATA_ID_UTF8:
	    if (paramAbrev == std::string("DSRP")) {
		if (pRDWMsg->AppendUTF8(item.GetDescription()))
		    return 5;
	    } else if (paramAbrev == std::string("PLCE")) {
		if (pRDWMsg->AppendUTF8(item.GetLocation()))
		    return 5;
	    } else if (paramAbrev == std::string("MEM1")) {
		if (pRDWMsg->AppendUTF8(item.GetNotes()))
		    return 5;
	    }
	    break;

	case DATA_ID_UCHAR:
	    if (paramAbrev == std::string("ADAY")) {
		if (pRDWMsg->AppendUChar(item.GetScheduleType()))
		    return 6;
	    } else if (paramAbrev == std::string("ARON")) {
		if (pRDWMsg->AppendUChar(item.GetAlarm()))
		    return 6;
	    } else if (paramAbrev == std::string("ARSD")) {
		if (pRDWMsg->AppendUChar(item.GetAlarmSetting()))
		    return 6;
	    } else if (paramAbrev == std::string("RTYP")) {
		if (pRDWMsg->AppendUChar(item.GetRepeatType()))
		    return 6;
	    } else if (paramAbrev == std::string("RDYS")) {
		if (pRDWMsg->AppendUChar(item.GetRepeatDate()))
		    return 6;
	    } else if (paramAbrev == std::string("REND")) {
		if (pRDWMsg->AppendUChar(item.GetRepeatEndDateSetting()))
		    return 6;
	    } else if (paramAbrev == std::string("MDAY")) {
		if (pRDWMsg->AppendUChar(item.GetMultipleDaysFlag()))
		    return 6;
	    }
	    break;

	case DATA_ID_WORD:
	    if (paramAbrev == std::string("ARMN")) {
		if (pRDWMsg->AppendWord(item.GetAlarmTime()))
		    return 7;
	    } else if (paramAbrev == std::string("RFRQ")) {
		if (pRDWMsg->AppendWord(item.GetRepeatPeriod()))
		    return 7;
	    } else if (paramAbrev == std::string("RPOS")) {
		if (pRDWMsg->AppendWord(item.GetRepeatPosition()))
		    return 8;
	    }
	    break;

	default:
	    break;
    }

    return 0;
}

/**
 * Set Calendar Item Parameter.
 *
 * Set a Calendar Item Parameter given a reference to the calendar item, a
 * pointer to the ADR message to obtain the data from and a reference to the
 * parameter information.
 * @param item Reference to calendar item to set parameter in.
 * @param pADRMsg A pointer to the ADR message to obtain the data.
 * @param paramInfo CardParamInfoType object containing the parameter info.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully set the calendar item parameter.
 */
int ZaurusType::SetCalendarItemParam(CalendarItemType &item,
				     ADRMessageType *const pADRMsg,
				     const CardParamInfoType &paramInfo) {
    // This function is basically a function which maps all of the calendar
    // item parameters to the associated parameter member functions of the
    // CalendarItemType. This function allows the mapping to exist in a single
    // location, eliminating the need to duplicate the mapping in other places
    // of the ZaurusType. Beyond that it also provides an easier way to write
    // the GetCalendarItem function.

    std::string paramAbrev;
    unsigned char paramTypeID;

    paramAbrev = paramInfo.GetAbrev();
    paramTypeID = paramInfo.GetTypeID();

    switch (paramTypeID) {
	case DATA_ID_BIT:
	    if (paramAbrev == std::string("ATTR")) {
		item.SetAttribute(pADRMsg->GetBit());
	    }
	    break;

	case DATA_ID_TIME:
	    if (paramAbrev == std::string("CTTM")) {
		item.SetCreatedTime(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("MDTM")) {
		item.SetModifiedTime(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("TIM1")) {
		item.SetStartTime(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("TIM2")) {
		item.SetEndTime(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("REDT")) {
		if (item.GetRepeatEndDateSetting() == 0) {
		    std::cout << "Repeat End Date is NOT set.\n";
		    // This is going to fail I am sure, but it still needs to
		    // obtain the data and adjust the pointers.
		    pADRMsg->GetTime();
		} else {
		    std::cout << "Repeat End Date IS set.\n";
		    item.SetRepeatEndDate(pADRMsg->GetTime());
		}
	    } else if (paramAbrev == std::string("ALSD")) {
		item.SetAllDayStartDate(pADRMsg->GetTime());
	    } else if (paramAbrev == std::string("ALED")) {
		item.SetAllDayEndDate(pADRMsg->GetTime());
	    }
	    break;

	case DATA_ID_ULONG:
	    if (paramAbrev == std::string("SYID")) {
		item.SetSyncID(pADRMsg->GetULong());
	    }
	    break;

	case DATA_ID_BARRAY:
	    if (paramAbrev == std::string("CTGR")) {
		item.SetCategory(pADRMsg->GetBarray());
	    }
	    break;

	case DATA_ID_UTF8:
	    if (paramAbrev == std::string("DSRP")) {
		item.SetDescription(pADRMsg->GetUTF8());
	    } else if (paramAbrev == std::string("PLCE")) {
		item.SetLocation(pADRMsg->GetUTF8());
	    } else if (paramAbrev == std::string("MEM1")) {
		item.SetNotes(pADRMsg->GetUTF8());
	    }
	    break;

	case DATA_ID_UCHAR:
	    if (paramAbrev == std::string("ADAY")) {
		item.SetScheduleType(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("ARON")) {
		item.SetAlarm(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("ARSD")) {
		item.SetAlarmSetting(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("RTYP")) {
		item.SetRepeatType(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("RDYS")) {
		item.SetRepeatDate(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("REND")) {
		item.SetRepeatEndDateSetting(pADRMsg->GetUChar());
	    } else if (paramAbrev == std::string("MDAY")) {
		item.SetMultipleDaysFlag(pADRMsg->GetUChar());
	    }
	    break;

	case DATA_ID_WORD:
	    if (paramAbrev == std::string("ARMN")) {
		item.SetAlarmTime(pADRMsg->GetWord());
	    } else if (paramAbrev == std::string("RFRQ")) {
		item.SetRepeatPeriod(pADRMsg->GetWord());
	    } else if (paramAbrev == std::string("RPOS")) {
		item.SetRepeatPosition(pADRMsg->GetWord());
	    }
	    break;

	default:
	    break;
    }

    // Return in success.
    return 0;
}

/**
 * Display the List of Card Params.
 *
 * Display the current List of Card Params. This is only here for testing
 * purposes and will be removed at some stage.
 */
void ZaurusType::PrintCardParams(void) {
    CardParamInfoType::List::iterator iter;

    for (iter = paramInfoList.begin(); iter != paramInfoList.end(); ++iter) {
	std::cout << "Parameter.\n";
	std::cout << "----------\n";
	std::cout << "Abrev: " << (*iter).GetAbrev() << ".\n";
	std::cout << "Desc: " << (*iter).GetDesc() << ".\n";
	std::cout << "TypeID: " << (int)(*iter).GetTypeID() << ".\n";
    }
}
