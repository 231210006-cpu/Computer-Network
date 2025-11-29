#include <stdio.h>
#include <errno.h>   // For errno

int main() {
    FILE *fp;

    // Trying to open a file that does not exist
    fp = fopen("nonexistent.txt", "r");

    if (fp == NULL) {
        perror("Error opening file");  // Print error
    }

    return 0;
}
