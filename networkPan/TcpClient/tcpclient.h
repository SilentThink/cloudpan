#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include "protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();

public slots:
    void showConnect();
    void recvMsg();

private slots:
//    void on_pb_send_released();

    void on_login_pb_released();

    void on_regist_pb_released();

    void on_logout_pb_released();

private:
    Ui::TcpClient *ui;
    QString m_strIP;
    quint16 m_usPort;//无符号短整型

    //连接服务器，和服务器进行数据交互
    QTcpSocket m_tcpSocket;
};
#endif // TCPCLIENT_H
