#ifndef __F_GPIO_H__
#define __F_GPIO_H__

#include "stm32f10x.h"                  // Device header


#define GPIO_PA_00  (u32)0x08000001
#define GPIO_PA_01  (u32)0x08000002
#define GPIO_PA_02  (u32)0x08000004
#define GPIO_PA_03  (u32)0x08000008
#define GPIO_PA_04  (u32)0x08000010
#define GPIO_PA_05  (u32)0x08000020
#define GPIO_PA_06  (u32)0x08000040
#define GPIO_PA_07  (u32)0x08000080
#define GPIO_PA_08  (u32)0x08000100
#define GPIO_PA_09  (u32)0x08000200
#define GPIO_PA_10  (u32)0x08000400
#define GPIO_PA_11  (u32)0x08000800
#define GPIO_PA_12  (u32)0x08001000
#define GPIO_PA_13  (u32)0x08002000
#define GPIO_PA_14  (u32)0x08004000
#define GPIO_PA_15  (u32)0x08008000

#define GPIO_PB_00  (u32)0x0C000001
#define GPIO_PB_01  (u32)0x0C000002
#define GPIO_PB_02  (u32)0x0C000004
#define GPIO_PB_03  (u32)0x0C000008
#define GPIO_PB_04  (u32)0x0C000010
#define GPIO_PB_05  (u32)0x0C000020
#define GPIO_PB_06  (u32)0x0C000040
#define GPIO_PB_07  (u32)0x0C000080
#define GPIO_PB_08  (u32)0x0C000100
#define GPIO_PB_09  (u32)0x0C000200
#define GPIO_PB_10  (u32)0x0C000400
#define GPIO_PB_11  (u32)0x0C000800
#define GPIO_PB_12  (u32)0x0C001000
#define GPIO_PB_13  (u32)0x0C002000
#define GPIO_PB_14  (u32)0x0C004000
#define GPIO_PB_15  (u32)0x0C008000

#define GPIO_PC_00  (u32)0x10000001
#define GPIO_PC_01  (u32)0x10000002
#define GPIO_PC_02  (u32)0x10000004
#define GPIO_PC_03  (u32)0x10000008
#define GPIO_PC_04  (u32)0x10000010
#define GPIO_PC_05  (u32)0x10000020
#define GPIO_PC_06  (u32)0x10000040
#define GPIO_PC_07  (u32)0x10000080
#define GPIO_PC_08  (u32)0x10000100
#define GPIO_PC_09  (u32)0x10000200
#define GPIO_PC_10  (u32)0x10000400
#define GPIO_PC_11  (u32)0x10000800
#define GPIO_PC_12  (u32)0x10001000
#define GPIO_PC_13  (u32)0x10002000
#define GPIO_PC_14  (u32)0x10004000
#define GPIO_PC_15  (u32)0x10008000

#define GPIO_PD_00  (u32)0x14000001
#define GPIO_PD_01  (u32)0x14000002
#define GPIO_PD_02  (u32)0x14000004
#define GPIO_PD_03  (u32)0x14000008
#define GPIO_PD_04  (u32)0x14000010
#define GPIO_PD_05  (u32)0x14000020
#define GPIO_PD_06  (u32)0x14000040
#define GPIO_PD_07  (u32)0x14000080
#define GPIO_PD_08  (u32)0x14000100
#define GPIO_PD_09  (u32)0x14000200
#define GPIO_PD_10  (u32)0x14000400
#define GPIO_PD_11  (u32)0x14000800
#define GPIO_PD_12  (u32)0x14001000
#define GPIO_PD_13  (u32)0x14002000
#define GPIO_PD_14  (u32)0x14004000
#define GPIO_PD_15  (u32)0x14008000

#define GPIO_PE_00  (u32)0x18000001
#define GPIO_PE_01  (u32)0x18000002
#define GPIO_PE_02  (u32)0x18000004
#define GPIO_PE_03  (u32)0x18000008
#define GPIO_PE_04  (u32)0x18000010
#define GPIO_PE_05  (u32)0x18000020
#define GPIO_PE_06  (u32)0x18000040
#define GPIO_PE_07  (u32)0x18000080
#define GPIO_PE_08  (u32)0x18000100
#define GPIO_PE_09  (u32)0x18000200
#define GPIO_PE_10  (u32)0x18000400
#define GPIO_PE_11  (u32)0x18000800
#define GPIO_PE_12  (u32)0x18001000
#define GPIO_PE_13  (u32)0x18002000
#define GPIO_PE_14  (u32)0x18004000
#define GPIO_PE_15  (u32)0x18008000

#define GPIO_PF_00  (u32)0x1C000001
#define GPIO_PF_01  (u32)0x1C000002
#define GPIO_PF_02  (u32)0x1C000004
#define GPIO_PF_03  (u32)0x1C000008
#define GPIO_PF_04  (u32)0x1C000010
#define GPIO_PF_05  (u32)0x1C000020
#define GPIO_PF_06  (u32)0x1C000040
#define GPIO_PF_07  (u32)0x1C000080
#define GPIO_PF_08  (u32)0x1C000100
#define GPIO_PF_09  (u32)0x1C000200
#define GPIO_PF_10  (u32)0x1C000400
#define GPIO_PF_11  (u32)0x1C000800
#define GPIO_PF_12  (u32)0x1C001000
#define GPIO_PF_13  (u32)0x1C002000
#define GPIO_PF_14  (u32)0x1C004000
#define GPIO_PF_15  (u32)0x1C008000

#define GPIO_PG_00  (u32)0x20000001
#define GPIO_PG_01  (u32)0x20000002
#define GPIO_PG_02  (u32)0x20000004
#define GPIO_PG_03  (u32)0x20000008
#define GPIO_PG_04  (u32)0x20000010
#define GPIO_PG_05  (u32)0x20000020
#define GPIO_PG_06  (u32)0x20000040
#define GPIO_PG_07  (u32)0x20000080
#define GPIO_PG_08  (u32)0x20000100
#define GPIO_PG_09  (u32)0x20000200
#define GPIO_PG_10  (u32)0x20000400
#define GPIO_PG_11  (u32)0x20000800
#define GPIO_PG_12  (u32)0x20001000
#define GPIO_PG_13  (u32)0x20002000
#define GPIO_PG_14  (u32)0x20004000
#define GPIO_PG_15  (u32)0x20008000

#define GPIO_PORT(p) ((GPIO_TypeDef *)(APB2PERIPH_BASE + ((p >> 16) & 0x0000FFFF)))
#define GPIO_PIN(p)  ((u16)(p & 0x0000FFFF))













void f_gpio_init(u32 pins,GPIOMode_TypeDef mode);
void f_gpio_ouput(GPIO_TypeDef* port,u16 value);
void f_gpio_set(u32 pins);
void f_gpio_reset(u32 pins);
void f_gpio_toggle(u32 pins);
u16 f_gpio_read(u32 pins);
u16 f_gpio_readoutput(u32 pins);

#endif

