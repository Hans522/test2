#include "H/Function_Init.H"


#define		FLASHLENGTH			32
unsigned char xdata ucFlashTemp[FLASHLENGTH];
unsigned char code *IapAddr = 0;


/*****************************************************
*�������ƣ�void IAPWrite(uchar Addr,uchar Value,uchar IAPArea)
*�������ܣ�IAPд��ʼ��
*��ڲ�����Addr,Value,IAPArea
*���ڲ�����void
*****************************************************/
//IAPд����
void IAPWrite(uint Addr,uchar Value,uchar IAPArea)
{	
    bit temp = EA;
	EA = 0;
    IAPADE = IAPArea;   //IAPArea=0x00��ѡ��ROM������  IAPArea=0x02��ѡ��EEPROM������
	IAPDAT = Value;     //д������Data
	IAPADH = (unsigned char)((Addr >> 8));   //д���ַ�ĸ�8λ
	IAPADL = (unsigned char)Addr;            //д���ַ�ĵ�8λ
	IAPKEY = 0xF0;		//IAP������������ʱ��
    IAPCTL = 0X10;      //ִ�С�д���������������󣬽���flash��̲���
	IAPCTL |= 0x02;     //ִ�С�д��������������
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	IAPADE = 0x00;      //MOVCָ��ROM
	EA = temp;
}

/*****************************************************
*�������ƣ�void IAPPageErase(uint Addr,uchar IAPArea)
*�������ܣ�IAPҳ����
*��ڲ�����Addr,IAPArea
*���ڲ�����void
*****************************************************/
void IAPPageErase(uint Addr,uchar IAPArea)
{
	bit temp = EA;
	EA = 0;
	IAPADE = IAPArea;
	
	IAPADH = (unsigned char)((Addr >> 8)); //д��������׵�ַ��λ
    IAPADL = (unsigned char)Addr;          //д��������׵�ַ��λ
	
	IAPKEY = 0XF0;			//д��һ������0x40��ֵn��1����IAP��2��n��ϵͳʱ�Ӻ���ܲ���IAPд�����IAP���ܹر�
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
*�������ƣ�uchar IAPRead(uchar Addr,uchar IAPArea)
*�������ܣ�IAP����ʼ��
*��ڲ�����Addr,IAPArea
*���ڲ�����ReadValue
*****************************************************/
uchar IAPRead(uint Addr,uchar IAPArea)
{
	uchar ReadValue = 0x00;
    bit temp = EA;
	EA = 0;
	IAPADE = IAPArea;
	ReadValue = *(IapAddr+Addr); //��ȡ������
	IAPADE = 0x00;               //MOVCָ��ROM
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