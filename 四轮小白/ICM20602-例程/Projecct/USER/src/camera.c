#include "camera.h"




//----------------------------------------------------------------------
//  @param      变量定义				
//----------------------------------------------------------------------
int LeftLine[ROW+1]={0};           //左边的边线
int RightLine[ROW+1]={0};          //右边的边线
int MidLine[ROW+1]={0};            //赛道的中点
int LeftAddLine[ROW+1]={0};        //左边赛道的补线边界
int RightAddLine[ROW+1]={0};       //右边赛道的补线边界
int LeftAddFlag[ROW+1]={0};        //左边的补线标志位
int RightAddFlag[ROW+1]={0};       //右边的补线标志位
int WidthReal[ROW+1]={0};          //实际赛套的宽度
int WidthAdd[ROW+1]={0};           //补线后的赛道宽度
int LeftStartAddLine=0;         //左边补线开始行
int RightStartAddLine=0;        //右边补线开始行
int LeftStopAddLine=0;          //左边补线结束行
int RightStopAddLine=0;         //右边补线结束行
int LineCound=0;                //确定赛道的行数
int OutSide=0;                  //出线或者摄像头出现错误
int WidthMin=0;                 //赛道宽度的最小值
int FindLeftLineFlag=0;         //找到左边的边界标志位
int FindRightLineFlag=0;        //找到右边的边界标志位
int FindLeftLine=0;             //记录找到左边的边界的行
int FindRightLine=0;            //记录右边一开始找到的行数
uint16 LeftLineErr=0;           //左边赛道的总偏差
uint16 RightLineErr=0;          //右边赛道的总偏差
float LeftKa;                   //左边边线的斜率
float LeftKb;                   //左边边线的截距
float RightKa;                  //右边边线的斜率
float RightKb;                  //右边边线的截距

int LeftErr;
int RightErr;


int Width[ROW+1]={0,0,0,0,0,0,0,0,0,37,
                  40,41,44,45,48,50,53,54,57,58,
                  61,63,65,67,70,72,74,76,78,80,
                  82,84,86,88,90,92,94,96,98,98,98};





