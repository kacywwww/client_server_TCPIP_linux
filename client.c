/**
 * @file clinet.c
 *
 * @date 02 Feb 2023
 * @author Kacy W
 * @description 
 * This software for client side include the following function:
 * a) read command line options for server IP/port
 * b) periodically (e.g., every second) send a message (e.g., a sent message count) to the server
 * c) print out server's response 
 */
//------------------------------------------------------------------------------
//  Include header
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//------------------------------------------------------------------------------
// Define
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// functions definition
//------------------------------------------------------------------------------
/**
 * @brief  main
 *
 * @param
 * @return
 */ 
int main(int argc, char *argv[])
{
    int sockfd, portno, sizeofmessage;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    clock_t t;
    int cnt = 0;

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    //a) read command line options for server IP/port
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR: Fail to open socket");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR: Wrong hostname\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("ERROR: Fail to connect");
        exit(1);
    }

    t = clock();
    cnt = 0;
    while(1)
    {
        if(clock() - t > CLOCKS_PER_SEC)
        {
            t = clock();
            cnt++;
            // printf("Please enter the message: ");
            bzero(buffer,256);
            // fgets(buffer,255,stdin);
            buffer[0] = cnt/10000 + '0';
            buffer[1] = cnt/1000 + '0';
            buffer[2] = cnt/100 + '0';
            buffer[3] = cnt/10 + '0';
            buffer[4] = cnt%10 + '0';
            buffer[5] = '\0';
            sizeofmessage = write(sockfd, buffer, strlen(buffer));
            if (sizeofmessage < 0) 
            {
                // perror("ERROR writing to socket");
            }
            bzero(buffer,256);
            sizeofmessage = read(sockfd, buffer, 255);
            if (sizeofmessage < 0) 
            {
                //perror("ERROR reading from socket");
            }
            else
            {
                printf("Server: %s\n", buffer);
            }
        }
        
        
    }
    
    close(sockfd);
    return 0;
}
