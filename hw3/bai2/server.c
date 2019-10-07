#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#define BUFF_SIZE 1024

int i;
struct hostent *host;
struct in_addr ipv4addr;

int checkIpAddress(char ipAddress[]) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    if (result != 0)
        return 1;
    else
        return 0;
}

char *fromIpToAddress(char* ip) {
    char *message = "Official name: ";
    char official[BUFF_SIZE];
    inet_pton(AF_INET, ip, &ipv4addr);
    if ((host = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET)) == NULL) {
        return "Not found information\n";
    } else {
        strcpy(official, message);
        message = strcat(official, host->h_name);
        strcat(message, "\nAlias name:\n");
        for (i = 0; host->h_aliases[i] != NULL; i++) {
            strcpy(official, message);
            message = strcat(official, host->h_aliases[i]);
            strcat(message, "\n");
        }
        return message;
    }
}

char *fromAddressToIp(char address[]) {
    char *message = "Official IP: \n";
    char official[BUFF_SIZE];
    char alias[BUFF_SIZE];
    if ((host = gethostbyname(address)) == NULL) {
        herror("Error");
        return "Error\n";
    } else {
        for (i = 0; host->h_addr_list[i] != NULL; i++) {
            strcpy(official, message);
            message = strcat(official, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
            strcat(message, "\n");
        }
        strcat(message, "Alias IP address: \n");

        for (i = 0; host->h_aliases[i] != NULL; i++) {
            strcpy(alias, message);
            message = strcat(alias, inet_ntoa(*(struct in_addr *)host->h_aliases[i]));
            strcat(message, "\n");
        }
        return message;
    }
}

int main(int argc, char const *argv[]) {

    int server_sock;
    char buffer[BUFF_SIZE];
    int bytes_sent, bytes_received;
    struct sockaddr_in server, client;
    int sin_size;

    if (argc != 2) {
        printf("Syntax is incorrect!\n");
        return 0;
    }

    //Step 1: Construct a UDP socket
    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed\n");
        return 0;
    }

    bzero(&server, sizeof(server));
    bzero(&client, sizeof(client));

    //Step 2: Bind address to socket
    server.sin_family = AF_INET; // IPv4
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));   // server port

    bzero(&(server.sin_zero), 8); /* zero the rest of the structure */

    if (bind(server_sock, (const struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        perror("Bind failed\n");
        return 0;
    }

    //Step 3: Communicate with clients
    printf("Server started\n");
    while (1) {
        sin_size = sizeof(struct sockaddr_in);

        bytes_received = recvfrom(server_sock, (char *)buffer, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&client, &sin_size);
        if (bytes_received < 0) {
            perror("Error: ");
            close(server_sock);
            return 0;
        }
        buffer[bytes_received - 1] = '\0';
        printf("\nRecieve from client : %s", buffer);

        if (checkIpAddress(buffer)) {
            printf("\nSending result: IP to Address \n");
            bytes_received = sendto(server_sock, (const char *)fromIpToAddress(buffer), strlen(fromIpToAddress(buffer)), MSG_CONFIRM, (const struct sockaddr *)&client, sin_size);
        }
        else {
            printf("\nSending result: Address to IP\n");
            bytes_sent = sendto(server_sock, (const char *)fromAddressToIp(buffer), strlen(fromAddressToIp(buffer)), MSG_CONFIRM, (const struct sockaddr *)&client, sin_size);
        }
        if (bytes_sent < 0) {
            perror("Error: ");
            close(server_sock);
            return 0;
        }

    }
    close(server_sock);
    return 0;
}
