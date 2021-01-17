#include "proxyserver.h"


ProxyServer::ProxyServer()
{
    //初始化
    Server = new QTcpServer();

//    connect(Server, &QTcpServer::newConnection, this, &ProxyServer::accept);
    connect(Server, SIGNAL(newConnection()), this, SLOT(accept()));
}

/****************************************************************
* @Method: ServerOn
* @FullName: ServerOn
* @Access: public
* @Returns: bool
* @Qualifier: 启动代理服务器，使用了Qt的QTcpServer库
* @Parameter: quint16 port
****************************************************************/
bool ProxyServer::ServerOn(quint16 port) {
    if(Server->listen(QHostAddress::AnyIPv4, port)) {
        return true;
    }
    return false;
}

/****************************************************************
* @Method: accept
* @FullName: accept
* @Access: private
* @Returns: void
* @Qualifier: 处理连接服务器的socket
* @Parameter: void
****************************************************************/
void ProxyServer::accept() {
    QTcpSocket* socket = Server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMsg()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnect()));

    //可添加IP过滤规则
    /***************************************************************
    * 过滤函数();
    ***************************************************************/

    /*
    if(socket->peerAddress() == QHostAddress("127.0.0.1")) {
        qDebug() << "限制访问IP: " << socket->peerAddress();
    }
    */
    qDebug() << "正常访问IP:" << socket->peerAddress();

}

/****************************************************************
* @Method: processHttpRequest
* @FullName: processHttpRequest
* @Access: protect slots
* @Returns: void
* @Qualifier: 接收客户端发送的http请求并处理
* @Parameter: void
****************************************************************/
void ProxyServer::readMsg() {
    char Buffer[1024];
    HttpHeader* htphd = new HttpHeader();
    QTcpSocket* ClientSock = static_cast<QTcpSocket*>(sender());

    QByteArray qba = ClientSock->readAll();
    qDebug() << "msg:" << qba;
    //std::memcpy(Buffer, msg, std::strlen(msg));
    std::strcpy(Buffer, qba);

    parseMsg(Buffer, htphd);

    //可添加http头过滤规则
    /***************************************************************
    * 过滤函数();
    ***************************************************************/

    TcpSocket* ProxySock = new TcpSocket;
    ProxySock->ClientSocket = ClientSock;
    connect(ProxySock, &TcpSocket::readFinal, this, &ProxyServer::test);
    connect(ProxySock, SIGNAL(readyRead()), this, SLOT(transportHostMsg()));
    connect(ProxySock, SIGNAL(disconnected()), this, SLOT(clientDisconnect()));

    ProxySock->connectToHost(htphd->host, 80);
    if(ProxySock->waitForConnected(1000)) {
        ProxySock->write(Buffer);
        qDebug() << "ProxyServer connect to[" << htphd->host << "]success...";
    }
    else
        qDebug() << "ProxyServer connect to[" << htphd->host << "]failed...";

    delete htphd;
}

/****************************************************************
* @Method: clientDisconnect
* @FullName: clientDisconnect
* @Access: protect slots
* @Returns: void
* @Qualifier: client断开连接后的析构操作
* @Parameter: void
****************************************************************/
void ProxyServer::clientDisconnect() {
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    qDebug() << "client" << socket->peerAddress() << ":" << socket->peerPort() << "disconnected...";
    socket->deleteLater();
}

/****************************************************************
* @Method: parseMsg
* @FullName: parseMsg
* @Access: private
* @Returns: void
* @Qualifier: 解析http头
* @Parameter: char* msg
* @Parameter: HttpHeader* htphd
****************************************************************/
void ProxyServer::parseMsg(char* msg, HttpHeader* htphd) {

 /*
 * GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection:close\r\n\r\n
 * GET / HTTP/1.1\\r\\nHost: www.baidu.com\\r\\nConnection:close\\r\\n\\r\\n
 */
    QString qs = msg;
    if(qs.startsWith("GET")) {
        std::strcpy(htphd->method, "GET");
    }
    else if(qs.startsWith("POST")) {
        std::strcpy(htphd->method, "POST");
    }

    char* path = qs.section(" " , 1, 1).toLatin1().data();
    std::strcpy(htphd->path, path);
    char* host = qs.section("\n\n", 1, 1).section("Host: ", 1, 1).toLatin1().data();
    std::strcpy(htphd->host, host);
}


/****************************************************************
* @Method: transportHostMsg
* @FullName: transportHostMsg
* @Access: protect slots
* @Returns: void
* @Qualifier: 接收client请求主机发送的消息
* @Parameter: void
****************************************************************/
void ProxyServer::transportHostMsg() {
    TcpSocket* socket = static_cast<TcpSocket*>(sender());
    QByteArray datagram = socket->readAll();
    emit socket->readFinal(socket->ClientSocket, datagram);
}

void ProxyServer::test(QTcpSocket* socket, QByteArray qba) {
    socket->write(qba);
}







