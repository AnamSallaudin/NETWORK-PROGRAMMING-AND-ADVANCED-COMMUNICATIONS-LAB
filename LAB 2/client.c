// Client Program (TCP)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

void client_tcp() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    // Get the user's input for the selected option
    printf("Enter the option (registration_number, student_name, subject_code): ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

    send(sockfd, buffer, strlen(buffer), 0);

    bzero(buffer, MAX_BUFFER_SIZE);
    n = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Server Response: %s\n", buffer);

    close(sockfd);
}

int main() {
    client_tcp();
    return 0;
}
