#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int sockfd;
	//struct sockaddr_in dest;
  struct sockaddr_in6 dest;
	char message[128];
	char buffer[128];
  strcpy(message, argv[1]);

	/* create socket */
	//sockfd = socket(PF_INET, SOCK_STREAM, 0);
  sockfd = socket(AF_INET6, SOCK_STREAM, 0);

	/* initialize value in dest */
	bzero(&dest, sizeof(dest));
  //
  dest.sin6_flowinfo = 0;
	//dest.sin_family = PF_INET;
	dest.sin6_family = AF_INET6;
  //dest.sin_port = htons(8888);
  dest.sin6_port = htons(8889);
	//inet_aton("127.0.0.1", &dest.sin_addr);
  inet_pton(AF_INET6, "::1", &dest.sin6_addr);

	/* Connecting to server */
	connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

	/* Send message */
	send(sockfd, message, sizeof(message), 0);

	/* Receive message from the server and print to screen */
	bzero(buffer, 128);
	recv(sockfd, buffer, sizeof(buffer), 0);
	printf("%s", buffer);
  printf("%s", "\n");

	/* Close connection */
	close(sockfd);

	return 0;
}
