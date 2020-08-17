#include "key.h"
#include "delay.h"

#define KEY_PRESS_TIME 700

void key_init(void)
{
	P3M5 = 0x61;                        //P35����Ϊ��ʩ�������ִ���������
	P2M5 = 0x61;                        //P25����Ϊ��ʩ�������ִ���������
	P2M6 = 0x61;                        //P26����Ϊ��ʩ�������ִ���������
}

//mode:0,��֧��������;1,֧��������;
u8 key_scan(void)
{
	static u8 key_up = 1;//�������ɿ���־
	static u16 key_pres_time = 0;

	// if(mode)key_up=1;  //֧������	
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
		delay_ms(50);//ȥ���� 
		key_up=0;
		if(KEY_2 == 0)
			return KEY_2_PRES;
		else if(KEY_3 == 0)
			return KEY_3_PRES;
	}else if(KEY_2 == 1 && KEY_3 == 1)
		key_up=1; 	

	if(KEY_1 == 1 && key_pres_time > 1)
		key_pres_time = 0;
 	return 0;// �ް�������
}