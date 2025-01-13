#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char filename[MAX], search_str[MAX], replace_str[MAX], buffer[MAX];
    socklen_t server_len = sizeof(server_addr);
    int choice, n;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Input file name to search
    printf("Enter the filename to operate on: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';  // Remove newline character

    // Send filename to server
    send(sockfd, filename, strlen(filename), 0);

    // Receive server response (file check)
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    printf("Server response: %s\n", buffer);
    if (strcmp(buffer, "File not present") == 0) {
        close(sockfd);
        return 0;
    }

    // Show menu
    printf("Choose an option:\n1. Search\n2. Replace\n3. Reorder\n4. Exit\n");
    scanf("%d", &choice);
    send(sockfd, &choice, sizeof(choice), 0);

    if (choice == 3) {
        // Reorder the content in file
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        buffer[n] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
