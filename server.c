/**
 * @file server.c
 *
 * @date 02 Feb 2023
 * @author Kacy W
 * @description
 * This software for server side include the following function:
 * a) TCP server listening on a port (e.g.,8080)
 * b) it could accept multiple clients
 * c) echo back clients' messages 
 */
//------------------------------------------------------------------------------
//  Include header
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

//------------------------------------------------------------------------------
// Define
//------------------------------------------------------------------------------
#define MaxClientQueue       (5)
#define MaxClient            (30)
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
    int opt;   
    struct sockaddr_in serverAddr, clientAddr;
    int serverPortNo;

    //client
    int masterSocket, sock_client[MaxClient], sock_new;
    int activity, valread, sd, maxsd;
    int addrLen;
    char buffer[256]; //buffer 
    int sizeofmessage;

    //descriptors
    fd_set readfds;

    //var and flag
    int i;
    int nErr = 0; //no error
    char *message = "Hello World\r\n";  

    //init
    for (i = 0; i < MaxClient; i++)
    {
        sock_client[i] = 0;
    }

    //check input correctness
    if (argc < 2) 
    {
        perror("ERROR: please provide port number\n");
        exit(1);
    }

    // create a socket : master socket
    // socket(int domain, int type, int protocol)
    masterSocket =  socket(AF_INET, SOCK_STREAM, 0);
    if (masterSocket < 0)   
    {
        perror("ERROR: Fail to open socket");
        exit(1);
    }

    nErr = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &opt, sizeof(opt));
    if(nErr < 0)
    {
        perror("ERROR: Fail to set socket opt");
        exit(1);
    }

    //init address structure
    bzero((char *) &serverAddr, sizeof(serverAddr));

    //Server address setup for bind call
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY;  //crrent host's IP address
    serverPortNo = atoi(argv[1]);  //get server port no from input
    serverAddr.sin_port = htons(serverPortNo);  //converted into network byte order

    //bind the socket to the input port number
    if (bind(masterSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("ERROR: Fail in binding");
        exit(1);
    }

    // Maximum pending connections for the master socket is set to 5
    printf("Listener on port %d \n", serverPortNo);
    nErr = listen(masterSocket, MaxClientQueue);
    if(nErr < 0)
    {
        perror("ERROR: Fail to start listen port");
        exit(1);
    }
    printf("Waiting for incoming connections.\n");
    //accepts an incoming connection
    addrLen = sizeof(clientAddr);

    //start looping for handling the mulitple clients
    while(1)
    {
        //clear the socket
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(masterSocket, &readfds);
        maxsd = masterSocket;

        //add children socket to set 
        for (i = 0; i < MaxClient; i++)
        {
            //socket descriptor 
            sd = sock_client[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
            {
                FD_SET(sd, &readfds);
            }

            //update the maximum file descriptor number for selection
            if(sd > maxsd)
            {
                maxsd = sd;
            }
        }

        //wait for an activity in one of the sockets, no timeout
        activity = select(maxsd+1, &readfds, NULL,NULL,NULL);
        if((activity < 0) && (errno!=EINTR))
        {
            printf("ERROR: select error");
        }

        //If master socket is updated, accept incoming connection.
        if(FD_ISSET(masterSocket, &readfds))
        {
            //communicating with the connected client.
            sock_new = accept(masterSocket,(struct sockaddr *) &clientAddr, &addrLen);
            if (sock_new < 0) 
            {
                perror("ERROR: Fail to accept socket");
                exit(1);
            }
            printf("New connection , socket fd is %d\n",sock_new );

            //send Hello World
            message = "Hello World\r\n"; 
            nErr = send(sock_new, message, strlen(message),0);
            if(nErr !=  strlen(message))
            {
                perror("WARNING: Message sending error");
            }

            //add new socket to tje sockets array
            for(i = 0; i < MaxClient; i++)
            {
                if(sock_client[i] == 0)
                {
                    sock_client[i] = sock_new;
                    break;
                }
            }
        }

        //IO operation in the list of socket
        for (i = 0; i < MaxClient; i++)
        {
            if(sock_client[i] > 0)
            {
                sd = sock_client[i];

                if(FD_ISSET(masterSocket, &readfds));
                {
                    //check status of the socket
                    valread = read(sd, buffer, 255);
                    if(valread == 0)
                    {
                        //client disconnected
                        close(sd);
                        sock_client[i] = 0;
                    }
                     if(valread > 0)
                    {
                        //Send Echo for the incoming message
                        buffer[valread] = '\0'; //NULL means end of string
                        printf("client %d: %s\n", sd, buffer);
                        send(sd, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }
    return 0; 
}
