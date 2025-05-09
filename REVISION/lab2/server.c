#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void analyzeFile(const char *filename, char *response) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        sprintf(response, "File not found");
        return;
    }

    char ch;
    int fileSize = 0, alphabets = 0, lines = 0, spaces = 0, digits = 0, others = 0;
    
    while ((ch = fgetc(file)) != EOF) {
        fileSize++;
        if (isalpha(ch))
            alphabets++;
        else if (isdigit(ch))
            digits++;
        else if (isspace(ch)) {
            spaces++;
            if (ch == '\n') lines++;
        } else {
            others++;
        }
    }
    fclose(file);

    sprintf(response, "File Size: %d bytes\nAlphabets: %d\nLines: %d\nSpaces: %d\nDigits: %d\nOther Characters: %d\n", 
            fileSize, alphabets, lines, spaces, digits, others);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytesReceived = read(new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived <= 0) break;

            buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
            printf("Client requested file: %s\n", buffer);

            if (strcmp(buffer, "stop") == 0) {
                printf("Client terminated the connection.\n");
                break;
            }

            char response[BUFFER_SIZE] = {0};
            analyzeFile(buffer, response);

            write(new_socket, response, strlen(response));
        }
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
