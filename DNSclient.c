#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
	
#define PORT	 8080
#define MAXLINE 1024
	

int main() {
	int sockfd;
	char buffer[MAXLINE];
	char hello[MAXLINE];
	int n, len, ind;
	
	struct sockaddr_in servaddr;
	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
		
	for(int i; i<5; i++){
		ind = 0;
		printf("Domain name: ");
		while ((hello[ind++] = getchar()) != '\n');
		sendto(sockfd, (const char *)hello, strlen(hello),
			MSG_CONFIRM, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));
		printf("Domain name sent\n");
				
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,
					MSG_WAITALL, (struct sockaddr *) &servaddr,
					&len);
		buffer[n] = '\0';
		printf("Server : %s\n", buffer);
	}
	
	close(sockfd);
	return 0;
}

