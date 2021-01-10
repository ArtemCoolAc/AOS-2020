#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <wait.h>
#include <sys/msg.h>
#include <arpa/inet.h>

int main() {
    char * hostName = "localhost";
    struct hostent * he;
    struct in_addr ** addresses;
    if ((he = gethostbyname(hostName)) == NULL) {
        herror("Ошибка");
    }
    addresses = (struct in_addr **) he->h_addr_list;
    char * address = inet_addr(he->h_addr_list[0]);
    char * otherAddr = inet_ntoa(*addresses[0]);
    printf("%s, %s, %d\n", he->h_name, he->h_addr_list[0], he->h_addrtype);
    printf("inet_addr = %s, inet_ntoa = %s\n", address, otherAddr);
    exit(0);
}