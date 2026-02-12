CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET_SERVER = server
TARGET_CLIENT = client

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): server.c
	$(CC) $(CFLAGS) -o $(TARGET_SERVER) server.c

$(TARGET_CLIENT): client.c
	$(CC) $(CFLAGS) -o $(TARGET_CLIENT) client.c

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT)

.PHONY: all clean
