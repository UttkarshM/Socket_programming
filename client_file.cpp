#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<cstring>
#include<string.h>
#define port 8080
#define size 1024

using namespace std;

void chat(int sockfd){
    char buffer[size];
    int n;
    while(1){
        n=0;
        memset(buffer,0,sizeof(buffer));
        read(sockfd,buffer,sizeof(buffer));
        cout<<"SERVER:"<<buffer<<endl;
        if(strncmp(buffer,"exit",4)==0){
            break;
        }
        memset(buffer,0,sizeof(buffer));
        cout<<"ENTER THE MESSAGE TO BE SENT TO THE SERVER:";
        while((buffer[n++]=getchar())!='\n'); //reads the string until you hit enter which is '\n
        write(sockfd,buffer,sizeof(buffer));
        if(strncmp(buffer,"exit",4)==0){
            break;
        }
    } 
    cout<<"SERVER CONNECTION IS OVER.."<<endl;
}
int main(){
    struct sockaddr_in client;
    int sockfd_c=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd_c<0){
        cout<<"ERROR IN CREATING A SOCKET";
        exit(1);
    }
    memset(&client,0,sizeof(client));
    client.sin_family=AF_INET;
    client.sin_port=htons(port);
    client.sin_addr.s_addr= inet_addr("127.0.0.1");//127.0.0.1 is the universal ip for a local host
    cout<<"SOCKET HAS BEEN CREATED:"<<endl;
   int status=connect(sockfd_c,(struct sockaddr*)&client,sizeof(client));
   if(status<0){
    cout<<"CONNECTION PROBLEM";
    exit(1);
   }
   cout<<"CONNECTED...."<<endl;
   chat(sockfd_c);
    close(sockfd_c);
    return 0;
}