#include <12F1840.h>
#device ADC = 10
#use delay(internal = 8MHz, restart_wdt)
#use i2c(Master, Fast, sda = PIN_A2, scl = PIN_A1)
#use rs232(rcv = PIN_A4, xmit = PIN_A0, bits = 8, PARITY = N)
#FUSES NOWDT // No Watch Dog Timer
