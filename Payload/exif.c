#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
	buffer[1024];
	FILE *fp = fopen("FILETOOPEN.txt","r");
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
    const char *message = "Hello from client!";
    write(client_fd, message, strlen(message));

    // ----------------------
    // Read response from server
    // ----------------------
    int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (bytes < 0) {
        perror("read failed");
    } else {
        buffer[bytes] = '\0';               // Null-terminate the received data
        printf("Server replied: %s\n", buffer);
    }

    // ----------------------
    // Close connection
    // ----------------------
    close(client_fd);                        // Close the socket

    return 0;                                // Successful exit
}
