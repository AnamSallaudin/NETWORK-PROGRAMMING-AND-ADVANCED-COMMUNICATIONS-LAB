// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

struct Book {
    int book_id;
    char book_name[50];
    char title[50];
    char publisher[50];
    char genre[30];
};

void display_books(struct Book books[], int count) {
    printf("\nCurrent Book Database:\n");
    for (int i = 0; i < count; i++) {
        if (books[i].book_id != 0) {
            printf("ID: %d | Name: %s | Title: %s | Publisher: %s | Genre: %s\n",
                books[i].book_id, books[i].book_name, books[i].title, books[i].publisher, books[i].genre);
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX] = {0};

    struct Book books[10];
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Insert\n2. Update\n3. Search\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        sprintf(buffer, "%d", choice);
        send(sock, buffer, strlen(buffer), 0);

        if (choice == 1) {
            struct Book new_book;
            printf("Enter Book ID: ");
            scanf("%d", &new_book.book_id);
            printf("Enter Book Name: ");
            scanf(" %[^\n]", new_book.book_name);
            printf("Enter Title (Author): ");
            scanf(" %[^\n]", new_book.title);
            printf("Enter Publisher Name: ");
            scanf(" %[^\n]", new_book.publisher);
            printf("Enter Genre: ");
            scanf(" %[^\n]", new_book.genre);

            send(sock, &new_book, sizeof(new_book), 0);
            recv(sock, &books, sizeof(books), 0);
            display_books(books, 10);

        } else if (choice == 2) {
            recv(sock, &books, sizeof(books), 0);
            printf("Genre for 'Harry Potter' updated.\n");
            display_books(books, 10);

        } else if (choice == 3) {
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);

        } else if (choice == 4) {
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
            break;

        } else {
            printf("Invalid choice.\n");
        }
    }

    close(sock);
    return 0;
}
