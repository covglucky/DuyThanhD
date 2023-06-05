#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Usage: %s <port> <log_file>\n", argv[0]);
        return 1;
    }

    int server_socket, new_socket, client_socket[MAX_CLIENTS], max_clients = MAX_CLIENTS;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if(listen(server_socket, 5) < 0) {
        perror("Listen failed");
        return 1;
    }


    FILE* log_file = fopen(argv[2], "a");
    if(log_file == NULL) {
        perror("Failed to create log file");
        return 1;
    }

  
    for(int i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    printf("Waiting for incoming connections...\n");

    while(1) {
        if((new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
            perror("Accept failed");
            return 1;
        }


        char* client_ip = inet_ntoa(client_addr.sin_addr);
        printf("Connection accepted from %s\n", client_ip);


        int bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if(bytes_received < 0) {
            perror("Receive failed");
            return 1;
        }

        buffer[bytes_received] = '\0';
        printf("Received data: %s\n", buffer);


        time_t current_time = time(NULL);
        struct tm* time_info = localtime(&current_time);
        char time_string[30];
        strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", time_info);

        fprintf(log_file, "%s %s %s\n", client_ip, time_string, buffer);

        send(new_socket, buffer, strlen(buffer), 0);

        for(int i = 0; i < max_clients; i++) {
            if(client_socket[i] == 0) {
                client_socket[i] = new_socket;
                break;
            }
        }
    }

    return 0;
}