//----------------------------------------------------------------------
//  @brief      大津法计算阈值
//  @param      data：像素数组起始地址   width: 图像宽度   height: 图像高度
//  @return     threshold：阈值					
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 OtsuThreshold(uint8 *data, uint16 width, uint16 height)
{
      int pixelCount[GrayScale];  //灰度为GrayScale的像素在整幅图像中的个数
      uint16 i, j, pixelSum = width * height;
      static int threshold = 0;        //阈值
      uint32 gSum=0;//灰度总值
      uint32 gSum0=0;//第一类灰度总值
      uint32 gSum1=0;//第二类灰度总值
      uint16 N0 = 0;//前景像素数
      uint16 N1 = 0;//背景像素数
      double u0 = 0;//前景像素平均灰度
      double u1 = 0;//背景像素平均灰度
      float w0 = 0;//前景像素点数占整幅图像的比例
      float w1 = 0;//背景像素点数占整幅图像的比例
      // double u = 0; //总平均灰度
      double deltaMax = -1;//临时类间方差
      double deltaTmp = -1;//类间方差
      uint16 MinValue = 0 , MaxValue = 0;
      uint8 addFlag= 0 ;  //方差处于增加状态的标志
      for (i = 0; i < GrayScale; i++)
      {
            pixelCount[i] = 0;
      } 
      //统计灰度级中每个像素在整幅图像中的个数  
      for (i = 0; i < height; i++)
      {
            for (j = 0; j < width; j++)
            {
                  pixelCount[data[i * width + j]]++; //将像素值作为计数数组的下标
            }
      }
      for (MinValue = 0; MinValue < GrayScale && pixelCount[MinValue]==0; MinValue++) ;        //获取最小灰度的值
      for (MaxValue = GrayScale-1; MaxValue > MinValue && pixelCount[MaxValue]==0; MaxValue--) ; //获取最大灰度的值
      
      for (i = MinValue; i <= MaxValue; i++)
      {          
            gSum += i*pixelCount[i];          
      }
      //遍历灰度级[0,255]  
      //   pit_time_start(pit1);
      for (i = MinValue; i <= MaxValue; i++)    // i作为阈值
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
            deltaTmp = w0*w1*pow((u0-u1),2);  //方差越大，意味着错分概率越小
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
//  @brief      图像处理
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void CameraHandle()
{
      uint8 i,j;                //控制行和列
      uint8 Res;                //第一行处理的情况
      
      LeftStartAddLine=0;
      RightStartAddLine=0;
      LeftStopAddLine=0;
      RightStopAddLine=0;
      LineCound=0;
      FindLeftLineFlag=0;         //找到左边的边界标志位
      FindRightLineFlag=0;        //找到右边的边界标志位
      FindLeftLine=0;             //记录找到左边的边界的行
      FindRightLine=0;            //记录右边一开始找到的行数
      
      LeftLineErr=0;
      RightLineErr=0;
      
      LeftErr=0;
      RightErr=0;
      
      LeftKa=0;                 //左边边线的斜率
      LeftKb=0;                 //左边边线的截距
      RightKa=0;                //右边边线的斜率
      RightKb=0;                //右边边线的截距
      
      
      //----------------------------第一行特殊处理-----------------------
      Res=FirstLineHeadle();
      if(Res==0)
      {
            OutSide=1;          //丢线
            return;
      }
      OutSide=0;
      LineCound=ROW-1;
      //-------------------------第一行特殊处理结束-------------------
      
      
      //------------------------对全幅图像进行处理--------------------
      for(i=ROW-1;i>=10;)
      {
            i--;
            if(i>=ROW-5)
            {
                  TraversalMidLine(i,MidLine[i+1],1,COL-1);
                  if(LeftAddFlag[i]==1)
                  {
                        LeftLine[i]=LeftLine[i+1];            //强制认为补线要从35行开始
                        LeftAddLine[i]=LeftAddLine[i+1];
                  }
                  if(RightAddFlag[i]==1)
                  {
                        RightLine[i]=RightLine[i+1];
                        RightAddLine[i]=RightAddLine[i+1];
                        
                  }
                  
                 // MidLine[i]=(LeftLine[i]+RightLine[i])/2;      //计算中线
                  WidthReal[i] = RightLine[i] - LeftLine[i];          //计算赛道宽度
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
                        TraversalMidToLeftLine(i,MidLine[i+1],1);               //如果上一行要补线，从上一行的中间向两边找线
                  }
                  else 
                  {
                        LeftAddFlag[i]=1;         //认为这一行要补线
                        
                        if(image[i][LeftLine[i+1]]>=Threshold)                  //边沿
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
                        RightAddFlag[i]=1;      //认为这一行要补线
                        
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
//                        MidLine[i]=(LeftLine[i]+RightLine[i])/2;      //计算中线
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
                   LeftLineErr+=LeftLine[i]-LeftLine[i+1];      //计算边线的累积误差
            }
            
            if(RightAddFlag[i]!=1&&RightAddFlag[i+1]!=1)
            {
                   RightLineErr+=RightLine[i+1]-RightLine[i];
            }
            
            MidLine[i]=(LeftLine[i]+RightLine[i])/2;      //计算中线
            
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
      
      FirstAddingLine();        //补线
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
//  @brief      图像第一行特殊处理
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
uint8 FirstLineHeadle()
{
      uint8 i=ROW-1;                            //第一行
      int WeightLeft,WeightRight;               //左右赛道的宽度
      
      
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
                  TraversalRightLine(i,1,COL-1);                //从右边开始扫，确定赛道的左右边界
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
      
      LeftLine[ROW]=LeftLine[ROW-1];            //更新数据
      RightLine[ROW]=RightLine[ROW-1];
      LeftAddLine[ROW]=LeftAddLine[ROW-1];
      RightAddLine[ROW]=RightAddLine[ROW-1];
      
      if(LeftAddFlag[ROW-1]&&RightAddFlag[ROW-1])               //两边都没有找到，需要补线
      {
            MidLine[ROW-1]=MidLine[ROW];
      }
      else 
      {
            MidLine[ROW-1]=(LeftLine[ROW-1]+RightLine[ROW-1])/2;
            MidLine[ROW]=MidLine[ROW-1];
      }
      
      WidthReal[ROW]=WidthReal[ROW-1];          //赛道宽度更新
      WidthAdd[ROW]=WidthAdd[ROW-1];
      WidthMin=WidthReal[ROW-1];
      
      return 1;
}



//----------------------------------------------------------------------
//  @brief      图像第一行的参数处理，只改变第一行
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
//  @brief      从左开始往右扫，只返回赛道宽度的大小，不保留左右边界
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
                  if(image[i][j]>Threshold)     //搜到第一个白点
                  {
                        LeftLine=j;             //把第一个白点作为左边界                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //搜到左边界后第一个黑点
                  {             
                        RightLine=j-1;          //把这一点的上一点作为右边界
                        
                        break;
                  }
            }
      }
      if(!WhiteFlag)                            //没有找到左边界
      {
            return 0;
      }
      else 
      {
            return RightLine-LeftLine;
      }
      
}

//----------------------------------------------------------------------
//  @brief      从右向左扫，只返回赛道宽度的大小，不保留左右边界
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
                  if(image[i][j]>Threshold)     //搜到第一个白点
                  {
                        RightLine=j;             //把第一个白点作为右边界                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //搜到右边界后第一个黑点
                  {             
                        LeftLine=j-1;          //把这一点的上一点作为左边界
                        
                        break;
                  }
            }
      }
      if(!WhiteFlag)            //没有找到右边界
      {
            return 0;
      }
      else 
      {
            return RightLine-LeftLine;
      }
}

