#include "PID.h"

PID S3010DutyCamera;
PID S3010DutyInductive;
PID LeftMotorDuty;
PID RightMotorDuty;



//----------------------------------------------------------------------
//  @brief      位置式PID初始化
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void SpacePIDInit(PID *sptr)
{
        sptr->SumErr=0;
        sptr->LastErr=0;
        sptr->Err=0;
}


//----------------------------------------------------------------------
//  @brief      位置式PID舵机控制
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
int SpaceCameraPID(PID *sptr,int NewPoint,int SetPoint)
{
        int Actual;             //最后得出的实际输出值
        
        sptr->Err=SetPoint-NewPoint;            //计算误差
        
        Actual =(int)(sptr->Kp*sptr->Err+sptr->Kd*(sptr->Err-sptr->LastErr));
        
        sptr->LastErr=sptr->Err;
        
        return ServoMid+Actual;
        
        
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief     计算误差
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
int SpaceAdPID(PID *sptr)
{
        int Actual;             //最后得出的实际输出值
        
        GET_AD_Voltage();
        AD_zero_to_one();
        sptr->Err=5000*(AD_Voltage_guiyi[0]-AD_Voltage_guiyi[1])/(AD_Voltage_guiyi[0]*AD_Voltage_guiyi[1]);
        sptr->Err=limit(sptr->Err,70);
       
//        if(sptr->Err>0)
//        {
//                sptr->Err-=3;
//        }
//        else if(sptr->Err<0)
//        {
//                sptr->Err+=3;
//        }
//        
       // OLED_Print_Num1(0,4,(int)sptr->Err);
        
        if(abs(sptr->Err)<2)
        {
            sptr->LastErr=sptr->Err;
            return ServoMid;
        }
        
        Actual =(int)(sptr->Kp*sptr->Err+sptr->Kd*(sptr->Err-sptr->LastErr));
        
        sptr->LastErr=sptr->Err;
        
        return ServoMid+Actual;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief     计算误差
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
int SpaceMotorPID(PID *sptr,int NewPoint,int SetPoint)
{
      int Actual;             //最后得出的实际输出值
        
      sptr->Err=SetPoint-NewPoint;            //计算误差
      sptr->SumErr+=sptr->Err;
      
//      if(sptr->SumErr>500)
//      {
//            sptr->SumErr=0;
//      }
      if(sptr->SumErr>1000)
      {
            sptr->SumErr=1000;
      }
      Actual =(int)(sptr->Kp*sptr->Err+sptr->Ki*sptr->SumErr);
      
      return Actual;
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief     zengliangshi 
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
int IncrementMotorPID(PID *sptr,int NewPoint,int SetPoint)
{
      int Actual;             //最后得出的实际输出值
        
      sptr->Err=SetPoint-NewPoint;            //计算误差
      
      Actual =(int)(sptr->Kp*(sptr->Err-sptr->LastErr)+sptr->Ki*sptr->Err+sptr->Kd*(sptr->Err-2*sptr->LastErr+sptr->PrevErr));
      
      sptr->PrevErr=sptr->LastErr;
      sptr->LastErr=sptr->Err;
      
      return Actual;
}