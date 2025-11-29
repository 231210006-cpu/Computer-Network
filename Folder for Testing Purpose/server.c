#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void save_image(int new_sock) {
    FILE *fp;
    char *filename = "/home/nitdelhi/231210006_CN/Lab2/server_received.bmp";
    char buffer[BUFFER_SIZE];
    int bytes_received;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    while ((bytes_received = recv(new_sock, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, sizeof(char), bytes_received, fp);
    }

    if (bytes_received == -1) {
        perror("recv failed");
    }

    printf("Image received and saved as %s\n", filename);
    fclose(fp);
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    new_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (new_sock == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    save_image(new_sock);

    close(new_sock);
    close(server_fd);
    return 0;
}