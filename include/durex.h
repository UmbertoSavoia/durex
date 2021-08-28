#ifndef DUREX_H
#define DUREX_H

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdbool.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
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
	char 	read_buffer[MAX_BUFF];
	char 	write_buffer[MAX_BUFF];
}				t_client;

typedef	struct	s_deamon
{
	int 		sockfd;
	t_client	clients[MAX_CLIENTS];
	fd_set		fd_write;
	fd_set		fd_read;
}				t_deamon;

#endif
