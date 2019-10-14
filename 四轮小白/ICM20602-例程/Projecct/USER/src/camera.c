#include "camera.h"




//----------------------------------------------------------------------
//  @param      ��������				
//----------------------------------------------------------------------
int LeftLine[ROW+1]={0};           //��ߵı���
int RightLine[ROW+1]={0};          //�ұߵı���
int MidLine[ROW+1]={0};            //�������е�
int LeftAddLine[ROW+1]={0};        //��������Ĳ��߽߱�
int RightAddLine[ROW+1]={0};       //�ұ������Ĳ��߽߱�
int LeftAddFlag[ROW+1]={0};        //��ߵĲ��߱�־λ
int RightAddFlag[ROW+1]={0};       //�ұߵĲ��߱�־λ
int WidthReal[ROW+1]={0};          //ʵ�����׵Ŀ��
int WidthAdd[ROW+1]={0};           //���ߺ���������
int LeftStartAddLine=0;         //��߲��߿�ʼ��
int RightStartAddLine=0;        //�ұ߲��߿�ʼ��
int LeftStopAddLine=0;          //��߲��߽�����
int RightStopAddLine=0;         //�ұ߲��߽�����
int LineCound=0;                //ȷ������������
int OutSide=0;                  //���߻�������ͷ���ִ���
int WidthMin=0;                 //������ȵ���Сֵ
int FindLeftLineFlag=0;         //�ҵ���ߵı߽��־λ
int FindRightLineFlag=0;        //�ҵ��ұߵı߽��־λ
int FindLeftLine=0;             //��¼�ҵ���ߵı߽����
int FindRightLine=0;            //��¼�ұ�һ��ʼ�ҵ�������
uint16 LeftLineErr=0;           //�����������ƫ��
uint16 RightLineErr=0;          //�ұ���������ƫ��
float LeftKa;                   //��߱��ߵ�б��
float LeftKb;                   //��߱��ߵĽؾ�
float RightKa;                  //�ұ߱��ߵ�б��
float RightKb;                  //�ұ߱��ߵĽؾ�

int LeftErr;
int RightErr;


int Width[ROW+1]={0,0,0,0,0,0,0,0,0,37,
                  40,41,44,45,48,50,53,54,57,58,
                  61,63,65,67,70,72,74,76,78,80,
                  82,84,86,88,90,92,94,96,98,98,98};





