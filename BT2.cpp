#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <port> <welcome message file> <output file>\n", argv[0]);
        return 1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        return 1;
    }

    int port = atoi(argv[1]);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 1) == -1) {
        perror("Error listening on socket");
        close(server_socket);
        return 1;
    }

    printf("Server is listening on port %d...\n", port);

    char welcome_message[256];
    FILE *welcome_file = fopen(argv[2], "r");
    if (welcome_file == NULL) {
        perror("Error opening welcome message file");
        close(server_socket);
        return 1;
    }
    fgets(welcome_message, sizeof(welcome_message), welcome_file);
    fclose(welcome_file);

    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        return 1;
    }

    if (send(client_socket, welcome_message, strlen(welcome_message), 0) == -1) {
        perror("Error sending welcome message to client");
        close(server_socket);
        close(client_socket);
        return 1;
    }

    char output_filename[256];
    sprintf(output_filename, "%s_%d", argv[3], client_socket);
    int output_file = open(output_filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (output_file == -1) {
        perror("Error opening output file");
        close(server_socket);
        close(client_socket);
        return 1;
    }

    char buffer[256];
    int bytes_received;
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        if (write(output_file, buffer, bytes_received) == -1) {
            perror("Error writing to output file");
            close(server_socket);
            close(client_socket);
            close(output_file);
            return 1;
        }
    }

    close(server_socket);
    close(client_socket);
    close(output_file);

    printf("Data from client saved to file: %s\n", output_filename);

    return 0;
}
