#include "mytcpsocket.h"
#include <QDebug>


MyTcpSocket::MyTcpSocket()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));
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
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
//    qDebug() << caName << caPwd <<pdu->uiMsgType;

}
