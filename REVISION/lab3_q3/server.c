#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to sort numeric characters in ascending order
void sort_numbers(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (str[i] > str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

// Function to sort alphabetic characters in descending order
void sort_letters(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (str[i] < str[j]) { // Descending order
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    addr_size = sizeof(client_addr)
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Read data from client
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Received string from client: %s\n", buffer);

    char num_str[BUFFER_SIZE], letter_str[BUFFER_SIZE];
    int num_idx = 0, letter_idx = 0;

    // Separate numbers and letters
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] >= '0' && buffer[i] <= '9') {
            num_str[num_idx++] = buffer[i];
        } else if ((buffer[i] >= 'A' && buffer[i] <= 'Z') || (buffer[i] >= 'a' && buffer[i] <= 'z')) {
            letter_str[letter_idx++] = buffer[i];
        }
    }
    num_str[num_idx] = '\0';
    letter_str[letter_idx] = '\0';

    pid_t pid = fork();
    if (pid == 0) { // Child process
        sort_numbers(num_str);
        char result[BUFFER_SIZE];
        snprintf(result, sizeof(result), "Child Process (PID: %d) - Sorted Numbers: %s", getpid(), num_str);
        send(client_fd, result, strlen(result), 0);
        printf("%s\n", result);
    } else { // Parent process
        sort_letters(letter_str);
        char result[BUFFER_SIZE];
        snprintf(result, sizeof(result), "Parent Process (PID: %d) - Sorted Letters: %s", getpid(), letter_str);
        send(client_fd, result, strlen(result), 0);
        printf("%s\n", result);
        wait(NULL); // Wait for child process to finish
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
