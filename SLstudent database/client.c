#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){

    int sock;
    struct sockaddr_in serv_addr;
    char input[BUFFER_SIZE], buffer[BUFFER_SIZE];

    sock=socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("Select an option:\n1. Registration Number\n2. Student Name\n3. Subject Code\nEnter choice: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")]=0;

    char data[BUFFER_SIZE];
    fgets(data, sizeof(data), stdin);
    data[strcspn(data,"\n")]=0;

    char message[BUFFER_SIZE];
    snprintf(message, sizeof(message), "%s:%s", input, data);
    send(sock, message, strlen(message), 0);
    
    int bytes=read(sock, buffer, BUFFER_SIZE-1);
    buffer[bytes]="\0";

    printf("Server response:\n%s\n", buffer);

    close(sock);
    return 0;
}
