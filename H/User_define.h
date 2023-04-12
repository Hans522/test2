#ifndef _User_define_H_
#define _User_define_H_



#define		SetUart2Int			P04 = 0;
#define 	SetUart2Out		    P04 = 1;
#define		SetUart2TxHigh		P44 = 1;

#define		SetPM25POWER		P42 = 0;
#define		ClrPM25POWER		P42 = 1;

#define		SetUartIn			P22 = 0;
#define 	SetUartOut		    P22 = 1;

#define		SetLED1		P14 = 0;
#define		ClrLED1		P14 = 1;
#define		SetLED2		P15 = 0;
#define		ClrLED2		P15 = 1;
#define		SetLED3		P16 = 0;
#define		ClrLED3		P16 = 1;
#define		SetLED4		P17 = 0;
#define		ClrLED4		P17 = 1;

  #define  SetStepMotNewFanBypassA()   P30 = 1
  #define  ClrStepMotNewFanBypassA()   P30 = 0
  
  #define  SetStepMotNewFanBypassB()   P31 = 1
  #define  ClrStepMotNewFanBypassB()   P31 = 0
  
  #define  SetStepMotNewFanBypassC()   P32 = 1
  #define  ClrStepMotNewFanBypassC()   P32 = 0
  
  #define  SetStepMotNewFanBypassD()   P33 = 1
  #define  ClrStepMotNewFanBypassD()   P33 = 0
  
  #define  SetStepMotOutFanBypassA()   P34 = 1
  #define  ClrStepMotOutFanBypassA()   P34 = 0
  
  #define  SetStepMotOutFanBypassB()   P35 = 1
  #define  ClrStepMotOutFanBypassB()   P35 = 0
  
  #define  SetStepMotOutFanBypassC()   P36 = 1
  #define  ClrStepMotOutFanBypassC()   P36 = 0
  
  #define  SetStepMotOutFanBypassD()   P37 = 1
  #define  ClrStepMotOutFanBypassD()   P37 = 0

  #define  set_k1   P40 = 1
  #define  clr_k1   P40 = 0
  #define  set_k2   P41 = 1
  #define  clr_k2   P41 = 0

#define		UartBufLength		8


sbit POWER_PIN = P5^5;


typedef union 
{
		struct
			{
				unsigned TF_Timer10ms1		:1;
				unsigned TF_Timer10ms2		:1;
				unsigned TF_Timer10ms3		:1;
				unsigned TF_Timer20ms		:1;
				unsigned :1;
				unsigned :1;
				unsigned F_AdcFlag			:1;
				unsigned TF_Timer500ms		:1;
					
				
			}Bits;
		unsigned char ucAllBits;
		
}unTimeFlagTypeDef;

extern	unTimeFlagTypeDef ucTimeF;

typedef union
{
		struct
			{
				unsigned F_SetTemper			:1;
				unsigned F_WriteEe				:1;
				unsigned F_Auto_Hand			:1;
				unsigned F_Lock				:1;
				unsigned F_A1A2				:1;
				unsigned F_IPL			:1;
				unsigned F_VENTVOL			:1;
				unsigned F_NEWVOL			:1;
					
				unsigned F_PWMORVOL				:1;
				unsigned F_VENTSPEED		:1;
				unsigned F_NEWSPEED		:1;
				unsigned F_POWER		:1;
				unsigned F_AlarmPrimary		:1;
				unsigned F_AlarmHigh			:1;
				unsigned VENT_VALVE			:1;
				unsigned F_PWM_OR_VOLtemp			:1;
					
				unsigned NEW_VALVE				:1;
				unsigned K2_SWITCH			:1;
				unsigned K1_SWITCH				:1;
				unsigned F_PWM_OR_VOL				:1;
				unsigned F_EeRight			:1;
				unsigned F_Uart2Receive		:1;
				unsigned F_Uart2Send			:1;
				unsigned F_Uart0Receive   :1;

				unsigned F_Uart0send   :1;
				unsigned F_UartReceive :1;
				unsigned F_UartSend :1;
				unsigned F1_NEWFAN_OPENED:1;
				unsigned F1_NEWFAN_CLOSED:1;
				unsigned F1_OUTFAN_OPENED:1;
				unsigned F1_OUTFAN_CLOSED:1;
			}Bits;
		struct
			{
				unsigned long ulAllBits;
			}Bytes;
			
}unFlagTypeDef;

