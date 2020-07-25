#include "MCAL_PIC16F8.h"
#include <stdio.h>
#include <math.h>
#include <xc.h>
 u8_t global_prescaler ,cnt,t1cont;
void MCAL_PIC_Timer0SetPS(u16_t prescaler){
global_prescaler=prescaler;
switch(prescaler){
case 256:
   OPTION_REGbits.PS0=1;
   OPTION_REGbits.PS1=1;
   OPTION_REGbits.PS2=1;
    break;
case 128:
   OPTION_REGbits.PS0=0;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;
    break;
case 64:
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=0;
    OPTION_REGbits.PS2=1;
    break;
case 32:
    OPTION_REGbits.PS0=0;
    OPTION_REGbits.PS1=0;
    OPTION_REGbits.PS2=1;
    break;
case 16:
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=0;
    break;
case 8:
    OPTION_REGbits.PS0=0;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=0;
    break;
case 4:
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=0;
    OPTION_REGbits.PS2=0;
    break;
case 2:
    OPTION_REGbits.PS0=0;
    OPTION_REGbits.PS1=0;
    OPTION_REGbits.PS2=0;
    break;
default:
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;}
return;}
void MCAL_PIC_Timer0_SetCM(u8_t OVT0){
TMR0=OVT0;
return;}
void MCAL_PIC_InitTimer0(u8_t mode,u16_t prescaler){
    SysTime.ticks=1;SysTime.milliseconds=0; SysTime.seconds=0; SysTime.minutes=0; //defaults value assigned
    TMR0= 0; //resetting
    OPTION_REGbits.T0CS=0; //same processor clock
    OPTION_REGbits.T0SE=0; //MORE ACCURATE on low - to high trans
    OPTION_REGbits.PSA=0; //prescaler assgined
    INTCONbits.TMR0IF=0;  //flag cleared before enabling intp
    INTCONbits.TMR0IE=1; //ENABLE INTERRUPT
switch(mode){
case bit8_OV:
    MCAL_PIC_Timer0SetPS(prescaler);
    break;
case sys_clk:
    MCAL_PIC_Timer0SetPS(256); //based on 1 MHZ freq every 2 ticks generates 1 MS
    MCAL_PIC_Timer0OVredirect(MCAL_PIC_Timer0SysClkHandler); //setting interrupt call back
    MCAL_PIC_SysClkredirect(MCAL_PIC_SysClkDef); //default
    break;
default:    break;}
return;}
void MCAL_PIC_Timer0SysClkHandler(void){
    if(SysTime.milliseconds==999){
        MCAL_PIC_CyclicSecond();
        if(SysTime.seconds==59){SysTime.minutes++;SysTime.seconds=0;}
        else {SysTime.seconds++;}
        SysTime.milliseconds=0;SysTime.ticks=0;}
    else { SysTime.ticks++;SysTime.milliseconds=(u8_t)(SysTime.ticks*0.512/FRQ);}
    MCAL_PIC_SysClkHandler();return;} //provides further functionality to the user
void MCAL_PIC_SysClkredirect(void (*ptr) (void)){MCAL_PIC_SysClkHandler=ptr;}
void MCAL_PIC_SysClkDef(void){return;}
void MCAL_PIC_ResetTime(void){SysTime.milliseconds=0;SysTime.seconds=0;SysTime.minutes=0;}
void MCAL_PIC_MSdelay ( u16_t ms){
u16_t  i;
u8_t j;
 for (i =0; i< ms*FRQ; i++){
  for (j =0 ; j < 200; j++){
        __asm( " nop " ); 
        __asm( " nop " ); 
        __asm( " nop " ); 
        __asm( " nop " ); 
        __asm( " nop " ); 
        }
    }
}
Time* MCAL_PIC_GetTime(void){return &SysTime;}
void MCAL_PIC_Timer1SetPS(void){
     T1CONbits.TMR1CS=1;
    T1CONbits.T1CKPS1=0; //prescale
 T1CONbits.T1CKPS0=0;
}
void MCAL_PIC_Timer1_SetCM(u16_t OVT1){
 TMR1H=(u8_t)(OVT1>>8) ;
 TMR1L=(u8_t)(OVT1&0x00FF);
 }
void MCAL_PIC_InitTimer1CM(u8_t prescaler , u8_t OVT0 , u16_t OVT1){
 MCAL_PIC_SetBitDir(PORTc,0,INPUT);
 MCAL_PIC_InitTimer0(bit8_OV,prescaler); //32
 MCAL_PIC_Timer0_SetCM(OVT0); //6
 cnt=125;
 //MCAL_PIC_EnGINTP(); //change interrupt handler  to the following function 
 MCAL_PIC_Timer0OVredirect(MCAL_PIC_Timer0CMHandler); //gain interrupt functionallity 
 MCAL_PIC_Timer1_SetCM(OVT1); //0
 T1CONbits.TMR1ON=1;
return;}
void MCAL_PIC_Timer0CMHandler(void){
    cnt--;
  if(!cnt){ 
    // t1cont=(((unsigned int)TMR1H<<8)|(TMR1L))/7; represents the currenty duty
    cnt=125;
    MCAL_PIC_Timer1_SetCM(0);
  }
  INTCONbits.T0IF=0;
  MCAL_PIC_Timer0_SetCM(6);
 return;}
void MCAL_PIC_Timer1_Stop(void){T1CONbits.TMR1ON=0;}