//----------------------------------------------------------------------
//  @brief      从左开始往右扫，保留左右边界
//  @param      
//  @return     返回0说明没找到边界。返回1说明找到了左边界				
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
                  if(image[i][j]>Threshold)     //搜到第一个白点
                  {
                        LeftLine[i]=j;             //把第一个白点作为左边界                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //搜到左边界后第一个黑点
                  {             
                        RightLine[i]=j-1;          //把这一点的上一点作为右边界
                        
                        break;
                  }
            }
      }
      if(WhiteFlag)                            //找到左边界
      {
            LeftAddLine[i] = LeftLine[i];
            RightAddLine[i] = RightLine[i];
            WidthReal[i] = RightLine[i] - LeftLine[i];          //计算赛道的宽度
            WidthAdd[i] = WidthReal[i];
      }
      
      
      return WhiteFlag;
      
      
}



//----------------------------------------------------------------------
//  @brief      从右向左扫
//  @param      
//  @return     返回0说明没找到边界。返回1说明找到了左边界				
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
                  if(image[i][j]>Threshold)     //搜到第一个白点
                  {
                        RightLine[i]=j;             //把第一个白点作为右边界                
                        WhiteFlag=1;
                        
                        continue;
                  }
            }
            else 
            {
                  if(image[i][j]<Threshold)     //搜到右边界后第一个黑点
                  {             
                        LeftLine[i]=j-1;          //把这一点的上一点作为左边界
                        
                        break;
                  }
            }
      }
      if(WhiteFlag)            //找到右边界
      {
            LeftAddLine[i] = LeftLine[i];
            RightAddLine[i] = RightLine[i];
            WidthReal[i] = RightLine[i] - LeftLine[i];          //计算赛道宽度
            WidthAdd[i] = WidthReal[i];
      }
      
      return WhiteFlag;
      
}




//----------------------------------------------------------------------
//  @brief      从上一行的中点向两边扫，得到左右边界
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void TraversalMidLine(uint8 i,uint8 Mid,uint8 LeftMin,uint8 RightMax)
{
      uint8 j;
      LeftAddFlag[i]=1;         //认为这一行要补线
      RightAddFlag[i]=1;
      
      LeftLine[i]=LeftMin;      //定义初始值
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
      WidthReal[i] = RightLine[i] - LeftLine[i];			// 计算实际赛道宽度
      WidthAdd[i] = RightAddLine[i] - LeftAddLine[i];	// 计算补线赛道宽度
}

