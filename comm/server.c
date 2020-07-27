/*
 * server.c
 *
 *  Created on: May 27, 2020
 *      Author: khubaibumer
 */

#include <thor.h>
#include <list.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>

#include <sys/sendfile.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __SOCKET_UTILITY__
#define __SOCKET_UTILITY__

enum {
	INET, UNIX
};

/*	_Type can be INET/UNIX, _SRV_PATH can be IP/Filesystem Path
 *
 * This Function Returns FileDescriptor of a running specified Server.
 * User should call accept() on this descriptor to recieve an incomming connection and deal accordingly
 *
 * 	*/
#define CREATE_SOCKET_SERVER(_Type, _SRV_PATH, _PORT, _MAX_CLIENTS)										\
	({																	\
		int srv_fd = 0;															\
		if(_Type == INET)														\
			srv_fd = CREATE_INET_SERVER(_SRV_PATH, _PORT, _MAX_CLIENTS);								\
		else if(_Type == UNIX)														\
			srv_fd = CREATE_UNIX_SERVER(_SRV_PATH, _MAX_CLIENTS);									\
		assert(srv_fd != -1);														\
		(srv_fd);															\
	})

#define CREATE_INET_SERVER(_SRV_PATH, _PORT, _MAX_CLIENTS) 											\
		({																\
			int server_fd = -1;													\
			struct sockaddr_in server_addr = { };											\
			server_fd = socket(AF_INET, SOCK_STREAM, 0);										\
			/*setsockopt(server_fd, SOL_TCP, TCP_NODELAY, &(int){1}, 4);	\
			setsockopt(server_fd, SOL_SOCKET, SO_KEEPALIVE, &(int){1}, 4);	\
			setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, 4); 	\
			int flags = fcntl(server_fd,F_GETFL,0);	\
			assert(flags != -1);	\
			fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);*/	\
			assert(server_fd != -1);												\
			server_addr.sin_family = AF_INET;											\
			server_addr.sin_port = htons(_PORT);											\
			server_addr.sin_addr.s_addr = inet_addr((const char*) _SRV_PATH);							\
			memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));								\
			assert(bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == 0);					\
			assert(listen(server_fd, _MAX_CLIENTS) == 0);										\
			(server_fd);														\
		})

#define CREATE_UNIX_SERVER(_SRV_PATH, _MAX_CLIENTS) 												\
	({																	\
		int server_fd = -1;														\
		struct sockaddr_un server_addr = { };												\
		unlink((const char*) _SRV_PATH);												\
		server_fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);											\
		assert(server_fd != -1);													\
		memset(&server_addr, 0, sizeof(server_addr.sun_path));										\
		server_addr.sun_family = AF_UNIX;												\
		strncpy(server_addr.sun_path, _SRV_PATH, sizeof(server_addr.sun_path) - 1);							\
		assert(bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == 0);						\
		assert(listen(server_fd, _MAX_CLIENTS) == 0);											\
		(server_fd);															\
	})

/*
 *	_Type Can be Unix/TCP, _SRV_PATH can be filesystem Path or IPv4, Port is only required in case of TCP Client
 *	This funtion gives back filedescriptor for client socket and user is expected to send/recieve data using this fd
 *
 *	*/
#define CREATE_SOCKET_CLIENT(_TYPE, _SRV_PATH, _PORT) 												\
	({																	\
		int cli_fd = 0;															\
		if(_TYPE == INET)														\
			cli_fd = CREATE_INET_CLIENT(_SRV_PATH, _PORT);										\
		else if(_TYPE == UNIX)														\
			cli_fd = CREATE_UNIX_CLIENT(_SRV_PATH);											\
		assert(cli_fd != -1);														\
		(cli_fd);															\
	})

#define CREATE_INET_CLIENT(_SRV_PATH, _PORT) 													\
	({																	\
		int client_fd = -1;														\
		struct sockaddr_in client_addr = { };												\
		socklen_t addr_size;														\
		client_fd = socket(AF_INET, SOCK_STREAM, 0);											\
		assert(client_fd != -1);													\
		client_addr.sin_family = AF_INET;												\
		client_addr.sin_port = htons(_PORT);												\
		client_addr.sin_addr.s_addr = inet_addr((const char*) _SRV_PATH);								\
		memset(client_addr.sin_zero, 0, sizeof(client_addr.sin_zero));									\
		addr_size = sizeof(client_addr);												\
		assert(connect(client_fd, (struct sockaddr*) &client_addr, addr_size) == 0);							\
		(client_fd);															\
	})

