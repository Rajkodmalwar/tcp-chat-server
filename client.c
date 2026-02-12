#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int sock = 0;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    int valread;
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        
        if (valread > 0) {
            printf("%s", buffer);
            fflush(stdout);
        } else if (valread == 0) {
            printf("\nDisconnected from server\n");
            exit(0);
        }
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char name[32];
    pthread_t recv_thread;
    
    // Get username
    printf("Enter your name: ");
    fgets(name, 32, stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address\n");
        return -1;
    }
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }
    
    // Send username
    send(sock, name, strlen(name), 0);
    
    // Create receive thread
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    
    printf("Connected to server. Start chatting!\n");
    
    // Send messages
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        
        if (strlen(buffer) > 1) {
            send(sock, buffer, strlen(buffer), 0);
        }
    }
    
    close(sock);
    return 0;
}
