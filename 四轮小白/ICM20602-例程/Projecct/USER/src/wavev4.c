#include "wavev4.h"



#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))




/**************************�������ڷ�һ���ֽ�***************************************
*******************************************************************************/
unsigned char Uart1_Put_Char(unsigned char DataToSend)
{
      //TxBuffer[count++] = DataToSend;  
      uart_putchar (WAVE_UART,DataToSend);
      return DataToSend;
}


/*----------------------------------------------------------------------------
//����8�������ֽ�
//--------------------------------------------------------------------------*/
void Uart1_Send_AF(signed int aa,signed int bb,signed int cc,signed int dd,signed int ee,signed int ff,signed int gg,signed int hh)
{
	unsigned char sum = 0;
	
	sum += Uart1_Put_Char(0XAA);
	sum += Uart1_Put_Char(0X05);
	sum += Uart1_Put_Char(0XAF);
                sum += Uart1_Put_Char(0XF1);
                sum += Uart1_Put_Char(16);
	sum += Uart1_Put_Char(BYTE1(aa));//1
	sum += Uart1_Put_Char(BYTE0(aa));
	sum += Uart1_Put_Char(BYTE1(bb));//2
	sum += Uart1_Put_Char(BYTE0(bb));
	sum += Uart1_Put_Char(BYTE1(cc));//3
	sum += Uart1_Put_Char(BYTE0(cc));
	sum += Uart1_Put_Char(BYTE1(dd));//4
	sum += Uart1_Put_Char(BYTE0(dd));
	sum += Uart1_Put_Char(BYTE1(ee));//5
	sum += Uart1_Put_Char(BYTE0(ee));
	sum += Uart1_Put_Char(BYTE1(ff));//6
	sum += Uart1_Put_Char(BYTE0(ff));
	sum += Uart1_Put_Char(BYTE1(gg));//7
	sum += Uart1_Put_Char(BYTE0(gg));
	sum += Uart1_Put_Char(BYTE1(hh));//8
	sum += Uart1_Put_Char(BYTE0(hh));
	Uart1_Put_Char(sum);
}







