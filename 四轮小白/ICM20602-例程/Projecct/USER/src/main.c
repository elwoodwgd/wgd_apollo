/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @company	   		成都逐飞科技有限公司
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN and MK60FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/

#include "headfile.h"
#include <string.h>

      
       
uint8 dis_image[40][100];
uint8 Threshold;


void S3010PIDCamera(PID *sptr)  //摄像头的舵机控制PD
{
      sptr->Kp=3.0;
      sptr->Kd=15;
}

void S3010PIDInductive(PID *sptr)//电感的舵机控制PD               
{
      sptr->Kp=2.25;            //2.45   8.5    63
      sptr->Kd=16;
}

void LeftMotorContural(PID *sptr)
{
      sptr->Kp=5;
      sptr->Ki=0.6;
}

void RightMotorContural(PID *sptr)
{
      sptr->Kp=5;
      sptr->Ki=0.6;
}

void LeftMotor(PID *sptr)
{
      sptr->Kp=5;
      sptr->Ki=3;
      sptr->Kd=0;
}

void RightMotor(PID *sptr)
{
      sptr->Kp=5;
      sptr->Ki=3;
      sptr->Kd=0;
}

int SpeedSet=75;

int main(void)
{
      get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
      
      SystemInit();
      
      S3010PIDCamera(&S3010DutyCamera);
      S3010PIDInductive(&S3010DutyInductive);
      LeftMotorContural(&LeftMotorDuty);
      RightMotorContural(&RightMotorDuty);
      LeftMotor(&LeftMotorDuty);
      RightMotor(&RightMotorDuty);
      
      pit_init_ms(pit0,5);
      set_irq_priority(PIT0_IRQn,0);		//璁剧疆浼樺厛绾?鏍规嵁鑷繁鐨勯渶姹傝缃?
      enable_irq(PIT0_IRQn);			//鎵撳紑pit0鐨勪腑鏂紑鍏?
      EnableInterrupts;
      
            
      for(;;)
      {
//            if(mt9v032_finish_flag)
//            {
//                  mt9v032_finish_flag=0;
//                  
//                  if(KEY_three==1)
//                  {
//                         uint8 num;
//                         Threshold=OtsuThreshold(&image[0][0],100,40)+5;
////                          CameraHandle();
//                          for(num=0; num<40; num++)
//                         {
//                              memcpy(dis_image[num],&image[num][0],100);
//                         }
//                          dis_bmp(40,100,dis_image[0],Threshold);
//                      //S3010DutyOutput=SpaceAdPID(&S3010DutyInductive);          //电感舵机输出
                      Uart1_Send_AF(LeftMotorSpeedSet,LeftSpeed,LeftMotorOutput,RightMotorSpeedSet,RightSpeed,RightMotorOutput,AD_Voltage[0],AD_Voltage[1]);
                     // send_wave();
//                  }
//            }
      }
}
