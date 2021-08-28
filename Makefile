TARGET = Durex
DAEMON = durex_daemon

CC = gcc
CFLAGS = -Wall -Werror -Wextra

RM = rm -f

PATH_DEAMON = ./src/daemon/
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
SRCS_DAEMON = $(wildcard src/daemon/*.c)
OBJS_DAEMON = $(SRCS_DAEMON:.c=.o)

all: $(TARGET) $(DAEMON)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

$(DAEMON) : $(OBJS_DAEMON)
	$(CC) $(CFLAGS) $^ -o $(addprefix $(PATH_DEAMON), $(DAEMON))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_DAEMON)

fclean: clean
	$(RM) $(TARGET) $(addprefix $(PATH_DEAMON), $(DAEMON))

cleanall: fclean
	$(RM) /etc/init.d/Durex /bin/Durex /var/run/Durex.pid

re: fclean all