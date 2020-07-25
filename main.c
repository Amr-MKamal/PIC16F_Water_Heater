/*
 * File:   main.c
 * Author: Amr Mohammed Kamal
 * Created on July 2, 2020, 7:48 PM
 */
#include <xc.h>
#include"MCAL_PIC16F8.h"
#include "HAL_PicGenios.h"
#include <stdio.h>
#define set_max 75
#define set_min 35
enum State{On,Temp_setting,Off};
enum State Cur_State=ON;
u8_t cyclicSecond_counter=0;
u8_t set_temp, cur_temp ,cur_avg=27, settingTimeMax=5;
int ij;
u8_t flg2;
void main(void){  
  PicGenios_Init();
  MCAL_PIC_SysClkredirect(second_update);
  MCAL_PIC_INTP0En();
  MCAL_PIC_INTP0redirect(Start_stop);
  MCAL_PIC_MCAL_PIC_CyclicSecondredirect(second_update);
  set_temp=read_last_set();
  cur_temp=read_last_temp();
    while(1){
        MCAL_PIC_EnGINTP();
        switch(Cur_State){
                case On:
                    MCAL_PIC_InitTimer0(sys_clk,256);
                     MCAL_PIC_MSdelay(1);
                    update_heater(); //update tempereture , heater , cooller , leds
                    MCAL_PIC_InitTimer0(sys_clk,256);
                    if(!MCAL_PIC_GetBitVal(PORTb,1))        {Cur_State=Temp_setting; } 
                    else if(!MCAL_PIC_GetBitVal(PORTb,2))   {MCAL_PIC_MSdelay(1);Cur_State=Temp_setting;}
                    break;
                case Temp_setting:
                   MCAL_PIC_MSdelay(1);MCAL_PIC_SetBitVal(PORTb,1,1) ;MCAL_PIC_SetBitVal(PORTb,2,1) ;
                   MCAL_PIC_SetBitDir(PORTb,1,INPUT);
                   MCAL_PIC_SetBitDir(PORTb,2,INPUT);
                   MCAL_PIC_MSdelay(50);
                    if(!MCAL_PIC_GetBitVal(PORTb,1)){
                          if(set_temp<set_max)  {
                              set_temp=set_temp+5;
                            write_last_set(set_temp);
                            }
                          cyclicSecond_counter=0;
                          MCAL_PIC_SetBitVal(PORTb,1,1) ;
                    }
                    if(!MCAL_PIC_GetBitVal(PORTb,2)){
                         if(set_temp>set_min)  {
                       set_temp=set_temp-5;
                       write_last_set(set_temp);
                         }
                         MCAL_PIC_SetBitVal(PORTb,2,1) ;
                         cyclicSecond_counter=0;  
                    }
                   for(ij=0 ; ij<10;ij++){ SEG_Flash(set_temp);}
                    break;
                case Off:
                break;}}}
void update_heater(void){
    u8_t temp,i ;   
    MCAL_PIC_DisGINT();
    update_temp_table();
    temp=get_averge_temp(); //causes around 3.67 ms delay
    MCAL_PIC_SetBitDir(PORTb,1,INPUT);
    MCAL_PIC_SetBitDir(PORTb,2,INPUT);
    MCAL_PIC_InitTimer0(sys_clk,256);
    MCAL_PIC_SysClkredirect(second_update);
    if (temp+5<=set_temp){
           Cont_Heater(ON);
               for(i=0 ; i<25;i++){ SEG_Flash(temp); } //10 ms delay
               SEG_Stop();
           Cont_Heater(OFF);
    }
    else if (temp+5>=set_temp){
           Cont_FAN(ON);
               for(i=0 ; i<25;i++){ SEG_Flash(temp); } //10 ms delay
               SEG_Stop();
           Cont_FAN(OFF);
    }
    return;}
void second_update(void){
   if(flg1){flg1=0;} else{flg1=1;} //flag toggle for the heater led
   if(Cur_State==Temp_setting) {
          SEG_Stop();
         if(cyclicSecond_counter<settingTimeMax){cyclicSecond_counter++;}
         else{
         cyclicSecond_counter=0;
         Cur_State=On;
         }
     }
 return;}
void Start_stop(void){
   if(Cur_State==Off){        
       SEG_Flash(55);
       MCAL_PIC_InitTimer0(sys_clk,256);
       Cur_State=On;
   }
   else{
        SEG_Flash(0);
       Cur_State=Off;
   }
}
void  __interrupt ()  isr(void){// serve interrupt source
MCAL_PIC_DirectISR(); // get interrupt source & assign HighPriINTPHandler pointer to it's handler function
MCAL_PIC_DisGINT();
MCAL_PIC_INTPHandler(); //activate the function needed (  blinking (state/7seg/led / sensing temperature )
MCAL_PIC_EnGINTP();
return;}