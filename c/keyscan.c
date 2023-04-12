#include "H/Function_Init.H"



#define		MODE		0x04
#define		TIME		0x08
#define		FAN		0x10
#define		UP			0x20
#define		DOWN		0x40
#define 		LOCK		0x60
#define		DEBUG		0x14

#define		KV_Power			1
#define		KV_Mode				2
#define		KV_Time				3
#define		KV_Fan				4
#define		KV_Up					5
#define		KV_Down				6
#define 		KV_Lock				7
#define		KV_Debug			8
#define		KV_CleanAlarm		9

#define		SHORTPRESS		6
#define		SHORTPRESS1		99
#define		LONGPRESS			130
#define		LOCKPRESS			300
#define		DEBUGPRESS		300	


unsigned char ucKeyvalue;


//----------------------------------------------------------
//				无符号数加减
//----------------------------------------------------------
bit	SetNumber(unsigned char *number,unsigned char limit_h,unsigned char limit_l)
{
		if(*number > limit_h)				
			{
				*number = limit_h;
				return 0;
			}
		if(*number < limit_l)				
			{
				*number = limit_l;
				return 0;
			}
			
		if(ucKeyvalue == KV_Up)
			{
				if(*number < limit_h)
					(*number) ++;
			}
			
		if(ucKeyvalue == KV_Down)
			{
				if(*number > limit_l)
					(*number) --;
			}
			
		if(ucKeyvalue == KV_Up	||	ucKeyvalue == KV_Down)
			return	1;
		else
			return 	0;
}


bit	SetLoopNumber(unsigned char *number,unsigned char limit_h,unsigned char limit_l)
{
		if(ucKeyvalue == KV_Up)
			{
				if(*number == limit_h)
					*number = limit_l;
				else
					(*number) ++;
			}
		
		if(ucKeyvalue == KV_Down)
			{
				if(*number == limit_l)
					*number = limit_h;
				else
					(*number) --;
			}
			
		if(ucKeyvalue == KV_Up	||	ucKeyvalue == KV_Down)
			return	1;
		else
			return 	0;

}


bit	SetNumberAlarm(unsigned int *number,unsigned int limit_h,unsigned int limit_l)
{
		if(*number > limit_h)
			{
				*number = limit_h;
				return 0;
			}
		if(*number < limit_l)
			{
				*number = limit_l;
				return 0;
			}
			
		if(ucKeyvalue == KV_Up)
			{
				if(ulFlag.Bits.F_Longkey_up)
					{
						if(*number < limit_h - 50)
							*number = *number + 50;
					}
				else
					{
						if(*number < limit_h)
							(*number) ++;
					}
			}
			
		if(ucKeyvalue == KV_Down)
			{
				if(ulFlag.Bits.F_Longkey_down)
					{
						if(*number > limit_l + 50)
							*number = *number - 50;
					}
				else
					{
						if(*number > limit_l)
							(*number) --;
					}
			}
			
		if(ucKeyvalue == KV_Up	||	ucKeyvalue == KV_Down)
			return	1;
		else
			return 	0;
}


//----------------------------------------------------------
//				有符号数加减
//----------------------------------------------------------
bit	SetNumberSign(signed char *number,signed char limit_h,signed char limit_l)
{
		if(*number > limit_h)				
			{
				*number = limit_h;
				return 0;
			}
		if(*number < limit_l)				
			{
				*number = limit_l;
				return 0;
			}
			
		if(ucKeyvalue == KV_Up)
			{
				if(*number < limit_h)
					(*number) ++;
			}
			
		if(ucKeyvalue == KV_Down)
			{
				if(*number > limit_l)
					(*number) --;
			}
			
		if(ucKeyvalue == KV_Up	||	ucKeyvalue == KV_Down)
			return	1;
		else
			return 	0;
}
//----------------------------------------------------------
//
//----------------------------------------------------------
void	CleanEveryMode()
{
		ucTimeMode = 0;
		ucDebugMode = 0;
		ulFlag.Bits.F_SetTemper = 0;
}


void	CleanAlarmPrimary()
{
		ulFlag.Bits.F_AlarmPrimary = 0;
		uiRunTimePrimary = 0;
		ulFlag.Bits.F_WriteEe = 1;
}


void	CleanAlarmHigh()
{
		ulFlag.Bits.F_AlarmHigh = 0;
		uiRunTimeHigh = 0;
		ulFlag.Bits.F_WriteEe = 1;
}

