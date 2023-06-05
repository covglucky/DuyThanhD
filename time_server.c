#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 8888

void handle_client(int client_socket);
void get_current_time(char *buffer, const char *format);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;
    pid_t child_pid;


    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Không thể tạo socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

   
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Ràng buộc socket thất bại");
        exit(1);
    }

 
    if (listen(server_socket, 5) < 0) {
        perror("Lỗi lắng nghe");
        exit(1);
    }

    printf("Time server đang lắng nghe trên cổng %d...\n", DEFAULT_PORT);

    while (1) {
       
        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Lỗi chấp nhận kết nối");
            exit(1);
        }

        child_pid = fork();
        if (child_pid < 0) {
            perror("Lỗi fork");
            exit(1);
        }

        if (child_pid == 0) {
      
            close(server_socket);
            handle_client(client_socket);
            close(client_socket);
            exit(0);
        } else {
           
            close(client_socket);
        }
    }

    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    memset(buffer, 0, BUFFER_SIZE);


    bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Lỗi đọc từ client");
        exit(1);
    }

 
    if (strncmp(buffer, "GET_TIME", 8) == 0) {
        char format[20];
        sscanf(buffer, "%*s %s", format);

      
        char current_time[100];
        get_current_time(current_time, format);

  
        if (send(client_socket, current_time, strlen(current_time), 0) < 0) {
            perror("Lỗi gửi thời gian về client");
            exit(1);
        }
    } else {
 
        const char *error_message = "Lệnh không hợp lệ";
        if (send(client_socket, error_message, strlen(error_message), 0) < 0) {
            perror("Lỗi gửi thông báo lỗi về client");
            exit(1);
        }
    }
}

void get_current_time(char *buffer, const char *format) {
    time_t current_time;
    struct tm *local_time;

    current_time = time(NULL);
    local_time = localtime(&current_time);
    strftime(buffer, 100, format, local_time);
}
