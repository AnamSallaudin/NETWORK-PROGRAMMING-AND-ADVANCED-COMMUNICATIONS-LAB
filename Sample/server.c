// server.c
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

struct Book books[10];
int book_count = 3; // initialize with 3 books

void initialize_books() {
    books[0] = (struct Book){1, "Harry Potter", "J.K. Rowling", "Bloomsbury", "Comic"};
    books[1] = (struct Book){2, "The Hobbit", "J.R.R. Tolkien", "HarperCollins", "Fantasy"};
    books[2] = (struct Book){3, "1984", "George Orwell", "Secker & Warburg", "Dystopian"};
}

void handle_client(int new_socket) {
    char buffer[MAX];
    int choice;

    while (1) {
        memset(buffer, 0, MAX);
        read(new_socket, buffer, sizeof(buffer));
        sscanf(buffer, "%d", &choice);

        if (choice == 1) {
            // Insert
            struct Book new_book;
            read(new_socket, &new_book, sizeof(new_book));
            books[book_count++] = new_book;
            send(new_socket, &books, sizeof(books), 0);

        } else if (choice == 2) {
            // Update Genre of Harry Potter
            for (int i = 0; i < book_count; i++) {
                if (strcmp(books[i].book_name, "Harry Potter") == 0) {
                    strcpy(books[i].genre, "Fantasy");
                    break;
                }
            }
            send(new_socket, &books, sizeof(books), 0);

        } else if (choice == 3) {
            // Search author with max publications
            char author_names[10][50];
            int count[10] = {0};
            int unique = 0;

            for (int i = 0; i < book_count; i++) {
                int found = 0;
                for (int j = 0; j < unique; j++) {
                    if (strcmp(books[i].title, author_names[j]) == 0) {
                        count[j]++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    strcpy(author_names[unique], books[i].title);
                    count[unique++] = 1;
                }
            }

            int max_index = 0;
            for (int i = 1; i < unique; i++) {
                if (count[i] > count[max_index]) {
                    max_index = i;
                }
            }

            char result[100];
            sprintf(result, "Author with most publications: %s", author_names[max_index]);
            send(new_socket, result, sizeof(result), 0);

        } else if (choice == 4) {
            // Exit
            char exit_msg[] = "Bye";
            send(new_socket, exit_msg, sizeof(exit_msg), 0);
            break;
        }
    }

    close(new_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    initialize_books();

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server listening on port %d...\n", PORT);
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    handle_client(new_socket);
    close(server_fd);
    return 0;
}
