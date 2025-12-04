#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8088

int main()
{
	

	
	char *ip = "127.0.0.1";
	int port = PORT;

	int sockfd;
	struct sockaddr_in addr;
	char buffer[1024] = {0};
	char payload[1024] = {0};
	socklen_t addr_size;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	memset(&addr,'\0',sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	while(1){
	bzero(buffer,1024);
	printf("The command: ");
	scanf("%s", buffer);
	sendto(sockfd,buffer,1024,0,(struct sockaddr*)&addr, sizeof(addr));
	
	}
return 0;
}
