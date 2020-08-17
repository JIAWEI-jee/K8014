#include "adc.h"
#include "delay.h"
#include "uart.h"

void adc_init(void)
{
    P0M1 = 0x01;		//P01设置为模拟输入
    P2M3 = 0x01;        //P00设置为模拟输入
	ADCC0 = 0x80;		//打开ADC转换电源			00 VDD
//											01 内部4V
//											10 内部3V
//											11 内部2V
//1、 内部参考电压选择为 2V 时， VDD 电压须高于 2.7V；
//    内部参考电压选择为 3V 时， VDD 电压须高于 3.5V； 
//    内部参考电压选择为 4V 时， VDD 电压须高于4.5V。
//2、 系统进入掉电模式前，建议将 ADC 参考电压选择非 VDD，可以进一步降低系统功耗。
	delay_us(20);						//延时20us，确保ADC系统稳定

//	ADCC1 = 0x01;						//选择ADC通道1
	ADCC2 = 0x49;						//转换结果12位数据，数据右对齐，ADC时钟4分频 4M/4 = 1MHz
}

u16 get_adc_val(void)
{
	u16 adc_val = 0;
	ADCC1 = 0x01;
	
	ADCC0 |= 0x40;					//启动ADC转换
	while(!(ADCC0&0x20));			//等待ADC转换结束
	ADCC0 &=~ 0x20;					//清除标志位
	adc_val = ADCR;					//获取ADC的值
 //	ADCC1 = 0x01;         

     ADC_printf("P01_ADC = %d \r\n",adc_val);
	return adc_val;
}

u16 get_adc_val_ch(void)
{
	u16 adc_val_ch = 0;
	ADCC1 = 0x0b;                     //选择ADC通道11
	
	ADCC0 |= 0x40;					//启动ADC转换
	while(!(ADCC0&0x20));			//等待ADC转换结束
	ADCC0 &=~ 0x20;					//清除标志位
	adc_val_ch = ADCR;					//获取ADC的值

    ADC_printf("P23_ADC = %d \r\n",adc_val_ch);
	return adc_val_ch;
}

u16 get_voltage_val(void)
{
	u32 tep = 0,temp_ch = 0;
	u8 i = 0;
    temp_ch = get_adc_val_ch();
	 temp_ch = temp_ch*5000/4096;
	 ADC_printf("P23 = %d V \r\n",temp_ch);
	ADC_printf("\r\n");
//	for(i = 0;i < 5;i++)
//	{
//		tep += get_adc_val();
//	}
//    tep /= 5;
	tep = get_adc_val();
    tep = tep*5000/4096;	//放大1000倍 保存小数点后三位 4096
	 ADC_printf("P01 = %d V \r\n",tep);
    return tep;
}





