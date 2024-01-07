#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef unsigned int uint;

struct PDU
{
    uint uiPDULen;      //总的协议数据单元大小
    uint uiMsgType;     //消息类型
    char caData[64];    //文件名
    uint uiMsgLen;      //实际消息长度
    int caMsg[];        //实际消息
};

PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
