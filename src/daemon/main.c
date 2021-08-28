#include "../../include/durex.h"

char 	init(t_deamon *durex, struct sockaddr_in *servaddr)
{
	if ((durex.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return 1;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if ((bind(durex.sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		return 1;
	if (listen(durex.sockfd, MAX_CLIENTS) != 0)
		return 1;
}

int 	main(void)
{
	//daemon(0, 0);
	t_deamon durex = {0};
	struct sockaddr_in servaddr, cli;
	socklen_t len = sizeof(cli);

	if (init(&durex, &servaddr))
		return 1;
}