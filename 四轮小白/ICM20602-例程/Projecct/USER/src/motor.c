#include "motor.h"




int LeftMotorSpeedSet;
int RightMotorSpeedSet;
int StopCarFlag=0;
int LeftSpeed;
int LeftMotorOutput;
int RightSpeed;
int RightMotorOutput;

//----------------------------------------------------------------------
//  @brief      电机闭环
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void LeftMotorSpeedContural()
{
      LeftSpeed=ftm_quad_get(ftm2);
      ftm_quad_clean(ftm2);
      
      //LeftMotorOutput=SpaceMotorPID(&LeftMotorDuty,LeftSpeed,LeftMotorSpeedSet);
      LeftMotorOutput+=IncrementMotorPID(&LeftMotorDuty,LeftSpeed,LeftMotorSpeedSet);
      LeftMotorOutput=(int)limit(LeftMotorOutput,900);
      //OLED_Print_Num1(0,2,LeftSpeed);
      if(LeftMotorOutput>=0)
      {
           ftm_pwm_duty(ftm0,ftm_ch0,0);
           ftm_pwm_duty(ftm0,ftm_ch1,LeftMotorOutput);
      }
      else if(LeftMotorOutput<0)
      {
            ftm_pwm_duty(ftm0,ftm_ch0,LeftMotorOutput);
            ftm_pwm_duty(ftm0,ftm_ch1,0);
      }
}


//----------------------------------------------------------------------
//  @brief      电机闭环
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void RightMotorSpeedContural()
{
      RightSpeed=lptmr_pulse_get();	
      lptmr_pulse_clean();
      //OLED_Print_Num1(0,4,RightSpeed);
      //RightMotorOutput=SpaceMotorPID(&RightMotorDuty,RightSpeed,RightMotorSpeedSet);
      RightMotorOutput+=IncrementMotorPID(&RightMotorDuty,RightSpeed,RightMotorSpeedSet);
      RightMotorOutput=(int)limit(RightMotorOutput,900);
      
      if(RightMotorOutput>=0)
      {
           ftm_pwm_duty(ftm0,ftm_ch2,0);
           ftm_pwm_duty(ftm0,ftm_ch3,RightMotorOutput);
      }
      if(RightMotorOutput<0)
      {
           ftm_pwm_duty(ftm0,ftm_ch2,RightMotorOutput);
           ftm_pwm_duty(ftm0,ftm_ch3,0);
      }
}



//----------------------------------------------------------------------
//  @brief      停车
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void StopCar()
{
      AD_Voltage[0]=adc_once(LeftAd,ADC_12bit);//左边的斜电感
      AD_Voltage[1]=adc_once(RightAd,ADC_12bit);//右边的斜电感
      if(AD_Voltage[0]+AD_Voltage[1]<AD_Voltage_Min[0]+AD_Voltage_Min[1]+50)
      {
            StopCarFlag=1;
      }
      if(StopCarFlag)
      {
            if((LeftSpeed)>5)
            {
                  ftm_pwm_duty(ftm0,ftm_ch0,300);
                  ftm_pwm_duty(ftm0,ftm_ch1,0);
            }
            else if(abs(LeftSpeed)<5)
            {
                  ftm_pwm_duty(ftm0,ftm_ch0,0);
                  ftm_pwm_duty(ftm0,ftm_ch1,0);
            }
            if((RightSpeed)>5)
            {
                  ftm_pwm_duty(ftm0,ftm_ch2,300);
                  ftm_pwm_duty(ftm0,ftm_ch3,0);
            }
            else if(abs(RightSpeed)<5)
            {
                  ftm_pwm_duty(ftm0,ftm_ch2,0);
                  ftm_pwm_duty(ftm0,ftm_ch3,0);
            }

      }
}
//----------------------------------------------------------------------
//  @brief      差速
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void Differential(float Err)
{
      if(Err>0)
      {
            LeftMotorSpeedSet=(int)(SpeedSet-0.3*Err);
            if(LeftMotorSpeedSet<0)
            {
                  LeftMotorSpeedSet=0;
            }
            RightMotorSpeedSet=SpeedSet;
      }
      else if(Err<-0)
      {
            RightMotorSpeedSet=(int)(SpeedSet+0.3*Err);
            if(RightMotorSpeedSet<0)
            {
                  RightMotorSpeedSet=0;
            }
            LeftMotorSpeedSet=SpeedSet;
      }
      else
      {
            LeftMotorSpeedSet=SpeedSet;
            RightMotorSpeedSet=SpeedSet;
      }
}