#include "H/Function_Init.H"





unsigned int code  Temperature_Tbl[]=
{
		//3470,			//-10
			3456,
		3440,
			3426,
		3410,
			3395,
		3379,
			3364,
		3347,
			3331,
		3314,
			3298,
		3280,
			3264,
		3245,
			3229,
		3210,
			3193,
		3174,
			3153,		
		784 * 4,//		  3136
		779 * 4,//0.5   
		774 * 4,//		
		769 * 4,//1.5   
		765 * 4,//		
		760 * 4,//2.5   
		755 * 4,//		
		750 * 4,//3.5   
		745 * 4,//		
		740 * 4,//4.5   
		735 * 4,//	   
		729 * 4,//5.5   
		724 * 4,//	   
		719 * 4,//6.5   
		714 * 4,//	   
		708 * 4,//7.5   
		703 * 4,//	   
		697 * 4,//8.5   
		692 * 4,//	   
		686 * 4,//9.5   
		681 * 4,//	   
		675 * 4,//10.5  
		670 * 4,//	   
		664 * 4,//11.5  
		659 * 4,//	   
		653 * 4,//12.5  
		648 * 4,//	   
		642 * 4,//13.5  
		637 * 4,//	   
		631 * 4,//14.5  
		625 * 4,//	   
		619 * 4,//15.5  
		614 * 4,//	   
		608 * 4,//16.5  
		603 * 4,//	   
		597 * 4,//17.5  
		591 * 4,//	   
		585 * 4,//18.5  
		580 * 4,//	   
		574 * 4,//19.5  
		569 * 4,//	   
		563 * 4,//20.5  
		557 * 4,//	   
		551 * 4,//21.5  
		545 * 4,//	   
		539 * 4,//22.5  
		534 * 4,//	   
		528 * 4,//23.5  
		522 * 4,//	   
		517 * 4,//24.5  
		512 * 4,//	   
		506 * 4,//25.5  
		500 * 4,//	   
		494 * 4,//26.5  
		489 * 4,//	   
		483 * 4,//27.5  
		478 * 4,//	   
		472 * 4,//28.5  
		467 * 4,//	   
		461 * 4,//29.5  
		456 * 4,//	   
		451 * 4,//30.5  
		446 * 4,//	   
		440 * 4,//31.5  
		435 * 4,//	   
		430 * 4,//32.5  
		425 * 4,//	   
		419 * 4,//33.5  
		414 * 4,//	   
		409 * 4,//34.5  
		404 * 4,//	   
		399 * 4,//35.5  
		394 * 4,//	   
		389 * 4,//36.5  
		384 * 4,//	   
		379 * 4,//37.5  
		374 * 4,//	   
		369 * 4,//38.5  
		365 * 4,//	   
		360 * 4,//39.5  
		355 * 4,	//    
		350 * 4,//40.5  
		346 * 4,//	   
		341 * 4,//41.5  
		337 * 4,//	   
		332 * 4,//42.5  
		328 * 4,//	   
		323 * 4,//43.5  
		319 * 4,//	   
		314 * 4,//44.5  
		310 * 4,//	   
		306 * 4,//45.5  
		302 * 4,//	   
		298 * 4,//46.5  
		294 * 4,//	   
		290 * 4,//47.5  
		286 * 4,//	   
		282 * 4,//48.5  
		278 * 4,//	   
		274 * 4,//49.5  
		270 * 4,//	   
		266 * 4,//50.5  
		263 * 4,//	   
		259 * 4,//51.5  
		255 * 4,//	   
		251 * 4,//52.5  
		248 * 4,//	   
		244 * 4,//53.5  
		241 * 4,//	   
		237 * 4,//54.5  
		234 * 4,//	   
		231 * 4,//55.5  
		228 * 4,//	   
		224 * 4,//56.5  
		221 * 4,//	   
		218 * 4,//57.5  
		215 * 4,//	   
		212 * 4,//58.5  
		209 * 4,//	   
		206 * 4,//59.5  
		203 * 4,//	
};


void Taxis(unsigned char tax[])
{
	unsigned char min,tem,i,j;
	for(i = 0;i < 7-1;i ++)
    {
	     min = i; 
	     for(j = i+1;j < 7;j ++)
	     if(tax[min]>tax[j]) 
	     		min = j;
	     tem = tax[i];
	     tax[i] = tax[min];
	     tax[min] = tem;
    }
}


void ReadTemperature()
{
		unsigned int uiADCValue;
		unsigned char temperature;
		unsigned char p;  
	   static unsigned char Adc_count;
	   static unsigned char Read_Temper_Add;
		static unsigned char Read_Temper_Sub;
		static unsigned char Adc_Value[8];
		
		
		ADCCON |= 0X40;   //开始ADC转换
		while(!ucTimeF.Bits.F_AdcFlag);	         		 //等待 ADC转换完成;
		ucTimeF.Bits.F_AdcFlag = 0;
		uiADCValue = (ADCVH<<4)+(ADCVL>>4);
		
		//scRoomTemper = uiADCValue;
		
		for(p = 0;p < 140;p ++)
	    {
	    	if(uiADCValue >= Temperature_Tbl[p])
	    	break;
	    }
    
    	if(p)     p--;					//温度调整
//    	if(p)     p--;
//    	if(p)     p--;

    
	    Adc_Value[Adc_count] = p;
	    
	    
	    Adc_count ++;
	    Adc_count &= 0x07;
	    
	    if(Adc_count == 0x07)
	    	{
	    		Taxis(&Adc_Value[0]);
	    		
	    		temperature = ucRoomTemper;
	    		
	    		if((temperature == 0) || (temperature == 140))		//75
	    			temperature = Adc_Value[4];	//第一次上电
	    			

	    		if(Adc_Value[4] > temperature)
	    			{
	    				Read_Temper_Add += 1;
						if(Read_Temper_Add > 10)
							{
								if(Adc_Value[4] > (temperature + 5)) 
									temperature = Adc_Value[4];
								else 
									{
										temperature += 1;
										Read_Temper_Add = 5;
									}
							}	
	    			}
	    			
	    		if(Adc_Value[4] < temperature)
					{
						Read_Temper_Sub += 1;
						if(Read_Temper_Sub > 10)
						{
							if(Adc_Value[4] < (temperature-5)) 
								temperature = Adc_Value[4];
							else 
							{
								temperature -= 1;
								Read_Temper_Sub = 5;
							}
						}
					}
						
				if(Adc_Value[4] == temperature)
					{
						Read_Temper_Add = 0;
						Read_Temper_Sub = 0;
					}

				ucRoomTemper = temperature;
				if(ucRoomTemper<20)
					ucRoomTemper=20;
			}

}