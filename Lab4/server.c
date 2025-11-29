#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // localhost
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server running on http://localhost:%d\n", PORT);

    while (1) {
        // 4. Accept a client connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                                 (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // 5. Read client request
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Client Request:\n%s\n", buffer);

        // 6. Open index.html
        FILE *html = fopen("index.html", "r");
        if (!html) {
            perror("index.html not found");
            close(new_socket);
            continue;
        }

        // Get file content
        fseek(html, 0, SEEK_END);
        long filesize = ftell(html);
        rewind(html);

        char *file_content = malloc(filesize + 1);
        fread(file_content, 1, filesize, html);
        file_content[filesize] = '\0';
        fclose(html);

        // 7. Prepare HTTP Response
        char header[BUFFER_SIZE];
        sprintf(header, "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %ld\r\n"
                        "Connection: close\r\n\r\n",
                        filesize);

        // 8. Send header + content
        write(new_socket, header, strlen(header));
        write(new_socket, file_content, filesize);

        // 9. Clean up
        free(file_content);
        close(new_socket);
    }

    return 0;
}

