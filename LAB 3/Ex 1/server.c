#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 3388
#define MAXSIZE 1024

void handle_client(int client_sockfd) {
    char buffer[MAXSIZE];
    int recedbytes, sentbytes;

    printf("Child process PID: %d, Parent PID: %d\n", getpid(), getppid());

    while (1) {
        // Receive message from client
        recedbytes = recv(client_sockfd, buffer, sizeof(buffer), 0);
        if (recedbytes == -1) {
            perror("Receive error");
            break;
        }
        if (recedbytes == 0) {
            printf("Client disconnected\n");
            break;
        }

        buffer[recedbytes] = '\0';
        printf("Client: %s\n", buffer);

        // Send a message to the client
        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

        sentbytes = send(client_sockfd, buffer, strlen(buffer), 0);
        if (sentbytes == -1) {
            perror("Send error");
            break;
        }
    }

    close(client_sockfd);
    exit(0);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    pid_t childpid;

    // Create server socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // Set server address structure
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("Binding error");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("Listen error");
        close(sockfd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection and handle communication in a child process
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &clientlen);
        if (newsockfd == -1) {
            perror("Accept error");
            continue;
        }

        printf("Client connected\n");

        // Create a child process to handle the client
        childpid = fork();
        if (childpid == -1) {
            perror("Fork error");
            close(newsockfd);
            continue;
        }

        if (childpid == 0) {
            // Child process handles communication
            close(sockfd);  // Close the server socket in the child
            handle_client(newsockfd);
        } else {
            close(newsockfd);  // Parent doesn't need the client socket
        }
    }

    close(sockfd);
    return 0;
}
