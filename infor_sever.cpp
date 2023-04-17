#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_socket, client_socket, client_address_length;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];
    char *token, *machine_name, *drive_info;
    int num_drives, i;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }


    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(1);
    }

  
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));


    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(1);
    }

    
    if (listen(server_socket, 10) == -1) {
        perror("Error listening on socket");
        exit(1);
    }

    printf("Server is listening on port %s...\n", argv[1]);

   
    client_address_length = sizeof(client_address);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length)) == -1) {
        perror("Error accepting connection");
        exit(1);
    }

  
    if (recv(client_socket, buffer, BUFFER_SIZE, 0) == -1) {
        perror("Error receiving data");
        exit(1);
    }

    
    token = strtok(buffer, "+");
    machine_name = token;
    token = strtok(NULL, "+");
    num_drives = atoi(token);
    drive_info = strtok(NULL, "+");

 
    printf("Machine Name: %s\n", machine_name);
    for (i = 0; i < num_drives; i++) {
        printf("Drive %d: %s\n", i + 1, strtok(drive_info, "-"));
        printf("Size: %s\n", strtok(NULL, "-"));
        drive_info = strtok(NULL, "+");
    }


    FILE *fp;
    fp = fopen("info_log.txt", "a");
    if (fp == NULL) {
        perror("Error opening log file");
        exit(1);
    }
    fprintf(fp, "%s\n", buffer);
    fclose(fp);

   
    close(client_socket);
    close(server_socket);

    return 0;
}
