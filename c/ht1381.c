#include "H/Function_Init.H"


/***********************************************************************
以下是DS1302芯片的操作程序
************************************************************************/

/*****************************************************
函数功能：延时若干微秒
入口参数：n
***************************************************/ 
void delaynus(unsigned char n)
{
 unsigned char p;
 for(p=0;p<n;p++)
      ;
}
/*****************************************************
函数功能：向1302写一个字节数据
入口参数：x
***************************************************/ 
void Write1302(unsigned char dat)
{
  unsigned char q; 
  SCLK=0;            //拉低SCLK，为脉冲上升沿写入数据做好准备
  
  HT1381_IO_OUT();
  
  delaynus(2);       //稍微等待，使硬件做好准备
  for(q=0;q<8;q++)      //连续写8个二进制位数据
    {
	    //DATA=dat&0x01;    //取出dat的第0位数据写入1302
	    if(dat & 0x01)
	    	DATA = 1;
	    else
	    	DATA = 0;
		delaynus(2);       //稍微等待，使硬件做好准备
		 SCLK=1;           //上升沿写入数据
		 delaynus(2);      //稍微等待，使硬件做好准备
		 SCLK=0;           //重新拉低SCLK，形成脉冲
		 dat>>=1;          //将dat的各数据位右移1位，准备写入下一个数据位
	  }
	
	HT1381_IO_INPUT();
 }
/*****************************************************
函数功能：根据命令字，向1302写一个字节数据
入口参数：Cmd，储存命令字；dat，储存待写的数据
***************************************************/ 
void WriteSet1302(unsigned char Cmd,unsigned char dat)
 {
   RST=0;           //禁止数据传递
   SCLK=0;          //确保写数居前SCLK被拉低
	RST=1;           //启动数据传输
	delaynus(2);     //稍微等待，使硬件做好准备
	Write1302(Cmd);  //写入命令字
	Write1302(dat);  //写数据
	SCLK=1;          //将时钟电平置于已知状态
	RST=0;           //禁止数据传递
 }
/*****************************************************
函数功能：从1302读一个字节数据
入口参数：x
***************************************************/ 
unsigned char Read1302(void)
 {
   unsigned char q,dat;
	delaynus(2);       //稍微等待，使硬件做好准备
	for(q=0;q<8;q++)   //连续读8个二进制位数据
	 {
	   dat>>=1;       //将dat的各数据位右移1位，因为先读出的是字节的最低位
		if(DATA==1)    //如果读出的数据是1
		 dat|=0x80;    //将1取出，写在dat的最高位
		 SCLK=1;       //将SCLK置于高电平，为下降沿读出
		 delaynus(2);  //稍微等待
		 SCLK=0;       //拉低SCLK，形成脉冲下降沿
		 delaynus(2);  //稍微等待
	 }
	DATA = 0;	 
  return dat;        //将读出的数据返回
}  
/*****************************************************
函数功能：根据命令字，从1302读取一个字节数据
入口参数：Cmd
***************************************************/ 
unsigned char  ReadSet1302(unsigned char Cmd)
 {
  unsigned char dat;
  RST=0;                 //拉低RST
  SCLK=0;                //确保写数居前SCLK被拉低
  RST=1;                 //启动数据传输
  Write1302(Cmd);       //写入命令字
  dat=Read1302();       //读出数据
  SCLK=1;              //将时钟电平置于已知状态
  RST=0;               //禁止数据传递
  return dat;          //将读出的数据返回
}




void 	Write_1381(void)
{
		WriteSet1302(0x8e,0x00);   
	   WriteSet1302(0x82,((ucTimeMinute / 10)<<4 | (ucTimeMinute % 10)));  
	   WriteSet1302(0x84,((ucTimeHour / 10)<<4 | (ucTimeHour % 10)));   
	   WriteSet1302(0x8a,((ucTimeWeek / 10)<<4 | (ucTimeWeek % 10)));            
   	WriteSet1302(0x8e,0x80);  
}


void	Read_1381(void)
{
		unsigned char ReadValue;
//		ReadValue = ReadSet1302(0x81);
//		ucTimeSecond =((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
				
		ReadValue = ReadSet1302(0x83);
		ucTimeMinute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		
		ReadValue = ReadSet1302(0x85);  
		ucTimeHour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
		
		ReadValue = ReadSet1302(0x8B);
		ucTimeWeek = ReadValue;
}


void	Enable_1381(void)
{
		unsigned char	ReadValue;
		
		ulFlag.Bits.F_TimeFunction = 1;
		ReadValue = ReadSet1302(0x81);
		if(ReadValue & 0x80)
			{
				WriteSet1302(0x8e,0x00);
				WriteSet1302(0x80,0x00);
				WriteSet1302(0x8e,0x80);  
			}
		
		ReadValue = ReadSet1302(0x81);
		if(ReadValue & 0x80)
			ulFlag.Bits.F_TimeFunction = 0;
}