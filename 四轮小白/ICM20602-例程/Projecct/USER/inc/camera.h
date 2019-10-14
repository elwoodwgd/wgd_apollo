#ifndef _camera_h
#define _camera_h

#include "headfile.h"

//----------------------------------------------------------------------
//  @param      ȫ�ֱ���				
//----------------------------------------------------------------------
extern uint8 Threshold;
extern int MidLine[ROW+1];




//----------------------------------------------------------------------
//  @param      �궨��				
//----------------------------------------------------------------------
#define GrayScale 256    //�Ҷȼ� 






//----------------------------------------------------------------------
//  @param      ȫ�ֺ���				
//------------------------------------------------------------------------
uint8 OtsuThreshold(uint8 *data, uint16 width, uint16 height);

void CameraHandle();
uint8 FirstLineHeadle();
void FirstLineInit();

uint8 TraversalLeft(uint8 i,uint8 LeftMin,uint8 RightMax);      //����ʼɨ�裬�����������
uint8 TraversalRight(uint8 i,uint8 LeftMin,uint8 RightMax);     //���ҿ�ʼɨ�裬�����������

uint8 TraversalLeftLine(uint8 i,uint8 LeftMin,uint8 RightMax);  //����߿�ʼɨ�裬����1˵���ҵ�����߽磬����0˵��û���ҵ���߽�
uint8 TraversalRightLine(uint8 i,uint8 LeftMin,uint8 RightMax); //����߿�ʼɨ�裬����1˵���ҵ�����߽磬����0˵��û���ҵ��ұ߽�

void TraversalMidLine(uint8 i,uint8 Mid,uint8 LeftMin,uint8 RightMax);          //���м�������ɨ�������ұ߽�

void TraversalMidToLeftLine(uint8 i,uint8 Mid,uint8 LeftMin);                   //���м������ɨ������߽�
void TraversalMidToRightLine(uint8 i,uint8 Mid,uint8 RightMax);                 //���м����ұ�ɨ�����ұ߽�

void FirstAddingLine();         //��һ�β���

void DrowingLine();             //huaxian 

void TwoPointGetLeftGradient(float *Ka,float *Kb,int StartPoint,int StopPoint);                 //�������б�����������
void TwoPointGetRightGradient(float *Ka,float *Kb,int StartPoint,int StopPoint);                //���ұ���б�����������

void LeftLineRepair(float Ka,float Kb,int Start,int Stop);                      //���б�����ֱ��
void RightLineRepair(float Ka,float Kb,int Start,int Stop);                     //�ұ�б�����ֱ��

void JudgeAddLine();


#endif