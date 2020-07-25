
#ifndef MCAL_PIC18F_H_
#define MCAL_PIC18F_H_
#include "stdtypes.h"
#include <xc.h>
#define ris_edge 	0
#define fal_edge 	1
#define log_change	2
#define low_lev		3
#define INT0		6
#define PORTa 0
#define PORTb 1
#define PORTc 2
#define PORTd 3
#define PORTe 4
#define DDRa 10
#define DDRb 11
#define DDRc 12
#define DDRd 13
#define DDRe 14
#define PINa 20
#define PINb 21
#define PINc 22
#define PINd 23
#define ICLK PORTCbits.RC3
#define IDAT PORTCbits.RC4
#define TIDAT TRISCbits.TRISC4
#define INPUT 0
#define OUTPUT 1
#define INPUT_FLOAT 2
#define PORT_OUT 0x00
#define PORT_IN 0xff
#define High 1
#define Low 0
#define bit8_OV 0
#define sys_clk 1
#define TIMER0 0
#define FRQ 1
#define Ticks  (1/0.512)*(1/FRQ)

/** ===============DIO.C Functions ===================================*/
/**Description: a structure for the PIC6F8 related port registers
Elements : volatile unsigned 8bit char pointer PORTX contains real address for PORTx that is used for reading or writing the port (INPUT/OUTPUT)
           volatile unsigned 8bit char pointer TRISX contains real address for data direction register that is used to determine port config (INPUT or OUPUT)
 */
struct GPIOX {
	volatile u8_t* PORTX;
	volatile u8_t* TRISX;
};
typedef struct GPIOX GPIOXX;
/**Description: resolves dummy port names to an actual address for further control
Input Value: unsigned 8bit char for the predefined dummy port names
Return Value:a volatile pointer to structure of type GPIO  */
volatile GPIOXX* MCAL_PIC_getgpio(u8_t PORTxx);
/**Description: releases certain ports from adc
Input Value: void
Return Value:void */
void MCAL_PIC_Init_Port(u8_t PORTxx);
/**Description: sets a PINx (bitx) in PORTx as INPUT/OUTPUT (dir)
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bitx location ,
             unsigned 8bit char for direction (0 or 1)
Return Value:void */
void MCAL_PIC_SetBitDir( u8_t  PORTxx , u8_t bitx , u8_t dir);
/**Description: sets a PINx (bitx)s in PORTx as INPUT/OUTPUT (dir)
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bitx start location ,
             unsigned 8bit char for number of bits starting bitx location ,
             unsigned 8bit char for direction (0 or 1)
Return Value:void */
void MCAL_PIC_SetBitsDir( u8_t  PORTxx , u8_t bitx , u8_t bit_counuter , u8_t dir);
/**Description: sets PORTx as INPUT/OUTPUT (dir)
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for direction (0 or 1)
Return Value:void */
void MCAL_PIC_SetPORTDir(u8_t  PORTxx ,u8_t dir);
/**Description: sets a PINx (bitx) in PORTx value.
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bitx location ,
             unsigned 8bit char for bit value
Return Value:void */
void MCAL_PIC_SetPORTDirVal(u8_t  PORTxx ,u8_t val);
void MCAL_PIC_SetBitVal( u8_t  PORTxx , u8_t bitx , u8_t val);
/**Description: sets a PINx (bitx)s in PORTx values.
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bitx location ,
             unsigned 8bit char for bitx number of bits to be changed  ,
             unsigned 8bit char for bits common value
Return Value:void */
void MCAL_PIC_SetBitSVal( u8_t  PORTxx , u8_t bitx , u8_t val,u8_t bit_counuter);
/**Description: togglesPINx (bitx) in PORTx value.
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bitx location ,
Return Value:void */
void MCAL_PIC_ToggleBit( u8_t  PORTxx , u8_t bitx );
/**Description: sets PORTx values
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bits common value
Return Value:void */
void MCAL_PIC_SetPORTVal( u8_t  PORTxx , u8_t val );
/**Description: reads the value of PINx (bitx) in PORTx .
Input Value: unsigned 8bit char for the predefined dummy port names ,
             unsigned 8bit char for bitx location .
Return Value:unsigned 8bit char holding the value of PORTx input register PINx bit */
u8_t MCAL_PIC_GetBitVal ( u8_t  PORTxx , u8_t bitx);
/**Description: reads the value of PORTx .
Input Value: unsigned 8bit char for the predefined dummy port names .
Return Value:unsigned 8bit char holding the value of PORTx input register */
u8_t MCAL_PIC_GetPINVal( u8_t  PORTxx );
/**Description: configures the adc and intialize it to channal 5 for the temp sensor.
Input Value: void
Return Value:void */
void adc_init(void);
/**Description: reads temperature channal(2) and return the current temp as unsigned int value
Input Value: channal to be red (0,1,2)
Return Value: analog to digital 16 bit result  */
u16_t adc_read(u8_t ch);
/**Description:configures and initializes i2c module
Input Value: void
Return Value:void */
void i2c_init(void);
/**Description: stops the transformation of data
Input Value: void
Return Value:void */
void i2c_stop(void);
/**Description: special pre-calculated assmbely delay.
Input Value: void
Return Value:void 
 Notes: assmbely function*/
