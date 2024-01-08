#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QByteArray>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>

TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    resize(500,250);
    loadConfig();

    connect(&m_tcpSocket,SIGNAL(connected())
            ,this,SLOT(showConnect()));

    connect(&m_tcpSocket,SIGNAL(readyRead())
            ,this,SLOT(recvMsg()));

    //连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);

}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();
        file.close();

        strData.replace("\r\n"," ");
        QStringList strList = strData.split(" ");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug() << "ip:" <<m_strIP <<" port:"<<m_usPort;
    }
    else
    {
        QMessageBox::critical(this,"open config","open config failed");
    }
}

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void TcpClient::recvMsg()
{
    //接收并打印数据
    qDebug()<<m_tcpSocket.bytesAvailable();   //协议的大小
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);
    pdu->uiPDULen=uiPDULen;
    m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        if(strcmp(pdu->caData,REGIST_OK) == 0)
        {
            QMessageBox::information(this,"注册",REGIST_OK);
        }
        else if(strcmp(pdu->caData,REGIST_FAILED) == 0)
        {
            QMessageBox::information(this,"注册",REGIST_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        if(strcmp(pdu->caData,LOGIN_OK) == 0)
        {
            QMessageBox::information(this,"登录",LOGIN_OK);
        }
        else if(strcmp(pdu->caData,LOGIN_FAILED) == 0)
        {
            QMessageBox::information(this,"登录",LOGIN_FAILED);
        }
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
}

#if 0
void TcpClient::on_pb_send_released()
{
    QString strMsg = ui->lineEdit->text();
    if(!strMsg.isEmpty())
    {
        PDU *pdu=mkPDU(strMsg.size());
        memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
        pdu->uiMsgType=8888;
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this,"信息发送","发送的信息不能为空");
    }
}
#endif

void TcpClient::on_login_pb_released()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this,"登录","登录失败：用户名或密码为空");
    }
}

void TcpClient::on_regist_pb_released()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isEmpty()&&!strPwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData, strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this,"注册","注册失败：用户名或密码为空");
    }
}

void TcpClient::on_logout_pb_released()
{

}
