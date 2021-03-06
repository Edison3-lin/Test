/*****************************************************************************
 ** File			: include.h
 ** Author			: Zheng23
 ** Date			: 20150608			
 ** Function		: 
/*****************************************************************************/
#ifndef _INCLUDE_H_
#define _INCLUDE_H_


typedef unsigned char	u8, U8, uint8, UINT8, BYTE;
typedef signed char		s8, S8, int8, INT8;

typedef unsigned short	u16, U16, uint16, UINT16, WORD;
typedef signed short	s16, S16, int16, INT16;

typedef unsigned long	u32, U32, uint32, UINT32, DWORD;
typedef signed long		s32, S32, int32, INT32;

//typedef unsigned short	string;

#define BIT(n) (1<<(n))

#define     BYTE0(n)            ((unsigned char)((unsigned short)(n)))
#define     BYTE1(n)            ((unsigned char)(((unsigned short)(n))>>8))
#define     BYTE2(n)            ((unsigned char)(((unsigned short)(((unsigned long)(n))>>8))>>8))
#define     BYTE3(n)            ((unsigned char)(((unsigned short)(((unsigned long)(n))>>16))>>8))

#define TRUE   1
#define FALSE  0
#define true   1
#define false  0
//#define NULL   0

//#include <Servo.h>
#include <Arduino.h>

#include "App.h"
#include "PWM.h"
//#include "PS2GamePad.h"
#include "PS2X_lib.h"
#include "PCMsg.h"
#include "Flash.h"
#include "RobotRun.h"
//#include "Bluetooth.h"
#include <Servo.h>

extern Servo myservo[6];  // create servo object to control a servo
extern uint16 ServoPwmDuty[];
extern bool ServoPwmDutyHaveChange;
extern uint8 Edison_DBG;
void ShowDec(uint32 num);
void ShowHEX(uint32 num);
void Dump_Index(void);

#endif
