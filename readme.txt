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

