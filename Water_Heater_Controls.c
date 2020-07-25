#include <xc.h>
#include "HAL_PicGenios.h"
#include "MCAL_PIC16F8.h"
u8_t SEG_Isinit=0 ;
u8_t temp_counter=0,temp_array[10]={27};
void PicGenios_Init (void){
        OPTION_REGbits.nRBPU=0; //enable pull-up on portb
        TempSensor_Init();
        MCAL_PIC_SetBitDir(LedPort,HeaterLedPin,OUTPUT);
        MCAL_PIC_SetBitDir(DecButPort,DecButPin,INPUT);
        MCAL_PIC_SetBitDir(IncButPort,IncButPin,INPUT);
        MCAL_PIC_SetBitDir(IncButPort,0,INPUT);
        MCAL_PIC_SetBitVal(DecButPort,DecButPin,0);
        MCAL_PIC_SetBitVal(IncButPort,IncButPin,0);
        MCAL_PIC_SetBitVal(IncButPort,0,0);
        SEG_Init();
        MCAL_PIC_InitTimer0(sys_clk,256);
        i2c_init();
        MCAL_PIC_EnGINTP();
}
void SEG_Flash(u8_t  _tim_){
    u8_t i;
    SEG_Init();
    SEG_GetValues(_tim_);
    for(i=0 ; i<2 ; i++){
            SEG_toogle(1); 
            MCAL_PIC_MSdelay(1);
            SEG_toogle(0);
            MCAL_PIC_MSdelay(1);
    }
return;}
void SEG_Init(void){
        SEG_init(SEG_Port,SEG_EnPort,SEG01_En);
        SEG_init(SEG_Port,SEG_EnPort,SEG02_En);
        MCAL_PIC_SetPORTDir(SEG_Port,OUTPUT);
return;}
void SEG_Blink(void){
    if(SEG_Isinit==1){
        SEG_Stop();
        SEG_Isinit=0;}
    else{
        MCAL_PIC_SetBitDir(SEG_EnPort,SEG01_En,OUTPUT);
        SEG_Isinit=1;
        MCAL_PIC_SetBitVal(SEG_EnPort,SEG01_En,1);
        MCAL_PIC_SetBitDir(SEG_EnPort,SEG02_En,OUTPUT);
        MCAL_PIC_SetBitVal(SEG_EnPort,SEG02_En,1);
    }
        SEG_Isinit=1;
return;}
void SEG_Stop(void){
        MCAL_PIC_SetBitDir(SEG_EnPort,SEG01_En,OUTPUT);
        MCAL_PIC_SetBitVal(SEG_EnPort,SEG01_En,0);
        MCAL_PIC_SetBitDir(SEG_EnPort,SEG02_En,OUTPUT);
        MCAL_PIC_SetBitVal(SEG_EnPort,SEG02_En,0);
        MCAL_PIC_SetPORTDir(SEG_Port,INPUT);
        SEG_Isinit=0;}