extern	unFlagTypeDef ulFlag;


extern unsigned char  ucKeyData;
extern unsigned char  ucTimeMode;
extern unsigned char  ucPowerOrReset;
extern unsigned char  ucTimeCount30s;
extern unsigned char  ucWorkMode;
extern unsigned char  ucFanSpeed;
extern unsigned char  ucRoomTemper;
extern unsigned char  ucDebugMode;
extern signed	char  scFillTemper;
extern unsigned char  ucTimeHour;
extern unsigned char  ucTimeMinute;
extern unsigned char  ucTimeSecond;
extern unsigned char  ucTimeWeek;
extern unsigned char  ucMotorOutCount;
extern unsigned int   uiSetTimePrimary;
extern unsigned int   uiSetTimeHigh;
extern unsigned int   uiRunTimePrimary;
extern unsigned int   uiRunTimeHigh;
extern unsigned char  ucFanDelay;
extern unsigned char  ucRunTimeMinute;
extern unsigned char  ucFanSpeedTemp;


extern unsigned char xdata ucUart2Buf[];
extern unsigned char xdata ucUart0Buf[];
extern unsigned char xdata ucUart2Receive;
extern unsigned char xdata ucUart2Send;
extern unsigned int xdata ucRs485Timeout;
extern unsigned char xdata ucIPH;
extern unsigned char xdata ucIPL;
extern unsigned char xdata ucUart0Receive;
extern unsigned char xdata BOMA_VALUE;
extern unsigned int  xdata PM25_VALUE;
extern unsigned char xdata ucUartSend;
extern unsigned char xdata ucUartReceive;
extern unsigned char xdata ucUARTTimeout;
extern unsigned char xdata ucUartBuf[];
extern float HTD20_TEM;
extern float HTD20_HUM;
extern unsigned char xdata VENT_PWM[];
extern unsigned char xdata VENT_VOLTAGE[];
extern unsigned char xdata NEW_PWM[];
extern unsigned char xdata NEW_VOLTAGE[];
extern unsigned char xdata NEW_SPEED;
extern unsigned char xdata VENT_SPEED;
extern unsigned char xdata ucPowerOrReset_temp;
extern unsigned char xdata ucWorkMode_temp;
extern unsigned char xdata NEW_SPEED_temp;
extern unsigned char xdata VENT_SPEED_temp;
extern unsigned char xdata ucIPL_temp;
extern unsigned char xdata VENT_PWM_temp[];
extern unsigned char xdata VENT_VOLTAGE_temp[];
extern unsigned char xdata NEW_PWM_temp[];
extern unsigned char xdata NEW_VOLTAGE_temp[];
extern unsigned int  xdata CO2_VALUE;
extern unsigned int  xdata TOVC_VALUE;
extern unsigned int xdata one_minuter_count;
//extern unsigned int xdata temp_value;
extern unsigned int xdata	SPEED1_CIRCUT;
extern unsigned int xdata  SPEED2_CIRCUT;

extern signed int xdata P50H_Cnt;
extern signed int xdata P51H_Cnt;
extern signed int xdata P50L_Cnt;
extern signed int xdata P51L_Cnt;

extern signed int xdata P50H_Set;
extern signed int xdata P51H_Set;
extern signed int xdata P50L_Set;
extern signed int xdata P51L_Set;

extern unsigned char xdata Correct_Percent_new[5];
extern unsigned char xdata Correct_Percent_vent[5];
extern unsigned char xdata Flag_WriteFlash;
extern unsigned int xdata PassWard;
#endif