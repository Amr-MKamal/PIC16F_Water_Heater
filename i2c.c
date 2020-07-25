#include <xc.h>
#include "HAL_PicGenios.h"
void I2C_delay(void){
     __asm( " nop " ); 
     __asm( " nop " ); 
     __asm( " nop " ); 
     __asm( " nop " ); 
     __asm( " nop " ); 
	 __asm( " nop " ); 
	 __asm( " nop " ); 
     __asm( " nop " ); 
return;}
void i2c_init(void){
  MCAL_PIC_SetBitDir(I2CPORT,3,OUTPUT);
  TIDAT=0;
  ICLK=1;
  IDAT=1;
return;}
void i2c_start(void){
  ICLK=1;
  IDAT=1;
  I2C_delay();
  IDAT=0;
  I2C_delay();
  return;}
void i2c_stop(void){
  ICLK=1;
  IDAT=0;
  I2C_delay();
  IDAT=1;
  I2C_delay();
return;}
void i2c_wb(u8_t val){
  u8_t i;
  ICLK=0;
  for(i=0;i<8;i++){
    IDAT=((val>>(7-i))& 0x01);
    ICLK=1;
    I2C_delay();
    ICLK=0;
}
  IDAT=1;
  I2C_delay();
  ICLK=1;
  I2C_delay();
  ICLK=0;
return;}
u8_t i2c_rb(u8_t ack){
  s8_t i;
  u8_t ret=0;
  ICLK=0;
  TIDAT=1;
  IDAT=1;
  for(i=0;i<8;i++){
    ICLK=1;
    I2C_delay();
    ret|=(IDAT<<(7-i));
    ICLK=0;
  }
  TIDAT=0;
  if(ack){IDAT=0;}
  else{IDAT=1;}
  I2C_delay();
  ICLK=1;
  I2C_delay();
  ICLK=0;
  return ret;}