//----------------------------------------------------------------------
//  @brief      从上一行的中点向左边扫，得到左边界
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void TraversalMidToLeftLine(uint8 i,uint8 Mid,uint8 LeftMin)
{
      uint8 j;
      
      LeftAddFlag[i]=1;         //认为这一行要补线
      
      LeftLine[i]=LeftMin;      //定义初始值
      
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
//  @brief      从上一行的中点向左边扫，得到左边界
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void TraversalMidToRightLine(uint8 i,uint8 Mid,uint8 RightMax)
{
      uint8 j;
      RightAddFlag[i]=1;         //认为这一行要补线
      
      RightLine[i]=RightMax;      //定义初始值
      
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
//  @brief      第一阶段补线
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void FirstAddingLine()
{
        int i;
        
//        if(FindLeftLineFlag==0) //说明整场图像都缺左边线
//        {
//                for(i=ROW-1;i>=10;i--)
//                {
//#if 1
//                        MidLine[i]=MidLine[i+1]+RightLine[i]-RightLine[i+1];    //重新更新中线  
//#endif
//#if 0
//                        MidLine[i]=RightLine[i]-Width[i]/2;
//#endif
//                        
//                        //image[i][MidLine[i]]=0;
//                }
//        }
//        if(FindRightLineFlag==0)                //说明整场图像都丢缺右边线
//        {
//                for(i=ROW-1;i>=10;i--)
//                {
//#if 1
//                        MidLine[i]=MidLine[i+1]+LeftLine[i]-LeftLine[i+1];      //重新更新中线 
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
        if(FindLeftLineFlag==0&&FindRightLineFlag) //说明整场图像都缺左边线
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
                if(LeftStartAddLine==9&&RightStartAddLine==9)   //说明两边都可以看到最远
                {
                        for(i=ROW-1;i>=10;i--)    
                        {
                                MidLine[i]=(LeftLine[i]+RightLine[i])/2;
                        }
                }
                else if(LeftStartAddLine==9&&RightStartAddLine>9)               //说明左边线可以看到最远
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
                else if(LeftStartAddLine>9&&RightStartAddLine==9)               //说明右边线可以看到最远
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
//  @brief      判断赛道类型（弯前弯后，十字）
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void StraightLenth()
{
        
}


//----------------------------------------------------------------------
//  @brief     求左边斜率
//  @param      
//  @return     				
//  @since      
//  Sample usage: y=Ka*x+Kb;//最正规的求斜率的方法
//------------------------------------------------------------------------
void TwoPointGetLeftGradient(float *Ka,float *Kb,int StartPoint,int StopPoint)
{
        *Ka=1.0*(LeftLine[StartPoint]-LeftLine[StopPoint])/(StartPoint-StopPoint);
        *Kb=1.0*LeftLine[StopPoint]-(*Ka*StopPoint);
}


//----------------------------------------------------------------------
//  @brief     求右边斜率
//  @param      
//  @return     				
//  @since      
//  Sample usage: y=Ka*x+Kb;//最正规的求斜率的方法
//------------------------------------------------------------------------
void TwoPointGetRightGradient(float *Ka,float *Kb,int StartPoint,int StopPoint)
{
        *Ka=1.0*(RightLine[StartPoint]-RightLine[StopPoint])/(StartPoint-StopPoint);
        *Kb=1.0*RightLine[StopPoint]-(*Ka*StopPoint);
}


//----------------------------------------------------------------------
//  @brief      判断怎么补线
//  @param      
//  @return     				
//  @since      
//  Sample usage: 
//------------------------------------------------------------------------
void JudgeAddLine()
{
        //if(FindLeftLineFlag==0)                 //没有找到有左边线
        {
               TwoPointGetRightGradient(&RightKa,&RightKb,FindRightLine,RightStartAddLine);     //计算斜率还有截距
               RightLineRepair(RightKa,RightKb,FindRightLine,RightStartAddLine);              //拟合直线                
        }
        
        //if(FindRightLineFlag==0)                //没有找到右右边线
        {
                TwoPointGetLeftGradient(&LeftKa,&LeftKb,FindLeftLine,LeftStartAddLine);         //计算斜率还有截距
                LeftLineRepair(LeftKa,LeftKb,FindLeftLine,LeftStartAddLine);                  //拟合直线   
        }
}

//----------------------------------------------------------------------
//  @brief      知左边线的斜率补左边线
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
//  @brief      知you边线的斜率补you边线
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

