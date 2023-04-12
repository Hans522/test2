/**
  ******************************************************************************
  * @file      HTU20.x
  * @brief     HTU20 function
  * @cpu       STM32F051
  * @compiler  Keil uVision V4.74
  * @author    MetalSeed
  * @copyright WSHHB
  * @version   V1.0.0
  * @date      18-Sept-2014
  * @modifydate20-Sept-2014
  ******************************************************************************
  * @attention
  */
#include "H/Function_Init.H"
#include<intrins.h>

/**
  * @brief  Start HTU GPIO port CLK
  * @retval None
  */



/*================================================================
                           IIC address
================================================================*/
#define SHT20ADDR      0x80

/*================================================================
                 ACK  and  NACK  defination
================================================================*/
#define ACK            0
#define NACK           1

/*================================================================
                       HTU20D Command Code
================================================================*/
/*
  Command                         Code              Comment
  Trigger Temperature Measurement 0xe3              Hold master
  Trigger Humidity Measurement    0xe5              Hold master
  Trigger Temperature Measuremeng 0xf3              No Hold master
  Trigger Humidity Measurement    0xf5              No Hold master
  Write user register             0xe6
  Read user register              0xe7
  Soft Reset                      0xfe
*/
#define SOFT_RESET      0xfe
#define READ_REGISTER   0xe7
#define WRITE_REGISTER  0xe6
#define TRIGGER_TEMP    0xf3
#define TRIGGER_HUMI    0xf5

/*================================================================
                    HTU20 operate interface
================================================================*/
//#define HTU20_SDA         P46
//#define HTU20_SCL         P47

/**
  * @brief  Set SDA pin dircate as output
  * @retval None
  */
void HTU20_SDA_OUT()//P46
{
  P4CON |= 0x40;
}

/**
  * @brief  Set SDA pin dircate as intput
  * @retval None
  */
void HTU20_SDA_IN()
{
 P4CON &= 0xBF;
}

/**
  * @brief  Set SDA pin as high
  * @retval None
  */
void HTU20_SDA_1()
{
  P46=1;
}

/**
  * @brief  Set SDA pin as low
  * @retval None
  */
void HTU20_SDA_0()
{
   P46=0;
}

/**
  * @brief  Set SCL pin dircate as output
  * @retval None
  */
void HTU20_SCL_OUT()//P47
{
   P4CON |= 0x80;
}

/**
  * @brief  Set SCL pin as high
  * @retval None
  */
void HTU20_SCL_1()
{
  P47=1;
}

/**
  * @brief  Set SCL pin as low
  * @retval None
  */
void HTU20_SCL_0()
{
    P47=0;
}

void Delay60ms()		//@32MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 8;
	j = 76;
	k = 168;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);

}


/*================================================================
                  Simulation I2C Function
================================================================*/

/**
  * @brief  A short delay  for iic synchronization
  * @param  t us
  * @retval None
  */
void I2CDelay (unsigned char t)
{
  while(t--)
  {
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
    _nop_();  _nop_();_nop_(); _nop_(); _nop_();
  }
}
/**
  * @brief  I2C init
  * @retval None
  */

/*void I2CInit(void)
{
  HTU20_SDA_OUT();
  HTU20_SCL_OUT();
  HTU20_SDA_1();
  I2CDelay(1);
  HTU20_SCL_1();
  I2CDelay(1);
}*/

/**
  * @brief   IIC start signal
  * @comment SCL��SDAͬΪ�ߣ�SDA����ɵ�֮��SCL����ɵ�
  * @retval  None
  */
void I2CStart(void)
{
  HTU20_SDA_OUT();
  HTU20_SCL_OUT();
  HTU20_SDA_1();
  HTU20_SCL_1();
  I2CDelay(10);     //����4.7us  5
  HTU20_SDA_0();
  I2CDelay(10);     //����4us  5
  HTU20_SCL_0();
  I2CDelay(10);//5
}

/**
  * @brief  IIC Stop Signal
  * @commet SCL��SDAͬΪ�ͣ�SCL����ɸ�֮��SDA����ɸ�
  * @retval None
  */
void I2CStop(void)
{
  HTU20_SDA_OUT();
  HTU20_SDA_0();
  HTU20_SCL_0();
  I2CDelay(10);
  HTU20_SCL_1();
  I2CDelay(10);
  HTU20_SDA_1();
  I2CDelay(10);
}

/**
  * @brief  simu IIC write byte
  * @param  Write_Byte: data
  * @retval None
  */
