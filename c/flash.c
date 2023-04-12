#include "H/Function_Init.H"


#define		FLASHLENGTH			32
unsigned char xdata ucFlashTemp[FLASHLENGTH];
unsigned char code *IapAddr = 0;


/*****************************************************
*函数名称：void IAPWrite(uchar Addr,uchar Value,uchar IAPArea)
*函数功能：IAP写初始化
*入口参数：Addr,Value,IAPArea
*出口参数：void
*****************************************************/
//IAP写操作
void IAPWrite(uint Addr,uchar Value,uchar IAPArea)
{	
    bit temp = EA;
	EA = 0;
    IAPADE = IAPArea;   //IAPArea=0x00：选择ROM区操作  IAPArea=0x02：选择EEPROM区操作
	IAPDAT = Value;     //写入数据Data
	IAPADH = (unsigned char)((Addr >> 8));   //写入地址的高8位
	IAPADL = (unsigned char)Addr;            //写入地址的低8位
	IAPKEY = 0xF0;		//IAP开启保护控制时间
    IAPCTL = 0X10;      //执行“写入或快擦除操作命令”后，进入flash编程操作
	IAPCTL |= 0x02;     //执行“写入或快擦除操作命令”
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	IAPADE = 0x00;      //MOVC指向ROM
	EA = temp;
}

/*****************************************************
*函数名称：void IAPPageErase(uint Addr,uchar IAPArea)
*函数功能：IAP页擦除
*入口参数：Addr,IAPArea
*出口参数：void
*****************************************************/
void IAPPageErase(uint Addr,uchar IAPArea)
{
	bit temp = EA;
	EA = 0;
	IAPADE = IAPArea;
	
	IAPADH = (unsigned char)((Addr >> 8)); //写入待擦除首地址高位
    IAPADL = (unsigned char)Addr;          //写入待擦除首地址低位
	
	IAPKEY = 0XF0;			//写入一个大于0x40的值n，1、打开IAP，2、n个系统时钟后接受不到IAP写入命令，IAP功能关闭
	IAPCTL = 0X20;
	IAPCTL |= 0X02;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
    _nop_();
	_nop_();
	_nop_();
	_nop_();
	IAPADE = 0x00;
	EA = temp;		
}

/*****************************************************
*函数名称：uchar IAPRead(uchar Addr,uchar IAPArea)
*函数功能：IAP读初始化
*入口参数：Addr,IAPArea
*出口参数：ReadValue
*****************************************************/
uchar IAPRead(uint Addr,uchar IAPArea)
{
	uchar ReadValue = 0x00;
    bit temp = EA;
	EA = 0;
	IAPADE = IAPArea;
	ReadValue = *(IapAddr+Addr); //读取的数据
	IAPADE = 0x00;               //MOVC指向ROM
	EA = temp;
	return ReadValue;
}







void FlashWrite()
{
		unsigned char cnt,cnt1,check = 0;
		
		IAPPageErase(0xFE00,IapROM);
		
		cnt = 0;
		ucFlashTemp[cnt ++] = ucPowerOrReset;
		ucFlashTemp[cnt ++] = ucFanSpeed;
		ucFlashTemp[cnt ++] = (unsigned char)scFillTemper;
		ucFlashTemp[cnt ++] = ucIPH;
		ucFlashTemp[cnt ++] = ucIPL;
		ucFlashTemp[cnt ++] = (unsigned char)(uiSetTimePrimary / 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiSetTimePrimary % 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiSetTimeHigh / 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiSetTimeHigh % 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiRunTimePrimary / 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiRunTimePrimary % 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiRunTimeHigh / 100);
		ucFlashTemp[cnt ++] = (unsigned char)(uiRunTimeHigh % 100);
		ucFlashTemp[cnt ++] = ucFanDelay;
		
		if(ulFlag.Bits.F_AlarmPrimary)
			ucFlashTemp[cnt ++] = 0xA5;
		else
			ucFlashTemp[cnt ++] = 0;
			
		if(ulFlag.Bits.F_AlarmHigh)
			ucFlashTemp[cnt ++] = 0xA5;
		else
			ucFlashTemp[cnt ++] = 0;
			
		if(ulFlag.Bits.F_Lock)
			ucFlashTemp[cnt ++] = 0xA5;
		else
			ucFlashTemp[cnt ++] = 0;
			

			
		for(cnt1 = 0;cnt1 < FLASHLENGTH - 1;cnt1 ++)
			{
				check += ucFlashTemp[cnt1];
			}
		ucFlashTemp[FLASHLENGTH - 1] = check ^ 0xA5;
		
		for(cnt1 = 0;cnt1 < FLASHLENGTH;cnt1 ++)
			{
				IAPWrite((0xFE00 + cnt1),ucFlashTemp[cnt1],IapROM);
			}
					//IAPWrite(0x7E00,ucPowerOrReset,IapROM);
//					IAPWrite(0x7E01,ucWorkMode,IapROM);
//					IAPWrite(0x7E02,ucFanSpeed,IapROM);
//					IAPWrite(0x7E03,ucSetTemperHand,IapROM);
}


void	FlashRead()
{
		unsigned char cnt,cnt1,check = 0;
		
		for(cnt1 = 0;cnt1 < FLASHLENGTH;cnt1 ++)
			{
				ucFlashTemp[cnt1] = IAPRead((0xFE00 + cnt1),IapROM);
			}
			
		for(cnt1 = 0;cnt1 < FLASHLENGTH - 1;cnt1 ++)
			{
				check += ucFlashTemp[cnt1];
			}
		check ^= 0xA5;
		
				
		
		if(check == ucFlashTemp[FLASHLENGTH - 1])
			{
				cnt = 0;
				ulFlag.Bits.F_EeRight = 1;

				ucPowerOrReset = ucFlashTemp[cnt ++];
				ucFanSpeed = ucFlashTemp[cnt ++];
				scFillTemper = (signed char)ucFlashTemp[cnt ++];
				ucIPH = ucFlashTemp[cnt ++];
				ucIPL = ucFlashTemp[cnt ++];
				uiSetTimePrimary = (unsigned int)ucFlashTemp[cnt ++] * 100 + (unsigned int)ucFlashTemp[cnt ++];
				uiSetTimeHigh = (unsigned int)ucFlashTemp[cnt ++] * 100 + (unsigned int)ucFlashTemp[cnt ++];
				uiRunTimePrimary = (unsigned int)ucFlashTemp[cnt ++] * 100 + (unsigned int)ucFlashTemp[cnt ++];
				uiRunTimeHigh = (unsigned int)ucFlashTemp[cnt ++] * 100 + (unsigned int)ucFlashTemp[cnt ++];
				ucFanDelay = ucFlashTemp[cnt ++];
				
				if(ucFlashTemp[cnt ++] == 0xA5)
					ulFlag.Bits.F_AlarmPrimary = 1;
				else
					ulFlag.Bits.F_AlarmPrimary = 0;
					
				if(ucFlashTemp[cnt ++] == 0xA5)
					ulFlag.Bits.F_AlarmHigh = 1;
				else
					ulFlag.Bits.F_AlarmHigh = 0;
					
				if(ucFlashTemp[cnt ++] == 0xA5)
					ulFlag.Bits.F_Lock = 1;
				else
					ulFlag.Bits.F_Lock = 0;

			}

		//ucPowerOrReset = IAPRead(0x7E00,IapROM);
//		ucWorkMode = IAPRead(0x7E01,IapROM);
//		ucFanSpeed = IAPRead(0x7E02,IapROM);
//		ucSetTemperHand = IAPRead(0x7E03,IapROM);
}