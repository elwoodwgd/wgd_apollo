#include "ad.h"


int AD_Voltage[4]={0};
int AD_Voltage_guiyi[4]={0};
int AD_Voltage_Max[4]={0,0,0,0};
int AD_Voltage_Min[4]={10000,10000,10000,10000};
int AD_Voltage_Last[4]={0,0,0,0};



//-------------------------------------------------------------------------------------------------------------------
//  @brief     初始化
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
void AD_init()
{
      adc_init(LeftAd);
      adc_init(RightAd);
      adc_init(ELeftAd);
      adc_init(ERightAd);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief     采集一次ad值
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
void GET_AD_Voltage()
{
      AD_Voltage[0]=adc_ave(LeftAd,ADC_12bit,10);//左边的直电感
      AD_Voltage[1]=adc_ave(RightAd,ADC_12bit,10);//右边的直电感
//      AD_Voltage[2]=adc_once(ELeftAd,ADC_12bit);//左边的斜电感
//      AD_Voltage[3]=adc_once(ERightAd,ADC_12bit);//右边的斜电感
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief     归一化
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
void AD_zero_to_one()
{
      for(uint8 i=0;i<2;i++)
      {
            AD_Voltage_guiyi[i]=1000*(AD_Voltage[i]-AD_Voltage_Min[i])/(AD_Voltage_Max[i]-AD_Voltage_Min[i]);
            if(AD_Voltage_guiyi[i]>1000)
              AD_Voltage_guiyi[i]=1000;
            if(AD_Voltage_guiyi[i]<0)
              AD_Voltage_guiyi[i]=1;
      }
//      OLED_P8x16Str(0,0,"L");
//      OLED_Print_Num1(30,0,AD_Voltage_guiyi[0]);
//      OLED_Print_Num1(80,0,AD_Voltage[0]);
//      OLED_P8x16Str(0,2,"R");
//      OLED_Print_Num1(30,2,AD_Voltage_guiyi[1]);
//      OLED_Print_Num1(80,2,AD_Voltage[1]);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief     获取电感的最大最小值
//  @param      
//  @return     NULL					
//  @since      
//  Sample usage: 
//-------------------------------------------------------------------------------------------------------------------
void GET_AD_Max_and_Min()
{
      uint8 COLLECT_SIGN = 0;       //写入数据标志位
      while(KEY_one==1)
      {
            
            COLLECT_SIGN=1;
            static  uint8  fan=0;
            GET_AD_Voltage();
            for(uint8 i=0;i<4;i++)
            {
                  if(AD_Voltage_Max[i]<MAX(AD_Voltage[i],AD_Voltage_Last[i]))
                  {
                        AD_Voltage_Max[i]=MAX(AD_Voltage[i],AD_Voltage_Last[i]);
                  }
            }
            if(fan==0)
            {
                  for(uint8 i=0;i<4;i++)
                  {
                    AD_Voltage_Last[i]=AD_Voltage[i];
                  }
                  fan++;
            }
            for(uint8 i=0;i<4;i++)
            {
                  if(AD_Voltage_Min[i]>MIN(AD_Voltage[i],AD_Voltage_Last[i]))
                  {
                        AD_Voltage_Min[i]=MIN(AD_Voltage[i],AD_Voltage_Last[i]);
                  }               
            }
            for(uint8 i=0;i<4;i++)
            {
                  AD_Voltage_Last[i]=AD_Voltage[i];
            }
            OLED_P8x16Str(0,0,"L");
            OLED_Print_Num(30,0,AD_Voltage_Max[0]);
            OLED_Print_Num(80,0,AD_Voltage_Min[0]);
            OLED_P8x16Str(0,2,"R");
            OLED_Print_Num(30,2,AD_Voltage_Max[1]);
            OLED_Print_Num(80,2,AD_Voltage_Min[1]);
            OLED_P8x16Str(0,4,"EL");
            OLED_Print_Num(30,4,AD_Voltage_Max[2]);
            OLED_Print_Num(80,4,AD_Voltage_Min[2]);
            OLED_P8x16Str(0,6,"ER");
            OLED_Print_Num(30,6,AD_Voltage_Max[3]);
            OLED_Print_Num(80,6,AD_Voltage_Min[3]);
      }
      if(COLLECT_SIGN==1)                                 //察除扇区
      {
            FLASH_EraseSector(diangan_max);
            FLASH_EraseSector(diangan_min);
            for(int i=0;i<4;i++)
            {
                  FLASH_WriteSector(diangan_max,(const uint8 *)&AD_Voltage_Max[i],4,4*i);                        //写入扇区
                  FLASH_WriteSector(diangan_min,(const uint8 *)&AD_Voltage_Min[i],4,4*i);
            }
            COLLECT_SIGN=0;
      }
            
      for(uint8 i=0;i<4;i++)     //读取数据
      {
            AD_Voltage_Max[i]=flash_read(diangan_max,4*i,uint32);
            AD_Voltage_Min[i]=flash_read(diangan_min,4*i,uint32); 
      }
      OLED_Fill(0x00);   
}



