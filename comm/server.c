/*
 * server.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>

void __up() {

	printf("%s Called!\n", __func__);
}

void __listen() {

	printf("%s Called!\n", __func__);
}

void __accept() {

	printf("%s Called!\n", __func__);
}

void __list() {
	printf("%s Called!\n", __func__);
}

void __kick(int _fd) {
	printf("%s Called!\n", __func__);
}

void __down() {
	printf("%s Called!\n", __func__);
}
