/*
 * writer.c
 *
 *  Created on: May 28, 2020
 *      Author: khubaiumer
 */

#include <thor.h>
#include <sys/socket.h>

extern ssize_t send(int __fd, const void *__buf, size_t __n, int __flags);

char* attach_footer(char *buf, size_t *len) {
	char *_buf = calloc(*len + 15, sizeof(char));
	sprintf(_buf, "%s,<$#EOT#$>\n", buf);
	*len = strnlen(_buf, *len);

	return _buf;
}

int thor_writer(void *ptr, const void *buf, size_t len) {

	int ret = -1;
	buf = attach_footer(buf, &len);

	if (CAST(ptr)->use_ssl) {
		ret = SSL_write(CAST(ptr)->ssl_tls.ssl, buf, len);
	} else {
		ret = send(CAST(ptr)->client.fd, buf, len, 0);
	}

	free(buf);
	return ret;
}

int thor_reader(void *ptr, void *buf, size_t len) {

	if (CAST(ptr)->use_ssl) {
		return (SSL_read(CAST(ptr)->ssl_tls.ssl, buf, len));
	} else {
		return (recv(CAST(ptr)->client.fd, buf, len, 0));
	}

	return -1;
}

