#ifndef _camera_h
#define _camera_h

#include "headfile.h"

//----------------------------------------------------------------------
//  @param      全局变量				
//----------------------------------------------------------------------
extern uint8 Threshold;
extern int MidLine[ROW+1];




//----------------------------------------------------------------------
//  @param      宏定义				
//----------------------------------------------------------------------
#define GrayScale 256    //灰度级 






//----------------------------------------------------------------------
//  @param      全局函数				
//------------------------------------------------------------------------
uint8 OtsuThreshold(uint8 *data, uint16 width, uint16 height);

void CameraHandle();
uint8 FirstLineHeadle();
void FirstLineInit();

uint8 TraversalLeft(uint8 i,uint8 LeftMin,uint8 RightMax);      //从左开始扫描，返回赛道宽度
uint8 TraversalRight(uint8 i,uint8 LeftMin,uint8 RightMax);     //从右开始扫描，返回赛道宽度

uint8 TraversalLeftLine(uint8 i,uint8 LeftMin,uint8 RightMax);  //从左边开始扫描，返回1说明找到了左边界，返回0说明没有找到左边界
uint8 TraversalRightLine(uint8 i,uint8 LeftMin,uint8 RightMax); //从左边开始扫描，返回1说明找到了左边界，返回0说明没有找到右边界

void TraversalMidLine(uint8 i,uint8 Mid,uint8 LeftMin,uint8 RightMax);          //从中间向两边扫，求左右边界

void TraversalMidToLeftLine(uint8 i,uint8 Mid,uint8 LeftMin);                   //从中间向左边扫，求左边界
void TraversalMidToRightLine(uint8 i,uint8 Mid,uint8 RightMax);                 //从中间向右边扫，求右边界

void FirstAddingLine();         //第一次补线

void DrowingLine();             //huaxian 

void TwoPointGetLeftGradient(float *Ka,float *Kb,int StartPoint,int StopPoint);                 //求左边线斜率最正规的求法
void TwoPointGetRightGradient(float *Ka,float *Kb,int StartPoint,int StopPoint);                //求右边线斜率最正规的求法

void LeftLineRepair(float Ka,float Kb,int Start,int Stop);                      //左边斜率拟合直线
void RightLineRepair(float Ka,float Kb,int Start,int Stop);                     //右边斜率拟合直线

void JudgeAddLine();


#endif