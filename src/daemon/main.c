#include "../../include/durex.h"

char 	init(t_deamon *durex)
{
	struct sockaddr_in servaddr;

	if ((durex->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return 1;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if ((bind(durex->sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		return 1;
	if (listen(durex->sockfd, MAX_CLIENTS) != 0)
		return 1;

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		durex->clients[i].fd = -1;
		durex->clients[i].auth = false;
	}
	return 0;
}

int 	main(int ac, char **av, char **env)
{
	//daemon(0, 0);
	(void)ac;
	(void)av;
	t_deamon durex = {0};

	if (init(&durex))
		return 1;
	server_loop(&durex, env);
}