/*unsigned char I2C_Write_Byte(unsigned char Write_Byte)
{
  unsigned char i,HTU20_SDA;
  unsigned char t=200;
  HTU20_SDA_OUT();
  HTU20_SCL_0();
  for(i = 0; i < 8; ++i)
  {
    if(Write_Byte & SHT20ADDR)//��sclΪ��ʱ�������ݸı䣬sclΪ��ʱ������Ч
    {
      HTU20_SDA_1();
    }
    else
    {
      HTU20_SDA_0();
    }
    I2CDelay(10);//1
    HTU20_SCL_1();       //���SDA�ȶ�������SCL���ӻ���⵽�����
    I2CDelay(10);         //��֤�㹻������ʱ�䣬ȷ���ӻ������ɹ�  5
    HTU20_SCL_0();
    //I2CDelay(2);  //1
    Write_Byte <<= 1;
  }
  HTU20_SDA_1();         //8λ��������ͷ������ߣ�׼������Ӧ���ź�
  I2CDelay(10);//4
  HTU20_SCL_1();         //�ٴ�����SCL�����ߴӻ�������ϣ��ȴ�Ӧ��
  I2CDelay(2);//5

  HTU20_SDA_IN();
  /*HTU20_SDA =P46;
  if(HTU20_SDA == 1)     //SDAΪ�ߣ��յ�NACK
  	{
  	P15 =1;
    return NACK;
  	}
  else                   //SDAΪ�ͣ��յ�ACK
  	{
  	 P14 =1;
    return ACK;
  	}
  HTU20_SDA =P46;
  while(HTU20_SDA)
  	{
		t--;
		I2CDelay(1);
		if(t==0)
			{
				HTU20_SCL_0(); 
				P15 ^=0x01;
				P16=1;
				return 1;

			}
		HTU20_SDA =P46;
		I2CDelay(1);
  	}
   //I2CDelay(2);
   //HTU20_SCL_1(); 
   I2CDelay(10);
   HTU20_SCL_0(); 
   I2CDelay(10);
   P16 ^=0x01;
   P15=1;
   return 0;
//  HTU20_SCL_0();
//  I2CDelay(15);
}*/


unsigned char I2C_Write_Byte(unsigned char txByte)
{
	  unsigned char  error =0;
	  unsigned char 	mask;
	
	  HTU20_SDA_OUT();
	  for(mask = 0x80; mask > 0; mask >>= 1)
	  {
		  if((mask & txByte) == 0) 
		  
			  HTU20_SDA_0(); 
		  else					   
			  HTU20_SDA_1();	

		   I2CDelay(10);//1
    	   HTU20_SCL_1();       //���SDA�ȶ�������SCL���ӻ���⵽�����
           I2CDelay(10);         //��֤�㹻������ʱ�䣬ȷ���ӻ������ɹ�  5
           HTU20_SCL_0();
           I2CDelay(10);  //1
	  }
	 // HTU20_SDA_1();						  
	 // HTU20_SDA_IN();
	 // I2CDelay(10);  //1			
	  //HTU20_SCL_1();  
	  ////I2CDelay(10);  //1
	 // HTU20_SDA=P46;
	 // if(HTU20_SDA)
	 //	  error = 1;	   // check ack from i2c slave
	 // HTU20_SCL_0();       //ֻ��SCLΪH��SDA��ֵ����Ч
	 // HTU20_SDA_1();	
	 // HTU20_SDA_OUT();
	 // I2CDelay(10);  //1 			   // wait to see byte package on scope	  	 
	return error;						  // return error code
}



/**
  * @brief  simu read byte form IIC
  * @param
  * @retval None
  */
signed char I2C_Read_Byte(unsigned char AckValue)//receivebyte
{
  unsigned char i, HTU20_SDA,RDByte = 0;
 // HTU20_SDA_OUT();
  //HTU20_SCL_0();
 // HTU20_SDA_1();      //�ͷ����ߣ�����������Ϊ����
  HTU20_SDA_IN();
  I2CDelay(10);
  for (i = 0; i < 8; ++i)
  {
    HTU20_SCL_1();    //����SCL�ڼ䣬����
    I2CDelay(10);
    RDByte <<= 1;
	HTU20_SDA=P46;
    if(HTU20_SDA == 1)
    {
      RDByte |= 0x01;
    }
    else
    {
      RDByte &= 0xfe;
    }
    I2CDelay(2);
    HTU20_SCL_0();    //�½��ظ�֪�ӻ�������һλ
    I2CDelay(10);
  }
  HTU20_SDA_OUT();   //������һ���ֽڣ�����ACK or NACK
  if(AckValue == 1)
  {
    HTU20_SDA_1();
  }
  else
  {
    HTU20_SDA_0();
  }
  I2CDelay(10);
  HTU20_SCL_1();
  I2CDelay(10);
  HTU20_SCL_0();     //��ʱ����
  I2CDelay(10);
  //HTU20_SDA_1();
  return RDByte;
}

/**
  * @brief  soft reset by transmit reset command
  * @retval None
  */
/*void SoftReset(void)
{
  I2CInit();
  I2CStart();
  I2C_Write_Byte(SHT20ADDR & 0xfe);  //I2C address + write
  I2C_Write_Byte(SOFT_RESET);        //soft reset
  I2CStop();
}*/

/**
  * @brief  SET HTU20D resolution by write register
  * @retval None
  */
