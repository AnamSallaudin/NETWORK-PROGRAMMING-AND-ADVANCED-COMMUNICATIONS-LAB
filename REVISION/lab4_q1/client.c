#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char option[10];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Choose an option:\n1. Registration Number\n2. Student Name\n3. Subject Code\nEnter: ");
    fgets(option, sizeof(option), stdin);
    
    write(sock, option, strlen(option));

    read(sock, buffer, BUFFER_SIZE);
    printf("Response from Server:\n%s\n", buffer);

    close(sock);
    return 0;
}
