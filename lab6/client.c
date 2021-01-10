#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <netdb.h>


#define PORT 8080
#define MAXLINE 1024


int getSocketFD() {
    int sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void networkInteraction(int argc, char * argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent * he;
    struct in_addr ** addresses;
    he = gethostbyname(argv[1]);
    if (!he) {
        fprintf(stderr, "Неизвестный хост!");
        exit(EXIT_FAILURE);
    }
    addresses = (struct in_addr **) he->h_addr_list;
    char * strIPAddr = inet_ntoa(*addresses[0]);
    sockfd = getSocketFD();
    bzero(&servaddr, sizeof(servaddr));
    //memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    //servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_addr.s_addr = inet_addr(strIPAddr);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("\n Error : Connect Failed \n");
        exit(EXIT_FAILURE);
    }
    int len;
    //char * toServer = (char *) malloc(MAXLINE * sizeof(char));
    char * toServer = (char *) calloc(MAXLINE, sizeof(char));
    for (int i = 2; i < argc; i++) {
        toServer = strcat(toServer, argv[i]);
        if (i != argc - 1) {
            toServer = strcat(toServer, "_");
        }
    }
    printf("Отсылается сообщение: %s\n", toServer);
    sendto(sockfd, (const char *)toServer, strlen(toServer),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    free(toServer);
    toServer = NULL;
    char * socketBuffer = (char *)calloc(MAXLINE, sizeof(char));
    recvfrom(sockfd, (char *)socketBuffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    printf("Сервер прислал : %s\n", socketBuffer);
    free(socketBuffer);
    socketBuffer = NULL;
    close(sockfd);
}


int main(int argc, char * argv[]) {
    networkInteraction(argc, argv);
    exit(EXIT_SUCCESS);
}