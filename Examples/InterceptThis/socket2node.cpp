/** 
 * Socket2Node Example
 * Author: Melissa
 * Notes: This is a poor example but an example none the less, this can be reworked easily but its out of scope untill I get more time to spend on the project
*/

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include "../../src/WebXLib/Common.hpp"

#include "socket2node.hpp"

#define PORT 8089 
   
CompBuffer socketServer() 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = (char*)"Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return CompBuffer((char*)"TEST", -2);
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return CompBuffer((char*)"TEST", -2);
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return CompBuffer((char*)"TEST", -2);
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    valread = read( sock , buffer, 1024); 
    // printf("%s\n",buffer ); 

    // Shutdown the Socket (Read & Write)
    shutdown(sock, SHUT_RDWR);
    // Close the Socket
    close(sock);
    return CompBuffer(buffer, strlen(buffer) + 1); 
} 