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
#define SOFT_VER "1.03.00"

u16 adc_cnt = 0;

void Set_Temp ( u16 temp );
void Controll_Heat ( u16 temp_set,u16 temp_now );
void Protect ( void );
void Error ( void );

void device_init ( void )
{
	/************************************系统初始化****************************************/
	// CLKSWR = 0x51;						//选择内部高频RC为系统时钟，内部高频RC 2分频，Fosc=16MHz
	CLKSWR = 0x53;  //8分频 32/8 = 4M
	CLKDIV = 0x01;						//Fosc 1分频得到Fcpu，Fcpu=16MHz
	P0M2 = 0x10; 						//P02设置为非施密特数字输入
}


static void key_handle ( void )
{
	u8 key_val = 0;

	key_val = key_scan();
	if ( key_val == KEY_1_PRES )
	{
		KEY_printf ( " key_scan\r\n" );
		if ( get_device_state() == ON )
		{
			set_device_state ( OFF );
			ht1621_all_clear();
			set_pwm ( 0 );
		}
		else
		{
			set_device_state ( ON );
			set_time_sec();
			if ( flash_info.gap > GAP_3 )
			{
				if ( flash_info.timer == TIMER_ON )
				{
					On_stay = 2;
				}
				else if ( flash_info.timer > TIMER_2H )
				{
					Gap_protect_std = 2;
				}
			}
			Heat_start_std = 1;
			Open_Heat_Value = corrected_value_warm_temp;
			lcd_display_gap ( flash_info.gap );
			lcd_display_time ( flash_info.timer );

		}
	}
	else if ( get_device_state() == ON )
	{



		if ( key_val == KEY_2_PRES ) //档位
		{

			KEY_printf ( " KEY_2_PRES\r\n" );
			if ( flash_info.gap < GAP_9 )
			{
				flash_info.gap++;

			}
			else
			{
				flash_info.gap = GAP_WARM;
			}
			if ( flash_info.gap > GAP_3 )
			{
				if ( flash_info.timer == TIMER_ON )
				{
					On_stay = 2;
				}
				else if ( flash_info.timer > TIMER_2H )
				{
					Gap_protect_std = 2;
				}
			}
			Heat_start_std = 1;
			Open_Heat_Value = corrected_value_warm_temp	;
			lcd_display_gap ( flash_info.gap );
			//set_time_sec();
			flah_save_data();
		}
		else if ( key_val == KEY_3_PRES ) //定时
		{

			if ( flash_info.timer < 0x05 )
			{
				flash_info.timer++;
			}
			else
			{
				flash_info.timer = TIMER_ON;
			}

//			KEY_printf ( "timer:%d \r\n", ( u16 ) flash_info.timer );
			lcd_display_time ( flash_info.timer );
			set_time_sec();
			flah_save_data();
		}
	}
}



u16 temp_calc ( u16 val )
{
	u8 i = 0;
	float u1;

	u8 basi_tmp = 50;
	u8 k = 10;


	if ( val == 0 )
	{
		return 0;
	}

	u1 = ( float ) val/1000;
//	gm_printf ( "real_v = %f  \r\n",u1 );
	u1 = 2550/u1-510;
// gm_printf ( "R = %f  \r\n",u1 );
	u1 = u1 / 8;
	// gm_printf("average R = %f \r\n",u1);
	u1 = u1 / 91.0624;
	//gm_printf("cmp = %f \r\n",u1);
	if ( u1*91.0624 > 91.0624 )
	{
		while ( 1 )
		{
			u1 = u1 / ( 1 + 0.0048 );
			i++;
			if ( u1 <= 1 )
			{
				break;
			}

			// gm_printf("b = %f \r\n",u1);
		}
		//gm_printf("i_max = %d \r\n",i);
		basi_tmp = basi_tmp + i;
	}
	else
	{
		while ( 1 )
		{
			u1 = u1 / ( 1 - 0.0048 );
			i++;
			if ( u1 >= 1 )
			{
				break;
			}

			//gm_printf("b = %f \r\n",u1);
		}
		//	gm_printf("i_min = %d \r\n",i);
		basi_tmp = basi_tmp - i;
	}
	// gm_printf("temp:%d \r\n",basi_tmp);
	return ( basi_tmp-Correct_Value );
}