/*void SET_Resolution(void)
{
  I2CStart();
  if(I2C_Write_Byte(SHT20ADDR & 0xfe) == ACK) //I2C address + write + ACK
  {
    if(I2C_Write_Byte(WRITE_REGISTER)==ACK)   //д�û��Ĵ���
    {
      //if(I2C_Write_Byte(0x83)==ACK)P17^=0X01;         //���÷ֱ���   11bit RH% ����ʱ�䣺12ms(typ.)
      P17^=0X01;
    }                                         //             11bit T�� ����ʱ�䣺9ms(typ.)
  }
  I2CStop();
}*/

/**
  * @brief  read sht20's converted result
  * @param  TempOrHumiCMD: operation command for temperature or humidity
  * @retval None
  */
/*float ReadSht20(char TempOrHumiCMD)
{
  float temp;
  signed char MSB,LSB;
  float Humidity, Temperature;

  //SET_Resolution();
  I2CStart();
  if(I2C_Write_Byte(SHT20ADDR & 0xfe) == ACK)      //I2C address + write + ACK
  {
  	
    //if(I2C_Write_Byte(0xf3) == ACK)       //command
    //{
    	
      /*do
      {
        Delay6ms();
        I2CStart();
      } while(I2C_Write_Byte(SHT20ADDR | 0x01) == NACK); //I2C address + read + NACK
      //P16 ^=0x01;
      MSB = I2C_Read_Byte(ACK);
      LSB = I2C_Read_Byte(ACK);
      I2C_Read_Byte(NACK);                        //Checksum  + NACK
      I2CStop();
      LSB &= 0xfc;                                //Data (LSB) �ĺ���λ�ڽ����������ǰǰ����0
      temp = (MSB << 8) + LSB;
       
      if (TempOrHumiCMD == ((char)TRIGGER_HUMI))
      {
        //-- calculate relative humidity [%RH] --
        //equation: RH% = -6 + 125 * SRH/2^16
        Humidity = (temp * 125) / 65536 - 6;
        return Humidity;
      }
      else
      {
        //-- calculate temperature [��C] --//
        //equation:T = -46.85 + 175.72 * ST/2^16
        Temperature = (temp * 175.72) / 65536 - 46.85;		
		//P17 ^=0x01;
        return Temperature;
      }
    //}
	 //I2CStop();
  }
  return 0;
}*/
unsigned char	IIC_wait_ACK()
{
	unsigned char error = 0;
	unsigned char t=200,HTU20_SDA;

	
	//SDA_OUT();
	HTU20_SDA_1();
	I2CDelay(10);//1
	//DelayMicroSeconds(1); 
	//SCL_LOW();
	//DelayMicroSeconds(1); //set off bus

	HTU20_SDA_IN();
	I2CDelay(10);//1
	HTU20_SDA=P46;
	while(HTU20_SDA)
		{
		t--;
		I2CDelay(10);//1
		if(t==0)
			{
			HTU20_SCL_1();
			I2CDelay(10);//1;//wait master read
			HTU20_SCL_0();
			I2CDelay(10);//1;//wait master read	
			return 2;
			}
		HTU20_SDA=P46;
		I2CDelay(10);//1		
		}
	//DelayMicroSeconds(1);
	HTU20_SCL_1();
	I2CDelay(10);//1;//wait master read
	HTU20_SCL_0();
	I2CDelay(10);//1;//wait master read	
	return 0;


}

float ReadSht20(char TempOrHumiCMD)
{
	unsigned char error;
	unsigned char read_cnt=20;
    signed char MSB,LSB;
	float Humidity, Temperature;
	float temp;
	
	I2CStart();
	error |=I2C_Write_Byte(0x80);
	error |= IIC_wait_ACK();
	error |=I2C_Write_Byte(0xf3);
	error |= IIC_wait_ACK();
	do
      {
      	WDTCON  = 0x10;	//500ms
        Delay60ms();
        I2CStart();
		I2C_Write_Byte(0x81);
		error |=IIC_wait_ACK();
	    read_cnt--;
      } while(error==2 && read_cnt>2); //I2C address + read + NACK

      MSB = I2C_Read_Byte(ACK);//�����귢��ACK
      LSB = I2C_Read_Byte(ACK);
      I2C_Read_Byte(NACK);                        //Checksum  + NACK
      I2CStop();
      LSB &= 0xfc;                                //Data (LSB) �ĺ���λ�ڽ����������ǰǰ����0
      temp = (MSB << 8) + LSB;
	  
	  if (TempOrHumiCMD == ((char)TRIGGER_HUMI))
      {
        //-- calculate relative humidity [%RH] --
        //equation: RH% = -6 + 125 * SRH/2^16
        Humidity = (temp * 125) / 65536 - 6;
        return Humidity;
      }
      else
      {
        //-- calculate temperature [��C] --//
        //equation:T = -46.85 + 175.72 * ST/2^16
        Temperature = (temp * 175.72) / 65536 - 46.85;		
		//P17 ^=0x01;
        return Temperature;
      }	  	
}


