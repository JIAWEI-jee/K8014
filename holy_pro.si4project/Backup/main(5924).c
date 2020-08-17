#define	ALLOCATE_EXTERN
#include "HC89F303.h"
#include "intrins.h"//nop
#include "uart.h"
#include "delay.h"
#include "key.h"
#include "flash.h"
#include "timer.h"
#include "pwm.h"
#include "lcd_display.h"
#include "adc.h"
#include "wdt.h"

#define SKU 9011      
#define SOFT_VER "1.00.00"

u16 adc_cnt = 0;

void device_init(void)
{
	/************************************系统初始化****************************************/
	// CLKSWR = 0x51;						//选择内部高频RC为系统时钟，内部高频RC 2分频，Fosc=16MHz
	CLKSWR = 0x53;  //8分频 32/8 = 4M
	CLKDIV = 0x01;						//Fosc 1分频得到Fcpu，Fcpu=16MHz 
	P0M2 = 0x10; 						//P02设置为非施密特数字输入
}

	
static void key_handle(void)
{
	u8 key_val = 0;

	key_val = key_scan();
	if(key_val == KEY_1_PRES)
	{
	  KEY_printf(" key_scan\r\n");
		if(get_device_state() == ON)
		{
			set_device_state(OFF);
			ht1621_all_clear();
			
		}
		else
		{
			set_device_state(ON);
			set_time_sec();
			lcd_display_gap(flash_info.gap);
			lcd_display_time(flash_info.timer);
           
		}
	}
		else if(get_device_state() == ON)
	{

//	 KEY_printf(" device_state=%d\r\n",device_state);
	 
		if(key_val == KEY_2_PRES)//档位
		{
		
		KEY_printf(" KEY_2_PRES\r\n");
			if(flash_info.gap < GAP_5)
				flash_info.gap++;
			else 
				flash_info.gap = GAP_WARM;
			lcd_display_gap(flash_info.gap);
			flah_save_data();		
		}
		else if(key_val == KEY_3_PRES)//定时
		{
	
			if(flash_info.timer < TIMER_8H)
				flash_info.timer++;
			else 
				flash_info.timer = TIMER_ON;
			KEY_printf("timer:%d \r\n",(u16)flash_info.timer);
			lcd_display_time(flash_info.timer);
			set_time_sec();
			flah_save_data();		
		}
	}
}



u16 temp_calc(u16 val)
{
	u8 i = 0;
	double u1 = (double)val;
	//float b = 0;
	u8 basi_tmp = 50;
	u8 k = 10;
	//float y = 0;

	if(val == 0)return 0;

	u1 = u1/1000;
	// gm_printf("u1 = %f  \r\n",u1);
	u1 = 2550/u1-510;
	// gm_printf("y1 = %f  \r\n",y);
	u1 = u1 /10;
	// gm_printf("y2 = %f \r\n",y);
	u1 = u1 / 91.0624;
	// gm_printf("y3 = %f \r\n",y);
	if(u1*91.0624 > 91.0624)
	{
		while(1)
		{
			u1 = u1 / (1 + 0.0048);
			i++;
			if(u1 <= 1)break;
			// gm_printf("b = %f \r\n",b);
		}
		basi_tmp += i; 
	}
	else
	{
		while(1)
		{
			u1 = u1 / (1 - 0.0048);
			i++;
			if(u1 >= 1)break;
			// gm_printf("b = %f \r\n",b);
		}
		basi_tmp -= i; 
	}
	// gm_printf("temp:%d \r\n",basi_tmp);
	return basi_tmp;
}



void temperature_handle(void)
{
	u16 temp = 0;
	u16 adc_val = 0;
	// if(get_device_state() == ON)
	//{
		adc_cnt++;
		if(adc_cnt > 20000)
		{
			adc_cnt = 0;
			adc_val = get_voltage_val();
			KEY_printf("adv %d \r\n",adc_val);  //pjw set 
			temp = temp_calc(adc_val);
			KEY_printf("temp val:%d \r\n",temp);	

			if(temp >= 80)
			{
				set_pwm(0);
			}
			else if(temp < 77)
			{
				set_pwm(10);
			}

		}
	//}

}




void main(void)
{
	device_init();

	uart_init();
	adc_init();
	EA = 1;//开启总中断
	key_init();
	time0_init();
	flash_init(); 
	pwm_init(200);
	init_lcd_ht1621b();
	// delay_ms(1000);
	ht1621_all_clear();
	wdt_init(2);
	
	gm_printf("\r\n==================================\r\n");
	gm_printf("sku:K%d \r\n",(u16)SKU);
	gm_printf("soft version:%s \r\n",SOFT_VER);
	gm_printf("gap %d \r\n",(u16)flash_info.gap);
	gm_printf("timer %d \r\n",(u16)flash_info.timer);
	gm_printf("==================================\r\n");
	
	while(1)
	{
		key_handle();
		temperature_handle();
		//uart_handle();
		clear_wdt();
		
	}
}

