#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 30

int main() {
    char buffer[BUFFER_SIZE];
    
    // Read input from the standard input (stdin)
    ssize_t n = read(0, buffer, BUFFER_SIZE); // 0 is the file descriptor for stdin
    
    if (n == -1) {
        perror("Read error");
        return 1;
    }
    
    printf("Number of characters read: %zd\n", n);
    
    // Write the buffer content to the standard output (stdout)
    ssize_t written = write(1, buffer, n); // 1 is the file descriptor for stdout
    
    if (written == -1) {
        perror("Write error");
        return 1;
    }
    
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 30

int main() {
    int fd = open("myfile.txt", O_RDWR);
    ssize_t n;
    char buffer[BUFFER_SIZE];

    printf("File descriptor is %d\n", fd);
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    printf("Reading first 10 characters from file: \n");
    n = read(fd, buffer, 10);
    if (n == -1) {
        perror("Failed to read file");
        close(fd);
        exit(1);
    }
    buffer[n] = '\0';
    write(1, buffer, n);
    printf("\n");

    printf("Skipping 5 characters from current position in the file: \n");
    if (lseek(fd, 5, SEEK_CUR) == -1) {
        perror("Failed to seek file");
        close(fd);
        exit(1);
    }
    n = read(fd, buffer, 10);
    if (n == -1) {
        perror("Failed to read file");
        close(fd);
        exit(1);
    }
    buffer[n] = '\0';
    write(1, buffer, n);
    printf("\n");

    printf("Going 10 characters before the current position in the file: \n");
    if (lseek(fd, -10, SEEK_CUR) == -1) {
        perror("Failed to seek file");
        close(fd);
        exit(1);
    }
    n = read(fd, buffer, 10);
    if (n == -1) {
        perror("Failed to read file");
        close(fd);
        exit(1);
    }
    buffer[n] = '\0';
    write(1, buffer, n);
    printf("\n");

    printf("Going to 5th last character in the file: \n");
    if (lseek(fd, -5, SEEK_END) == -1) {
        perror("Failed to seek file");
        close(fd);
        exit(1);
    }
    n = read(fd, buffer, 5);
    if (n == -1) {
        perror("Failed to read file");
        close(fd);
        exit(1);
    }
    buffer[n] = '\0';
    write(1, buffer, n);
    printf("\n");

    printf("Going to the 3rd character in the file: \n");
    if (lseek(fd, 3, SEEK_SET) == -1) {
        perror("Failed to seek file");
        close(fd);
        exit(1);
    }
    n = read(fd, buffer, 5);
    if (n == -1) {
        perror("Failed to read file");
        close(fd);
        exit(1);
    }
    buffer[n] = '\0';
    write(1, buffer, n);
    printf("\n");

    close(fd);
    return 0;
}
