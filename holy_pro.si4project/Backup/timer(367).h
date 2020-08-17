#ifndef __TIMER_H__
#define __TIMER_H__
#include "HC89F303.h"

#define overrang_time   (60*60*2)
extern u8 over_rang_time_std;
extern u8 Gap_protect_std;
extern u8 On_stay;

void time0_init(void);
void set_time_sec_val(u16 sec);
void set_time_sec(void);
#endif

