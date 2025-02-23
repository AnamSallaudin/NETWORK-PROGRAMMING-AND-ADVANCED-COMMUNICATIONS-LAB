#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
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
    int sock;
    struct sockaddr_in server_addr;
    char data[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Get user input
    printf("Enter binary data (0s and 1s only): ");
    scanf("%s", data);

    int parity_type;
    printf("Choose Parity Type (1 for Odd, 0 for Even): ");
    scanf("%d", &parity_type);

    // Calculate parity bit
    int parity_bit = calculate_parity(data, parity_type);
    
    // Append parity bit to data
    char transmitted_data[BUFFER_SIZE];
    sprintf(transmitted_data, "%s%d", data, parity_bit);

    // Send data to server
    send(sock, transmitted_data, strlen(transmitted_data) + 1, 0);
    
    // Send parity type to server
    send(sock, &parity_type, sizeof(parity_type), 0);

    printf("Data sent to server: %s\n", transmitted_data);

    // Close socket
    close(sock);

    return 0;
}
