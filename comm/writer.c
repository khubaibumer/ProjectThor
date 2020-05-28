/*
 * writer.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaiumer
 */

#include <thor.h>
#include <sys/socket.h>

extern ssize_t send (int __fd, const void *__buf, size_t __n, int __flags);

int thor_writer(void *ptr, const void *buf, size_t len) {

	if (CAST(ptr)->use_ssl) {
		return (SSL_write(CAST(ptr)->ssl_tls.ssl, buf, len));
	} else {
		return (send(CAST(ptr)->client.fd, buf, len, 0));
	}

	return -1;
}

int thor_reader(void *ptr, void *buf, size_t len) {

	if (CAST(ptr)->use_ssl) {
		return (SSL_read(CAST(ptr)->ssl_tls.ssl, buf, len));
	} else {
		return (recv(CAST(ptr)->client.fd, buf, len, 0));
	}

	return -1;
}

