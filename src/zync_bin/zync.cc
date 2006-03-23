/*
 * Copyright 2003-2006 Andrew De Ponte
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

// Standard Input/Output Includes
#include <iostream>
#include <string>

// Includes for fork()
#include <unistd.h>

// Includes for loading shared objects
#include <dlfcn.h>

//#include <pthread.h>

// Includes for waitpid()
#include <sys/types.h>
#include <sys/wait.h>

// Zaurus specific Includes.
#include <zmsg.h>
#include <zdata_lib/zdata.hh>

#include <ConfigManagerType.h>

#include "TodoPluginType.hh"
#include "CalendarPluginType.hh"
#include "ZaurusType.hh"

#define APP_VERSION "0.2.6"

// This is the port that the Zaurus listens on waiting for a connection to
// initiate a synchronization from the Desktop.
#define ZLISTPORT 4244

#define CONF_WIN_Z 1
#define CONF_WIN_D 2
#define CONF_WIN_B 3

struct sData {
    pthread_mutex_t ready_mutex;
    pthread_cond_t ready_cond;
    pthread_mutex_t confManager_mutex;
    ConfigManagerType *pConfManager;
    pthread_mutex_t conf_winner_mutex;
    unsigned short int conf_winner;
};

void DispWelcomeMsg(void);
void DispUsageMsg(void);
void DispVersion(void);
void DispRetVals(void);
int PerformTodoSync(unsigned short int confWinner,
            ConfigManagerType *pConfManager);
int PerformCalendarSync(unsigned short int confWinner,
            ConfigManagerType *pConfManager);
TodoItemType::List::iterator FindSyncID(TodoItemType::List &todoList,
                    unsigned long int syncID);
void ResolveDelModConflicts(SyncIDListType &zDelTodoItemIDList,
                TodoItemType::List &zModTodoItemList,
                TodoItemType::List &zAddTodoItemList,
                SyncIDListType &dDelTodoItemIDList,
                TodoItemType::List &dModTodoItemList,
                TodoItemType::List &dAddTodoItemList);
void ResolveModModConflicts(TodoItemType::List &zModTodoItemList,
                TodoItemType::List &dModTodoItemList,
                TodoItemType::List &zAddTodoItemList,
                TodoItemType::List &dAddTodoItemList,
                unsigned short int conflict_winner);

int main(int argc, char **argv) {
    // Generic Variable used for return values of functions.
    int retval;

    // Variables used to for creating the proper socket.
    int sockfd;
    struct sockaddr_in servaddr;

    // Variables used to obtain data from config file.
    ConfigManagerType confManager;
    char *pEnvVarVal;
    char optVal[256];
    std::string confPath;

    // Variables used to store the IP of the Zaurus which to connect to.
    char zIpAddr[256];
    int zIpCmdLine = 0;

    // Variables used to handle the child sync process.
    pid_t childPid;
    int childStatus;

    // This variable is used as a flag to represent when a sync type has been
    // chosen and when a sync type has NOT been chosen.
    int choseSyncType = 0;

    // This variable is used to store the type of synchronization.
    unsigned char syncType;

    // This is a variable used to store the conflict winner.
    unsigned short int confWinner;
    ConfigManagerType *pConfManager;

    // The two variables below are used with getopt() to parse the command
    // line arguments.
    int optchar;
    extern char *optarg;

    pConfManager = &confManager;

    // The first thing I do is parse the command line arguments and set the
    // synchronization type and possibly the Zaurus IP address if it is so
    // specified.
    while (1) {
        optchar = getopt(argc, argv, "tacd:vhr");

        // If optchar is -1 the getopt function failed to find any more
        // options and it set optind to the first item in argv that was not an
        // option.
        if (optchar == -1)
            break;

        switch ((char)optchar) {
            case 't':
                if (choseSyncType) {
                    std::cout << "Command Argument Error: Multiple types of" \
                        " synchronizations specified.\n";
                    DispUsageMsg();
                    return 1;
                } else {
                    syncType = 0x06;
                    choseSyncType = 1;
                }
                break;
            case 'a':
                if (choseSyncType) {
                    std::cout << "Command Argument Error: Multiple types of" \
                        " synchronizations specified.\n";
                    DispUsageMsg();
                    return 1;
                } else {
                    syncType = 0x07;
                    choseSyncType = 1;
                }
                break;
            case 'c':
                if (choseSyncType) {
                    std::cout << "Command Argument Error: Multiple types of" \
                        " synchronizations specified.\n";
                    DispUsageMsg();
                    return 1;
                } else {
                    syncType = 0x01;
                    choseSyncType = 1;
                }
                break;
            case 'd':
                strncpy(zIpAddr, optarg, 256);
                zIpCmdLine = 1;
                break;
            case 'v':
                DispVersion();
                return 0;
            case 'h':
                DispUsageMsg();
                return 0;
            case 'r':
                DispRetVals();
                return 0;
            case '?':
                std::cout << "Command Argument Error: Unsupported option" \
                    " used.\n";
                DispUsageMsg();
                return 2;
        }
    }

    // Here I check to make sure that a synchronization type has been
    // specified on the command line. If not I display an error message along
    // with a usage message and return in error.
    if (!choseSyncType) {
        std::cout << "Command Argument Error: No synchronization type" \
            " specified.\n";
        DispUsageMsg();
        return 3;
    }

    // Now that I have parsed the command line arguments I load in the options
    // from the config file.

    // Obtain the value of the HOME environment variable and build the path to
    // the config file so that it can be loaded.
    pEnvVarVal = getenv("HOME");
    if (!pEnvVarVal) {
        std::cout << "Failed to obtain the HOME environment variable.\n";
        return 11;
    }

    confPath.assign(pEnvVarVal);
    confPath.append("/.zync.conf");

    // Now I attempt to open and load the config file.
    retval = confManager.Open((char *)confPath.c_str());
    if (retval != 0) {
        if (retval == -1)
            return 12;
    }

    // If I reach this point I know that the ConfigManager has opened the
    // file. Hence, I try to load the supported options.
    retval = confManager.GetValue("zaurus_ip", optVal, 256);
    if (retval != 0) {
        if (zIpCmdLine != 0) {
            std::cout << "An IP for the Zaurus was not specified in the" \
                " config or on the command line. Please specify an IP" \
                " address via one of these methods.\n";
            return 13;
        }
    } else {
        if (!zIpCmdLine) {
            strncpy(zIpAddr, optVal, 256);
        }
    }

    // If I reach this point I know that the ConfigManager has opened the
    // file. Hence, I try to load the conflict_winner option.
    retval = confManager.GetValue("conflict_winner", optVal, 256);
    if (retval != 0) {
        std::cout << "A conflict_winner was not specified in the config" \
            " file. Please specify the conflict_winner option in the config" \
            " file with one of the three values \"zaurus\", \"desktop\"," \
            " \"both\".\n";
        return 14;
    } else {
        if (strcmp(optVal, "zaurus") == 0) {
            confWinner = CONF_WIN_Z;
        } else if (strcmp(optVal, "desktop") == 0) {
            confWinner = CONF_WIN_D;
        } else if (strcmp(optVal, "both") == 0) {
            confWinner = CONF_WIN_B;
        } else {
            std::cout << "The conflict_winner option was specified in the" \
                " config file with an incorrect value. The value must be" \
                " one of the following three, \"zaurus\", \"desktop\"," \
                " \"both\".\n";
            return 15;
        }
    }

    // After parsing the command line arguments I display the welcome message
    // and then move onto running the proper synchronization process based on
    // the type of synchronization specified in the command line arguments.
    DispWelcomeMsg();

    // Now before I fork any processes I set the stdout file descriptor to not
    // buffer.
    setbuf(stdout, NULL);

    // Initiate the synchronization based on the type specified by the command
    // line argument.
    if (syncType == 0x06) {
    // Note: I had to use fork() here for creating the other process
    // because I ran into problems with using libkcal in the thread due to
    // it not being thread safe.
        if ((childPid = fork()) == 0) {
        // This is the child process. In this case it is actually the
        // sync server which is going to listen for a connection from the
        // Zaurus and perform the synchronization.

            retval = PerformTodoSync(confWinner, pConfManager);
            exit(retval);
        }

        if (childPid == -1) {
            std::cout << "zync: Error in forking for To-Do sync.\n";
            perror("main");
            return 4;
        }
    } else if (syncType == 0x07) {
        std::cout << "Address Book synchronization has NOT been" \
            " implemented yet.\n";
        return 0;
    } else if (syncType == 0x01) {
    // Note: I had to use fork() here for creating the other process
    // because I ran into problems with using libkcal in the thread due to
    // it not being thread safe.
        if ((childPid = fork()) == 0) {
        // This is the child process. In this case it is actually the
        // sync server which is going to listen for a connection from the
        // Zaurus and perform the synchronization.

            retval = PerformCalendarSync(confWinner, pConfManager);
            exit(retval);
        }

        if (childPid == -1) {
            std::cout << "zync: Error in forking for Calendar sync.\n";
            perror("main");
            return 4;
        }
    }

    // This is where execution returns to the parent process. In this case we
    // are using the parent process as the Desktop Client which basically
    // connects to the Zaurus and tells the Zaurus that it should initiate a
    // synchronization.

    // Attempt to create a socket to be used to connect to the Zaurus
    // synchronization server.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("main");
        return 7;
    }

    /// Now that I have the socket I have to create a address for the socket
    /// to attempt to connect to. The address consists of an ip address and a
    /// port for which I would like to connect.
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(ZLISTPORT);
    if (inet_pton(AF_INET, zIpAddr, &servaddr.sin_addr) <= 0) {
        perror("main");
        return 8;
    }

    // Here, I call a wait on a condition to determine if the synchronization
    // server thread spawned above is ready to accept connections.
    /*
    pthread_mutex_lock(&data.ready_mutex);
    pthread_cond_wait(&data.ready_cond, &data.ready_mutex);
    pthread_mutex_unlock(&data.ready_mutex);
    */

    sleep(3);

    // Now that I have the address set appropriateley for the server I would
    // like to make a connection to, I make the connection.
    retval = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (retval == -1) {
        perror("main");
        return 9;
    }

    std::cout << "Connected to Zaurus on port " << ZLISTPORT << ".\n";

    // Now that the connection has been made I initiate the synchronization by
    // sending an RAY message to the Zaurus over the new connection.
    SendRAY(sockfd);

    std::cout << "Sent RAY message to Zaurus to initiate sync.\n";

    // I wait for the sync server process to exit and then I send the finish
    // message and close the socket. If I did not due this the child process
    // would get killed and terminate thy sync process part way through.
    retval = waitpid(childPid, &childStatus, 0);
    if (retval == -1) {
        std::cout << "zync: Error while waiting for child pid.\n";
        perror("main");
        return 10;
    }

    // Now that the actual synchronization portion has finished I close the
    // connection to the Zaurus synchronization server.
    send_fin_msg(sockfd);
    std::cout << "Sent FIN message to Zaurus to end sync.\n";
    close(sockfd);
    std::cout << "Close the connection to the Zaurus.\n";

    // Return in success
    return 0;
}

