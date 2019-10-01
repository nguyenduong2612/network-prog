#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFF_SIZE 1024

char *resultMessage(char buffer[], int len) {
    int n = 0, m = 0, isError = 0;
    char number[BUFF_SIZE];
    char alpha[BUFF_SIZE];
    for (int i = 0; i < len - 1; i++) {
        char ch = buffer[i];
        if (ch >= '0' && ch <= '9') {   //check if character is number
            number[n] = ch;
            n++;
        } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {  //check if character is alphabet;
            alpha[m] = ch;
            m++;
        } else {
            isError = 1;
        }
    }
    number[n] = '\n';
    number[n + 1] = '\0';
    alpha[m] = '\0';

    if (isError == 1) {
        return "Error";
    } else {
        return strcat(number, alpha);
    }
}

int main(int argc, char const *argv[]) {

    if (argc != 2) {
        printf("Syntax is incorrect!\n");
        return 0;
    }
    int server_sock;
    char buffer[BUFF_SIZE];
    int bytes_sent, bytes_received;
    struct sockaddr_in server, client;
    int sin_size;

   //Step 1: Construct a UDP socket
    if ((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("\nError: ");
        exit(0);
    }

    bzero(&server, sizeof(server));
    bzero(&client, sizeof(client));

    //Step 2: Bind address to socket
    server.sin_family = AF_INET; // IPv4
    server.sin_port = htons(atoi(argv[1]));   // server port
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server.sin_zero), 8); /* zero the rest of the structure */

    if (bind(server_sock, (const struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        perror("Bind failed\n");
        return 0;
    }

    //Step 3: Communicate with clients
    printf("Server started\n");
    while (1) {
        int result_len;
        sin_size = sizeof(struct sockaddr_in);

        bytes_received = recvfrom(server_sock, (char *)buffer, BUFF_SIZE, MSG_WAITALL, (struct sockaddr *)&client, &sin_size);
        if (bytes_received < 0) {
            perror("Error: ");
            close(server_sock);
            return 0;
        }
        buffer[bytes_received] = '\0';
        printf("\nRecieve from client : %s", buffer);
        result_len = strlen(resultMessage(buffer, strlen(buffer)));
        bytes_sent = sendto(server_sock, (const char *)resultMessage(buffer, strlen(buffer)), result_len, MSG_CONFIRM, (const struct sockaddr *)&client, sin_size);
        if (bytes_sent < 0) {
            perror("Error: ");
            close(server_sock);
            return 0;
        }
    }
    return 0;
}
