#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024


void handle_registration(int client_socket){
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response), "Child PID: %d\nResponse for choice 1", getpid());
    send(client_socket, response, strlen(response),0);
}

void handle_name(int client_socket){
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response), "Child PID: %d\nResponse for choice 2", getpid());
    send(client_socket, response, strlen(response),0);
}

void handle_subject(int client_socket){
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response), "Child PID: %d\nResponse for choice 3", getpid());
    send(client_socket, response, strlen(response),0);
}

int main(){
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen= sizeof(address);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd,5);
    printf("Server is listening on port %d...\n", PORT);

    while(1){
        client_socket=accept(server_fd, (struct sockaddr*)&address, &addrlen);
        read(client_socket, buffer, BUFFER_SIZE);
        printf("Received: %s\n", buffer);

        pid_t pid=fork();

        if(pid==0){
            if(strncmp(buffer, "1:", 1)==0){
                handle_registration(client_socket);
            }
            else if(strncmp(buffer, "2:", 1)==0){
                handle_name(client_socket);
            }
            else if(strncmp(buffer, "1:", 1)==0){
                handle_subject(client_socket);
            }
            else{
                char *error = "Invalid Option";
                send(client_socket, error, strlen(error),0);
            }
            close(client_socket);
            exit(0);
        }
    }

    close(server_fd);
    return 0;


}
