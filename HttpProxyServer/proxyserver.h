/*****************************************************************
* @version: ??
* @author: Wuke
* @license: MIT Licence
* @contact: wukeee.wisdom@qq.com
* @software: Qt Creator
* @file: proxyserver.h
* @time: 2020/12/27 20：34
* @lastEdit: 2021/01/04 21:17
******************************************************************/

#ifndef PROXYSERVER_H
#define PROXYSERVER_H


#include <QtCore/QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include <cstring>

#define MAXSIZE 65507 //发送数据报文的最大长度
#define DATELENGTH 40


typedef struct HttpHeader {
    char method[4];
    char path[1024];
    char host[1024];
    char cookie[1024 * 10];
    HttpHeader() {
        memset(this, 0, sizeof(struct HttpHeader));
    }
} HttpHeader;

//缓存的http头，节省了cookie空间
typedef struct cache_HttpHeader {
    char method[4];
    char path[1024];
    char host[1024];
    cache_HttpHeader() {
        memset(this, 0, sizeof(struct cache_HttpHeader));
    }
} cache_HttpHeader;

typedef struct __cache {
    cache_HttpHeader httphd;
    char buffer[MAXSIZE];
    char date[DATELENGTH];
    __cache() {
        memset(this, 0, sizeof(struct __cache));
    }
}__cache;

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    QTcpSocket* ClientSocket;
signals:
    void readFinal(QTcpSocket*, QByteArray);
};

class ProxyServer : public QObject
{
    Q_OBJECT
public:
    ProxyServer();
    bool ServerOn(quint16);
    void test(QTcpSocket*, QByteArray);

protected slots:
    void accept();
    void clientDisconnect();
    void readMsg();
    void transportHostMsg();

private:
    QByteArray ByteStream;
    QTcpServer* Server;
    void parseMsg(char*, HttpHeader*);
};

#endif // PROXYSERVER_H
