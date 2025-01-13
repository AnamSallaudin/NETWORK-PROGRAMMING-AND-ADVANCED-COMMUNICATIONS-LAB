#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#define PORT 8080
#define MAX 1024

// Function to reorder the file content based on ASCII values
int reorder_file_content(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        return -1;  // File not found or can't be opened
    }

    char content[MAX];
    int length = 0;

    // Read the entire content of the file
    while (fgets(content + length, sizeof(content) - length, file)) {
        length += strlen(content + length);
    }

    // Sort content based on ASCII values of characters
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            if (content[i] > content[j]) {
                char temp = content[i];
                content[i] = content[j];
                content[j] = temp;
            }
        }
    }

    // Rewrite the sorted content back into the file
    fseek(file, 0, SEEK_SET);
    fputs(content, file);

    fclose(file);
    return 1;  // Success
}

// Function to handle client requests
void handle_client(int client_socket) {
    char filename[MAX], search_str[MAX], replace_str[MAX];
    ssize_t n;
    int choice;

    // Receive file name from client
    n = recv(client_socket, filename, sizeof(filename), 0);
    if (n <= 0) {
        perror("Error receiving filename");
        close(client_socket);
        return;
    }

    filename[n] = '\0';  // Null-terminate the filename string

    // Check if file exists
    if (access(filename, F_OK) == -1) {
        send(client_socket, "File not present", 17, 0);
        close(client_socket);
        return;
    }

    // File exists, send menu to client
    char menu[] = "1. Search\n2. Replace\n3. Reorder\n4. Exit\n";
    send(client_socket, menu, strlen(menu), 0);

    // Receive choice from client
    n = recv(client_socket, &choice, sizeof(choice), 0);
    if (n <= 0 || (choice != 1 && choice != 2 && choice != 3)) {
        close(client_socket);
        return;
    }

    if (choice == 3) {  // Reorder operation
        // Perform the reorder operation on the file
        int result = reorder_file_content(filename);
        if (result == -1) {
            send(client_socket, "File not found or can't be opened", 32, 0);
        } else {
            send(client_socket, "File content reordered by ASCII values", 36, 0);
        }
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Listen for incoming connections
    listen(server_fd, 5);
    client_len = sizeof(client_addr);

    // Accept incoming client connection
    client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Error accepting connection");
        exit(1);
    }

    printf("Client connected!\n");

    // Handle client request
    handle_client(client_socket);

    // Close server socket
    close(server_fd);
    return 0;
}

