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
	
	int s_fd,n,flag;
	socklen_t len;
	char buffer[1024] = {0};
	struct sockaddr_in sadr,cadr;
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

	sadr.sin_family = AF_INET;
	sadr.sin_port = htons(PORT);
	sadr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int frame_no = 0;
	
	Frame recv,sendf;
	
	
	if(bind(s_fd,(const struct sockaddr*)&sadr,sizeof(sadr)) < 0) {
		printf("bind error");
		return -1;
	}

	len = sizeof(cadr);

    	while(1){
		flag = recvfrom(s_fd, &recv, sizeof(Frame), 0, (struct sockaddr*)&cadr, &len);
		if (flag > 0 && recv.frame_type == 1 && recv.sqn == frame_no) {
			printf("Frame Received: ");
			printf("%s\n",recv.packet.data);	
			sendf.frame_type = 0;
			sendf.sqn = recv.sqn + 1;
			sendto(s_fd, &sendf, sizeof(sendf), 0, (struct sockaddr*)&cadr, len);
		}
		else if(recv.sqn != frame_no) {
		}
		else if(recv.frame_type == 2) {
			break;
		}  
		else {
			printf("Nothing Received\n");
		}
		frame_no += 1;
	}
	

        close(s_fd);
    	return 0; 

}
