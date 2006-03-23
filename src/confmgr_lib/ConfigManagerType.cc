/*
 * Copyright 2004 Andrew De Ponte
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

/*
 * Copyright 2004 Andrew De Ponte.
 *
 * This file is part of oam_player.
 *
 * oam_player is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * oam_player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with oam_player; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file ConfigManagerType.cc
 * @brief A implimentation file for the ConfigManager type.
 * @author Andrew De Ponte
 *
 * This type was developed to provide an easy to use interface to handle the
 * management of config files within applications, specifically oam_player.
 */

#include "ConfigManagerType.h"

/**
 * Constructs a default ConfigManagerType object.
 *
 * Constructs a default ConfigManagerType object by creating an object that
 * represents a physical config file. Since this is in the default case the
 * config starts out as an empty config file.
 */
ConfigManagerType::ConfigManagerType(void) {
    // Since I am implimenting this as a double linked list I need to set the
    // list control variables to their inital values ot state that the list is
    // empty.
    pFirstConfigItem = NULL;
    pLastConfigItem = NULL;
}

/**
 * Destructs the ConfigManagerType object.
 *
 * Destructs the ConfigManagerType object by deallocating any dynamically
 * allocated memory.
 */
ConfigManagerType::~ConfigManagerType(void) {
    struct ConfigItemType *pCurConfigItem, *pTempConfigItem;

    // Set the current config item pointer to the head of the list.
    pCurConfigItem = pFirstConfigItem;

    // Loop through the entire linked list deallocating all the dynamically
    // allocated config items.
    while (pCurConfigItem) {
	// Set the temp config item pointer to the next item in the list.
	pTempConfigItem = pCurConfigItem->pNextConfigItem;

	D_CONFMANAGER("ConfigManagerType: Destructor():: Removing title = %s" \
		      ", value = %s\n", pCurConfigItem->title,
		      pCurConfigItem->value);

	// Deallocated the current config item.
	delete pCurConfigItem;

	// Set the current config item pointer to the temp.
	pCurConfigItem = pTempConfigItem;
    }
}

/**
 * Open a config file in the appropriate format.
 *
 * Open a config file in the appropriate format, loading its contents into the
 * ConfigManagerType object.
 * @param pConfigPath A pointer to a character array that is the config path.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Successfully opened and loaded the config.
 * @retval -1 Failed to open the config specified by pConfigPath.
 * @retval -2 Failed to find an equals on a non comment line, but continued
 * loading what it could.
 */
int ConfigManagerType::Open(char *pConfigPath) {
    std::fstream inFile;
    char tmpFilePath[PATH_MAX];
    char value[256];
    char *pEquals;
    int retval;

    retval = 0;

    // Attempt to open the file at path pConfigPath for reading so I can
    // load the config data into the object.
    inFile.open(pConfigPath, std::ios::in);
    if (!inFile.is_open()) {
	    D_CONFMANAGER("ConfigManager: Open():: Failed to open config(%s)" \
		      " for reading.\n", pConfigPath);
	    return -1;
    }

    while (!inFile.eof()) {
	// Here I read the first line of the config filein.
	inFile.getline(tmpFilePath, PATH_MAX, '\n');

	if (tmpFilePath[0] != '#') {
	    // Well, if I am here I know that the line is not a comment so I
	    // want to find the fist equals sign because I know that any valid
	    // line in the config has to have a equals sign.
	    pEquals = index(tmpFilePath, (int)'=');

	    if (pEquals) {
		// Here I copy the value associated with the line into its own
		// character array so I can null terminate at pEquals and make
		// the name a string of its own.
		strcpy(value, pEquals+1);

		// Here, I null terminate at pEquals making it a c-string of
		// the name of the config option.
		*pEquals = '\0';

		D_CONFMANAGER("ConfigManagerType: Open():: Name = %s, " \
			      "Value = %s.\n", tmpFilePath, (pEquals + 1));

		AddItem(tmpFilePath, (pEquals + 1));
	    } else {
		if (strcmp(tmpFilePath, "") != 0) {
		    D_CONFMANAGER("ConfigManagerType: Open():: Failed to" \
				  " find the equals sign in the line read in" \
				  " entry.\n");

		    retval = -2;
		}
	    }
	}
    }

    // Here I close the opened playlist file.
    inFile.close();

    // Return in success.
    return retval;
}