//----------------------------------------------------------
//
//----------------------------------------------------------
void	Readkey(void)
{
		static unsigned char ucKeyPower_buf;
		static unsigned char ucKeyMode_buf;
		static unsigned char ucKeyTime_buf;
		static unsigned int  ucKeyFan_buf;
		static unsigned char ucKeyUp_buf;
		static unsigned char ucKeyDown_buf;
		static unsigned int	 uiKeyLock_buf;
		static unsigned int	 uiKeyDebug_buf;
		

/**************************************************
			读按键
**************************************************/
		ucKeyvalue = 0;
		
				 
		if(POWER_PIN == 0)
			{
				ucKeyPower_buf ++;
				if(ucKeyPower_buf == 255)
					ucKeyPower_buf = 254;
			}
		else
			ucKeyPower_buf = 0;


		if(ucKeyData == MODE)
			{
				ucKeyMode_buf ++;
				if(ucKeyMode_buf == 255)
					ucKeyMode_buf = 254;
			}
		else
			ucKeyMode_buf = 0;
			
			
		if(ucKeyData == TIME)
			{
				ucKeyTime_buf ++;
				if(ucKeyTime_buf == 255)
					ucKeyTime_buf = 254;
			}
		else
			ucKeyTime_buf = 0;
			

		if(ucKeyData == FAN)
			{
				ucKeyFan_buf ++;
				if(ucKeyFan_buf == LOCKPRESS)
					{
						ucKeyvalue = KV_CleanAlarm;
					}
				if(ucKeyFan_buf == 305)
					{
						ucKeyFan_buf = 301;
					}
			}
		else
			{
				if(ucKeyFan_buf >= SHORTPRESS && ucKeyFan_buf <= SHORTPRESS1)
					{
						ucKeyvalue = KV_Fan;
					}
				ucKeyFan_buf = 0;
			}
			
		
		if(ucKeyData == UP)
			{
				ucKeyUp_buf ++;
				if(ucKeyUp_buf == LONGPRESS)
					{
						ucKeyvalue = KV_Up;
						ulFlag.Bits.F_Longkey_up = 1;
						ucKeyUp_buf = 120;
					}
			}
		else
			{
				if(ucKeyUp_buf >= SHORTPRESS && ucKeyUp_buf <= SHORTPRESS1)
					{
						ucKeyvalue = KV_Up;
					}
				ulFlag.Bits.F_Longkey_up = 0;
				ucKeyUp_buf = 0;
			}
			
			
		if(ucKeyData == DOWN)
			{
				ucKeyDown_buf ++;
				if(ucKeyDown_buf == LONGPRESS)
					{
						ucKeyvalue = KV_Down;
						ulFlag.Bits.F_Longkey_down = 1;
						ucKeyDown_buf = 120;
					}
			}
		else
			{
				if(ucKeyDown_buf >= SHORTPRESS && ucKeyDown_buf <= SHORTPRESS1)
					{
						ucKeyvalue = KV_Down;
					}
				ulFlag.Bits.F_Longkey_down = 0;
				ucKeyDown_buf = 0;
			}
			
			
		if(ucKeyData == LOCK)
			{
				uiKeyLock_buf ++;
				if(uiKeyLock_buf == 600)
					uiKeyLock_buf = 599;
			}
		else
			uiKeyLock_buf = 0;
			
			
		if(ucKeyData == DEBUG)
			{
				uiKeyDebug_buf ++;
				if(uiKeyDebug_buf == 600)
					uiKeyDebug_buf = 599;
			}
		else
			uiKeyDebug_buf = 0;
			
		
		
		if(ucKeyPower_buf == SHORTPRESS)
			{
				ucKeyvalue = KV_Power;
			}
			
		if(ucKeyMode_buf == SHORTPRESS)
			{
				ucKeyvalue = KV_Mode;
			}
			
		if(ucKeyTime_buf == SHORTPRESS)
			{
				ucKeyvalue = KV_Time;
			}
			
		if(uiKeyLock_buf == LOCKPRESS)
			{
				ucKeyvalue = KV_Lock;
			}
			
		if(uiKeyDebug_buf == DEBUGPRESS)
			{
				ucKeyvalue = KV_Debug;
			}
}



