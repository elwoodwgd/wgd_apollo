#ifndef _ad_h
#define _ad_h
#include "headfile.h"

#define LeftAd                  ADC0_SE9        //��ߵĵ��
#define RightAd                 ADC0_SE8        //�ұߵĵ��
#define ELeftAd                 ADC0_SE10       //��ߵ�б���
#define ERightAd                ADC1_SE4a       //�ұߵ�б���

#define diangan_max             FLASH_SECTOR_NUM-1
#define diangan_min             FLASH_SECTOR_NUM-2
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )


extern int AD_Voltage_guiyi[4];
extern int AD_Voltage[4];
extern int AD_Voltage_Min[4];


void AD_init();
void GET_AD_Max_and_Min();
void GET_AD_Voltage();
void AD_zero_to_one();


#endif