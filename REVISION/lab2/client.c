#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_address;
    char filename[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection to server failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter filename (or 'stop' to exit): ");
        fgets(filename, BUFFER_SIZE, stdin);
        filename[strcspn(filename, "\n")] = 0;  // Remove newline

        write(sock, filename, strlen(filename));

        if (strcmp(filename, "stop") == 0) {
            printf("Exiting...\n");
            break;
        }

        memset(response, 0, BUFFER_SIZE);
        read(sock, response, BUFFER_SIZE);
        printf("\nServer Response:\n%s\n", response);
    }

    close(sock);
    return 0;
}
