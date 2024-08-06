//-------------use of I/O system calls for console//
#include <unistd.h>
#include <stdio.h>
#define BUFFER_SIZE 30
int main() {
// ssize_t read(int fd, void *buf, size_t count);
// ssize_t write(int fd, const void *buf, size_t count);
char buffer[BUFFER_SIZE];
int n = read(0, buffer, BUFFER_SIZE); // 0 is the file discriptor for stdin
printf("Number of charecters read: %d\n", n);
write(1, buffer, n); // 1 is the file discriptor for stdout
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