#define CREATE_UNIX_CLIENT(_SRV_PATH)														\
	({																	\
		int client_fd = -1;														\
		struct sockaddr_un client_addr = { };												\
		client_fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);											\
		assert(client_fd != -1);													\
		memset(&client_addr, 0, sizeof(struct sockaddr_un));										\
		client_addr.sun_family = AF_UNIX;												\
		strncpy(client_addr.sun_path, _SRV_PATH, sizeof(client_addr.sun_path) - 1);							\
		assert(connect(client_fd, (struct sockaddr*) &client_addr, sizeof(struct sockaddr_un)) == 0);					\
		(client_fd);															\
	})

#define CLOSE_SOCKET_CONN(_TYPE, _SFD)														\
	({																	\
		assert(shutdown(_SFD, SHUT_RDWR) != 0);												\
		close(_SFD);															\
	})

#endif /*	__SOCKET_UTILITY__	*/

DECLARE_STATIC_SYMBOL(const struct timeval, timeout) = { .tv_sec = 0, .tv_usec =
		300 * 1000 };

PRIVATE
void get_ip(void *ptr) {
	DECLARE_SYMBOL(struct ifaddrs, *ifAddrStruct) = NULL;
	DECLARE_SYMBOL(struct ifaddrs, *ifa) = NULL;
	DECLARE_SYMBOL(void, *tmpAddrPtr) = NULL;

	if (getifaddrs(&ifAddrStruct) != 0)
		perror("Error Getting NIC");

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr) {
			if (ifa->ifa_addr->sa_family == AF_INET) { // Check it is
				// a valid IPv4 address
				tmpAddrPtr = &((struct sockaddr_in*) ifa->ifa_addr)->sin_addr;
				DECLARE_SYMBOL(char, addressBuffer[INET_ADDRSTRLEN]) = {};
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				if (strcmp(addressBuffer, "127.0.0.1") != 0) {
					CAST(ptr)->server.ip = strdup(addressBuffer);
				}
			}
		}
	}
	if (ifAddrStruct != NULL)
		freeifaddrs(ifAddrStruct);
}

PRIVATE
void __up(void *ptr) {

	CAST(ptr)->set_state(STATE_HELD);
	assert(
			pthread_create(&CAST(ptr)->thread.tid, NULL, CAST(ptr)->thread.thread_func, ptr) == 0);

	CAST(ptr)->ssl_tls.ssl_init(ptr);

	if (CAST(ptr)->server.ip == NULL)
		get_ip(ptr);

	if (CAST(ptr)->server.port == 0)
		CAST(ptr)->server.port = 50002;

	CAST(ptr)->server.sock.fd = CREATE_INET_SERVER(CAST(ptr)->server.ip,
			CAST(ptr)->server.port, CAST(ptr)->client.max_count);

	size_t ipl = strlen(CAST(ptr)->server.ip);

	char *url = calloc(ipl + 15, sizeof(char)); // should be large enough to have IP:PORT

	sprintf(url, "%s:%d", CAST(ptr)->server.ip, CAST(ptr)->server.port);

	CAST(ptr)->ui.to_ui(ptr, stdout, url);
	CAST(ptr)->ui.to_ui(ptr, log.get(), url); // write to logfile as well

	log.i("::Server Info::\n");
	log.i("IP:%s, Port:%d\n", CAST(ptr)->server.ip, CAST(ptr)->server.port);

	CAST(ptr)->set_state(STATE_RUN);

}

PRIVATE
void __accept(void *ptr) {

	while (CAST(ptr)->ctrl.actv_client_count < /*4*/CAST(ptr)->client.max_count) {
		// Accept a connection
		// Get user-name & passwd
		// Authenticate
		// Add to active user list
		struct sockaddr_in clientAddr = { };
		int len = sizeof(clientAddr);
		int cfd = accept(CAST(ptr)->server.sock.fd,
				(struct sockaddr*) &clientAddr, (socklen_t*) &len);
		if (CAST(ptr)->get_state() == STATE_CLOSE)
			if (CAST(ptr)->server.down)
				CAST(ptr)->server.down(ptr);

		int usr_lvl = CAST(ptr)->user.secure.auth(ptr, cfd);
		switch (usr_lvl) {
		case DFL_USR:
		case ELVT_USR:
		case ROOT_USR: {
			void *dnode = CAST(ptr)->mknod(usr_lvl);
			CAST(dnode)->client.mode = usr_lvl;
			CAST(dnode)->client.fd = cfd;
			CAST(dnode)->is_logged = CAST(ptr)->tmp_cli_info.log_bit;
			setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout,
					sizeof(timeout));
			CAST(dnode)->client.port = clientAddr.sin_port;
			CAST(dnode)->client.ip = strdup(inet_ntoa(clientAddr.sin_addr));
			CAST(dnode)->client.is_connected = 1;
			if (CAST(ptr)->use_ssl) {
				CAST(dnode)->ssl_tls.ssl = SSL_dup(
				CAST(ptr)->tmp_cli_info.tssl);
				CAST(dnode)->ssl_tls.bio = CAST(ptr)->tmp_cli_info.tbio;
				CAST(dnode)->ssl_tls.session = SSL_get1_session(
				CAST(dnode)->ssl_tls.ssl);
				/*	Reset	*/
				CAST(ptr)->tmp_cli_info.tssl = NULL;
				CAST(ptr)->tmp_cli_info.tbio = NULL;
			}

			char response[25] = { };
			sprintf(response, "auth,ok,%d", usr_lvl);

			if (CAST(dnode)->ssl_tls.write(dnode, response,
					strlen(response) + 1) <= 0)
				perror("Error Writing!");

			insert_node(&CAST(ptr)->ctrl.list_head, dnode);
			++CAST(ptr)->ctrl.actv_client_count;
			CAST(ptr)->set_state(STATE_RUN);
		}
			break;
		default: {
			CAST(ptr)->server.kick(ptr, cfd);
			continue;
		}
			break;
			;
		};
	}
}

