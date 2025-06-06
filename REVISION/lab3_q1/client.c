#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. PID: %d, PPID: %d\n", getpid(), getppid());

    // Fork process for full-duplex communication
    pid_t pid = fork();

    if (pid == 0) {  // Child process for receiving messages
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                printf("Server disconnected.\n");
                break;
            }
            printf("Server: %s\n", buffer);
        }
    } else {  // Parent process for sending messages
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            printf("Client: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character
            send(sockfd, buffer, strlen(buffer), 0);
        }
    }

    close(sockfd);
    return 0;
}
