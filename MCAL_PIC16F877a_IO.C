#include "stdtypes.h"
#include "MCAL_PIC16F8.h"
#include <stdio.h>
#include <xc.h>
volatile GPIOXX GPIOx;
volatile GPIOXX* MCAL_PIC_getgpio(u8_t PORTxx){
switch (PORTxx){
case PORTa :
	GPIOx.PORTX=(volatile u8_t *)(0x0005);
	GPIOx.TRISX=(volatile u8_t *)(0x0085);
	return &GPIOx;break;
case PORTb :
	GPIOx.PORTX=(volatile u8_t *)(0x0006);
	GPIOx.TRISX=(volatile u8_t *)(0x0086);
	return &GPIOx;break;
case PORTc :
	GPIOx.PORTX=(volatile u8_t *)(0x0007);
	GPIOx.TRISX=(volatile u8_t *)(0x0087);
	return &GPIOx;break;
case PORTd :
	GPIOx.PORTX=(volatile u8_t *)(0x0008);
	GPIOx.TRISX=(volatile u8_t *)(0x0088);
	return &GPIOx;break;
case PORTe :
	GPIOx.PORTX=(volatile u8_t *)(0x0009);
	GPIOx.TRISX=(volatile u8_t *)(0x0089);
	return &GPIOx;break;
default:
    return NULL;}

return 0;}
void MCAL_PIC_Init_Port(u8_t PORTxx){
if ((PORTxx==PORTd) || (PORTxx==PORTe)) {ADCON1=0xff;}
return;}
void MCAL_PIC_SetBitDir( u8_t  PORTxx , u8_t bitx , u8_t dir){
	GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
	if (dir==INPUT){set_bit(*(gpio->TRISX),bitx);}
			else {clr_bit(*(gpio->TRISX),bitx);}
return;}
void MCAL_PIC_SetBitsDir( u8_t  PORTxx , u8_t bitx , u8_t bit_counuter , u8_t dir){
int i ; for (i=0 ; i<bit_counuter;i++){MCAL_PIC_SetBitDir(PORTxx,bitx,dir); bitx++;}return ;}
void MCAL_PIC_SetPORTDir(u8_t  PORTxx ,u8_t dir){
	GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
	if (dir==INPUT){*(MCAL_PIC_getgpio(PORTxx)->TRISX)=PORT_IN;}
			else {*(MCAL_PIC_getgpio(PORTxx)->TRISX)=PORT_OUT;}
}
void MCAL_PIC_SetPORTDirVal(u8_t  PORTxx ,u8_t val){
	GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
	*(MCAL_PIC_getgpio(PORTxx)->TRISX)=val;
}
void MCAL_PIC_SetBitVal( u8_t  PORTxx , u8_t bitx , u8_t val){
	GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
    if (val==1){set_bit(*(gpio->PORTX),bitx);}
    else {clr_bit(*(gpio->PORTX),bitx);}
return;}
void MCAL_PIC_SetBitSVal( u8_t  PORTxx , u8_t bitx , u8_t val,u8_t bit_counuter){
    GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
	if (val==0){set_bits(*(gpio->PORTX),bitx,val);}
	else if(val==1) {clr_bits(*(gpio->PORTX),bitx,val);}
}
void MCAL_PIC_ToggleBit( u8_t  PORTxx , u8_t bitx ){
	GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
    toogle_bit(*(gpio->PORTX),bitx);
return;}
void MCAL_PIC_SetPORTVal( u8_t  PORTxx , u8_t val ){
*(MCAL_PIC_getgpio(PORTxx)->PORTX)=val; return;}
u8_t MCAL_PIC_GetBitVal ( u8_t  PORTxx , u8_t bitx){
GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);
return get_bit(*(gpio->PORTX),bitx);}
u8_t MCAL_PIC_GetPINVal( u8_t  PORTxx ){
    GPIOXX* gpio=MCAL_PIC_getgpio(PORTxx);return *(gpio->PORTX);
    }
//adc
void adc_init(void){
  ADCON1=0x02; //configuration mode
  ADCON0=0x41; //on and channel 5
}

u16_t adc_read(u8_t ch){
     switch(ch){
      case 0: //    10/204 ration
        ADCON0=0x01;
        break;
      case 1:
        ADCON0=0x09;
        break;
      case 2: //temperature ch  10/2;
        ADCON0=0x11;
        break;
    }
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

   return ((((u16_t)ADRESH)<<2)|(ADRESL>>6));
}