static void I2C_delay(void);
/**Description: starts the transformation of data saved in the register
Input Value: void
Return Value:void */
void i2c_start(void);
/**Description: sends a certain byte (val) through i2c.
Input Value: void
Return Value:void */
void i2c_wb(u8_t val);
/**Description: reads a certain byte from i2c receiver register with acknolowgment.
Input Value:u8_t ack flag
Return Value:the recieved data as u8_t  */
u8_t i2c_rb(u8_t ack);
/**========================= InterruptManager.c Functions ========================= */
enum _Mode {OnChange,Ris_edge,Fal_edge};
extern void (* MCAL_PIC_Timer0OVHandler)       (void);
extern void (* MCAL_PIC_HighPriINTPHandler)    (void);
extern void (* MCAL_PIC_LowPriINTPHandler)     (void);
extern void (* MCAL_PIC_INTPHandler)          (void);
extern void (* MCAL_PIC_INTP0Handler)          (void);
extern void (* MCAL_PIC_INTPRBHandler)         (void);
extern void (* MCAL_PIC_SysClkHandler)         (void);
extern void (* MCAL_PIC_CyclicSecond)          (void);
/**Description: return interrupt source in case get & serve are used
Input Value: void
Return Value:a predefined interrupt source signal mentioned in the header file*/
u8_t MCAL_PIC_GetINTSource(void);
/**Description: enables external interrupt sources on PORTB it also unmasks the interrupt & configures it
Input Value: interrupt sources as unsigned 8bit char:
             INTx0 :external interrupt on PORTB RB0
             INTx5..7 :external interrupt onchange  PORTB RB6 RB6 RB5
             interrupt modes as unsigned 8bit char :
             OnChange: for RB7 through RB5 only
Return Value: void
Note:calls MCAL_PIC_EnGINTP() :  */
void MCAL_PIC_EnINTPx(u8_t source , enum _Mode mode);
/**Description: rewraps general interrupt enable
Input Value: void
Return Value:void*/
void MCAL_PIC_EnGINTP(void);
/**Description: rewraps general interrupt disable
Input Value: void
Return Value:void*/
void MCAL_PIC_DisGINT(void);
/**Description: enables an interrupt INT0 on port RB0 to send an interrupt signal
 *  and sets it's flags , & enables pull up resitor and general interrupt
Input Value: void
Return Value: void*/
void MCAL_PIC_INTP0En(void);//
/**Description: serve an interrupt for it's source by calling their default handler
Input Value: interrupt source as unsigned 8bit char
Return Value:void
Note: handlers are pointers to function's of type void and input void
Note2: calling this function can serve an interrupt handler without knowing if interrupt signal actually happened in hardware */
void MCAL_PIC_IntpServe(u8_t source);
/**Description: Serves the reason of interrupt by searching for it and assigning the default
               high priority interrupt handler to it's handler then clears the interrupt flag.
Input Value: void
Return Value:void*/
void MCAL_PIC_DirectISR(void);
/**void MCAL_PIC_xxxxxredirect(void (*ptr) (void))
 * Description: these functions are used for interfacing with files that need to use an external interrupt
               the function links the interrupt handler to the inputed function location
               interrupt handler are always called interrupt functions should not be called directly
Input Value: a pointer to a function that takes void and returns void
Return Value:void*/
void MCAL_PIC_INTP0redirect(void (*ptr) (void));
void MCAL_PIC_INTPRBredirect(void (*ptr) (void));
void MCAL_PIC_Timer0OVredirect(void (*P) (void));
void MCAL_PIC_SysClkredirect(void (*ptr) (void));
void MCAL_PIC_MCAL_PIC_CyclicSecondredirect(void (*ptr) (void));
/**========================= Timer0Module.C Functions ========================= */
/**Description: a structure for a simple concept of time
Elements : double ticks : used as the number of overflows needed to generate 1 ms and sysclk overrides it to 1
           unsigned long integer  minutes: to hold the value of counted minutes since sysclk has been enabled
           unsigned short integer milliseconds: to hold the value of counted milliseconds relative to seconds since sysclk has been enabled
           unsigned 8bit char     seconds: to hold the value of counted seconds relative to minutes since sysclk has been enabled*/
