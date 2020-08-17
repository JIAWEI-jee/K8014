#ifndef __TIMER_H__
#define __TIMER_H__
#include "HC89F303.h"

#define overrang_time   (60*60*2)
#define Heat_Value      (60*4)

extern u8 over_rang_time_std;
extern u8 Gap_protect_std;
extern u8 On_stay;
extern u8 Heat_start_std;
extern u8  Open_Heat_Value;

void time0_init(void);
void set_time_sec_val(u16 sec);
void set_time_sec(void);
#endif