/**
 * Display the Welcome Message.
 *
 * Display the Welcome Message including the license statement.
 */
void DispWelcomeMsg(void) {
    using namespace std;
    cout << "Welcome to the zync!" << endl;
    cout << "zync is released under the GNU General Public License" \
    " version 2." << endl << endl;
    cout << "You should have received a copy of the GNU General Public" \
    " License" << endl;
    cout << "along with zync; if not, write to the Free Software\n";
    cout << "Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA" \
    " 02111-1307 USA" << endl << endl;
    cout << "Please enjoy our teams efforts to bring Zaurus" \
    " Synchronization to the Linux" << endl;
    cout << "users, ranging from novice to expert." << endl << endl;
}

/**
 * Display the Usage Message.
 *
 * Display the Message which shows the proper usage of the tool.
 */
void DispUsageMsg(void) {
    using namespace std;
    cout << "Usage:\n";
    cout << "------\n";
    cout << "zync [ -t | -a | -c ] [ -d <zaurus ip> ] [ -v ] [ -h ] [ -r ]\n";
    cout << "t - Synchronize To-Do.\n";
    cout << "a - Synchronize Address Book.\n";
    cout << "c - Synchronize Calendar.\n";
    cout << "d - Specify Zaurus IP address (overrides config).\n";
    cout << "v - Display application version information.\n";
    cout << "h - Display the application usage.\n";
    cout << "r - Display the applications possible return values.\n";
}