/**
 * Save the object to a config file.
 *
 * Save the config items of the object to a simple formatted (easy to edit)
 * file for later loading.
 * @return An integer representing success (zero), failure (non-zero).
 * @retval 0 Successfully wrote the config file.
 * @retval -1 Failed to open pSavePath for writing.
 */
int ConfigManagerType::Save(char *pSavePath) {
    std::fstream outFile;
    struct ConfigItemType *pCurConfigItem;

    char buff[513];

    // Attempt to open the file at path pSavePath for writing so I can
    // save the data from the object into a config file.
    outFile.open(pSavePath, std::ios::out);
    if (!outFile.is_open()) {
	D_CONFMANAGER("ConfigManager: Save():: Failed to open config(%s)" \
		      " for writing.\n", pSavePath);
	return -1;
    }

    pCurConfigItem = GetFirstItem();

    // Loop through writing each items data to the file.
    while (pCurConfigItem) {
	snprintf(buff, 512, "%s=%s\n", pCurConfigItem->title,
		 pCurConfigItem->value);

	outFile.write(buff, strlen(buff));

	pCurConfigItem = pCurConfigItem->pNextConfigItem;
    }

    outFile.close();

    // Return in success.
    return 0;
}

/**
 * Set the value of a config item.
 *
 * Set the value of a config item given the title of the item and value. If
 * the item does not exist with the given title one is created.
 * @param pTitle A pointer to a character array containing the item title.
 * @param pValue A pointer to a character array containing the item value.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Success.
 * @retval X Any return value other than 0 consult AddItem() docs.
 */
int ConfigManagerType::SetValue(char *pTitle, char *pValue) {
    struct ConfigItemType *pCurConfigItem;

    pCurConfigItem = GetFirstItem();

    while (pCurConfigItem) {
	if (strcmp(pTitle, pCurConfigItem->title) == 0) {
	    D_CONFMANAGER("ConfigManagerType: SetValue():: Found a matching" \
			  " item.\n");

	    strncpy(pCurConfigItem->value, pValue, 256);

	    // Return in success.
	    return 0;
	}

	pCurConfigItem = pCurConfigItem->pNextConfigItem;
    }

    // If I hit this point I know that it failed to find a matching titled
    // item so I want to add one and return the return value of it.
    return AddItem(pTitle, pValue);
}

/**
 * Get the value of a config item.
 *
 * Obtain the value of a config item given the title of the item and a
 * character array to store the result in.
 * @param pTitle A pointer to a character array containing the item title.
 * @param pValue A pointer to a charactor array to store the value in.
 * @param maxlen The maximum len of characters to write to pValue.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Success.
 * @retval -1 Failed to find matching titled item.
 */
int ConfigManagerType::GetValue(char *pTitle, char *pValue, int maxlen) {
    struct ConfigItemType *pCurConfigItem;

    pCurConfigItem = GetFirstItem();

    while (pCurConfigItem) {
	if (strcmp(pTitle, pCurConfigItem->title) == 0) {
	    D_CONFMANAGER("ConfigManagerType: GetValue():: Found a matching" \
			  " item.\n");

	    strncpy(pValue, pCurConfigItem->value, maxlen);

	    // Return in success.
	    return 0;
	}

	// Go on to the next config item.
	pCurConfigItem = pCurConfigItem->pNextConfigItem;
    }

    // Return -1 to signify that no matching item was found.
    return -1;
}

/**
 * Delete an item.
 *
 * Delete an item given the name of the item.
 * @param pTitle A pointer to a character array containing the item title.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Success.
 * @retval -1 Failed to find matching titled item.
 */
int ConfigManagerType::DeleteItem(char *pTitle) {
    struct ConfigItemType *pCurConfigItem;

    pCurConfigItem = GetFirstItem();

    while (pCurConfigItem) {
	if (strcmp(pTitle, pCurConfigItem->title) == 0) {
	    D_CONFMANAGER("ConfigManagerType: GetValue():: Found a matching" \
			  " item.\n");

	    if ((pCurConfigItem->pNextConfigItem == NULL) &&
		(pCurConfigItem->pPrevConfigItem == NULL)) {
		pFirstConfigItem = NULL;
		pLastConfigItem = NULL;
	    } else if (pCurConfigItem->pNextConfigItem == NULL) {
		pCurConfigItem->pPrevConfigItem->pNextConfigItem = NULL;
		pLastConfigItem = pCurConfigItem->pPrevConfigItem;
	    } else if (pCurConfigItem->pPrevConfigItem == NULL) {
		pCurConfigItem->pNextConfigItem->pPrevConfigItem = NULL;
		pFirstConfigItem = pCurConfigItem->pNextConfigItem;
	    } else {
		pCurConfigItem->pPrevConfigItem->pNextConfigItem = pCurConfigItem->pNextConfigItem;
		pCurConfigItem->pNextConfigItem->pPrevConfigItem = pCurConfigItem->pPrevConfigItem;
	    }

	    delete pCurConfigItem;

	    // Return in success.
	    return 0;
	}

	// Go on to the next config item.
	pCurConfigItem = pCurConfigItem->pNextConfigItem;
    }

    // Return -1 to signify that no matching item was found.
    return -1;
}

