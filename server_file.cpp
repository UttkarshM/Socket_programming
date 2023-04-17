#include<iostream>
#include<unistd.h>
#include<chrono>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstring>
#include<netdb.h>
#define port 8080
#define size 1024
using namespace std;

void chat(int sockfd){
    char buffer[size];
    int n;
    while(1){
        n=0;
        memset(buffer,0,size);//empties all the elements in buffer
        cout<<"ENTER THE MESSAGE TO BE SENT TO THE CLIENT:";
        while((buffer[n++]=getchar())!='\n'); // until you hit enter key which stands for \n it keeps taking input.,here n gets incremented on its own
        write(sockfd,buffer,sizeof(buffer)); // send the message to the server
        if(strncmp(buffer,"exit.",4)==0){//strncmp is required as its an (character) array
            break;
        }
        memset(buffer,0,size);//resetting the values of the elements in the buffer
        read(sockfd,buffer,sizeof(buffer));//msg from the server is stored in buffer
        cout<<buffer<<endl;//displays the info
        if(strncmp(buffer,"exit.",4)==0){//strncmp is required as its an (character) array
            break;
        }
    
    }
    cout<<"CLIENT CONNECTION OVER..."<<endl; // if either the client or the server exits the connection then this happens
}

int main(){

    using namespace literals::chrono_literals;
    struct sockaddr_in server;
    int opt=1,valread;
    int newsockfd,sockfd=socket(AF_INET,SOCK_STREAM,0);
    //char buffer[size],response[size];
    socklen_t siz;

    std::chrono::time_point<std::chrono::system_clock>start,end;
    std::chrono::duration<float> duration;

    start=std::chrono::system_clock::now();
    if(sockfd<0){
        cout<<"ERROR IN CREATING A SOCKET"<<endl;
        exit(1);
    }
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt))){
        cout<<"SETSOCKOPT"<<endl;//this is for reusing a port if its not free and is dealing with another
        exit(1);
    }
    cout<<"SOCKET HAS BEEN CREATED SUCCESSFULLY"<<endl;
    memset(&server,'\0',sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(port);
    server.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sockfd,(struct sockaddr*)&server,sizeof(server))<0){
        cout<<"ERROR IN BINDING";
        exit(1);
    }
    cout<<"BINDING has been successful"<<endl;
    
    if(listen(sockfd,100)<0){
        cout<<"NO DEVICES FOUND";
        exit(0);
    };

    cout<<"DEVICE CONNECTED"<<endl;
   
    siz=sizeof(server);

    newsockfd=accept(sockfd,(struct sockaddr*)&server,&siz); //need in socklen format for socket file descriptors.
    end=std::chrono::system_clock::now();
    float timing=duration.count()*1000; // the time is small hence we are giving it in milli second format by mulitplying it by 1000.
    cout<<"THE TIME TAKEN TO CONNECT IS (idk why im using caps lmaooo):"<<timing<<"ms"<<endl;
    chat(newsockfd);
    close(newsockfd);
    shutdown(sockfd,SHUT_RDWR);
    return 0;
}
