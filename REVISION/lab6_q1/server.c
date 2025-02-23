#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to calculate parity
int calculate_parity(char data[], int parity_type) {
    int count = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1') {
            count++;
        }
    }
    return (parity_type == 1) ? (count % 2 == 0) : (count % 2 != 0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data
    recv(new_socket, buffer, BUFFER_SIZE, 0);
    printf("Received Data: %s\n", buffer);

    // Extract data and parity bit
    int len = strlen(buffer);
    char data[BUFFER_SIZE];
    strncpy(data, buffer, len - 1);
    data[len - 1] = '\0';
    int received_parity = buffer[len - 1] - '0';

    // Get parity type
    int parity_type;
    recv(new_socket, &parity_type, sizeof(parity_type), 0);
    
    // Validate parity
    int calculated_parity = calculate_parity(data, parity_type);
    if (calculated_parity == received_parity) {
        printf(" Data received correctly (No corruption detected)\n");
    } else {
        printf(" Data is corrupt (Parity mismatch)\n");
    }

    // Close connections
    close(new_socket);
    close(server_fd);

    return 0;
}
