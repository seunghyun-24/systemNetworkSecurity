//server

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <set>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// if you want to use window -> #include <winsock2.h>
using namespace std;

set<int> clients;

struct Param {
    bool echo = false;
    bool broadcast = false;
    uint16_t port;

    bool parse(int argc, char* argv[]){
        
        if(argc < 2){
            cout << "Error - parameter" << endl;
            return false;
        }

        int c;
        while( (c = getopt(argc, argv, "eb")) != -1) {
        // -1 means getopt() parse all options
            switch(c) {
                case 'e':
                    echo = true;
                    break;
                case 'b':
                    broadcast = true;
                    break;
                default:
                    return false;
                }
        }
        port = stoi(argv[1]);

        return (port != 0);
    }
} param;

void tryThread(int client_socket){ //thread 함수는 일반적으로 void 

    int BUFFERSIZE = 1024;
    char buf[BUFFERSIZE];

    int byteSize;

    while(1){
        byte_size = recv(client_socket, buf, BUFFERSIZE, 0);

        if(byte_size == 0 || byte_size < 0 /*|| byte_size == SOCKET_ERROR*/){
            cout << "Error no data" << endl;
            return;
        }

        buf[byte_size] = '\0';

        cout << buf << endl;


        if(param.echo){
            int check2 = send(client_socket, buf, byte_size, 0);
            if(check2 == -1 /*|| check2 == SOCKET_ERROR */){
                cout << "Error - send" << endl;
                return -1;
            }
        }
        
        if(param.broadcast){
            for(int sock: clients){
                int check3 = send(sock, buf, byte_size, 0);
                if(check3 == -1 /*|| check2 == SOCKET_ERROR */){
                    cout << "Error - send" << endl;
                    return -1;
                }
            }
        }
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

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        cout << "Error - socket" << endl;
        return -1;
    }

    //음 그니까 서버쪽은 무슨 소켓을 먼저 열어두고 (오픈? 헬로우?)
    int optval = 1;
    int check = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (check == -1){
        cout << "Error - setsockopt" << endl;
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
	  addr.sin_family = AF_INET;
    addr.sin_port = htons(param.port);
	  addr.sin_addr.s_addr = INADDR_ANY; //htonl 안해줘도 되나 

    //bind 해줘야함
    int check2 = bind(server_socket, (struct sockaddr*)&addr, sizeof(addr));

    if(check2 == -1){
        cout << "Error - bind" << endl;
        return -1;
    }

    int check3 = listen(server_socket, 5);
    if (check3 == -1){
        cout << "Error - listen" << endl;
        return -1;
    }

    while(1){
        struct sockaddr_in client_addr;
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, sizeof(client_addr));

        if (client_socket == -1) {
            cout << "Error - accpet" << endl;
            return -1;
        }
        else clients.insert(client_socket);
        thread* _thread = new thread( tryThread, client_socket);
		   _thread.detach();
    }

    close(server_socket);
}