/**
 * Display the Version.
 *
 * Display the Version of the tool.
 */
void DispVersion(void) {
    using namespace std;
    cout << "zync " << APP_VERSION << "\n";
}

/**
 * Display the return values.
 *
 * Display a list of the applications return values and their meanings.
 */
void DispRetVals(void) {
    using namespace std;
    cout << "Return Values:\n";
    cout << "--------------\n";
    cout << "00: The application performed as it should have, success.\n";
    cout << "01: Command line specified multiple synchronization types.\n";
    cout << "02: Unsupported command line option was passed.\n";
    cout << "03: No synchronization type specified in the command line.\n";
    cout << "04: Failed to fork process for Todo sync server.\n";
    cout << "05: Failed to fork process for Address Book sync server.\n";
    cout << "06: Failed to fork process for Calendar sync server.\n";
    cout << "07: Failed to create socket for synchronization client to use.\n";
    cout << "08: Failed to convert string IP address to net address struct.\n";
    cout << "09: Failed to connect to the Zaurus synchronization server.\n";
    cout << "10: Failed to wait for Desktop synchronization server process.\n";
    cout << "11: Failed to obtain value of the HOME environment variable.\n";
    cout << "12: Failed to open the .zync.conf file for reading.\n";
    cout << "13: Zaurus IP not set in the .zync.conf file or cmd line.\n";
    cout << "14: The conflict_winner is not set in the .zync.conf file.\n";
    cout << "15: The conflict_winner is set to an illegal vaule.\n";
}

/**
 * Find item with sync ID.
 *
 * Find an item within a list of To-Do items which has the given
 * synchronization ID. If this function fails to find the item the empty end
 * nodes iterator is returned else the iterator of the item is found. Note:
 * This returns the first instance found.
 * @return The iterator of the item with the matching sync ID if found, empty
 * end node of the passed list if not found.
 */
TodoItemType::List::iterator FindSyncID(TodoItemType::List &todoList,
                    unsigned long int syncID) {
    TodoItemType::List::iterator iter;

    for (iter = todoList.begin(); iter != todoList.end(); ++iter)
    if ((*iter).GetSyncID() == syncID)
        return iter;

    return todoList.end();
}

/**
 * Resolve Deltion/Modification conflicts.
 *
 * This function searches for and resolves any conflicts found between
 * deletion of items and modification of items. If a conflict is found it is
 * handled by replacing the deleted item with the modified item.
 */
void ResolveDelModConflicts(SyncIDListType &zDelTodoItemIDList,
                TodoItemType::List &zModTodoItemList,
                TodoItemType::List &zAddTodoItemList,
                SyncIDListType &dDelTodoItemIDList,
                TodoItemType::List &dModTodoItemList,
                TodoItemType::List &dAddTodoItemList) {

    //TodoItemType::List::iterator zIter;
    SyncIDListType::iterator zSyncIDIter;
    TodoItemType::List::iterator fndItemIter;
    SyncIDListType::iterator tmpIDIter;

    // Now in this case in this function I am resolving any conflicts between
    // deletion and modification. This is the only type of conflict that
    // should ever be created other than a deletion on both ends. Since
    // deletion is such a harsh action and can cause data loss, which can be
    // very bad in certain situations, the choice for resolving such conflicts
    // is that a modification wins in conflict with a delete. Hence, if one
    // deletes an item from their Desktop PIM software and modifies that same
    // item on the Zaurus. When the next synchronization is performed the item
    // should be recreated on their Desktop PIM software with the updated
    // information.

    std::cout << "Entered the ResolveDelmodConflicts() function.\n";

    zSyncIDIter = zDelTodoItemIDList.begin();
    while (zSyncIDIter != zDelTodoItemIDList.end()) {

    // First I want to check if there are any conflicts with the sync ID
    // of what ever the current deleted item is.
    fndItemIter = FindSyncID(dModTodoItemList, (*zSyncIDIter));
    if (fndItemIter != dModTodoItemList.end()) {
        std::cout << "Found a matching sync ID.\n";
        // If I have gotten to this point I know that there is a conflict
        // and that fndItemIter is the iterator for the item in conflict.

        // The first thing I want to do in this case is Add the
        // Modification of the conflicting item to the appropriate add
        // list so that it will be added.
        dAddTodoItemList.push_front(*fndItemIter);

        std::cout << "Added the item to the proper add list.\n";

        // The second thing I want to do is remove the item from the
        // modification list that it was found in so that after the item
        // it is not needlessly modified with the same data.
        dModTodoItemList.erase(fndItemIter);

        std::cout << "Erased the item from the proper Mod list.\n";

        // The third thing I want to do is remove the item from the
        // deletion list so that it is not deleted after it has been
        // added again.
        if (zSyncIDIter == zDelTodoItemIDList.begin()) {
        zDelTodoItemIDList.erase(zSyncIDIter);
        zSyncIDIter = zDelTodoItemIDList.begin();
        } else {
        tmpIDIter = zSyncIDIter;
        tmpIDIter--;
        zDelTodoItemIDList.erase(zSyncIDIter);
        zSyncIDIter = tmpIDIter;
        zSyncIDIter++;
        }

        std::cout << "Erased the sync id from the deleted list.\n";
    } else {
        // If I have gotten to this point I know that there was no
        // conflict and I want to go to the next item in the list and
        // check for a conflict.
        zSyncIDIter++;
    }
    }

    // Now that the conflicts have been resolved in the above case I handle
    // the conflicts in the case where the Desktop PIM application has deleted
    // an item and the Zaurus has modified an item.
    zSyncIDIter = dDelTodoItemIDList.begin();
    while (zSyncIDIter != dDelTodoItemIDList.end()) {

    // First I want to check if there are any conflicts with the sync ID
    // of what ever the current deleted item is.
    fndItemIter = FindSyncID(zModTodoItemList, (*zSyncIDIter));
    if (fndItemIter != zModTodoItemList.end()) {
        // If I have gotten to this point I know that there is a conflict
        // and that fndItemIter is the iterator for the item in conflict.

        // The first thing I want to do in this case is Add the
        // Modification of the conflicting item to the appropriate add
        // list so that it will be added.
        zAddTodoItemList.push_front(*fndItemIter);

        // The second thing I want to do is remove the item from the
        // modification list that it was found in so that after the item
        // it is not needlessly modified with the same data.
        zModTodoItemList.erase(fndItemIter);

        // The third thing I want to do is remove the item from the
        // deletion list so that it is not deleted after it has been
        // added again.
        if (zSyncIDIter == dDelTodoItemIDList.begin()) {
        dDelTodoItemIDList.erase(zSyncIDIter);
        zSyncIDIter = dDelTodoItemIDList.begin();
        } else {
        tmpIDIter = zSyncIDIter;
        tmpIDIter--;
        dDelTodoItemIDList.erase(zSyncIDIter);
        zSyncIDIter = tmpIDIter;
        zSyncIDIter++;
        }
    } else {
        // If I have gotten to this point I know that there was no
        // conflict and I want to go to the next item in the list and
        // check for a conflict.
        zSyncIDIter++;
    }
    }

    std::cout << "Exited the ResolveDelmodConflicts() function.\n";

}

