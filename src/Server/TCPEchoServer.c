#include <stdio.h>      // printf(), fprintf()
#include <sys/socket.h> // socket(), connect(), send(), recv()
#include <arpa/inet.h>  // socketaddr_in, inet_addr()
#include <stdlib.h>     // atoi(), exit()
#include <string.h>     // memset()
#include <unistd.h>     // close()

#define MAXPENDING 5    // 同時にキュー可能な接続要求の最大数

void DieWithError(char *errorMessage);
void HandleTCPClient(int clntSocket);

int main(int argc, char *argv[]){
    int servSock;                       // サーバーのソケットディスクリプタ
    int clntSock;                       // クライアントのソケットディスクリプタ
    struct sockaddr_in echoServAddr;    // ローカルアドレス
    struct sockaddr_in echoClntAddr;    // クライアントアドレス
    unsigned short echoServPort;        // サーバーポート
    unsigned int clntLen;               // クライアントのアドレスの構造体の長さ

    // 引数に関してのエラー処理
    if (argc != 2){
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    // 1つ目の引数はサーバーのローカルポート
    echoServPort = atoi(argv[1]);

    // 着信接続用のソケットを作成
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        DieWithError("socket() failed");
    }

    // ローカルアドレスの構造体を作成
    memset(&echoServAddr, 0, sizeof(echoServAddr));     // 構造体の占有するメモリを0埋め
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // IPアドレス
    echoServAddr.sin_port = htons(echoServPort);        // ポート番号

    // ローカルアドレスへバインド
    if(bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
        DieWithError("bind() failed");
    }

    // 接続要求を待ち受け
    if(listen(servSock, MAXPENDING) < 0){
        DieWithError("listen() failed");
    }

    for(;;){
        clntLen = sizeof(echoClntAddr);

        if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0){
            DieWithError("accept() failed");
        }

        

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        HandleTCPClient(clntSock);
    }
}