struct time{
float ticks;
u32_t minutes;
u16_t milliseconds;
u8_t seconds;
};
typedef struct time Time;
Time SysTime;
/**Description: initialize Timer0 in predefined mode that has different levels of configuration
Input Value: 1-unsigned 8bit char for possible modes :
               bit8_OV: configures timer0 to work as 8bit timer that generates an interrupt on Overflow (256)
               sys_clk: configures timer0 to work an bit8_OV and configures that to provide system time functionality through time structure global SysTime
Return Value:void*/
void MCAL_PIC_InitTimer0(u8_t mode,u16_t prescaler);
/**Description: sets the prescale value of Timer0
Input Value: void
Return Value:void*/
void MCAL_PIC_Timer0SetPS(u16_t prescaler);
/**Description: sets the compare match register value of Timer0
Input Value: a value from 0 to 255 
Return Value:void*/
void MCAL_PIC_Timer0_SetCM(u8_t OVT0);
/**Description: optimizes timer0 prescaler to generate an interrupt every 1 ms based on frequency settings & and prescaler old values uses MATH.c for rounding
Input Value: void
Return Value:void*/
void MCAL_PIC_Timer0_GetTick(void);
/**Description: default function to be called for SysClk interrupt handling just increases time 
 * adds further functionality to the user by calling MCAL_PIC_SysClkHandler() 4
 * which is a function pointer that can be redirected to serve a certain tasks or multi task or to build a schedualr 
Input Value: void
Return Value:void */
void MCAL_PIC_Timer0SysClkHandler(void);
/**Description: a getter to access systime
Input Value: void
Return Value: pointer to Systime structure is returned */
Time* MCAL_PIC_GetTime(void);
/**Description: resets SysTime global to 0
Input Value: void
Return Value: void */
void MCAL_PIC_ResetTime(void);
/**Description: dummy empty function for SysClk interrupt call by timer0sysclkhandeler j
Input Value: void
Return Value:void */
void MCAL_PIC_SysClkDef(void);
/**Description: dummy empty function for SysClk interrupt call by timer0sysclkhandeler j
Input Value: void
Return Value:void */
void MCAL_PIC_MSdelay ( u16_t ms);
/**Description: sets the prescale value of Timer1
Input Value: void
Return Value:void*/
void MCAL_PIC_Timer1SetPS(void);
/**Description: sets the compare match register value of Timer1
Input Value: an unsigned int value for the compare match register
Return Value:void*/
void MCAL_PIC_Timer1_SetCM(u16_t OVT1);
/**Description: initialize Timer0 & timer1 in compare match mode , with prescaler and values of the compare matches 
Input Value: u8_t prescaler :-unsigned 8bit char for possible prescaled values:
               8bit OVT0: configures timer0 to work as 8bit timer with compare match register value ovt0
              u16_t OVT1: configures timer1 to work as 16bit timer with compare match register value ovt0
Return Value:void
 Notes: this function redirects timer interrupt from systemclk to serve pwm application & enables general interrupt*/
void MCAL_PIC_InitTimer1CM(u8_t prescaler , u8_t OVT0 , u16_t OVT1);
/**Description: default function to be called on timer0 compre match interrupt arises in timer1 cm mode,
 * sets a certain pwm signal with duty to the appropiate compare match channel 
Input Value: void
Return Value:void 
Notes: overrides systemclk functionality */
void MCAL_PIC_Timer0CMHandler(void);
/**Description: stops timer1 via timer1 stop bit
Input Value: void
Return Value: void */
void MCAL_PIC_Timer1_Stop(void);
#endif /** MCAL_PIC18F_H_ */
