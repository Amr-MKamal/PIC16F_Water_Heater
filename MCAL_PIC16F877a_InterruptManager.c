#include"MCAL_PIC16F8.h"
#include <xc.h>
#define INTPnot_recognised 0
#define INTP0xSig    2
#define INTPRBxSig   3
#define Timer0OVSig  4
#define Timer0CMSig  5
u8_t current_source=0;
void (* MCAL_PIC_Timer0OVHandler)       (void);
void (* MCAL_PIC_INTPHandler)           (void);
void (* MCAL_PIC_INTP0Handler)          (void);
void (* MCAL_PIC_INTPRBHandler)         (void);
void (* MCAL_PIC_SysClkHandler)         (void);
void (* MCAL_PIC_CyclicSecond)          (void);
void MCAL_PIC_INTP0redirect(void (*ptr) (void)){MCAL_PIC_INTP0Handler=ptr; return;}
void MCAL_PIC_MCAL_PIC_CyclicSecondredirect(void (*ptr) (void)){MCAL_PIC_CyclicSecond=ptr; return;}
void MCAL_PIC_INTPRBredirect(void (*ptr) (void)){MCAL_PIC_INTPRBHandler=ptr; return;}
void MCAL_PIC_Timer0OVredirect(void (*P) (void)){MCAL_PIC_Timer0OVHandler=P;return;}
void MCAL_PIC_EnGINTP(void) {INTCONbits.GIE = 1;} //enables general interrupt
void MCAL_PIC_DisGINT(void){ INTCONbits.GIE = 0;}
void MCAL_PIC_INTP0En(void){
    OPTION_REGbits.nRBPU=0; //enable pull-up on portb
    MCAL_PIC_SetBitVal(PORTb,0,1);
    MCAL_PIC_SetBitDir(PORTb,0,INPUT);
    INTCONbits.INTE=1;
    INTCONbits.INTF=0;
    INTCONbits.RBIE=0;
    MCAL_PIC_EnGINTP();
    return;
}
u8_t MCAL_PIC_GetINTSource(void){ 
    //check for all enabled interrupt sources for interrupt source
if( INTCONbits.INTF){return INTP0xSig;}
else if(INTCONbits.RBIF)   {return INTPRBxSig;}
else if(INTCONbits.TMR0IF){return Timer0OVSig;}
else {return INTPnot_recognised;}}
void MCAL_PIC_IntpServe(u8_t source){
switch (source){
    case INTP0xSig:     MCAL_PIC_INTP0Handler();break;
    case INTPRBxSig:    MCAL_PIC_INTPRBHandler();break;
    case Timer0OVSig:   MCAL_PIC_Timer0OVHandler();break;
    case INTPnot_recognised:  break; //these cases for error handling should handle error by resetting
    }
return;}
void MCAL_PIC_DirectISR(void){
//if(INTCONbits.RBIF)        {MCAL_PIC_INTPHandler=MCAL_PIC_INTP0Handler;    INTCONbits.RBIF=0;}
 if(INTCONbits.INTF) {MCAL_PIC_INTPHandler=MCAL_PIC_INTP0Handler;     INTCONbits.INTF=0;}
else if(INTCONbits.TMR0IF){MCAL_PIC_INTPHandler=MCAL_PIC_Timer0OVHandler;     INTCONbits.TMR0IF=0;}
return;}