PRIVATE
void print_nodes(data_node_t *node, char *arg) {
	log.v("IP:%s, Port:%d, Mode:%d\n", CAST(node->data)->client.ip,
	CAST(node->data)->client.port, CAST(node->data)->user.uid);
	size_t len = strlen(arg);
	sprintf(&arg[len],
			" { \"IP\" : \"%s\" , \"PORT\" : \"%d\" , \"MODE\" : \"%d\" , \"UID\" : \"%d\" } ,",
			GETTHOR(node)->client.ip, GETTHOR(node)->client.port,
			GETTHOR(node)->client.mode, GETTHOR(node)->user.uid);
}

PRIVATE
void __list(void *ptr) {
	log.v("::Active Client List::\n");
	CAST(ptr)->rpc.return_value.ret.value = calloc(MB(2), sizeof(char));
	sprintf(CAST(ptr)->rpc.return_value.ret.value, "%s ", "[");
	foreach_node_callback(&CAST(THIS)->ctrl.list_head, print_nodes,
			CAST(ptr)->rpc.return_value.ret.value);

	CAST(ptr)->rpc.return_value.ret.len = strlen(
			CAST(ptr)->rpc.return_value.ret.value);
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len
			- 1] = ']';
	CAST(ptr)->rpc.return_value.ret.value[CAST(ptr)->rpc.return_value.ret.len] =
			'\0';
}

PRIVATE
void __kick(void *ptr, int _fd) {

	if (CAST(ptr)->use_ssl) {
		SSL_write(CAST(ptr)->tmp_cli_info.tssl, "auth,fail,-1,<$#EOT#$>\n",
				sizeof("auth,fail,-1,<$#EOT#$>\n"));
	} else {
		send(_fd, "You are being kicked out!", 1024, 0);
	}

	shutdown(_fd, SHUT_RDWR);
	close(_fd);
}

PRIVATE
void __close_client(void *ptr) {

	CAST(ptr)->client.is_connected = 0;
	if (CAST(ptr)->use_ssl) {
		SSL_SESSION_free(CAST(ptr)->ssl_tls.session);
		BIO_free_all(CAST(ptr)->ssl_tls.bio);
		SSL_CTX_free(CAST(ptr)->ssl_tls.ctx);
		SSL_free(CAST(ptr)->ssl_tls.ssl);
		close(CAST(ptr)->client.fd);
	} else {
		close(CAST(ptr)->client.fd);
	}

}

PRIVATE
void close_all_clients(data_node_t *node, void *ptr) {

	GETTHOR(node)->client.is_connected = 0;
	if (GETTHOR(node)->use_ssl) {
		SSL_SESSION_free(GETTHOR(node)->ssl_tls.session);
		BIO_free_all(GETTHOR(node)->ssl_tls.bio);
		SSL_CTX_free(GETTHOR(node)->ssl_tls.ctx);
		SSL_free(GETTHOR(node)->ssl_tls.ssl);
		close(GETTHOR(node)->client.fd);
	} else {
		close(GETTHOR(node)->client.fd);
	}
}

PRIVATE
void __down(void *ptr) {

	CAST(ptr)->set_state(STATE_CLOSE);
	foreach_node_callback(&CAST(ptr)->ctrl.list_head, close_all_clients, NULL);
	foreach_node_free(&CAST(ptr)->ctrl.list_head);
	close(CAST(ptr)->server.sock.fd);

	pthread_join(CAST(ptr)->thread.tid, 0);
}
