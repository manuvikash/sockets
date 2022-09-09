#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


#define MAXLINE 1024
#define PORT 8080

struct dns{
	char domain[100];
	char ip[25];
};

void initDns(struct dns d[5]){
	strcpy(d[0].domain,"www.google.com");
	strcpy(d[0].ip ,"142.250.77.142");

	strcpy(d[1].domain,"www.facebook.com");
	strcpy(d[1].ip ,"157.240.16.35");

	strcpy(d[2].domain,"www.reddit.com");
	strcpy(d[2].ip , "151.101.1.140");

	strcpy(d[3].domain,"www.psgtech.edu");
	strcpy(d[3].ip , "45.127.108.10");

	strcpy(d[4].domain,"www.youtube.com");
	strcpy(d[4].ip , "142.250.195.46");

}

int main(){
	struct dns d[5];
	initDns(d);
	
	int sockfd;
	char buffer[MAXLINE];
	char hello[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;
	int len, n, ind;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("Socket creation failed");
		exit(0);
	}
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("bind failed");
		exit(0);
	}
	
	for(int i=0; i<5; i++){
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		buffer[n-1] = '\0'; 
		printf("Client : %s\n", buffer);
		int flag = 0;
		for(int j=0; j<5; j++){
			if(strcmp(buffer, d[j].domain) == 0){
				flag++;
				strcpy(hello, d[j].ip);
			}
		}
		
		if(flag == 0){
			strcpy(hello, "Domain not found");
		}
				
		 
		sendto(sockfd, (const char *)hello, strlen(hello),  
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
		printf("IP address sent\n\n");  
	}
		
	    return 0; 
}

