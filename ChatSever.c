#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // tao socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error: Failed to create socket.\n");
        exit(1);
    }

    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error: Failed to bind.\n");
        exit(1);
    }

    
    if (listen(server_socket, 5) == -1) {
        perror("Error: Failed to listen.\n");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        //chap nhan ket noi tu client
        socklen_t client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        if (client_socket == -1) {
           perror("Error: Failed to accept.\n");
           exit(1);
        }

        printf("Client connected: %s\n", inet_ntoa(client_address.sin_addr));

        // xu li ket noi tu client
        while (1) {
            memset(buffer, 0, sizeof(buffer));

            // doc tin nahn tu client
            if (read(client_socket, buffer, sizeof(buffer)) == -1) {
                perror("Error: Failed to read from client.\n");
                exit(1);
            }

            printf("Client says: %s", buffer);

            // gui tin nhan tu clinet den sever
            printf("Server says: ");
            fgets(buffer, sizeof(buffer), stdin);
            if (write(client_socket, buffer, sizeof(buffer)) == -1) {
                perror("Error: Failed to write to client.\n");
                exit(1);
            }
        }

        // dong ket noi client
        close(client_socket);
    }

    // donnng socket server
    close(server_socket);

    return 0;
}
