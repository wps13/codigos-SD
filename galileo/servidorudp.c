#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define porta 1313

int main(){
	int number;
	struct sockaddr_in servidor;
	int servidorsize = sizeof(servidor);
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1){
		perror("socket ");
		exit(1);
	}
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(porta);
	servidor.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(servidor.sin_zero, 0, 8);

	if(bind(sock, (struct sockaddr*) &servidor, sizeof(servidor)) == -1){
		perror("bind ");
		exit(1);
	}

	while(1){
		if(recvfrom(sock, &valorCurva, sizeof(valorCurva), 0, (struct sockaddr*) &servidor, &servidorsize) > 0){
			printf("%d\n", valorCurva);
		}
		if(recvfrom())
	}
}