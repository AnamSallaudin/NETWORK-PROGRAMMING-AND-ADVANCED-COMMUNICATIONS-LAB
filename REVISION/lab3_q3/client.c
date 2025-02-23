#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Get input from user
    printf("Enter an alphanumeric string: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline

    // Send input to server
    send(sockfd, buffer, strlen(buffer), 0);

    // Receive response from server (child process)
    recv(sockfd, response, BUFFER_SIZE, 0);
    printf("Response from server: %s\n", response);

    // Receive response from server (parent process)
    recv(sockfd, response, BUFFER_SIZE, 0);
    printf("Response from server: %s\n", response);

    // Close socket
    close(sockfd);
    return 0;
}
