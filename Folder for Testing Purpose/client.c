#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_image(FILE *fp, int sockfd) {
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0) {
        // printf(bytes_read);
        if (send(sockfd, buffer, bytes_read, 0) == -1) {
            perror("Send failed");
            return;
        }
    }
    printf("Image sent successfully\n");
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char *filename = "/home/nitdelhi/231210006_CN/Lab2/sample1.bmp"; // Path to your image file

    // Open the image file
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    send_image(fp, sockfd);

    fclose(fp);
    close(sockfd);

    return 0;
}