/**
 * Add an item to the config manager.
 *
 * Add a new item to the config manager given the title of the item and the
 * value to be associated with the item. Note: There is no protection from
 * creating multiple items with the same title. The results of such an action
 * are undefined.
 * @param pTitle Pointer to character array containing the title of the item.
 * @param pValue Pointer to character array containing the value of the item.
 * @return An integer representing success (zero) or failure (non-zero).
 * @retval 0 Success.
 * @retval -1 Failed to allocate memory for the new config item.
 */
int ConfigManagerType::AddItem(char *pTitle, char *pValue) {
    struct ConfigItemType *pNewConfigItem;

    // Allocate momory for the new config item.
    pNewConfigItem = new struct ConfigItemType;
    if (pNewConfigItem == NULL) {
	D_CONFMANAGER("ConifgManagerType: AddItem():: Failed to allocate" \
		      " memory for the now config item.\n");
	return -1;
    }

    // Copy the title and value into the new config item.
    strncpy(pNewConfigItem->title, pTitle, 256);
    strncpy(pNewConfigItem->value, pValue, 256);

    // Check if this is the first item in the linked list.
    if (pFirstConfigItem == NULL) {
	// If it is the first node in the doubly linked list then I know that
	// its previous pointer MUST BE NULL and its next MUST BE NULL.
	pNewConfigItem->pPrevConfigItem = NULL;
	pNewConfigItem->pNextConfigItem = NULL;

	// So now that I have the enxt and previous pointers of the new node
	// handled I need to have the root or start of the list point to this
	// node since it is the first item in the list. Also since its the
	// only node in the list lets set it to be the last node as well.
	pFirstConfigItem = pNewConfigItem;
	pLastConfigItem = pNewConfigItem;
    } else {
	// If it is NOT the first node in the doubly linked list then it is
	// going to be the last because this function appends a new node to
	// the doubly linked list. The last node in a doubly linked list has
	// its previous pointer point to the old last node and its next
	// pointer pointing to NULL.
	pNewConfigItem->pPrevConfigItem = pLastConfigItem;
	pNewConfigItem->pNextConfigItem = NULL;

	pNewConfigItem->pPrevConfigItem->pNextConfigItem = pNewConfigItem;

	// Now since I have appended a node to the list I want to set the old
	// tail pointer to point to the new node.
	pLastConfigItem = pNewConfigItem;
    }

    // Return in success.
    return 0;
}

/**
 * Get the first config item.
 *
 * Obtain the first config item from the list.
 * @return A pointer to the playlist item at head. If fails then NULL.
 * @retval NULL Failed to retrieve the item.
 */
struct ConfigItemType *ConfigManagerType::GetFirstItem(void) {
    if (pFirstConfigItem == NULL) {
	D_CONFMANAGER("ConfigManagerType: GetFirstItem():: Failed to" \
		      " retrieve config item -- pFirstConfigItem is equal" \
		      " to NULL. Hence, there are no items in the config" \
		      " object at this point.\n");
	return NULL;
    } else {
	return pFirstConfigItem;
    }
}

/**
 * Get the last config item.
 *
 * Obtain the last config item from the list.
 * @return A pointer to the config item at tail. If fails then NULL.
 * @retval NULL Failed to retrieve the item.
 */
struct ConfigItemType *ConfigManagerType::GetLastItem(void) {
    if (pLastConfigItem == NULL) {
	D_CONFMANAGER("ConfigManagerType: GetLastItem():: Failed to retrieve" \
		      " config item -- pLastConfigItem is equal to NULL." \
		      " Hence, there are no items in the config object at" \
		      " this point.\n");
	return NULL;
    } else {
	return pLastConfigItem;
    }
}
