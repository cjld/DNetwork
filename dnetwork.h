#ifndef DNETWORK_H
#define DNETWORK_H

#include <QObject>
#include <string>
#include <windows.h>
#include <QThread>

#define MAX_PATH 100000

using std::string;

class DServer;
class DSocket;
class DServerListenThread;
class DSocketRecvThread;

class DServer : public QObject
{
    Q_OBJECT
public:
    friend class DServerListenThread;
    explicit DServer(QObject *parent = 0);
    bool listen(const char *ip, int port);

signals:
    void newConnection(DSocket*);

private:
    DServerListenThread *listenThread;
    WSADATA  Ws;
    SOCKET ServerSocket, CientSocket;
    struct sockaddr_in LocalAddr, ClientAddr;
    int Ret = 0;
    int AddrLen = 0;
    HANDLE hThread = NULL;
};

class DSocket : public QObject
{
    Q_OBJECT
public:
    friend class DSocketRecvThread;
    explicit DSocket(SOCKET *soc=0,QObject *parent = 0);
    bool connectToHost(const char *ip, int port);
    void write(string msg);

signals:
    void getMsg(string);

private:
    SOCKET socket;
    DSocketRecvThread *recvThread;

    WSADATA  Ws;
    SOCKET CientSocket;
    struct sockaddr_in ServerAddr;
    int Ret = 0;
    int AddrLen = 0;
    char SendBuffer[MAX_PATH];

};

class DServerListenThread : public QThread {
    Q_OBJECT

    DServer *server;
public:
    DServerListenThread(DServer *fa) : QThread(fa),server(fa) {}
    void run();
};

class DSocketRecvThread : public QThread {
    Q_OBJECT

    DSocket *socket;
public:
    DSocketRecvThread(DSocket *fa) : QThread(fa),socket(fa) {}
    void run();
};

#endif // DNETWORK_H
