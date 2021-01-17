/*
*************************************************
* @version: ??
* @author: Wuke
* @license: MIT Licence
* @contact: wukeee.wisdom@qq.com
* @software: Qt Creator
* @file: main.cpp
* @time: 2020/12/27 20：34
* @lastEdit: 2020/12/28 10:05
*************************************************
*/

#include <iostream>
#include <proxyserver.h>
#include <QApplication>

using namespace std;

#define LISTEN_PORT 8080

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ProxyServer* ps = new ProxyServer();
    if(!ps->ServerOn(LISTEN_PORT)){
        cout << "服务器运行失败..." << endl;
        return -1;
    }
    cout << "代理服务器正在运行，监听端口:" << LISTEN_PORT << "..." << endl;

    return a.exec();
}












