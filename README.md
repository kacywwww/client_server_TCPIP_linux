# client_server_TCPIP_linux
client_server_TCP/IP_linux

This is a software for TCP/IP server and client under liunx.

software language: C
OS: Linux

# server.c

@description
This software for server side include the following function:
a) TCP server listening on a port (user input)
b) it could accept multiple clients
c) echo back clients' messages 

@Run - Server side
1. Open Terminal and enter the file dir
2. Run "gcc -o server server.c" to excute program img
3. Run "./server port#", 
        port# means the server port no. 
    ie. user input "./server 8080" to open TCP/IP server
4. wait for connection from client, server will echo what client send to server

# client.c

@description
This software for client side include the following function:
a) read command line options for server IP/port
b) periodically (e.g., every second) send a message (e.g., a sent message count) to the server
c) print out server's response 

@Run - Client side
1. Open Terminal and enter the file dir
2. Run "gcc -o client client.c" to excute program img
3. Run "./client hostname port#",
        hostname means the server hostname
        port# means the server port no. 
    ie. user input "./server localhost 8080" to open TCP/IP client
4. client will auto send package to server everysecond and print out what server respond. 

