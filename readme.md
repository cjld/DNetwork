#socket简单封装

socket的简单封装，附带多线程和异步，使用Qt信号槽开发

用法

-   `DServer`

    -   `bool listen(const char *ip, int port)`
        监听ip:port，返回是否监听成功

    -   `void newConnection(DSocket*)` [signal]
        当有用户连接服务器时，将会发射此信号，并且给出`DSocket`

-   `DSocket`

    -   `bool connectToHost(const char *ip, int port)`
        连接服务器ip:port，返回是否连接成功

    -   `void write(string msg)`
        向socket发送信息msg

    -   `void getMsg(string)` [signal]
        从socket接受到信息是，发出信号

DEMO的简单用法
1.  打开一个新窗口，点`创建`，作为服务器
2.  打开一个新窗口，输入ip后点`连接`，若为本机则输入`127.0.0.1`，作为客户端
3.  可重复步骤2，连接多个客户端
4.  在输入框输入，点击发送，即实现信息发送
