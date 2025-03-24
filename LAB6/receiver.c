#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 1024

// Function to calculate parity
int calculate_parity(char *data, int parity_type) {
    int count = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1') {
            count++;
        }
    }

    // Even parity
    if (parity_type == 0) {
        return (count % 2 == 0) ? 0 : 1;
    }
    // Odd parity
    else {
        return (count % 2 == 0) ? 1 : 0;
    }
}

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char data[MAX_SIZE];
    int parity_bit, received_parity_bit, calculated_parity;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connection...\n");
    addr_size = sizeof(client_addr);

    // Accept incoming connection
    if ((new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data and parity bit
    recv(new_sock, data, MAX_SIZE, 0);
    recv(new_sock, &received_parity_bit, sizeof(received_parity_bit), 0);

    printf("Received Data: %s\n", data);
    printf("Received Parity Bit: %d\n", received_parity_bit);

    // Ask for parity type to verify
    int parity_type;
    printf("Enter Parity Type to Check (0 for Even, 1 for Odd): ");
    scanf("%d", &parity_type);

    // Calculate parity at receiver end
    calculated_parity = calculate_parity(data, parity_type);

    // Check for errors
    if (calculated_parity == received_parity_bit) {
        printf("✅ No error detected. Data is correct.\n");
    } else {
        printf("❌ Error detected! Data is corrupted.\n");
    }

    // Close socket
    close(new_sock);
    close(sockfd);
    return 0;
}
