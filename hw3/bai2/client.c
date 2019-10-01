#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFF_SIZE 1024

int checkIpAddress(const char *ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    if (result != 0)
        return 1;
    else
        return 0;
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        printf("Syntax is incorrect!\n");
        return 0;
    }

    if (checkIpAddress(argv[1]) == 0) {  //check if ip address is valid
        printf("Not a valid ip address\n");
        return 0;
    }

    int client_sock;
    char buffer[BUFF_SIZE];
    struct sockaddr_in server_addr;
    int bytes_sent, bytes_received, sin_size;

    //Step 1: Construct a UDP socket
    if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed\n");
        return 0;
    }

    //Step 2: Define the address of the server
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //Step 3: Communicate with server
    while (1) {
        printf("\nSend to server: ");
        memset(buffer, '\0', (strlen(buffer) + 1));
        fgets(buffer, BUFF_SIZE, stdin);
        // if input string is empty, exit;
        if (strcmp(buffer, "\n") == 0) {
            printf("Exiting\n");
            close(client_sock);
            return 0;
            exit(1);
        }
        sin_size = sizeof(struct sockaddr_in);

        bytes_sent = sendto(client_sock, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &server_addr, sin_size);
        if(bytes_sent < 0){
            perror("Error: ");
            close(client_sock);
            return 0;
        }


        bytes_received = recvfrom(client_sock, buffer, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);

        buffer[bytes_received] = '\0';
        printf("Reply from server_addr: \n%s\n", buffer);
    }
    return 0;
}