void Cont_Heater(u8_t mode){
switch (mode){
case ON:
    MCAL_PIC_SetBitDir(HeaterPort,HeaterPin,OUTPUT);
    MCAL_PIC_SetBitDir(LedPort,HeaterLedPin,OUTPUT);
    if(!flg1){MCAL_PIC_SetBitVal(LedPort,HeaterLedPin,1);}
    else{MCAL_PIC_SetBitVal(LedPort,HeaterLedPin,0);}
    MCAL_PIC_SetBitVal(HeaterPort,HeaterPin,1);
    break;
case OFF:
    MCAL_PIC_SetBitVal(HeaterPort,HeaterPin,0);
    MCAL_PIC_SetBitVal(LedPort,HeaterLedPin,0);
    break;}
}
void Cont_FAN(u8_t mode){
MCAL_PIC_SetBitDir(FanPort,FanPin,OUTPUT);
MCAL_PIC_SetBitDir(LedPort,FanLedPin,OUTPUT);
switch (mode){
case ON:
     MCAL_PIC_InitTimer1CM(32,6,0);
     MCAL_PIC_DisGINT();
     MCAL_PIC_SetBitVal(LedPort,FanLedPin,1);
     MCAL_PIC_SetBitVal(PORTc,2,1);
    break;
case OFF:
    MCAL_PIC_SetBitVal(LedPort,FanLedPin,0);
    MCAL_PIC_Timer1_Stop();
    MCAL_PIC_SetBitVal(PORTc,2,0);
    MCAL_PIC_InitTimer0(sys_clk,256);
    break;}
}
void e2pex_InitDef(void){
    u8_t i;
    e2pext_w(init_flag,5);
    e2pext_w(set_last,60);
    for (i=0 ; i<9 ; i++){
        temp_array[i]=27;
        e2pext_w(temp_first+i,temp_array[i]);
    }
return;}
u8_t e2pext_r(u16_t addr){
  u8_t ret;
  u8_t ah;
  u8_t al;
  ah=(addr&0x0100)>>8;
  al=addr&0x00FF;
  i2c_start();
  if(ah){i2c_wb(0xA2);}
  else  {i2c_wb(0xA0);}
  i2c_wb(al);
  i2c_start();
  if(ah){i2c_wb(0xA3);}
  else{i2c_wb(0xA1);}
  ret=i2c_rb(1);
  i2c_stop();
  return ret;}
void e2pext_w(u16_t addr, u8_t val){
  u16_t tmp=val;
  u8_t ah=(addr&0x0100)>>8 , al=addr&0x00FF ,nt=0;
  do{
    i2c_start();
    if(ah)  { i2c_wb(0xA2);}
    else    { i2c_wb(0xA0);}
    i2c_wb(al);i2c_wb(tmp);
    i2c_stop();
    nt++;}while((tmp != e2pext_r(addr))&&(nt < 10));
return;}
u8_t read_last_set(){return e2pext_r(set_last); }
void write_last_set(u8_t t){ e2pext_w(set_last,t);}
u8_t read_last_temp(){return e2pext_r(temp_last); }
u8_t get_averge_temp(){   
u8_t tem;    
u16_t n,i;
u16_t avg=0;
for (i=0 ; i<temp_counter ; i++){
    n=(u16_t)e2pext_r(temp_first+i);
    if(n==100){
        if(temp_array[9]!=100){avg=avg+e2pext_r(temp_last);}
        else{avg=avg+27;}
        }
    else{avg=avg+n;}}
if(!i){avg=75;}
else{avg=avg/i;}
tem=(u8_t)avg;
return tem;}
void update_temp_table(void){
u8_t m;
m=return_CurrentTemp();
if(m!=100){
    push_table();
    if(temp_counter<10) {temp_array[temp_counter]=m;temp_counter++;}
    else{temp_counter=10;temp_array[9]=m;}
}
e2pext_w(temp_last,temp_array[9]);
}
void push_table(void){
u16_t i ;
for (i=1 ; i<temp_counter ; i++){
    temp_array[i-1]=temp_array[i];
    e2pext_w(temp_first+i,temp_array[i]);}
return;}
void TempSensor_Init(void){
    TRISB=0x01;
    TRISC=0x01;
    MCAL_PIC_SetBitDir(PORTe,0,OUTPUT);
    MCAL_PIC_SetBitDir(PORTa,0,INPUT);
    MCAL_PIC_SetBitDir(PORTa,1,INPUT);
    MCAL_PIC_SetBitDir(PORTa,2,INPUT);
    adc_init();
    if(e2pext_r(init_flag)!=5){e2pex_InitDef();}
return;}
u8_t return_CurrentTemp(void){
    TempSensor_Init();
    MCAL_PIC_MSdelay(1);
    u16_t mi=((adc_read(2)*10)/2);
    temp_array[9]=((u8_t)((mi%100)/10+((mi%1000)/10)));;
    if(temp_array[9]){return temp_array[9];}
    else{return 100;}
    
}