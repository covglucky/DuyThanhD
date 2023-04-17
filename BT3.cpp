#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        return 1;
    }

    char buffer[MAX_BUFFER_SIZE];

    printf("Enter student information:\n");

    printf("MSSV: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // remove trailing newline
    send(client_socket, buffer, strlen(buffer), 0);

    printf("Full name: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // remove trailing newline
    send(client_socket, buffer, strlen(buffer), 0);

    printf("Date of birth: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // remove trailing newline
    send(client_socket, buffer, strlen(buffer), 0);

    printf("GPA: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // remove trailing newline
    send(client_socket, buffer, strlen(buffer), 0);

    close(client_socket);

    return 0;
}
