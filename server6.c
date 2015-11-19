#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void connection_handler(int,int);

int main()
{
	int sockfd, pid;
	//struct sockaddr_in dest;
  struct sockaddr_in6 dest;
  //
  char client_addr_ipv6[100];
	char start[20] = "Server start.\n";

	printf("%s", start);

	/* create socket */
	//sockfd = socket(PF_INET, SOCK_STREAM, 0);
  sockfd = socket(AF_INET6, SOCK_STREAM, 0);

	/* initialize structure dest */
	bzero(&dest, sizeof(dest));
  //
  dest.sin6_flowinfo = 0;
	//dest.sin_family = AF_INET;
  dest.sin6_family = AF_INET6;
	//dest.sin_port = htons(8888);
  dest.sin6_port = htons(8888);
	/* this line is different from client */
	//dest.sin_addr.s_addr = INADDR_ANY;
  dest.sin6_addr = in6addr_any;

	/* Assign a port number to socket */
	bind(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	/* make it listen to socket with max 10 connections */
	listen(sockfd, 20);

	/* infinity loop -- accepting connection from client forever */
	while(1)
	{
    int clientfd;
		//struct sockaddr_in client_addr;
    struct sockaddr_in6 client_addr;
		int addrlen = sizeof(client_addr);

    clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &addrlen);

    //
    inet_ntop(AF_INET6, &(client_addr.sin6_addr), client_addr_ipv6, 100);
    printf("Incoming connection from client having IPv6 address: %s\n", client_addr_ipv6);

    if (clientfd < 0) {
      perror("ERROR on accept");
      exit(1);
    }

    /* Create child process */
    pid = fork();

    if (pid < 0) {
      perror("ERROR on fork");
      exit(1);
    }

    if (pid == 0) {
      /* This is the client process */
      close(sockfd);
      connection_handler(clientfd,pid);
      printf("%d",pid);
      printf("%s","\n");
      exit(0);
    } else {
      close(clientfd);
    }
	}

	/* close(server) , but never get here because of the loop */
	close(sockfd);
	return 0;
}

void connection_handler(int clientfd, int pid) {
  char buffer[128];

  for(int i=0;i<100000000;i++) {};
  //printf("%d",pid);
  //printf("%s","\n");

  /* Receive message from the server and print to screen */
	bzero(buffer, 128);
	recv(clientfd, buffer, sizeof(buffer), 0);
	printf("%s", buffer);
  printf("%s", "\n");

	/* Send message */
	send(clientfd, buffer, sizeof(buffer), 0);

	/* close(client) */
	close(clientfd);
}
