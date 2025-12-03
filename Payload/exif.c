#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>         // For read, write, close
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#define PORT 1337
int main(int argc, char *argv[])
{
	pid_t ppid = fork();
	if(ppid == 0)
	{
	while (1) {
        if (kill(atoi(argv[1]), SIGUSR1) == -1) {
            perror("kill failed");
            return 1;
        }
        printf("Sent SIGUSR1 to %d\n", ppid);
        sleep(1);
    }
	}
	else{
	char buffer[1024];
	int fd = open("FILETOOPEN.txt",O_RDONLY);
	read(fd,buffer,sizeof(buffer));
	int client_fd;                              // File descriptor for client socket
    	struct sockaddr_in server_addr;             // Server address structure

    // ----------------------
    // Create TCP socket
    // ----------------------
    client_fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP
    if (client_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // ----------------------
    // Configure server address
    // ----------------------
    memset(&server_addr, 0, sizeof(server_addr)); // Clear memory (good practice)
    server_addr.sin_family = AF_INET;             // IPv4
    server_addr.sin_port = htons(PORT);           // Convert port to network byte order

    // Convert human-readable IP address to binary form
    // Change "127.0.0.1" if your server is on another machine
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("invalid address");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // ----------------------
    // Connect to server
    // ----------------------
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // ----------------------
    // Send message to the server
    // ----------------------
    write(client_fd, buffer, strlen(buffer));
	}
}