void	Keyscan(void)
{
		Readkey();
		
		while(ucKeyvalue)
			{
				if(ucPowerOrReset == 0xA5 && ulFlag.Bits.F_Lock == 0)
					{
						SETBACKLED;						
					}
				ucTimeCount30s = 0;
				ucMotorOutCount = 0;
				
				if(ucKeyvalue == KV_Mode || ucKeyvalue == KV_Fan || ucKeyvalue == KV_Up || ucKeyvalue == KV_Down)
					{
						if(ucPowerOrReset == 0x5A && ucDebugMode == 0)
							break;		//跳出while											
					}
					
				break;			//跳出while	
			}
		
		if(ucKeyvalue == KV_Power)
			{
				if(ulFlag.Bits.F_Lock && ulFlag.Bits.F_LockMode)			return;
					
				ucPowerOrReset ^= 0xFF;
				CleanEveryMode();
				MotorOutClean();
				ulFlag.Bits.F_WriteEe = 1;
				
				if(ucPowerOrReset == 0xA5)
					{
						SETBACKLED;
					}
				else
					{
						CLRBACKLED;
					}
			}
			
		if(ucKeyvalue == KV_Lock)
			{
				ulFlag.Bits.F_Lock = ~ulFlag.Bits.F_Lock;
				ulFlag.Bits.F_WriteEe = 1;
			}
			
		if(ulFlag.Bits.F_Lock)			return;

			
		if(ucPowerOrReset == 0xA5)
			{
				if(ucKeyvalue == KV_Fan)
					{
						ucFanSpeed ++;
						
						if(ucFanSpeed == 4)
							ucFanSpeed = 1;
							
						CleanEveryMode();
						ulFlag.Bits.F_WriteEe = 1;
					}
					
					
				if(ucKeyvalue == KV_Time)
					{
						ucTimeMode ++;
						
						if(ucTimeMode == 4)
							ucTimeMode = 0;
					}
					
				switch(ucTimeMode)
					{
						case	0:
							break;
							
						case	1:
							if(SetLoopNumber(&ucTimeMinute,59,0))
								ulFlag.Bits.F_WriteTime = 1;
							break;
							
						case	2:
							if(SetLoopNumber(&ucTimeHour,23,0))
								ulFlag.Bits.F_WriteTime = 1;
							break;
							
						case	3:
							if(SetLoopNumber(&ucTimeWeek,7,1))
								ulFlag.Bits.F_WriteTime = 1;
							break;

						default:
							break;
					}


				if(ucKeyvalue == KV_CleanAlarm && (ulFlag.Bits.F_AlarmPrimary || ulFlag.Bits.F_AlarmHigh))
					{
						if(ulFlag.Bits.F_AlarmHigh)
							{
								CleanAlarmHigh();
							}
						else if(ulFlag.Bits.F_AlarmPrimary)
							{
								CleanAlarmPrimary();
							}
					}
			}
		else
			{
				if(ucDebugMode)
					{
						switch(ucDebugMode)
							{
								case	1:
									if(SetNumberSign(&scFillTemper,9,-9))
										ulFlag.Bits.F_WriteEe = 1;
									break;
									
								case	2:
									if(SetNumber(&ucIPH,255,0))
										ulFlag.Bits.F_WriteEe = 1;
									if(ucIPH == 0x00 && ucIPL == 0x00)
										ucIPL = 0x10;
									break;
									
								case	3:
									if(SetNumber(&ucIPL,255,0))
										ulFlag.Bits.F_WriteEe = 1;
									if(ucIPH == 0x00 && ucIPL == 0x00)
										ucIPL = 0x10;
									break;
									
								case	4:
									if(SetNumberAlarm(&uiSetTimePrimary,9999,100))
										{
											CleanAlarmPrimary();
										}
									break;
									
								case	5:
									if(SetNumberAlarm(&uiSetTimeHigh,9999,100))
										{
											CleanAlarmHigh();
										}
									break;
									
								case	6:
									break;
									
								case	7:
									if(SetNumber(&ucFanDelay,9,1))
										ulFlag.Bits.F_WriteEe = 1;
									break;
							}
						
						if(ucKeyvalue == KV_Mode)
							{
								ucDebugMode ++;
								if(ucDebugMode == 8)
									ucDebugMode = 1;
							}
					}
				else
					{
						if(ucKeyvalue == KV_Debug)
							{
								ucDebugMode = 1;
								SETBACKLED;
							}
					}
			}
}