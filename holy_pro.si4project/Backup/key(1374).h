#ifndef __KEY_H__
#define __KEY_H__
#include "HC89F303.h"

#define KEY_1 	P2_5
#define KEY_2 	P2_6
#define KEY_3 	P3_5

#define KEY_1_PRES 	1	//KEY1╟╢об
#define KEY_2_PRES	2	//KEY2╟╢об
#define KEY_3_PRES	3	//KEY3╟╢об

#define KEY_STATE_0         0       // О©╫О©╫О©╫О©╫в╢л╛н╩
#define KEY_STATE_1         1
#define KEY_STATE_2         2
#define KEY_STATE_3         3
 
#define LONG_KEY_TIME       200    //О©╫О©╫О©╫О©╫О©╫О©╫3О©╫О©╫й╠О©╫О©╫
#define SINGLE_KEY_TIME     3      // О©╫л╟О©╫О©╫О©╫О©╫О©╫О©╫О©╫й╠О©╫О©╫
 
#define N_KEY         0             // О©╫О©╫в╢л╛



void key_init(void);
u8 key_scan(void);

#endif

