#!/bin/bash

gcc ../src/DieWithError.c ../src/Server/HandleTCPClient.c ../src/Server/TCPEchoServer.c -o Server
gcc ../src/Client/TCPEchoClient.c ../src/DieWithError.c -o Client