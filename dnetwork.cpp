#include "dnetwork.h"
#include <QDebug>

DServer::DServer(QObject *parent) : QObject(parent) {
    listenThread=new DServerListenThread(this);
    qRegisterMetaType<string>("string");
}

DSocket::DSocket(SOCKET *soc, QObject *parent) : QObject(parent) {
    qRegisterMetaType<string>("string");
    recvThread=new DSocketRecvThread(this);
    if (soc!=0) socket=*soc, recvThread->start();
}

bool DServer::listen(const char *ip, int port) {
    //Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 ) {
        qDebug()<<"Init Windows Socket Failed::"<<GetLastError();
        return 0;
    }

    //Create Socket
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( ServerSocket == INVALID_SOCKET ) {
        qDebug()<<"Create Socket Failed::"<<GetLastError();
        return 0;
    }

    LocalAddr.sin_family = AF_INET;
    LocalAddr.sin_addr.s_addr = inet_addr(ip);
    LocalAddr.sin_port = htons(port);
    memset(LocalAddr.sin_zero, 0x00, 8);

    //Bind Socket
    Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
    if ( Ret != 0 ) {
        qDebug()<<"Bind Socket Failed::"<<GetLastError();
        return 0;
    }

    Ret = ::listen(ServerSocket, 10);
    if ( Ret != 0 ) {
        qDebug()<<"listen Socket Failed::"<<GetLastError();
        return 0;
    }

    qDebug()<<"服务端已经启动";
    listenThread->start();
    return 1;
}

void DServerListenThread::run() {
    while (1) {
        server->AddrLen = sizeof(server->ClientAddr);
        server->CientSocket = accept(server->ServerSocket, (struct sockaddr*)&(server->ClientAddr), &(server->AddrLen));
        if ( server->CientSocket == INVALID_SOCKET ) {
            qDebug()<<"Accept Failed::"<<GetLastError();
            break;
        }
        DSocket *socket=new DSocket(&(server->CientSocket));
        emit server->newConnection(socket);
    }
}

void DSocketRecvThread::run() {
    int Ret = 0;
    char RecvBuffer[MAX_PATH];

    while ( true ) {
        memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
        Ret = recv(socket->socket, RecvBuffer, MAX_PATH, 0);
        if ( Ret == 0 || Ret == SOCKET_ERROR ) {
            qDebug()<<"客户端退出!";
            break;
        }
        emit socket->getMsg(string(RecvBuffer));
    }
}

bool DSocket::connectToHost(const char *ip, int port) {
    static bool cd=0;
    if (cd) {
        qDebug()<<"已连接";
        return 1;
    }
    //Init Windows Socket
    if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 ) {
        qDebug()<<"Init Windows Socket Failed::"<<GetLastError();
        return 0;
    }

    //Create Socket
    CientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( CientSocket == INVALID_SOCKET ) {
        qDebug()<<"Create Socket Failed::"<<GetLastError();
        return 0;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(ip);
    ServerAddr.sin_port = htons(port);
    memset(ServerAddr.sin_zero, 0x00, 8);

    Ret = ::connect(CientSocket,(struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if ( Ret == SOCKET_ERROR ) {
        qDebug()<<"Connect Error::"<<GetLastError();
        return 0;
    } else {
        qDebug()<<"连接成功!";
    }
    socket=CientSocket;
    recvThread->start();
    return cd=1;
}

void DSocket::write(string msg) {
    Ret = send(socket, msg.c_str(), (int)msg.size(), 0);
    if ( Ret == SOCKET_ERROR )
        qDebug()<<"Send Info Error::"<<GetLastError();
}
