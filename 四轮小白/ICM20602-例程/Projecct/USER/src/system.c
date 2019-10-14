#include "system.h"




void SystemInit()
{
//      IIC_init();               //ģ��IIC��ʼ��
//      InitMPU6050();            //6050��ʼ��
      OLED_Init();
      //camera_init();
      uart_init(uart4,115200);
      
      ftm_pwm_init(ftm1,ftm_ch1,50,ServoMid);        //�������
      ftm_pwm_init(ftm0,ftm_ch0,15000,0);     //��ߵ����ת
      ftm_pwm_init(ftm0,ftm_ch1,15000,0);
      ftm_pwm_init(ftm0,ftm_ch2,15000,0);       //�ұߵ����ת
      ftm_pwm_init(ftm0,ftm_ch3,15000,0);
      AD_init();
      Switch_init();
      GET_AD_Max_and_Min();
      
      ftm_quad_init(ftm2);
      lptmr_pulse_init(lpt0_alt2,0xFFFF,LPT_Falling);
      
      SpacePIDInit(&S3010DutyCamera);
      SpacePIDInit(&S3010DutyInductive);
      SpacePIDInit(&LeftMotorDuty);
      SpacePIDInit(&RightMotorDuty);
      
      
}