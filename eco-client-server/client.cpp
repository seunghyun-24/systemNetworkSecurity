//client

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <thread>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// if you want to use window -> #include <winsock2.h>
using namespace std;

struct Param {
    struct in_addr IPv4;
    uint16_t port;

    bool parse(int argc, char* argv[]){
        
        if(argc < 2){
            cout << "Error - parameter" << endl;
            return false;
        }

        int add = inet_pton(AF_INET, argv[1], &IPv4);
        port = stoi(argv[2]);

        return (add) && (port != 0);
        // return (IPv4.s_addr != 0) && (port != 0)
    }
} param;

void tryThread(int client_socket){ //thread 함수는 일반적으로 void 

    int BUFFERSIZE = 1024;
    char buf[BUFFERSIZE];

    int byteSize;

    while(1){
        byteSize = recv(client_socket, buf, BUFFERSIZE, 0);

        if(byteSize == 0 || byteSize < 0 /*|| byteSize == SOCKET_ERROR*/){
            cout << "Error no data" << endl;
            return;
        }

        buf[byteSize] = '\0';

        cout << "Received : " << buf << endl;
    }

    close(client_socket);
    return;
}

int main(int argc, char* argv[]){

    if(!param.parse(argc, argv)){
        cout << "Error" << endl;
        // 형식 알려주기
        return -1;
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        cout << "Error - socket" << endl;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(param.port);
	addr.sin_addr = param.IPv4;

    int check = connect(client_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (check == -1){
        cout << "Error - connect" << endl;
        return -1;
    }

    //thread
    thread _thread(tryThread, client_socket);
    _thread.detach(); //_thread.join() ??

    int check2;
    while(1){
        // 한 줄씩 읽어주라고..? 그건 server인가?
        string str;
        getline(cin, str);
        check2 = send(client_socket, str.c_str(), str.size(), 0);

        if(check2 == -1 /*|| check2 == SOCKET_ERROR */){
            cout << "Error - send" << endl;
            return -1;
        }
    }
    
    close(client_socket);

}
