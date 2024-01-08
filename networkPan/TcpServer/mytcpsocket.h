#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QTcpSocket>
#include "protocol.h"
#include "opedb.h"


class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString getName();

public slots:
    void recvMsg();
    void clientOffline();

signals:
    void offline(MyTcpSocket* mySocket);

private:
    QString m_strName;  //用户名
};

#endif // MYTCPSOCKET_H