/**
 * Resolve Modification/Modification conflicts.
 *
 * This function searches for and resolves any conflicts found between
 * modification of items and modification of items. If a conflict is found it
 * is handled using the conflict winner specified config option.
 */
void ResolveModModConflicts(TodoItemType::List &zModTodoItemList,
                TodoItemType::List &dModTodoItemList,
                TodoItemType::List &zAddTodoItemList,
                TodoItemType::List &dAddTodoItemList,
                unsigned short int conflict_winner) {

    std::cout << "Entered the ResolveModModConflicts() function.\n";

    TodoItemType::List::iterator fndItemIter;
    TodoItemType::List::iterator iter;
    TodoItemType::List::iterator tmpIter;

    iter = zModTodoItemList.begin();
    while (iter != zModTodoItemList.end()) {

    // First I want to check if there are any conflicts with the sync ID
    // of what ever the current modified item is.
    fndItemIter = FindSyncID(dModTodoItemList, (*iter).GetSyncID());
    if (fndItemIter != dModTodoItemList.end()) {
        std::cout << "Found a matching sync ID.\n";
        // If I have gotten to this point I know that there is a conflict
        // and that fndItemIter is the iterator for the item in conflict.

        if (conflict_winner == CONF_WIN_Z) {
        dModTodoItemList.erase(fndItemIter);
        } else if (conflict_winner == CONF_WIN_D) {
        if (iter == zModTodoItemList.begin()) {
            zModTodoItemList.erase(iter);
            iter = zModTodoItemList.begin();
        } else {
            tmpIter = iter;
            tmpIter--;
            zModTodoItemList.erase(iter);
            iter = tmpIter;
            iter++;
        }
        } else if (conflict_winner == CONF_WIN_B) {
        dAddTodoItemList.push_front((*fndItemIter));
        dModTodoItemList.erase(fndItemIter);

        zAddTodoItemList.push_front((*iter));
        if (iter == zModTodoItemList.begin()) {
            zModTodoItemList.erase(iter);
            iter = zModTodoItemList.begin();
        } else {
            tmpIter = iter;
            tmpIter--;
            zModTodoItemList.erase(iter);
            iter = tmpIter;
            iter++;
        }
        }
    } else {
        // If I have gotten to this point I know that there was no
        // conflict and I want to go to the next item in the list and
        // check for a conflict.
        iter++;
    }
    }

    std::cout << "Exited the ResolveModModConflicts() function.\n";

}


/**
 * Perform a Todo synchronization.
 *
 * Perform a Todo synchronization.
 */
