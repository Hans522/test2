#include "H/Function_Init.H"


/***********************************************************************
������DS1302оƬ�Ĳ�������
************************************************************************/

/*****************************************************
�������ܣ���ʱ����΢��
��ڲ�����n
***************************************************/ 
void delaynus(unsigned char n)
{
 unsigned char p;
 for(p=0;p<n;p++)
      ;
}
/*****************************************************
�������ܣ���1302дһ���ֽ�����
��ڲ�����x
***************************************************/ 
void Write1302(unsigned char dat)
{
  unsigned char q; 
  SCLK=0;            //����SCLK��Ϊ����������д����������׼��
  
  HT1381_IO_OUT();
  
  delaynus(2);       //��΢�ȴ���ʹӲ������׼��
  for(q=0;q<8;q++)      //����д8��������λ����
    {
	    //DATA=dat&0x01;    //ȡ��dat�ĵ�0λ����д��1302
	    if(dat & 0x01)
	    	DATA = 1;
	    else
	    	DATA = 0;
		delaynus(2);       //��΢�ȴ���ʹӲ������׼��
		 SCLK=1;           //������д������
		 delaynus(2);      //��΢�ȴ���ʹӲ������׼��
		 SCLK=0;           //��������SCLK���γ�����
		 dat>>=1;          //��dat�ĸ�����λ����1λ��׼��д����һ������λ
	  }
	
	HT1381_IO_INPUT();
 }
/*****************************************************
�������ܣ����������֣���1302дһ���ֽ�����
��ڲ�����Cmd�����������֣�dat�������д������
***************************************************/ 
void WriteSet1302(unsigned char Cmd,unsigned char dat)
 {
   RST=0;           //��ֹ���ݴ���
   SCLK=0;          //ȷ��д����ǰSCLK������
	RST=1;           //�������ݴ���
	delaynus(2);     //��΢�ȴ���ʹӲ������׼��
	Write1302(Cmd);  //д��������
	Write1302(dat);  //д����
	SCLK=1;          //��ʱ�ӵ�ƽ������֪״̬
	RST=0;           //��ֹ���ݴ���
 }
/*****************************************************
�������ܣ���1302��һ���ֽ�����
��ڲ�����x
***************************************************/ 
unsigned char Read1302(void)
 {
   unsigned char q,dat;
	delaynus(2);       //��΢�ȴ���ʹӲ������׼��
	for(q=0;q<8;q++)   //������8��������λ����
	 {
	   dat>>=1;       //��dat�ĸ�����λ����1λ����Ϊ�ȶ��������ֽڵ����λ
		if(DATA==1)    //���������������1
		 dat|=0x80;    //��1ȡ����д��dat�����λ
		 SCLK=1;       //��SCLK���ڸߵ�ƽ��Ϊ�½��ض���
		 delaynus(2);  //��΢�ȴ�
		 SCLK=0;       //����SCLK���γ������½���
		 delaynus(2);  //��΢�ȴ�
	 }
	DATA = 0;	 
  return dat;        //�����������ݷ���
}  
/*****************************************************
�������ܣ����������֣���1302��ȡһ���ֽ�����
��ڲ�����Cmd
***************************************************/ 
unsigned char  ReadSet1302(unsigned char Cmd)
 {
  unsigned char dat;
  RST=0;                 //����RST
  SCLK=0;                //ȷ��д����ǰSCLK������
  RST=1;                 //�������ݴ���
  Write1302(Cmd);       //д��������
  dat=Read1302();       //��������
  SCLK=1;              //��ʱ�ӵ�ƽ������֪״̬
  RST=0;               //��ֹ���ݴ���
  return dat;          //�����������ݷ���
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
//		ucTimeSecond =((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
				
		ReadValue = ReadSet1302(0x83);
		ucTimeMinute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		
		ReadValue = ReadSet1302(0x85);  
		ucTimeHour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //����������ת��
		
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