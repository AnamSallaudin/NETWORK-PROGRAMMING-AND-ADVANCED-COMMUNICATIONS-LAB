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
    int sockfd;
    struct sockaddr_in server_addr;
    char data[MAX_SIZE];
    int parity_type, parity_bit;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to receiver
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Input binary data
    printf("Enter binary data to send: ");
    scanf("%s", data);

    // Choose parity type
    printf("Choose Parity Type (0 for Even, 1 for Odd): ");
    scanf("%d", &parity_type);

    // Calculate and append parity bit
    parity_bit = calculate_parity(data, parity_type);
    printf("Parity bit: %d\n", parity_bit);

    // Send data and parity bit
    send(sockfd, data, strlen(data), 0);
    send(sockfd, &parity_bit, sizeof(parity_bit), 0);

    printf("Data sent successfully!\n");

    // Close socket
    close(sockfd);
    return 0;
}
