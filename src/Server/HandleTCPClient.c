#include <stdio.h>      //printf(), fprintf()
#include <sys/socket.h> //recv(), send()
#include <unistd.h>     //close()

#define RCVBUFSIZE 32
void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket){
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;

    /*
        ソケットでのデータ受信
        echoBufferに受信内容を格納する(最大RCVBUFSIZEバイト)
    */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
        DieWithError("recv() failed");
    }

    while(recvMsgSize > 0){ //0は転送終了

        //メッセージをそのままクライアントへ返す.
        if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize){
            DieWithError("send() failed");
        }

        //受信するデータが残っていないか確認する.
        if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
            DieWithError("recv() failed");
        }
    }

    close(clntSocket);
}