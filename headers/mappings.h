/*
 * mappings.h
 *
 *  Created on: Jul 2, 2020
 *      Author: khubaibumer
 */

#ifndef MAPPINGS_H_
#define MAPPINGS_H_

#include <rpc.h>

typedef struct {
	int key;
	char *value;
} UserInfoTable_t;

static const UserInfoTable_t UserInfoTable[] = {
		/*	User Information Table	*/
		{ userid, "UserName" },
		{ userpass, "UserPsswd" },
		{ usermode, "UserMode" },
		{ userlogged, "IsLogged" },

		/*	Item Information Table	*/
		{ itemupc, "UPC" },
		{ itemprice, "Price" },
		{ itemcount, "Quantity" },
		{ itemname, "Name" },

		/*	Item Image Table	*/
		{ images, "Image" },
		{ imageextra, "Extra" },
};

static inline const char* get_mapping(int key) {
	for (int i = 0; i < sizeof(UserInfoTable) / sizeof(UserInfoTable_t); i++) {
		if (UserInfoTable[i].key == key)
			return UserInfoTable[i].value;
	}

	return NULL;
}


#endif /* MAPPINGS_H_ */