int PerformTodoSync(unsigned short int confWinner,
            ConfigManagerType *pConfManager) {
    void *libHandle;
    create_todo_t pCreateFunc;
    destroy_todo_t pDestroyFunc;
    TodoPluginType *pTodoPlugin;
    char optVal[256];
    int retval;
    ZaurusType zaurus;

    // The following three lists exist to contain the new, mod, del item
    // information obtained from the Zaurus for conflict management and
    // synchronization.
    TodoItemType::List zNewTodoItemList;
    TodoItemType::List zModTodoItemList;
    SyncIDListType zDelTodoItemIDList;

    // The following three lists exist to contain the new, mod, del item
    // information obtain from the Desktop Todo Plugin for conflict management
    // and synchronization.
    TodoItemType::List dNewTodoItemList;
    TodoItemType::List dModTodoItemList;
    SyncIDListType dDelTodoItemIDList;

    TodoItemType::List mapIdList;

    SyncIDListType::iterator zSyncIDIter;
    time_t lastTimeSynced;
    TodoItemType::List::iterator zIter;
    TodoItemType curItem;

    // Normally the path to the selected Todo synchronization plugin would be
    // loaded from the configuration file. However, the configuration file has
    // not been implemented into this application yet. Hence, it is hard coded
    // into the code right now for development of the first plugin.
    retval = pConfManager->GetValue("todo_plugin_path", optVal, 256);
    if (retval != 0) {
        std::cout << "Error: No todo_plugin_path entry found in the" \
            " .zync.conf configuration file.\n";
        return 1;
    }

    // Open the plugin and load the creation and destroy symbols.
    libHandle = dlopen(optVal, RTLD_LAZY);
    if (!libHandle) {
        std::cout << "zync: Failed to open the KOrganizer Todo Plugin.\n";
        std::cout << dlerror() << std::endl;
        return 2;
    }

    pCreateFunc = (create_todo_t)dlsym(libHandle, "createTodoPlugin");
    if (!pCreateFunc) {
        std::cout << "zync: Failed to find the create symbol in plugin.\n";
        std::cout << dlerror() << std::endl;
        dlclose(libHandle);
        return 3;
    }

    pDestroyFunc = (destroy_todo_t)dlsym(libHandle, "destroyTodoPlugin");
    if (!pDestroyFunc) {
        std::cout << "zync: Failed to find the destroy symbol in plugin.\n";
        std::cout << dlerror() << std::endl;
        dlclose(libHandle);
        return 4;
    }

    // Create an instance of the plugin.
    pTodoPlugin = pCreateFunc();
    if (!pTodoPlugin) {
        std::cout << "zync: Failed to create intance of the plugin object.\n";
        dlclose(libHandle);
        return 5;
    }

    // At this point I have opened the plugin and have loaded the creation and
    // destroy methods. This means I have the capability to create an instance
    // of the TodoPluginType class and destroy it. Hence, I know that all the
    // class functions have been implemented (because they are pure virtual)
    // and assume that they work correctly. It should be fine to perform the
    // synchronization now.

    // Display the general plugin information.
    std::cout << "Plugin Information\n";
    std::cout << "------------------\n";
    std::cout << "Plugin Name: " << pTodoPlugin->GetPluginName() + "\n";
    std::cout << "Plugin Version: " << pTodoPlugin->GetPluginVersion() + "\n";
    std::cout << "Plugin Author: " << pTodoPlugin->GetPluginAuthor() + "\n";
    std::cout << "Plugin Desc: " << pTodoPlugin->GetPluginDescription() + "\n";
    std::cout << std::endl;

    // Attempt to initialize the plugin.
    retval = pTodoPlugin->Initialize();
    if (retval != 0) {
        std::cout << "zync: Failed to Initialize To-Do Plugin (" << retval;
        std::cout << ")." << std::endl;
        pDestroyFunc(pTodoPlugin);
        dlclose(libHandle);
        return 6;
    }

    std::cout << "Initialized the Todo plugin.\n";

    // Tell the Desktop synchronization server to listen for a connection from
    // a Zaurus.
    zaurus.ListenConnection();

    std::cout << "Desktop Sync Server now listening.\n";

    // Set the type of synchronization to the value that represents the To-Do
    // synchronization type.
    zaurus.SetSyncType(0x06);

    std::cout << "Set the server sync type to 0x06.\n";

    // Perform the actual todo synchronization.
    if(zaurus.RequiresPassword()) {
        // This first step in the authentication process is to obtain the
        // password to send to the Zaurus for authentication. In this case the
        // password should be in the config file. Hence, I try and obtain the
        // passcode from the config file.
        retval = pConfManager->GetValue("passcode", optVal, 256);
        if (retval != 0) {
            std::cout << "zync: Failed to obtain password from config.\n";
            zaurus.FinishSync();
            pDestroyFunc(pTodoPlugin);
            dlclose(libHandle);
            return 7;
        }

        // The second phase of the Authentication process is authenticating
        // the password with the Zaurus. Hence, I attempt to authenticate the
        // obtained password.
        if (zaurus.AuthenticatePassword(optVal) != 0) {
            std::cout << "zync: Failed to authenticate password.\n";
            pDestroyFunc(pTodoPlugin);
            dlclose(libHandle);
            return 8;
        }
    }

    lastTimeSynced = zaurus.GetLastTimeSynced();

    std::cout << "Obtained \"Last Time Synced\".\n";

    // If I move the KOrgTodoPlugin calls here I don't get the segfault with
    // them. However, if I leave them down below then I still get the
    // segfault.

    // Check if the Full Sync is required then try and clear the log, reset
    // the log and exit with out saving sync state. Hence, all items should be
    // seen as new items the next time one syncs (we hope).
    if (zaurus.RequiresFullSync()) {
        std::cout << "Note: Zaurus Requires Full Sync.\n";
        zaurus.ResetSyncLog();
        std::cout << "Reset Sync Log on the Zaurus.\n";
        zaurus.SetNextSyncAnch();
        std::cout << "Set the next sync anchor on the Zaurus.\n";
        zaurus.SendRSSMsg();
        std::cout << "Sent RSS message to the Zaurus.\n";
    } else {
        zaurus.SetNextSyncAnch();
        std::cout << "Set the next sync anchor on the Zaurus.\n";
    }

    if (zaurus.ObtainParamInfo() != 0) {
        std::cout << "Failed to ObtainParamInfo().\n";
    }
    std::cout << "Obtained parameter info from the Zaurus.\n";

    // Obtain the changes from the Zaurus.
    if (zaurus.GetAllTodoSyncItems(zNewTodoItemList, zModTodoItemList,
        zDelTodoItemIDList) != 0) {
        std::cout << "Failed to get all sync items.\n";
    }
    std::cout << "Obtained all Todo sync items from the Zaurus.\n";

    // Display the Zaurus changes information.
    std::cout << "Zaurus Changes\n";
    std::cout << "--------------\n";
    std::cout << "Found " << zNewTodoItemList.size() << " new items on" \
        " the Zaurus.\n";
    std::cout << "Found " << zModTodoItemList.size() << " modified items" \
        " on the Zaurus.\n";
    std::cout << "Found " << zDelTodoItemIDList.size() << " items deleted" \
        " from the Zaurus.\n";

    // Obtain the changes from the Desktop PIM application todo plugin.
    if (zaurus.RequiresFullSync()) {
        std::cout << "Note: Zaurus Requires Full Sync.\n";
        dNewTodoItemList = pTodoPlugin->GetAllTodoItems();
        std::cout << "Obtained Todo items from the PIM Plugin.\n";
    } else {
        dNewTodoItemList = pTodoPlugin->GetNewTodoItems(lastTimeSynced);
        std::cout << "Obtained New Todo Items from PIM Plugin.\n";
        dModTodoItemList = pTodoPlugin->GetModTodoItems(lastTimeSynced);
        std::cout << "Obtained Modified Todo Items from PIM Plugin.\n";
        dDelTodoItemIDList = pTodoPlugin->GetDelTodoItemIDs(lastTimeSynced);
        std::cout << "Obtained Deleted Todo Item IDs from PIM Plugin.\n";
    }

    // Display the To-Do plugin changes.
    std::cout << "To-Do Plugin Changes\n";
    std::cout << "--------------------\n";
    std::cout << "Found " << dNewTodoItemList.size() << " new items on" \
        " the To-Do plugin.\n";
    std::cout << "Found " << dModTodoItemList.size() << " modified items" \
        " on the To-Do plugin.\n";
    std::cout << "Found " << dDelTodoItemIDList.size() << " items deleted" \
        " from the To-Do plugin.\n";

    if (!zaurus.RequiresFullSync()) {
        std::cout << "Note: Zaurus does NOT require Full Sync.\n";
        // Compare the todo item lists for conflicts and resolve the conflicts.
        ResolveDelModConflicts(zDelTodoItemIDList, zModTodoItemList,
            zNewTodoItemList, dDelTodoItemIDList,
            dModTodoItemList, dNewTodoItemList);
        std::cout << "Compared DelMod Conflicts and resolved them.\n";

        ResolveModModConflicts(zModTodoItemList, dModTodoItemList,
            zNewTodoItemList, dNewTodoItemList,
            confWinner);
        std::cout << "Compared ModMod Conflicts and resolved them.\n";
    }

    // I then want to do what I just did above but for the opposite
    // component. Once I do that the deletion conflicts should all be handled.

    if (!zaurus.RequiresFullSync()) {
        std::cout << "Note: Zaurus does NOT require Full Sync.\n";
        // Perform the Desktop side of the synchronization.
        std::cout << "Plugin About to Del Todo Items.\n";
        pTodoPlugin->DelTodoItems(zDelTodoItemIDList);
        std::cout << "Plugin Deleted Todo Items.\n";
        std::cout << "Plugin About to Mod Todo Items.\n";
        pTodoPlugin->ModTodoItems(zModTodoItemList);
        std::cout << "Plugin Modified Todo Items.\n";
        std::cout << "Plugin About to Add Todo Items.\n";
        pTodoPlugin->AddTodoItems(zNewTodoItemList);
        std::cout << "Plugin Added Todo Items.\n";

        // Perform the Zaurus side of the synchronization.
        zaurus.DelTodoItems(dDelTodoItemIDList);
        std::cout << "Zaurus deleted Del Todo Items.\n";
        zaurus.ModTodoItems(dModTodoItemList);
        std::cout << "Zaurus modified Mod Todo Items.\n";
        mapIdList = zaurus.AddTodoItems(dNewTodoItemList);
        std::cout << "Zaurus added Add Todo Items.\n";

        // Map the proper IDs.
        pTodoPlugin->MapItemIDs(mapIdList);
        std::cout << "Mapped item IDs.\n";
    } else {
        std::cout << "Note: Zaurus DOES require Full Sync.\n";
        std::cout << "Attempting to add todo items to the plugin.\n";
        pTodoPlugin->AddTodoItems(zNewTodoItemList);
        std::cout << "Added the todo items to the plugin.\n";

        std::cout << "Attempting to add todo items to the Zaurus.\n";
        mapIdList = zaurus.AddTodoItems(dNewTodoItemList);
        std::cout << "Added the todo items to the Zaurus.\n";

        std::cout << "Attempting to Map Item IDs.\n";
        pTodoPlugin->MapItemIDs(mapIdList);
        std::cout << "Mapped item IDs.\n";
    }

    zaurus.TerminateSync();
    std::cout << "Terminated the Synchronization with the Zaurus.\n";

    /////////////////////////////////////////////////////////////////////////
    // The code below needs to stay to handle destruction of the plugin and
    // closing of the shared object that is the plugin.
    /////////////////////////////////////////////////////////////////////////

    retval = pTodoPlugin->CleanUp();
    if (retval != 0) {
        std::cout << "ERROR: Failed to Clean up To-Do Plugin (" << retval;
        std::cout << ")." << std::endl;
        pDestroyFunc(pTodoPlugin);
        dlclose(libHandle);
        return 9;
    }
    std::cout << "Performed the Plugin Clean Up.\n";

    // Destroy the plugin object and close the plugin.
    pDestroyFunc(pTodoPlugin);
    dlclose(libHandle);

    std::cout << "Closed the plugin.\n";
    std::cout << "Exiting the PerformTodoSync() function.\n";

    return 0;
}

