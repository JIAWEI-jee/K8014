#ifndef __KEY_H__
#define __KEY_H__
#include "HC89F303.h"

#define KEY_1 	P2_5
#define KEY_2 	P2_6
#define KEY_3 	P3_5

#define KEY_1_PRES 	1	//KEY1����
#define KEY_2_PRES	2	//KEY2����
#define KEY_3_PRES	3	//KEY3����

void key_init(void);
u8 key_scan(void);

#endif

