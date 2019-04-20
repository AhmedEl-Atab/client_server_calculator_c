#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>

void terminate(char err[20])
{
    printf("%s",err);
    exit(-1);
}

void main()
{
    int status,portserver,accepted,sock;
    socklen_t lgclient;
    struct sockaddr_in server,client;
    char message[1024],*operation;
    double num1,num2,result=0.0;

    printf("TCP Server Calculator:\n");
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
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    ///Attaching the created socket to the specified IP.
    status=bind(sock,(const struct sockaddr*)&server,sizeof(server));

    if(status==-1)
        terminate("Invalid bind!\tThe program will terminate!\n");

    printf("Waiting for new connection on the port:%d\n",portserver);

    ///Waiting for a connection request from a client.
    status=listen(sock,1);

    if(status==-1)
        terminate("Invalid listen!\tThe program will terminate!\n");

    ///fork() to duplicate the rest of the code for multi-client support.
    do
    {
        ///Accepts connection and returns ID.
        accepted=accept(sock,(struct sockaddr *)&client,&lgclient);
        status=fork();
    }while(status>0);

    if(accepted==-1)
        terminate("Invalid accept!\tThe program will terminate!\n");

    ///inet_ntoa() converts from binary(network byte order) to IPv4(numbers and dots).
    printf("Connection was accepted from client: %s\n",inet_ntoa(client.sin_addr));

    strcpy(message,"Welcome!\tThis is calculator program.\nEnter 'end' once you want to exit.\n\n");

    ///send function returns the number of characters sent.
    status=send(accepted,message,strlen(message),0);

    if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    while(1){
    usleep(50);
    strcpy(message,"Enter the first number:");
    status=send(accepted,message,strlen(message),0);

    if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    ///Waiting the first number.
    printf("Waiting the first number...\n");

    ///recv returns the number of characters received.
    status=recv(accepted,message,1024,0);
    message[status]='\0'; ///Terminating the string.
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    if(strcmp(message,"end")==0)
        break;

    ///atof() converts from string to double.
    num1=atof(message);

    printf("The first number received is:%f\n\n",num1);

    strcpy(message,"Enter the second number:");
    status=send(accepted,message,strlen(message),0);

     if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    printf("Waiting the second number...\n");
    status=recv(accepted,message,1024,0);
    message[status]='\0';
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    if(strcmp(message,"end")==0)
        break;
    num2=atof(message);

    printf("The second number received is:%f\n\n",num2);

    strcpy(message,"Enter the operation:");
    status=send(accepted,message,strlen(message),0);

    if(status!=strlen(message))
        terminate("Error sending!\tThe program will terminate!\n");

    printf("Waiting the operation...\n");
    status=recv(accepted,message,1024,0);
    message[status]='\0';
    if(status!=strlen(message))
        terminate("Error receiving!\tThe program will terminate!\n");

    if(strcmp(message,"end")==0)
        break;
    strcpy(operation,message);

    printf("The operation received is:%s\n\n",operation);

    ///Calculation.
    char err[20]; ///Error string.
    if(strcmp(operation,"+")==0)
            result=num1+num2;
    else if(strcmp(operation,"-")==0)
            result=num1-num2;
    else if(strcmp(operation,"*")==0)
            result=num1*num2;
    else if(strcmp(operation,"/")==0)
            {if(num2!=0)
                result=num1/num2;
             else
                strcpy(err,"Math Error!");}
    else if(strcmp(operation,"**")==0)
            result=pow(num1,num2);
    else
        strcpy(err,"Unknown operation!");

    char final_message[25];
    if(strlen(err)>0)
    {
        strcpy(final_message,err);
    }
    else
    {
        strcpy(final_message,"The result is: ");

        ///sprintf() allows to fill a buffer. We used it here to convert double to string.
        sprintf(message,"%f",result);
        strcat(final_message,message);
    }

    printf("The result is:%s\tSending to the client...\n",final_message);
    strcat(final_message,"\n\n");
    status=send(accepted,final_message,strlen(final_message),0);

    if(status!=strlen(final_message))
        terminate("Error sending!\tThe program will terminate!\n");

    printf("The result was sent successfully!\n\n");
    ///Resetting the error string.
    strcpy(err,"\0");}

    ///Closing the connection.
    close(accepted);
    printf("The program will terminate!");
    exit(-1);
}
