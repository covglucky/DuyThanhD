#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345

#define MAX_BUF_LEN 256

int main() {
    char machine_name[MAX_BUF_LEN];
    char drive_info[MAX_BUF_LEN];
    char send_buf[MAX_BUF_LEN];
    int sock;
    struct sockaddr_in server_addr;
    ssize_t bytes_sent;
    ssize_t bytes_received;

  
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }


    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_port = htons(SERVER_PORT);


    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

  
    printf("Enter machine name: ");
    fgets(machine_name, MAX_BUF_LEN, stdin);
    machine_name[strcspn(machine_name, "\n")] = 0;


    printf("Enter drive info: ");
    fgets(drive_info, MAX_BUF_LEN, stdin);
    drive_info[strcspn(drive_info, "\n")] = 0;

    sprintf(send_buf, "%s+%s", machine_name, drive_info);
    bytes_sent = send(sock, send_buf, strlen(send_buf), 0);
    if (bytes_sent < 0) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }


    close(sock);

    return 0;
}