void temperature_handle ( void )
{
	u16 temp = 0;
	u16 adc_val = 0;
	static u8 error_std = 0;
	// if(get_device_state() == ON)
	//{
	adc_cnt++;
	if ( adc_cnt > 12000 )
	{
		adc_cnt = 0;
		adc_val = get_voltage_val();
		//KEY_printf ( "adv %d \r\n",adc_val );  //pjw set
		temp = temp_calc ( adc_val );
		//KEY_printf ( "temp val:%d \r\n",temp );

		if ( adc_val >0 )
		{
			if ( get_device_state() == ON )
			{
				lcd_display_time ( flash_info.timer );
				lcd_display_gap ( flash_info.gap );
				Set_Temp ( temp );
			}
			else
			{
				ht1621_all_clear();
			}
		}
		else
		{
			ht1621_all_clear();
			set_pwm ( 0 );
			error_std = Error_STD;
			lcd_display_gap ( error_std );

		}
//			if(temp >= 80)
//			{
//				set_pwm(0);
//			}
//			else if(temp < 77)
//			{
//				set_pwm(10);
//			}


	}


}




void main ( void )
{
	device_init();

	uart_init();
	adc_init();
	EA = 1;//开启总中断
	key_init();
	time0_init();
	flash_init();
	pwm_init ( 200 );
	init_lcd_ht1621b();
	delay_ms ( 800 );
	ht1621_all_clear();
	wdt_init ( 2 );
	set_pwm ( 0 );
	gm_printf ( "\r\n==================================\r\n" );
	gm_printf ( "sku:K%d \r\n", ( u16 ) SKU );
	gm_printf ( "soft version:%s \r\n",SOFT_VER );
	gm_printf ( "gap %d \r\n", ( u16 ) flash_info.gap );      //挡位
	gm_printf ( "timer %d \r\n", ( u16 ) flash_info.timer );  // 时间长度
	gm_printf ( "==================================\r\n" );

	while ( 1 )
	{
		key_handle();
		temperature_handle();
		Protect();
		//uart_handle();
		clear_wdt();

	}
}


void Controll_Heat ( u16 temp_set,u16 temp_now )
{
	if ( temp_now >=  temp_set   )
	{
		//KEY_printf ( "close_heat \r\n");
		set_pwm ( 0 ); // 关闭加热丝
	}
	else if ( temp_now <= ( temp_set - Open_Heat_Value ) )
	{
		//KEY_printf ( "open_heat \r\n");
		set_pwm ( 10 ); //打开加热丝
	}
}

void Set_Temp ( u16 temp )
{


	switch ( flash_info.gap )
	{
		case GAP_WARM:
			Controll_Heat ( GAP_WARM_temp,temp );
			break;
		case GAP_1:
			Controll_Heat ( GAP_1_temp,temp );
			break;
		case GAP_2:
			Controll_Heat ( GAP_2_temp,temp );
			break;
		case GAP_3:
			Controll_Heat ( GAP_3_temp,temp );
			break;
		case GAP_4:
			Controll_Heat ( GAP_4_temp,temp );
			break;
		case GAP_5:
			Controll_Heat ( GAP_5_temp,temp );
			break;
		case GAP_6:
			Controll_Heat ( GAP_6_temp,temp );
			break;
		case GAP_7:
			Controll_Heat ( GAP_7_temp,temp );
			break;
		case GAP_8:
			Controll_Heat ( GAP_8_temp,temp );
			break;
		case GAP_9:
			Controll_Heat ( GAP_9_temp,temp );
			break;

	}
}




void Protect ( void )
{
	if ( over_rang_time_std == 1 )
	{
		if ( flash_info.gap > GAP_3 )
		{
			flash_info.gap = GAP_3;
			lcd_display_gap ( flash_info.gap );
			flah_save_data();
			over_rang_time_std = 0;
		}
	}
}
void Error ( void )
{
	static u8 error_std = 0;
	if ( Insert_Test == 1 )
	{
		error_std = Error_STD;
		lcd_display_gap ( error_std );
	}
}