//----------------------------------------------------------------------
//  @brief      ��򷨼�����ֵ
//  @param      data������������ʼ��ַ   width: ͼ����   height: ͼ��߶�
//  @return     threshold����ֵ					
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 OtsuThreshold(uint8 *data, uint16 width, uint16 height)
{
      int pixelCount[GrayScale];  //�Ҷ�ΪGrayScale������������ͼ���еĸ���
      uint16 i, j, pixelSum = width * height;
      static int threshold = 0;        //��ֵ
      uint32 gSum=0;//�Ҷ���ֵ
      uint32 gSum0=0;//��һ��Ҷ���ֵ
      uint32 gSum1=0;//�ڶ���Ҷ���ֵ
      uint16 N0 = 0;//ǰ��������
      uint16 N1 = 0;//����������
      double u0 = 0;//ǰ������ƽ���Ҷ�
      double u1 = 0;//��������ƽ���Ҷ�
      float w0 = 0;//ǰ�����ص���ռ����ͼ��ı���
      float w1 = 0;//�������ص���ռ����ͼ��ı���
      // double u = 0; //��ƽ���Ҷ�
      double deltaMax = -1;//��ʱ��䷽��
      double deltaTmp = -1;//��䷽��
      uint16 MinValue = 0 , MaxValue = 0;
      uint8 addFlag= 0 ;  //���������״̬�ı�־
      for (i = 0; i < GrayScale; i++)
      {
            pixelCount[i] = 0;
      } 
      //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
      for (i = 0; i < height; i++)
      {
            for (j = 0; j < width; j++)
            {
                  pixelCount[data[i * width + j]]++; //������ֵ��Ϊ����������±�
            }
      }
      for (MinValue = 0; MinValue < GrayScale && pixelCount[MinValue]==0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
      for (MaxValue = GrayScale-1; MaxValue > MinValue && pixelCount[MaxValue]==0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
      
      for (i = MinValue; i <= MaxValue; i++)
      {          
            gSum += i*pixelCount[i];          
      }
      //�����Ҷȼ�[0,255]  
      //   pit_time_start(pit1);
      for (i = MinValue; i <= MaxValue; i++)    // i��Ϊ��ֵ
      {    
            N0 += pixelCount[i];			
            N1 = pixelSum - N0;
            w0 = (float)N0 / pixelSum;
            w1 = 1-w0;    
            gSum0 += i*pixelCount[i];
            u0 = (double)gSum0/N0;
            gSum1 = gSum - gSum0;
            u1 = (double)gSum1/N1;     
            // u = w0 * u0 + w1 * u1;
            // deltaTmp=w0*pow((u0-u),2)+w1*pow((u1-u),2);   
            deltaTmp = w0*w1*pow((u0-u1),2);  //����Խ����ζ�Ŵ�ָ���ԽС
            // printf("w0=%f,w1=%f,u0=%f,u1=%f,deltaTmp=%f\n",w0,w1,u0,u1,deltaTmp);
            //systick_delay_ms(1000);
            // printf("%f\n",deltaTmp);   
            if (deltaTmp > deltaMax)
            {
                  deltaMax = deltaTmp;
                  threshold = i;
                  addFlag = 1;      
            }
            if (addFlag && deltaTmp < deltaMax)
                  break;   
      }
      return threshold;
}






//----------------------------------------------------------------------
//  @brief      ͼ����
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void CameraHandle()
{
      uint8 i,j;                //�����к���
      uint8 Res;                //��һ�д�������
      
      LeftStartAddLine=0;
      RightStartAddLine=0;
      LeftStopAddLine=0;
      RightStopAddLine=0;
      LineCound=0;
      FindLeftLineFlag=0;         //�ҵ���ߵı߽��־λ
      FindRightLineFlag=0;        //�ҵ��ұߵı߽��־λ
      FindLeftLine=0;             //��¼�ҵ���ߵı߽����
      FindRightLine=0;            //��¼�ұ�һ��ʼ�ҵ�������
      
      LeftLineErr=0;
      RightLineErr=0;
      
      LeftErr=0;
      RightErr=0;
      
      LeftKa=0;                 //��߱��ߵ�б��
      LeftKb=0;                 //��߱��ߵĽؾ�
      RightKa=0;                //�ұ߱��ߵ�б��
      RightKb=0;                //�ұ߱��ߵĽؾ�
      
      
      //----------------------------��һ�����⴦��-----------------------
      Res=FirstLineHeadle();
      if(Res==0)
      {
            OutSide=1;          //����
            return;
      }
      OutSide=0;
      LineCound=ROW-1;
      //-------------------------��һ�����⴦�����-------------------
      
      
      //------------------------��ȫ��ͼ����д���--------------------
      for(i=ROW-1;i>=10;)
      {
            i--;
            if(i>=ROW-5)
            {
                  TraversalMidLine(i,MidLine[i+1],1,COL-1);
                  if(LeftAddFlag[i]==1)
                  {
                        LeftLine[i]=LeftLine[i+1];            //ǿ����Ϊ����Ҫ��35�п�ʼ
                        LeftAddLine[i]=LeftAddLine[i+1];
                  }
                  if(RightAddFlag[i]==1)
                  {
                        RightLine[i]=RightLine[i+1];
                        RightAddLine[i]=RightAddLine[i+1];
                        
                  }
                  
                 // MidLine[i]=(LeftLine[i]+RightLine[i])/2;      //��������
                  WidthReal[i] = RightLine[i] - LeftLine[i];          //�����������
                  WidthAdd[i] = WidthReal[i];
                  
                  if(FindLeftLineFlag==0&&LeftAddFlag[i]==0&&LeftAddFlag[i+1]==0)
                  {
                        FindLeftLineFlag=1;
                        FindLeftLine=i+1;
                  }
                  
                  if(FindRightLineFlag==0&&RightAddFlag[i]==0&&RightAddFlag[i+1]==0)
                  {
                        FindRightLineFlag=1;
                        FindRightLine=i+1;
                  }
            }
            else 
            {   
                  
                  if(LeftAddFlag[i+1]==1)
                  {
                        TraversalMidToLeftLine(i,MidLine[i+1],1);               //�����һ��Ҫ���ߣ�����һ�е��м�����������
                  }
                  else 
                  {
                        LeftAddFlag[i]=1;         //��Ϊ��һ��Ҫ����
                        
                        if(image[i][LeftLine[i+1]]>=Threshold)                  //����
                        {
                              j=LeftLine[i+1];
                              while(j--)
                              {
                                    if(j<=LeftLine[i+1]-5||j<2)
                                    {
                                          break;
                                    }
                                    
                                    if(image[i][j]<Threshold&&image[i][j-1]<Threshold)
                                    {
                                          LeftLine[i]=j+1;
                                          LeftAddFlag[i]=0;
                                          LeftAddLine[i] =j+1;
                                          break;
                                    }
                              }
                        }
                        else 
                        {
                              j=LeftLine[i+1];
                              while(j++)
                              {
                                    if(image[i][j]>=Threshold&&image[i][j+1]>=Threshold)
                                    {
                                          LeftLine[i]=j;
                                          LeftAddFlag[i]=0;
                                          LeftAddLine[i] =j;
                                          break;
                                    }
                                    if(j>=LeftLine[i+1]+5)
                                    {
                                          break;
                                    }
                              }
                        }
                  }
                  if(RightAddFlag[i+1]==1)
                  {
                        TraversalMidToRightLine(i,MidLine[i+1],COL-1);
                  }
                  else 
                  {
                        RightAddFlag[i]=1;      //��Ϊ��һ��Ҫ����
                        
                        if(image[i][RightLine[i+1]]>=Threshold)
                        {
                              j=RightLine[i+1];
                              while(j++)
                              {
                                    if(j>=RightLine[i+1]+5||j>COL-2)
                                    {
                                          break;
                                    }
                                    
                                    if(image[i][j]<Threshold&&image[i][j+1]<Threshold)
                                    {
                                          RightLine[i]=j-1;
                                          RightAddFlag[i]=0;
                                          RightAddLine[i] =j-1;
                                          break;
                                    }
                              }
                        }
                        else 
                        {
                              j=RightLine[i+1];
                              while(j--)
                              {
                                    if(image[i][j-1]>=Threshold&&image[i][j-2]>=Threshold)
                                    {
                                          RightLine[i]=j-1;
                                          RightAddFlag[i]=0;
                                          RightAddLine[i] =j-1;
                                          break;
                                    }
                                    if(j<=RightLine[i+1]-5)
                                    {
                                          break;
                                    }
                              }
                        }
                  }
                  
                  if(FindLeftLineFlag==0&&LeftAddFlag[i]==0&&LeftAddFlag[i+1]==0)
                  {
                        FindLeftLineFlag=1;
                        FindLeftLine=i+1;
                  }
                  
                  if(FindRightLineFlag==0&&RightAddFlag[i]==0&&RightAddFlag[i+1]==0)
                  {
                        FindRightLineFlag=1;
                        FindRightLine=i+1;
                  }
                  
                  if(FindLeftLineFlag&&LeftAddFlag[i]==1&&LeftStartAddLine==0&&LeftAddFlag[i+1]==1)
                  {
                        LeftStartAddLine=i+1;
                  }
                  
                  if(FindRightLineFlag&&RightAddFlag[i]==1&&RightStartAddLine==0&&RightAddFlag[i+1]==1)
                  {
                        RightStartAddLine=i+1;
                  }
                  
                  if(RightStartAddLine>0&&RightAddFlag[i]==0&&RightAddFlag[i+1]==0)
                  {
                        RightStopAddLine=i+1;
                  }
                  
                  if(LeftAddFlag[i]==0&&LeftStartAddLine>0&&LeftAddFlag[i+1]==0)
                  {
                        LeftStopAddLine=i+1;
                  }
//                  if(LeftAddFlag[i+1]==1&&RightAddFlag[i+1]==1)
//                  {
//                        MidLine[i]=(LeftLine[i]+RightLine[i])/2;      //��������
//                  }
//                  else if(LeftAddFlag[i+1]==1&&RightAddFlag[i+1]==0)
//                  {
//                        MidLine[i]=MidLine[i+1]+RightLine[i]-RightLine[i+1];
//                  }
//                  else if(LeftAddFlag[i+1]==0&&RightAddFlag[i+1]==1)
//                  {
//                        MidLine[i]=MidLine[i+1]+LeftLine[i]-LeftLine[i+1];
//                  }
//                  else 
//                  {
//                        
//                  }
            }
            
            if(LeftAddFlag[i]!=1&&LeftAddFlag[i+1]!=1)
            {
                   LeftLineErr+=LeftLine[i]-LeftLine[i+1];      //������ߵ��ۻ����
            }
            
            if(RightAddFlag[i]!=1&&RightAddFlag[i+1]!=1)
            {
                   RightLineErr+=RightLine[i+1]-RightLine[i];
            }
            
            MidLine[i]=(LeftLine[i]+RightLine[i])/2;      //��������
            
            WidthReal[i] = RightLine[i] - LeftLine[i];
            
            image[i][LeftLine[i]+2]=0;
            image[i][RightLine[i]-2]=0;
            //
            image[i][COL/2]=0;
      }
      
      if(FindLeftLineFlag==1)
      {
          if(LeftStartAddLine==0)
          {
              LeftStartAddLine=9;
          }
          if(LeftStartAddLine>0)
          {
              if(LeftStopAddLine==0)
              {
                  LeftStopAddLine=9;
              }
          }
      }
      
      if(FindRightLineFlag==1)
      {
          if(RightStartAddLine==0)
          {
              RightStartAddLine=9;
          }
          if(RightStartAddLine>0)
          {
              if(RightStopAddLine==0)
              {
                  RightStopAddLine=9;
              }
          }
      }
      
      FirstAddingLine();        //����
      JudgeAddLine();
      
      
      OLED_Print_Num1(0,6,LeftLineErr);
      OLED_Print_Num1(40,6,RightLineErr);
      OLED_Print_Num1(80,6,MidLine[28]);
      OLED_Print_Num1(0,7,COL/2-MidLine[28]);
//      OLED_Print_Num1(40,7,LeftStopAddLine);
//      OLED_Print_Num1(80,7,RightStopAddLine);
      //Uart1_Send_AF(LeftLineErr,RightLineErr,abs(RightLineErr-LeftLineErr),FindLeftLineFlag,FindRightLineFlag,RightStopAddLine,FindLeftLine,COL/2-MidLine[28]);
      Uart1_Send_AF1(LeftErr,RightErr,abs(RightLineErr-LeftLineErr),FindLeftLineFlag,FindRightLineFlag,FindLeftLine,FindRightLine,COL/2-MidLine[28]);
      send_wave();
      
}




//----------------------------------------------------------------------
//  @brief      ͼ���һ�����⴦��
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 FirstLineHeadle()
{
      uint8 i=ROW-1;                            //��һ��
      int WeightLeft,WeightRight;               //���������Ŀ��
      
      
      FirstLineInit();
      if(image[i][MidLine[ROW]]<Threshold)
      {
            WeightLeft=TraversalLeft(i,1,COL-1);
            WeightRight=TraversalRight(i,1,COL-1);
            if(WeightLeft>=WeightRight&&WeightLeft)
            {
                  TraversalLeftLine(i,1,COL-1);
            }
            else if(WeightLeft<=WeightRight&&WeightRight)
            {
                  TraversalRightLine(i,1,COL-1);                //���ұ߿�ʼɨ��ȷ�����������ұ߽�
            }
            else 
            {
                  return 0;
            }
      }
      else 
      {
            TraversalMidLine(i,MidLine[ROW],1,COL-1);
      }
      
      LeftLine[ROW]=LeftLine[ROW-1];            //��������
      RightLine[ROW]=RightLine[ROW-1];
      LeftAddLine[ROW]=LeftAddLine[ROW-1];
      RightAddLine[ROW]=RightAddLine[ROW-1];
      
      if(LeftAddFlag[ROW-1]&&RightAddFlag[ROW-1])               //���߶�û���ҵ�����Ҫ����
      {
            MidLine[ROW-1]=MidLine[ROW];
      }
      else 
      {
            MidLine[ROW-1]=(LeftLine[ROW-1]+RightLine[ROW-1])/2;
            MidLine[ROW]=MidLine[ROW-1];
      }
      
      WidthReal[ROW]=WidthReal[ROW-1];          //������ȸ���
      WidthAdd[ROW]=WidthAdd[ROW-1];
      WidthMin=WidthReal[ROW-1];
      
      return 1;
}



//----------------------------------------------------------------------
//  @brief      ͼ���һ�еĲ�������ֻ�ı��һ��
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void FirstLineInit()
{
      LeftLine[ROW]=1;
      RightLine[ROW]=COL-1;
      MidLine[ROW]=COL/2;
      LeftAddLine[ROW]=1;
      RightAddLine[ROW]=COL-1;
      WidthReal[ROW]=COL-2;
      WidthAdd[ROW]=COL-2;
}


//----------------------------------------------------------------------
//  @brief      ����ʼ����ɨ��ֻ����������ȵĴ�С�����������ұ߽�
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 TraversalLeft(uint8 i,uint8 LeftMin,uint8 RightMax)
{
      uint8 j,WhiteFlag=0;
      uint8 LeftLine=LeftMin,RightLine=RightMax;
      
      for(j=LeftMin;j<=RightMax;j++)
      {
            if(!WhiteFlag)
            {
                  if(image[i][j]>Threshold)     //�ѵ���һ���׵�
                  {
                        LeftLine=j;             //�ѵ�һ���׵���Ϊ��߽�                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //�ѵ���߽���һ���ڵ�
                  {             
                        RightLine=j-1;          //����һ�����һ����Ϊ�ұ߽�
                        
                        break;
                  }
            }
      }
      if(!WhiteFlag)                            //û���ҵ���߽�
      {
            return 0;
      }
      else 
      {
            return RightLine-LeftLine;
      }
      
}

//----------------------------------------------------------------------
//  @brief      ��������ɨ��ֻ����������ȵĴ�С�����������ұ߽�
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 TraversalRight(uint8 i,uint8 LeftMin,uint8 RightMax)
{
      uint8 j,WhiteFlag=0;
      uint8 LeftLine=LeftMin,RightLine=RightMax;
      
      for(j=RightMax;j>=LeftMin;j--)
      {
            if(!WhiteFlag)
            {
                  if(image[i][j]>Threshold)     //�ѵ���һ���׵�
                  {
                        RightLine=j;             //�ѵ�һ���׵���Ϊ�ұ߽�                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //�ѵ��ұ߽���һ���ڵ�
                  {             
                        LeftLine=j-1;          //����һ�����һ����Ϊ��߽�
                        
                        break;
                  }
            }
      }
      if(!WhiteFlag)            //û���ҵ��ұ߽�
      {
            return 0;
      }
      else 
      {
            return RightLine-LeftLine;
      }
}

//----------------------------------------------------------------------
//  @brief      ����ʼ����ɨ���������ұ߽�
//  @param      
//  @return     ����0˵��û�ҵ��߽硣����1˵���ҵ�����߽�				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 TraversalLeftLine(uint8 i,uint8 LeftMin,uint8 RightMax)
{
      uint8 j,WhiteFlag=0;
      LeftLine[i]=1;
      RightLine[i]=79;
      
      for(j=LeftMin;j<=RightMax;j++)
      {
            if(!WhiteFlag)
            {
                  if(image[i][j]>Threshold)     //�ѵ���һ���׵�
                  {
                        LeftLine[i]=j;             //�ѵ�һ���׵���Ϊ��߽�                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //�ѵ���߽���һ���ڵ�
                  {             
                        RightLine[i]=j-1;          //����һ�����һ����Ϊ�ұ߽�
                        
                        break;
                  }
            }
      }
      if(WhiteFlag)                            //�ҵ���߽�
      {
            LeftAddLine[i] = LeftLine[i];
            RightAddLine[i] = RightLine[i];
            WidthReal[i] = RightLine[i] - LeftLine[i];          //���������Ŀ��
            WidthAdd[i] = WidthReal[i];
      }
      
      
      return WhiteFlag;
      
      
}



//----------------------------------------------------------------------
//  @brief      ��������ɨ
//  @param      
//  @return     ����0˵��û�ҵ��߽硣����1˵���ҵ�����߽�				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 TraversalRightLine(uint8 i,uint8 LeftMin,uint8 RightMax)
{
      uint8 j,WhiteFlag=0;
      LeftLine[i]=LeftMin;
      RightLine[i]=RightMax;
      
      for(j=RightMax;j>=LeftMin;j--)
      {
            if(!WhiteFlag)
            {
                  if(image[i][j]>Threshold)     //�ѵ���һ���׵�
                  {
                        RightLine[i]=j;             //�ѵ�һ���׵���Ϊ�ұ߽�                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //�ѵ��ұ߽���һ���ڵ�
                  {             
                        LeftLine[i]=j-1;          //����һ�����һ����Ϊ��߽�
                        
                        break;
                  }
            }
      }
      if(WhiteFlag)            //�ҵ��ұ߽�
      {
            LeftAddLine[i] = LeftLine[i];
            RightAddLine[i] = RightLine[i];
            WidthReal[i] = RightLine[i] - LeftLine[i];          //�����������
            WidthAdd[i] = WidthReal[i];
      }
      
      return WhiteFlag;
      
}




//----------------------------------------------------------------------
//  @brief      ����һ�е��е�������ɨ���õ����ұ߽�
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void TraversalMidLine(uint8 i,uint8 Mid,uint8 LeftMin,uint8 RightMax)
{
      uint8 j;
      LeftAddFlag[i]=1;         //��Ϊ��һ��Ҫ����
      RightAddFlag[i]=1;
      
      LeftLine[i]=LeftMin;      //�����ʼֵ
      RightLine[i]=RightMax;
      
      for(j=Mid;j>=LeftMin+1;j--)
      {
            if(image[i][j]<Threshold)
            {
                  LeftLine[i] =j;
                  LeftAddFlag[i] =0;
                  LeftAddLine[i] =j;
                  break;
            }
      }
      for(j=Mid;j<=RightMax-1;j++)
      {
            if(image[i][j]>=Threshold&&image[i][j+1]<Threshold)
            {
                  RightLine[i] =j;
                  RightAddFlag[i] =0;
                  RightAddLine[i] =j;
                  break;
            }
      }
      WidthReal[i] = RightLine[i] - LeftLine[i];			// ����ʵ���������
      WidthAdd[i] = RightAddLine[i] - LeftAddLine[i];	// ���㲹���������
}

//----------------------------------------------------------------------
//  @brief      ����һ�е��е������ɨ���õ���߽�
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void TraversalMidToLeftLine(uint8 i,uint8 Mid,uint8 LeftMin)
{
      uint8 j;
      
      LeftAddFlag[i]=1;         //��Ϊ��һ��Ҫ����
      
      LeftLine[i]=LeftMin;      //�����ʼֵ
      
      for(j=Mid;j>=LeftMin+1;j--)
      {
            if(image[i][j]>=Threshold&&image[i][j-1]<Threshold)
            {
                  LeftLine[i] =j;
                  LeftAddFlag[i] =0;
                  LeftAddLine[i] =j;
                  break;
            }
      }
}
//----------------------------------------------------------------------
//  @brief      ����һ�е��е������ɨ���õ���߽�
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void TraversalMidToRightLine(uint8 i,uint8 Mid,uint8 RightMax)
{
      uint8 j;
      RightAddFlag[i]=1;         //��Ϊ��һ��Ҫ����
      
      RightLine[i]=RightMax;      //�����ʼֵ
      
      for(j=Mid;j<=RightMax-1;j++)
      {
            if(image[i][j]>=Threshold&&image[i][j+1]<Threshold)
            {
                  RightLine[i] =j;
                  RightAddFlag[i] =0;
                  RightAddLine[i] =j;
                  break;
            }
      }
      
}

//----------------------------------------------------------------------
//  @brief      ��һ�׶β���
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void FirstAddingLine()
{
        int i;
        
//        if(FindLeftLineFlag==0) //˵������ͼ��ȱ�����
//        {
//                for(i=ROW-1;i>=10;i--)
//                {
//#if 1
//                        MidLine[i]=MidLine[i+1]+RightLine[i]-RightLine[i+1];    //���¸�������  
//#endif
//#if 0
//                        MidLine[i]=RightLine[i]-Width[i]/2;
//#endif
//                        
//                        //image[i][MidLine[i]]=0;
//                }
//        }
//        if(FindRightLineFlag==0)                //˵������ͼ�񶼶�ȱ�ұ���
//        {
//                for(i=ROW-1;i>=10;i--)
//                {
//#if 1
//                        MidLine[i]=MidLine[i+1]+LeftLine[i]-LeftLine[i+1];      //���¸������� 
//#endif
//#if 0
//                        MidLine[i]=LeftLine[i]+Width[i]/2;
//#endif
//                        //image[i][MidLine[i]]=0;
//                }
//        }
//        if(FindLeftLineFlag==1&&LeftStartAddLine>0)
//        {
//                for(i=ROW-1;i>=FindLeftLine;i--)
//                {
//                        MidLine[i]=(LeftLine[i]+RightLine[i])/2;
//                        //image[i][MidLine[i]]=0;
//                }

//        }
//        if(FindRightLineFlag==1&&RightStartAddLine>0)
//        {
//                for(i=ROW-1;i>=FindRightLine;i--)
//                {
//                        MidLine[i]=(LeftLine[i]+RightLine[i])/2;
//                        //image[i][MidLine[i]]=0;
//                }

//        }
        if(FindLeftLineFlag==0&&FindRightLineFlag) //˵������ͼ��ȱ�����
        {
                for(i=ROW-1;i>=10;i--)
                {
                        if(RightAddFlag[i]==0)
                        {
                                MidLine[i]=RightLine[i]-Width[i]/2;
                        }
                }
        }
        else if(FindLeftLineFlag&&FindRightLineFlag==0)
        {
                 for(i=ROW-1;i>=10;i--)
                 {
                        if(LeftAddFlag[i]==0)
                        {
                                MidLine[i]=LeftLine[i]+Width[i]/2;
                        }
                 }
        }
        else if(FindLeftLineFlag&&FindRightLineFlag)
        {
                if(LeftStartAddLine==9&&RightStartAddLine==9)   //˵�����߶����Կ�����Զ
                {
                        for(i=ROW-1;i>=10;i--)    
                        {
                                MidLine[i]=(LeftLine[i]+RightLine[i])/2;
                        }
                }
                else if(LeftStartAddLine==9&&RightStartAddLine>9)               //˵������߿��Կ�����Զ
                {
                        for(i=ROW-1;i>=RightStartAddLine;i--)
                        {
                                MidLine[i]=(LeftLine[i]+RightLine[i])/2;
                        }
                        for(i=RightStartAddLine;i>RightStopAddLine;i--)         
                        {
                                MidLine[i]=MidLine[i+1]+LeftLine[i]-LeftLine[i+1];
                        }
                }
                else if(LeftStartAddLine>9&&RightStartAddLine==9)               //˵���ұ��߿��Կ�����Զ
                {
                        for(i=ROW-1;i>=RightStartAddLine;i--)
                        {
                                MidLine[i]=(LeftLine[i]+RightLine[i])/2;
                        }
                        for(i=LeftStartAddLine;i>LeftStopAddLine;i--)
                        {
                                MidLine[i]=MidLine[i+1]+RightLine[i]-RightLine[i+1];
                           
                        }
                }
                else if(LeftStartAddLine>9&&RightStartAddLine>9)
                {
                        if(RightAddFlag[i]!=0&&LeftAddFlag[i]==0)
                        {
                                MidLine[i]=LeftLine[i]+Width[i]/2;
                        }
                        else if(RightAddFlag[i]==0&&LeftAddFlag[i]!=0)
                        {
                                MidLine[i]=RightLine[i]-Width[i]/2;
                        }
                }
        }
        
        for(i=ROW-1;i>=10;i--)
        {
//                if(RightAddFlag[i]!=0&&LeftAddFlag[i]==0)
//                {
//                        MidLine[i]=LeftLine[i]+Width[i]/2;
//                }
//                else if(RightAddFlag[i]==0&&LeftAddFlag[i]!=0)
//                {
//                        MidLine[i]=RightLine[i]-Width[i]/2;
//                }
//                else if(RightAddFlag[i]!=0&&LeftAddFlag[i]!=0)
//                {
//                        
//                }
                image[i][MidLine[i]]=0;
        }
}



//----------------------------------------------------------------------
//  @brief      �ж��������ͣ���ǰ���ʮ�֣�
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void StraightLenth()
{
        
}


//----------------------------------------------------------------------
//  @brief     �����б��
//  @param      
//  @return     				
//  @since      
//  Sample usage: y=Ka*x+Kb;//���������б�ʵķ���
//------------------------------------------------------------------------
void TwoPointGetLeftGradient(float *Ka,float *Kb,int StartPoint,int StopPoint)
{
        *Ka=1.0*(LeftLine[StartPoint]-LeftLine[StopPoint])/(StartPoint-StopPoint);
        *Kb=1.0*LeftLine[StopPoint]-(*Ka*StopPoint);
}


//----------------------------------------------------------------------
//  @brief     ���ұ�б��
//  @param      
//  @return     				
//  @since      
//  Sample usage: y=Ka*x+Kb;//���������б�ʵķ���
//------------------------------------------------------------------------
void TwoPointGetRightGradient(float *Ka,float *Kb,int StartPoint,int StopPoint)
{
        *Ka=1.0*(RightLine[StartPoint]-RightLine[StopPoint])/(StartPoint-StopPoint);
        *Kb=1.0*RightLine[StopPoint]-(*Ka*StopPoint);
}


//----------------------------------------------------------------------
//  @brief      �ж���ô����
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void JudgeAddLine()
{
        //if(FindLeftLineFlag==0)                 //û���ҵ��������
        {
               TwoPointGetRightGradient(&RightKa,&RightKb,FindRightLine,RightStartAddLine);     //����б�ʻ��нؾ�
               RightLineRepair(RightKa,RightKb,FindRightLine,RightStartAddLine);              //���ֱ��                
        }
        
        //if(FindRightLineFlag==0)                //û���ҵ����ұ���
        {
                TwoPointGetLeftGradient(&LeftKa,&LeftKb,FindLeftLine,LeftStartAddLine);         //����б�ʻ��нؾ�
                LeftLineRepair(LeftKa,LeftKb,FindLeftLine,LeftStartAddLine);                  //���ֱ��   
        }
}

//----------------------------------------------------------------------
//  @brief      ֪����ߵ�б�ʲ������
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void LeftLineRepair(float Ka,float Kb,int Start,int Stop)
{
        int i;
        int res;
        for (i = Stop; i <= Start; i++)
        {
            res = (int)(i * Ka + Kb);
            LeftAddLine[i] = limit_ab(res,1,COL-1);
            LeftErr+=LeftLine[i]-LeftAddLine[i];
            image[i][LeftAddLine[i]+2]=0;
        }
}



//----------------------------------------------------------------------
//  @brief      ֪you���ߵ�б�ʲ�you����
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void RightLineRepair(float Ka,float Kb,int Start,int Stop)
{
        int i;
        int res;
        for (i = Stop; i <= Start; i++)
        {
            res = (int)(i * Ka + Kb);
            RightAddLine[i] = limit_ab(res,1,COL-1);
            RightErr+=RightLine[i]-RightAddLine[i];
            image[i][RightAddLine[i]-2]=0;
        }
}

