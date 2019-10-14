#ifndef _motor_h
#define _motor_h

#include "headfile.h"

extern int SpeedSet;
extern int LeftSpeed;
extern int LeftMotorOutput;
extern int RightSpeed;
extern int RightMotorOutput;
extern int LeftMotorSpeedSet;
extern int RightMotorSpeedSet;


void LeftMotorSpeedContural();
void RightMotorSpeedContural();
void StopCar();
void Differential(float Err);

#endif