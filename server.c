#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int fd;
    char name[32];
    int active;
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

void init_clients() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].fd = -1;
        clients[i].active = 0;
        memset(clients[i].name, 0, 32);
    }
}

int add_client(int fd, const char *name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) {
            clients[i].fd = fd;
            clients[i].active = 1;
            strncpy(clients[i].name, name, 31);
            client_count++;
            return i;
        }
    }
    return -1;
}

void remove_client(int index) {
    if (index >= 0 && index < MAX_CLIENTS && clients[index].active) {
        close(clients[index].fd);
        clients[index].active = 0;
        clients[index].fd = -1;
        memset(clients[index].name, 0, 32);
        client_count--;
    }
}

void broadcast_message(const char *message, int sender_index) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "[%s]: %s", 
             clients[sender_index].name, message);
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active && i != sender_index) {
            send(clients[i].fd, buffer, strlen(buffer), 0);
        }
    }
}

int main() {
    int server_fd, new_socket, max_sd, sd, activity;
    struct sockaddr_in address;
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    
    init_clients();
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // Bind to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Chat server started on port %d\n", PORT);
    printf("Waiting for connections...\n");
    
    int addrlen = sizeof(address);
    
    while (1) {
        // Clear and set file descriptor set
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;
        
        // Add client sockets to set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].fd;
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }
        
        // Wait for activity
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        
        if ((activity < 0) && (errno != EINTR)) {
            printf("Select error\n");
        }
        
        // Handle new connection
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                    (socklen_t*)&addrlen)) < 0) {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }
            
            // Get client name
            memset(buffer, 0, BUFFER_SIZE);
            recv(new_socket, buffer, BUFFER_SIZE, 0);
            
            int index = add_client(new_socket, buffer);
            if (index >= 0) {
                printf("New client connected: %s [fd: %d]\n", buffer, new_socket);
                char welcome[BUFFER_SIZE];
                snprintf(welcome, BUFFER_SIZE, 
                        "Welcome to chat server, %s! (Active users: %d)\n", 
                        buffer, client_count);
                send(new_socket, welcome, strlen(welcome), 0);
            } else {
                char *msg = "Server full. Connection rejected.\n";
                send(new_socket, msg, strlen(msg), 0);
                close(new_socket);
            }
        }
        
        // Handle client messages
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clients[i].fd;
            
            if (FD_ISSET(sd, &readfds)) {
                memset(buffer, 0, BUFFER_SIZE);
                int valread = recv(sd, buffer, BUFFER_SIZE, 0);
                
                if (valread == 0) {
                    // Client disconnected
                    printf("Client %s disconnected\n", clients[i].name);
                    remove_client(i);
                } else if (valread > 0) {
                    // Broadcast message
                    buffer[valread] = '\0';
                    printf("%s: %s", clients[i].name, buffer);
                    broadcast_message(buffer, i);
                }
            }
        }
    }
    
    close(server_fd);
    return 0;
}
