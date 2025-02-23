#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int new_socket) {
    char buffer[BUFFER_SIZE];
    read(new_socket, buffer, BUFFER_SIZE);
    
    pid_t pid = fork(); // Create a child process

    if (pid == 0) { // Child process
        char response[BUFFER_SIZE];

        if (strncmp(buffer, "1", 1) == 0) {
            sprintf(response, "Child PID: %d\nName: John Doe\nAddress: 123 Main St, City", getpid());
        } 
        else if (strncmp(buffer, "2", 1) == 0) {
            sprintf(response, "Child PID: %d\nDept: CSE\nSemester: 6\nSection: B\nCourses: OS, CN, AI", getpid());
        } 
        else if (strncmp(buffer, "3", 1) == 0) {
            sprintf(response, "Child PID: %d\nSubject: CN\nMarks: 89", getpid());
        } 
        else {
            sprintf(response, "Invalid option received!");
        }

        write(new_socket, response, strlen(response));
        close(new_socket);
        exit(0);
    } else if (pid > 0) { // Parent process
        wait(NULL); // Wait for child process to finish
    } else {
        perror("Fork failed");
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        handle_client(new_socket);
    }

    close(server_fd);
    return 0;
}
