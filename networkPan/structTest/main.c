#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//弹性结构体
typedef struct PDU
{
    int a;
    int b;
    int c;
    int d[];
}PDU;

int main()
{
    printf("%ld\n",sizeof(PDU));

    PDU *pdu = (PDU*)malloc(sizeof(PDU)+100*sizeof(int));
    printf("%ld\n",sizeof(*pdu));
    return 0;
}
