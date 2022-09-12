#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/time.h>

#define PORT     8080 
#define MAXLINE 1024 

typedef struct packet{
    char data[1024];
}Packet;

typedef struct frame{
    int frame_type; 
    int sqn;
    Packet packet;
}Frame;

int main() {
	
	int s_fd,len,n,flag = 1,frame_no = 0,rs;
	char buffer[1024] = {0};
	struct sockaddr_in sadr;
	s_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_fd < 0) {
		printf("socket error");
		return -1;
	}
	struct timeval tv;
	tv.tv_sec = 10;
	if (setsockopt(s_fd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    		perror("Error");
	}
	memset(&sadr,0,sizeof(sadr));
	
	Frame recv,sendf;
		


	sadr.sin_family = AF_INET;
	sadr.sin_port = htons(PORT);
	sadr.sin_addr.s_addr = INADDR_ANY;	

	int addr_size = sizeof(sadr);
	
	
	while(1){
		sendf.sqn = frame_no;
		sendf.frame_type = 1;		
		bzero(buffer, 1024);
		printf("Enter Input: ");
		scanf("%s", buffer);
		if(strcmp(buffer, "end") == 0) break;
		strcpy(sendf.packet.data, buffer);
		sendto(s_fd, &sendf, sizeof(Frame), 0, (struct sockaddr*)&sadr, sizeof(sadr));
		printf("Frame Sent\n");
		rs = recvfrom(s_fd, &recv, sizeof(recv), 0 ,(struct sockaddr*)&sadr, &addr_size);
		
		if(rs > 0 && recv.sqn == frame_no+1 && recv.frame_type==0){
			printf("Ack Received\n");
		}else{
			printf("Ack Not Received; Resending Frame\n");
			sendto(s_fd, &sendf, sizeof(Frame), 0, (struct sockaddr*)&sadr, sizeof(sadr));
		}	
  			
		frame_no++;	
	}
	sendf.frame_type = 2;
    	sendto(s_fd, &sendf, sizeof(Frame), 0, (struct sockaddr*)&sadr, sizeof(sadr));
	close(s_fd);
    	return 0; 

}
