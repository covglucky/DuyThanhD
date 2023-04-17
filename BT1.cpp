#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Winsock2.h>
#include <WinSock.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <IP address> <port>\n", argv[0]);
        return 1;
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("can not creat socket");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("can not connect to server");
        close(client_socket);
        return 1;
    }

    printf("Connected to server %s:%s\n", argv[1], argv[2]);

    char buffer[256];
    int bytes_received;

    while (1) {
        printf(" message: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("can not send message to server");
            close(client_socket);
            return 1;
        }
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error receiving message from server");
            close(client_socket);
            return 1;
        }
        buffer[bytes_received] = '\0';
        printf("Server replied: %s", buffer);
    }

    close(client_socket);

    return 0;
}

