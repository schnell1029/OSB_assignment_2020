#include <stdio.h>      // printf(), fprintf()
#include <sys/socket.h> // socket(), connect(), send(), recv()
#include <arpa/inet.h>  // sockaddr_in, inet_addr()
#include <stdlib.h>     // atoi(), exit()
#include <string.h>     // memset()
#include <unistd.h>     // close()

#define RCVBUFSIZE 32   // 受信バッファのサイズ

void DieWithError(char *errorMessage);  //エラー処理関数

int main(int argc, char *argv[]){
    int sock;                           // ソケットディスクリプタ
    struct sockaddr_in echoServAddr;    // エコーサーバーのアドレス
    unsigned short echoServPort;        // エコサーバーのポート番号
    char *servIP;                       // エコーサーバーのIPアドレス
    char *echoString;                   // エコーサーバに送信する文字列
    char echoBuffer[RCVBUFSIZE];        // エコー文字列用のバッファ
    unsigned int echoStringLen;         // エコーする文字列のサイズ
    int bytesRcvd, totalBytesRcvd;      // 一回のrecv()で読み取られるバイト数と全バイト数


    if((argc < 3) || (argc > 4)){   // 引数の数は3か4
        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];       // 1つ目の引数: サーバーのIPアドレス
    echoString = argv[2];   // 2つ目の引数: 送信する文字列

    if(argc == 4){
        echoServPort = atoi(argv[3]);   // ポートを指定
    }else{
        echoServPort = 7;
    }

    // 信頼性の高いストリームソケットを作成
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        DieWithError("socket() failed");
    }

    // サーバーのアドレス構造体を作成
    memset(&echoServAddr, 0, sizeof(echoServAddr)); // 0埋め
    echoServAddr.sin_family         = AF_INET;
    echoServAddr.sin_addr.s_addr    = inet_addr(servIP);
    echoServAddr.sin_port           = htons(echoServPort);

    // サーバーへの接続を確率
    if(connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0){
        DieWithError("connect() failed");
    }

    // 入力データの長さを調べる.
    echoStringLen = strlen(echoString);

    // 文字列をサーバーに送信
    if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
        DieWithError("send() sent a different number of bytes than expected");
    }

    // 同じ文字列をサーバーから受信
    totalBytesRcvd = 0;
    printf("Recieved: ");   // エコーからされた文字列を表示するための準備
    while(totalBytesRcvd < echoStringLen){
        // バッファサイズに達するまでサーバーからのデータを受信する
        if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
            DieWithError("recv() failed or connection closed prematurely");
        }
        totalBytesRcvd += bytesRcvd;    // 総バイト数の集計
        echoBuffer[bytesRcvd] = '\0';   // 文字列の終端
        printf("%s", echoBuffer);       // エコーバッファの表示
    }
    printf("\n");                       // 最後の開業を出力

    close(sock);
    exit(0);
}