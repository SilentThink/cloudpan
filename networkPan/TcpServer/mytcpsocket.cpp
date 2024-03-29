#include "mytcpsocket.h"
#include <QDebug>


MyTcpSocket::MyTcpSocket()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffline()));
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::recvMsg()
{
    //接收并打印数据
    qDebug()<<this->bytesAvailable();   //协议的大小
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU *pdu = mkPDU(uiMsgLen);
    pdu->uiPDULen=uiPDULen;
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    //根据消息类型的不同进行相应的操作
    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        strncpy(caPwd,pdu->caData+32,32);
        bool ret = OpeDB::getInstance().handleRegist(caName,caPwd);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;
        if(ret)
        {
            strcpy(respdu->caData,REGIST_OK);
        }
        else
        {
            strcpy(respdu->caData,REGIST_FAILED);
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu=NULL;
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        char caName[32] = {'\0'};
        char caPwd[32] = {'\0'};
        strncpy(caName,pdu->caData,32);
        strncpy(caPwd,pdu->caData+32,32);
        bool ret = OpeDB::getInstance().handleLogin(caName,caPwd);
        PDU *respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
        //如果登录成功
        if(ret)
        {
            strcpy(respdu->caData,LOGIN_OK);
            m_strName = caName;

        }
        //如果登录失败
        else
        {
            strcpy(respdu->caData,LOGIN_FAILED);
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu=NULL;

        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
//    qDebug() << caName << caPwd <<pdu->uiMsgType;

}

void MyTcpSocket::clientOffline()
{
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this);
}
