#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 8080
#define ROOT_DIRECTORY "./"

void send_response(int sock, const char* response) {
    send(sock, response, strlen(response), 0);
}

void send_file_content(int sock, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        send_response(sock, "HTTP/1.1 404 Not Found\r\n\r\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    
    send_response(sock, "HTTP/1.1 200 OK\r\n\r\n");

    
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        send(sock, buffer, strlen(buffer), 0);
    }

    fclose(file);
}

void handle_request(int sock, const char* request) {
    char path[BUFFER_SIZE];
    sscanf(request, "GET /%s", path);

    
    if (strcmp(path, "") == 0 || strcmp(path, "/") == 0) {
        DIR* dir = opendir(ROOT_DIRECTORY);
        struct dirent* entry;

        
        char content[BUFFER_SIZE];
        sprintf(content, "<html><body>");

        
        while ((entry = readdir(dir)) != NULL) {
            char entry_path[BUFFER_SIZE];
            sprintf(entry_path, "%s%s", ROOT_DIRECTORY, entry->d_name);

            if (entry->d_type == DT_DIR) {
                
                sprintf(content + strlen(content), "<b><a href=\"%s/\">%s</a></b><br>", entry->d_name, entry->d_name);
            } else if (entry->d_type == DT_REG) {
                
                sprintf(content + strlen(content), "<i><a href=\"%s\">%s</a></i><br>", entry->d_name, entry->d_name);
            }
        }

        sprintf(content + strlen(content), "</body></html>\r\n");

      
        send_response(sock, "HTTP/1.1 200 OK\r\n");
        send_response(sock, "Content-Type: text/html\r\n");
        send_response(sock, "\r\n");
        send_response(sock, content);

        closedir(dir);
    } else {
        // Handle file request
        send_file_content(sock, path);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

   
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEFAULT_PORT);

    
    if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

   
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is up and running on port %d\n", DEFAULT_PORT);

    while (1) {
      
        if ((new_socket = accept(server_fd, (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));

        recv(new_socket, buffer, BUFFER_SIZE, 0);
        printf("Received request:\n%s\n", buffer);

       
        handle_request(new_socket, buffer);

       
        close(new_socket);
    }

    return 0;
}