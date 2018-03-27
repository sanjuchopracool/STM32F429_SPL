How to flash with stlink
st-flash write file.bin 0x8000000

Jlink

How to launch GDB Server
JLinkGDBServer -select USB -device STM32F429ZI -If SWD -speed auto -port 4242

Jlink
by default halt the processor
so we should quit the Jlink

Command for debugging
load
---------------------------------


CLOCK CONFIGURATION

SYSCLK      180MHz
HCLK        180MHz(RCC->CFGR |= RCC_CFGR_HPRE_DIV1)
PCLK1       45MHz
PLCLK2      90MHz
APB2_TIMER  180MHz
APB2_PERI   90MHz
APB1_TIMER  90MHz
APB1_PERI   45MHz
SYS_TICK SOURCE is selected to processor clock by cmsis(SysTick->CTRL)
