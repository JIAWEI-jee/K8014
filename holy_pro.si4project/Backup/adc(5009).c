#include "adc.h"
#include "delay.h"
#include "uart.h"

void adc_init(void)
{
    P0M1 = 0x01;		//P01����Ϊģ������
    P2M3 = 0x01;        //P00����Ϊģ������
	ADCC0 = 0x80;		//��ADCת����Դ			00 VDD
//											01 �ڲ�4V
//											10 �ڲ�3V
//											11 �ڲ�2V
//1�� �ڲ��ο���ѹѡ��Ϊ 2V ʱ�� VDD ��ѹ����� 2.7V��
//    �ڲ��ο���ѹѡ��Ϊ 3V ʱ�� VDD ��ѹ����� 3.5V�� 
//    �ڲ��ο���ѹѡ��Ϊ 4V ʱ�� VDD ��ѹ�����4.5V��
//2�� ϵͳ�������ģʽǰ�����齫 ADC �ο���ѹѡ��� VDD�����Խ�һ������ϵͳ���ġ�
	delay_us(20);						//��ʱ20us��ȷ��ADCϵͳ�ȶ�

//	ADCC1 = 0x01;						//ѡ��ADCͨ��1
	ADCC2 = 0x49;						//ת�����12λ���ݣ������Ҷ��룬ADCʱ��4��Ƶ 4M/4 = 1MHz
}

u16 get_adc_val(void)
{
	u16 adc_val = 0;
	ADCC1 = 0x01;
	
	ADCC0 |= 0x40;					//����ADCת��
	while(!(ADCC0&0x20));			//�ȴ�ADCת������
	ADCC0 &=~ 0x20;					//�����־λ
	adc_val = ADCR;					//��ȡADC��ֵ
 //	ADCC1 = 0x01;         

     ADC_printf("P01_ADC = %d \r\n",adc_val);
	return adc_val;
}

u16 get_adc_val_ch(void)
{
	u16 adc_val_ch = 0;
	ADCC1 = 0x0b;                     //ѡ��ADCͨ��11
	
	ADCC0 |= 0x40;					//����ADCת��
	while(!(ADCC0&0x20));			//�ȴ�ADCת������
	ADCC0 &=~ 0x20;					//�����־λ
	adc_val_ch = ADCR;					//��ȡADC��ֵ

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
    tep = tep*5000/4096;	//�Ŵ�1000�� ����С�������λ 4096
	 ADC_printf("P01 = %d V \r\n",tep);
    return tep;
}