/**
 * Perform a Calendar synchronization.
 *
 * Perform a Calendar synchronization.
 */
int PerformCalendarSync(unsigned short int confWinner,
            ConfigManagerType *pConfManager) {
    void *libHandle;
    create_cal_t pCreateFunc;
    destroy_cal_t pDestroyFunc;
    CalendarPluginType *pPlugin;
    char optVal[256];
    int retval;
    ZaurusType zaurus;

    // The following three lists exist to contain the new, mod, del item
    // information obtained from the Zaurus for conflict management and
    // synchronization.
    CalendarItemType::List zNewCalItemList;
    CalendarItemType::List zModCalItemList;
    SyncIDListType zDelCalItemIDList;

    // The following three lists exist to contain the new, mod, del item
    // information obtain from the Desktop Todo Plugin for conflict management
    // and synchronization.
    CalendarItemType::List dNewCalItemList;
    CalendarItemType::List dModCalItemList;
    SyncIDListType dDelCalItemIDList;

    CalendarItemType::List mapIdList;

    SyncIDListType::iterator zSyncIDIter;
    time_t lastTimeSynced;
    CalendarItemType::List::iterator zIter;
    CalendarItemType curItem;

    retval = pConfManager->GetValue("cal_plugin_path", optVal, 256);
    if (retval != 0) {
        std::cout << "Error: No cal_plugin_path entry found in the" \
            " .zync.conf configuration file.\n";
        return 1;
    }

    // Open the plugin and load the creation and destroy symbols.
    libHandle = dlopen(optVal, RTLD_LAZY);
    if (!libHandle) {
        std::cout << "zync: Failed to open the Calendar Plugin.\n";
        std::cout << dlerror() << std::endl;
        return 2;
    }

    pCreateFunc = (create_cal_t)dlsym(libHandle, "createCalendarPlugin");
    if (!pCreateFunc) {
        std::cout << "zync: Failed to find the create symbol in plugin.\n";
        std::cout << dlerror() << std::endl;
        dlclose(libHandle);
        return 3;
    }

    pDestroyFunc = (destroy_cal_t)dlsym(libHandle, "destroyCalendarPlugin");
    if (!pDestroyFunc) {
        std::cout << "zync: Failed to find the destroy symbol in plugin.\n";
        std::cout << dlerror() << std::endl;
        dlclose(libHandle);
        return 4;
    }

    // Create an instance of the plugin.
    pPlugin = pCreateFunc();
    if (!pPlugin) {
        std::cout << "zync: Failed to create intance of the plugin object.\n";
        dlclose(libHandle);
        return 5;
    }

    // At this point I have opened the plugin and have loaded the creation and
    // destroy methods. This means I have the capability to create an instance
    // of the CalendarPluginType class and destroy it. Hence, I know that all
    // the class functions have been implemented (because they are pure
    // virtual) and assume that they work correctly. It should be fine to
    // perform the synchronization now.

    // Display the general plugin information.
    std::cout << "Plugin Information\n";
    std::cout << "------------------\n";
    std::cout << "Plugin Name: " << pPlugin->GetPluginName() + "\n";
    std::cout << "Plugin Version: " << pPlugin->GetPluginVersion() + "\n";
    std::cout << "Plugin Author: " << pPlugin->GetPluginAuthor() + "\n";
    std::cout << "Plugin Desc: " << pPlugin->GetPluginDescription() + "\n";
    std::cout << std::endl;

    // Attempt to initialize the plugin.
    retval = pPlugin->Initialize();
    if (retval != 0) {
        std::cout << "zync: Failed to Initialize Plugin (" << retval;
        std::cout << ")." << std::endl;
        pDestroyFunc(pPlugin);
        dlclose(libHandle);
        return 6;
    }

    std::cout << "zync: Initialized the plugin.\n";

    // Tell the Desktop synchronization server to listen for a connection from
    // a Zaurus.
    zaurus.ListenConnection();

    // Set the type of synchronization to the value that represents the
    // Calendar synchronization type.
    zaurus.SetSyncType(0x01);

    // Perform the actual synchronization.
    if(zaurus.RequiresPassword()) {
    // This first step in the authentication process is to obtain the
    // password to send to the Zaurus for authentication. In this case the
    // password should be in the config file. Hence, I try and obtain the
    // passcode from the config file.
    retval = pConfManager->GetValue("passcode", optVal, 256);
    if (retval != 0) {
        std::cout << "zync: Failed to obtain passcode from config.\n";
        zaurus.FinishSync();
        pDestroyFunc(pPlugin);
        dlclose(libHandle);
        return 7;
    }

    std::cout << "zync: Obtained passcode from config.\n";

    // The second phase of the Authentication process is authenticating
    // the password with the Zaurus. Hence, I attempt to authenticate the
    // obtained password.
    if (zaurus.AuthenticatePassword(optVal) != 0) {
        std::cout << "zync: Failed to authenticate password.\n";
        pDestroyFunc(pPlugin);
        dlclose(libHandle);
        return 8;
    }

    std::cout << "zync: Successfully authenticated the passcode.\n";
    }

    lastTimeSynced = zaurus.GetLastTimeSynced();

    std::cout << "zync: Obtained the Last Time Synchronized Anchor.\n";

    // If I move the KOrgTodoPlugin calls here I don't get the segfault with
    // them. However, if I leave them down below then I still get the
    // segfault.

    // Check if the Full Sync is required then try and clear the log, reset
    // the log and exit with out saving sync state. Hence, all items should be
    // seen as new items the next time one syncs (we hope).
    if (zaurus.RequiresFullSync()) {
        std::cout << "zync: Zaurus claimed that Full Sync is required.\n";
        zaurus.ResetSyncLog();
        std::cout << "zync: Reset the synchronization log.\n";
        zaurus.SetNextSyncAnch();
        std::cout << "zync: Set the next synchronization anchor.\n";
        zaurus.SendRSSMsg();
        std::cout << "zync: Sent the RSS message.\n";
    } else {
        std::cout << "zync: Zaurus claimed that Normal Sync is required.\n";
        zaurus.SetNextSyncAnch();
        std::cout << "zync: Set the nexct synchronization anchor.\n";
    }

    if (zaurus.ObtainParamInfo() != 0) {
        std::cout << "zync: Failed to ObtainParamInfo().\n";
    }
    std::cout << "zync: Obtained the parameter info for the sync.\n";

    // Obtain the changes from the Zaurus.
    if (zaurus.GetAllCalendarSyncItems(zNewCalItemList, zModCalItemList,
                       zDelCalItemIDList) != 0) {
        std::cout << "zync: Failed to get all sync items.\n";
    }

    std::cout << "zync: Obtained all the items from the Zaurus.\n";

    // Display the Zaurus changes information.
    std::cout << "Zaurus Changes\n";
    std::cout << "--------------\n";
    std::cout << "Found " << zNewCalItemList.size() << " new items on" \
    " the Zaurus.\n";
    std::cout << "Found " << zModCalItemList.size() << " modified items" \
    " on the Zaurus.\n";
    std::cout << "Found " << zDelCalItemIDList.size() << " items deleted" \
    " from the Zaurus.\n";

    // Obtain the changes from the Desktop PIM application todo plugin.
    if (zaurus.RequiresFullSync()) {
        std::cout << "zync: In Full Sync mode.\n";
        std::cout << "zync: Attemting to get all plugin items.\n";
        dNewCalItemList = pPlugin->GetAllCalendarItems();
        std::cout << "zync: Obtained all plugin items.\n";
    } else {
        std::cout << "zync: In Normal Sync mode.\n";
        std::cout << "zync: Obtaining New Items from plugin.\n";
        dNewCalItemList = pPlugin->GetNewCalendarItems(lastTimeSynced);
        std::cout << "zync: Obtained New Items from plugin.\n";
        std::cout << "zync: Obtaining Mod Items from plugin.\n";
        dModCalItemList = pPlugin->GetModCalendarItems(lastTimeSynced);
        std::cout << "zync: Obtained Mod Items from plugin.\n";
        std::cout << "zync: Obtaining Del Items from plugin.\n";
        dDelCalItemIDList = pPlugin->GetDelCalendarItemIDs(lastTimeSynced);
        std::cout << "zync: Obtained Del Items from plugin.\n";
    }

    std::cout << "zync: Obtained items from plugin.\n";

    // Display the plugin changes.
    std::cout << "Calendar Plugin Changes\n";
    std::cout << "-----------------------\n";
    std::cout << "Found " << dNewCalItemList.size() << " new items on" \
    " the plugin.\n";
    std::cout << "Found " << dModCalItemList.size() << " modified items" \
    " on the plugin.\n";
    std::cout << "Found " << dDelCalItemIDList.size() << " items deleted" \
    " from the plugin.\n";

    /*
    if (!zaurus.RequiresFullSync()) {
    // Compare the todo item lists for conflicts and resolve the conflicts.
    ResolveDelModConflicts(zDelCalItemIDList, zModCalItemList,
                   zNewCalItemList, dDelCalItemIDList,
                   dModCalItemList, dNewCalItemList);

    ResolveModModConflicts(zModCalItemList, dModCalItemList,
                   zNewCalItemList, dNewCalItemList,
                   confWinner);
    }
    */

    // I then want to do what I just did above but for the opposite
    // component. Once I do that the deletion conflicts should all be handled.

    if (!zaurus.RequiresFullSync()) {
    std::cout << "zync: In Normal Sync mode.\n";
    // Perform the Desktop side of the synchronization.
    std::cout << "Plugin About to Del Items.\n";
    pPlugin->DelCalendarItems(zDelCalItemIDList);
    std::cout << "Plugin Deleted Items.\n";
    std::cout << "Plugin About to Mod Items.\n";
    pPlugin->ModCalendarItems(zModCalItemList);
    std::cout << "Plugin Modified Items.\n";
    std::cout << "Plugin About to Add Items.\n";
    pPlugin->AddCalendarItems(zNewCalItemList);
    std::cout << "Plugin Added Todo Items.\n";

    // Perform the Zaurus side of the synchronization.
    std::cout << "zync: Zaurus about to delete items.\n";
    zaurus.DelCalendarItems(dDelCalItemIDList);
    std::cout << "zync: Zaurus deleted items.\n";
    std::cout << "zync: Zaurus about to mod items.\n";
    zaurus.ModCalendarItems(dModCalItemList);
    std::cout << "zync: Zaurus modified items.\n";
    std::cout << "zync: Zaurus about to Add items.\n";
    mapIdList = zaurus.AddCalendarItems(dNewCalItemList);
    std::cout << "zync: Zaurus added items.\n";

    std::cout << "zync: About to map IDs.\n";
    // Map the proper IDs.
    pPlugin->MapItemIDs(mapIdList);
    std::cout << "zync: Plugin mapped IDs.\n";
    } else {
    std::cout << "zync: In Full Sync Mode.\n";
    std::cout << "Attempting to add items to the plugin.\n";
    pPlugin->AddCalendarItems(zNewCalItemList);
    std::cout << "Added the items to the plugin.\n";

    std::cout << "Attempting to add items to the Zaurus.\n";
    mapIdList = zaurus.AddCalendarItems(dNewCalItemList);
    std::cout << "Added the items to the Zaurus.\n";

    std::cout << "Attempting to Map Item IDs.\n";
    pPlugin->MapItemIDs(mapIdList);
    std::cout << "Mapped item IDs.\n";
    }

    std::cout << "zync: About to Terminate Sync.\n";
    zaurus.TerminateSync();
    std::cout << "zync: Terminated Sync.\n";

    /////////////////////////////////////////////////////////////////////////
    // The code below needs to stay to handle destruction of the plugin and
    // closing of the shared object that is the plugin.
    /////////////////////////////////////////////////////////////////////////

    std::cout << "zync: About to clean up plugin.\n";
    retval = pPlugin->CleanUp();
    if (retval != 0) {
    std::cout << "ERROR: Failed to Clean up Plugin (" << retval;
    std::cout << ")." << std::endl;
    pDestroyFunc(pPlugin);
    dlclose(libHandle);
    return 9;
    }
    std::cout << "zync: Cleaned up the plugin.\n";

    std::cout << "zync: About to destroy plugin instance.\n";
    // Destroy the plugin object and close the plugin.
    pDestroyFunc(pPlugin);
    std::cout << "zync: Destroyed the plugin instance.\n";
    std::cout << "zync: About to close share object.\n";
    dlclose(libHandle);
    std::cout << "zync: Closed the shared object.\n";
    
    std::cout << "zync: FINISHED SYNC PROCESS.\n";

    return 0;
}
