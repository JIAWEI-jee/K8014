#include "key.h"
#include "delay.h"

#define KEY_PRESS_TIME 700

void key_init(void)
{
	P3M5 = 0x61;                        //P35设置为非施密特数字带上拉输入
	P2M5 = 0x61;                        //P25设置为非施密特数字带上拉输入
	P2M6 = 0x61;                        //P26设置为非施密特数字带上拉输入
}

//mode:0,不支持连续按;1,支持连续按;
u8 key_scan(void)
{
	static u8 key_up = 1;//按键按松开标志
	static u16 key_pres_time = 0;

	// if(mode)key_up=1;  //支持连按	
	if(KEY_1 == 0)
	{
		key_pres_time++;
		if(key_pres_time >= KEY_PRESS_TIME)
		{
			key_pres_time = 0;
			return KEY_1_PRES;
		}
		delay_ms(1);
	}
	else if(key_up && (KEY_2 == 0 || KEY_3 == 0))
	{
		delay_ms(50);//去抖动 
		key_up=0;
		if(KEY_2 == 0)
			return KEY_2_PRES;
		else if(KEY_3 == 0)
			return KEY_3_PRES;
	}else if(KEY_2 == 1 && KEY_3 == 1)
		key_up=1; 	

	if(KEY_1 == 1 && key_pres_time > 1)
		key_pres_time = 0;
 	return 0;// 无按键按下
}