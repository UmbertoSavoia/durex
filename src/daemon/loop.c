#include "../../include/durex.h"

void 	set_fd(t_deamon *durex)
{
	FD_ZERO(&durex->fd_read);
	for (int i = 0; i < MAX_CLIENTS; ++i)
		if (durex->clients[i].fd != -1)
			FD_SET(durex->clients[i].fd, &durex->fd_read);
	FD_SET(durex->sockfd, &durex->fd_read);
}

void	add_client(t_deamon *durex)
{
	int new_fd = -1;
	struct sockaddr_in cli = {0};
	socklen_t len = sizeof(cli);

	new_fd = accept(durex->sockfd, (struct sockaddr *)&cli, &len);
	if (durex->connected_clients <= MAX_CLIENTS)
	{
		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			if (durex->clients[i].fd == -1)
			{
				durex->clients[i].fd = new_fd;
				send(durex->clients[i].fd, "ciao", 4, 0);
				durex->connected_clients++;
				break ;
			}
		}
	}
	else
	{
		send(new_fd, "Maximum capacity reached\n", 25, 0);
		close(new_fd);
	}
}

void 	server_loop(t_deamon *durex)
{
	while (1)
	{
		set_fd(durex);
		durex->fd_write = durex->fd_read;
		select(FD_SETSIZE, &durex->fd_read, &durex->fd_write, 0, 0);

		if (FD_ISSET(durex->sockfd, &durex->fd_read))
			add_client(durex);
	}
}