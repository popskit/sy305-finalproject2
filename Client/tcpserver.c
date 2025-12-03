#include <stdio.h>         
#include <stdlib.h>         
#include <string.h>         
#include <unistd.h>         
#include <arpa/inet.h>      

#define PORT 1337          
#define BUFFER_SIZE 1024   

int main() {
    int server_fd, client_fd;                 
    struct sockaddr_in server_addr, client_addr;  
    socklen_t client_len = sizeof(client_addr);   
    char buffer[BUFFER_SIZE];                  
 
    server_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (server_fd < 0) {
        perror("socket failed");                 
        exit(EXIT_FAILURE);                      
    }


    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;            
    server_addr.sin_addr.s_addr = INADDR_ANY;     
    server_addr.sin_port = htons(PORT);           


    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }


    if (listen(server_fd, 5) < 0) {           
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);


    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);  
    if (bytes < 0) {
        perror("read failed");
    } else {
        buffer[bytes] = '\0';              
        printf("Received: %s\n", buffer);  

        const char* message = "Hello from server!\n";
        write(client_fd, message, strlen(message));  

    close(client_fd);  
    close(server_fd);
    }

    return 0;         
}