#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void terminate(char err[20])
{
    printf("%s",err);
    exit(-1);
}

void main()
{
    int status,portserver,sock,end=0;
    struct sockaddr_in server;
    char *adrserver,message[1024];
    struct sockaddr name;

    ///Allocating memory for the IP address.
    adrserver=(char*)malloc(32);

    printf("TCP Client Calculator:\n");
    printf("Enter the IP address of the server (a.b.c.d):");

    do
    {
        scanf("%s",adrserver);

        ///Converting the IPv4(numbers and dots) to binary(network byte order).
        if(inet_aton(adrserver, &server.sin_addr)!=0)
        end=2;
    else
    printf("Invalid Address!\tRe-enter it correctly please:");
    }while(end==0);

    printf("Enter the server port number:");
    scanf("%d",&portserver);
    printf("Socket creation...\n");

    ///socket(communication domain,type,protocol).
    ///AF_INET(IPv4): AF stands for Address Family. INET stands for INTERNET(INternET).
    ///SOCK_STREAM: Socket stream used by TCP.
    ///6: TCP choice.
    sock=socket(AF_INET,SOCK_STREAM,6); ///Returns socket ID.

    if(sock==-1)
        terminate("Error creating socket!\tThe program will terminate!\n");

    server.sin_family=AF_INET; ///AF_INET means IPv4.
    server.sin_port=htons(portserver); ///convert from host to network.
    status=connect(sock,(const struct sockaddr*)&server,sizeof(server));

    if(status!=0)
        terminate("Error Connection!\tThe program will terminate!\n");

    printf("Connection was established successfully!\n\n");

    ///recv() returns the number of characters received.
    status=recv(sock,message,1024,0);
    message[status]='\0'; ///Terminating the string.

    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    printf("%s",message);

    while(1){
    status=recv(sock,message,1024,0);
    message[status]='\0';
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    printf("%s",message);
    ///Reading and sending the first number.
    scanf("%s",message);

    ///send() returns the number of characters send.
    status=send(sock,message,strlen(message),0);
    if(strcmp(message,"end")==0)
        break;
    if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    ///Waiting for the ask of the second number.
    status=recv(sock,message,1024,0);
    message[status]='\0';
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    printf("%s",message);
    ///Reading and sending the second number.
    scanf("%s",message);
    status=send(sock,message,strlen(message),0);
    if(strcmp(message,"end")==0)
        break;
    if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    status=recv(sock,message,1024,0);
    message[status]='\0';
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    printf("%s",message);
    ///Reading and sending the operation.
    scanf("%s",message);
    status=send(sock,message,strlen(message),0);
    if(strcmp(message,"end")==0)
        break;
    if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    status=recv(sock,message,1024,0);
    message[status]='\0';
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    printf("%s",message);
    }
    printf("The program will terminate!");
 }
