#ifndef _PID_h
#define _PID_h

#include "system.h"

typedef struct PID
{
        long SumErr;            //累积误差
        float LastErr;            //上一次误差
        float PrevErr;            //上上次的误差
        float Err;
        float Kp;
        float Ki;
        float Kd;
}PID;

extern PID S3010DutyCamera;
extern PID S3010DutyInductive;
extern PID LeftMotorDuty;
extern PID RightMotorDuty;


void SpacePIDInit(PID *sptr);
int SpaceCameraPID(PID *sptr,int NewPoint,int SetPoint);
int SpaceAdPID(PID *sptr);
int SpaceMotorPID(PID *sptr,int NewPoint,int SetPoint);
int IncrementMotorPID(PID *sptr,int NewPoint,int SetPoint);

#endif