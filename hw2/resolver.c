#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void resolve_name(char input[]) {
  struct hostent *hen;
  int i = 0;

  printf("Hostname: %s\n", input);
  hen = gethostbyname(input);

  if (hen == NULL) {
    printf("Not found information\n");
    return;
  }

  printf("Offcial IP: %s \n", inet_ntoa(*((struct in_addr *)hen->h_addr)));

  char **pAlias;

  for (pAlias = hen->h_aliases; *pAlias != NULL; pAlias++) {
    printf("Alternate IP #%d: %s\n", ++i, inet_ntoa(*((struct in_addr *)pAlias)));
  }
}

void resolve_ip_addr(char input[]) {
  struct hostent *hen;
  struct in_addr addr;
  int i = 0;

  printf("IP address: %s\n", input);
  addr.s_addr = inet_addr(input);
  if (addr.s_addr == INADDR_NONE) {
    printf("Not found information\n");
    return;
  } else
    hen = gethostbyaddr((char *) &addr, 4, AF_INET);

  if (hen == NULL) {
    printf("Not found information\n");
    return;
  }

  printf("Offcial name: %s \n", hen->h_name);

  char **pAlias;

  for (pAlias = hen->h_aliases; *pAlias != NULL; pAlias++) {
    printf("Alternate name #%d: %s\n", ++i, *pAlias);
  }
}

int main(int argc, char const *argv[]) {
  char input[30];

  if (argc != 2) {
    printf("Syntax is incorrect!\n");
    return 0;
  }

  strcpy(input, argv[1]);

  if (isalpha(input[0])) {        /* input is a name */
    resolve_name(input);
  } else {
    resolve_ip_addr(input);       /* input is an ip address */
  }

  // while (hen->h_aliases[i] != 0) {
  //   addr.s_addr = *(u_long *) hen->h_aliases[i++];
  //   printf("\tIPv4 Address #%d: %s\n", i, inet_ntoa(addr));
  // }
  return 0;
}
