#include "HAL_PicGenios.h"
#define SEG_En1 2
#define SEG_En2 3
#define SEG_En3 4
#define SEG_En4 5
#define seg0 0b11000000 //192
#define seg1 0b11111001 //249
#define seg2 0b10100100 //164
#define seg3 0b10110000
#define seg4 0b10011001
#define seg5 0b10010010
#define seg6 0b10000010
#define seg7 0b11111000
#define seg8 0b10000000
#define seg9 0b10010000
/*
RD0 Segmento A
RD1 Segmento B
RD2 Segmento C
RD3 Segmento D
RD4 Segmento E
RD5 Segmento F
RD6 Segmento G
RD7 Segmento Ponto
RA2 Seleção do Display1 (Disp1)
RA3 Seleção do Display2 (Disp2)
RA4 Seleção do Display3 (Disp3)
RA5 Seleção do Display4 (Disp4)
Void SEG_init (u8_t pin)
*/
static u8_t values[2]={0};
const u8_t _SEG[10]=  {0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01101111};
//const u8_t _SEG[10]={0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10010000};
void SEG_init (u8_t pin ,u8_t contport ,u8_t contpin) {
MCAL_PIC_SetPORTDir(pin,OUTPUT);
MCAL_PIC_SetBitDir(contport,contpin,OUTPUT);
return;}
void SEG_GetValues(u8_t t){
values[0]=(t)/10;
values[1]=t-values[0]*10 ;
return;}
void SEG_toogle (u8_t id){
u8_t i;
for (i=0 ; i<2 ; i++){
    MCAL_PIC_SetBitDir(SEG_EnPort,i+2,OUTPUT);
    MCAL_PIC_SetBitVal(SEG_EnPort,i+2,0);
    }
MCAL_PIC_SetBitDir(SEG_EnPort,id+2,OUTPUT);
MCAL_PIC_SetBitVal(SEG_EnPort,id+2,1);
MCAL_PIC_SetPORTDir(SEG_Port,OUTPUT);
MCAL_PIC_SetPORTVal(SEG_Port,_SEG[values[id]]);
return;}
void SEG_ToogleVal (u8_t id ,u8_t val){
u8_t i;
//shut down all SEGs first
for (i=0 ; i<4 ; i++){
 MCAL_PIC_SetBitDir(SEG_EnPort,i+2,OUTPUT);
 MCAL_PIC_SetBitVal(SEG_EnPort,i+2,0);}
MCAL_PIC_SetBitDir(SEG_EnPort,id+2,OUTPUT);
MCAL_PIC_SetBitVal(SEG_EnPort,id+2,1);
MCAL_PIC_SetPORTDir(SEG_Port,OUTPUT);
MCAL_PIC_SetPORTVal(SEG_Port,_SEG[val]);
return;}

