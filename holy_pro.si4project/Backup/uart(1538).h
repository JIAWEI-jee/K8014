#ifndef __UART_H__
#define __UART_H__
#include "HC89F303.h"
#include "stdio.h"//printf

#define _DEBUG_PRINTF_ 1

#if _DEBUG_PRINTF_
	#define	gm_printf printf
#else
	#define	gm_printf (...)
#endif


#define KEY_DEBUG_PRINT   1

#if KEY_DEBUG_PRINT
	#define	gm_printf printf
#else
	#define	gm_printf 
#endif

void uart_init(void);




#endif


