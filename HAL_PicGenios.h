#ifndef HAL_PICGENIOS_H_INCLUDED
#define HAL_PICGENIOS_H_INCLUDED
#include "MCAL_PIC16F8.h"
#include "stdtypes.h"
#include "config_877A.h"
#define ON 1
#define OFF 0
#define No_Read 11
#define I2CPORT PORTc
#define SEG_Port PORTd //all port
#define SEG_EnPort PORTa //RA2 ..5
#define SEG01_En  2  //RA2 ..5
#define SEG02_En  3 //RA2 ..5
#define FanPort PORTc
#define FanPin 2
#define HeaterPort PORTc
#define HeaterPin 5
#define DecButPort PORTb
#define DecButPin 1
#define IncButPort PORTb
#define IncButPin 2
#define LedPort PORTb
#define HeaterLedPin 6
#define FanLedPin 4
#define temp_first 0x10
#define temp_last temp_first+10
#define set_last temp_first+12
#define init_flag temp_first+13
u8_t flg1=0;
/**void PicGenios_Init (void)
Description: initializesthe following for the water heater pic genios application :
 * I2C, Temperature  sensor(and the external eeprom) , 7-SEG , Systemclock , buttons , interrupts
Input Value: void
Return Value: void
Notes:this function redirects interrupt functionality for timer0ov to Systemclock*/
void PicGenios_Init (void);
/**Description: initializes the 2 7SEG 
Input Value: void
Return Value: void*/
void SEG_Init(void);
/**Description:  initializes the a 7SEG by enabling it en pin and configures it's port
Input Value: u8_t pin for port choice ,
 * u8_t contport fo enable bit ,
 * u8_t contpin for enable bit port
Return Value: void*/
void SEG_init (u8_t pin ,u8_t contport ,u8_t contpin);
/**void SEG_Flash(u8_t _tim_)
Description: shows the inputed value in 2 segments with a delay
Input Value: inputed number as integer value
Return Value:  void
 note: causes 10 milli second delay*/
void SEG_Flash(u8_t _tim_);
/**void SEG_Blink(void)
Description: toggle the state of the 2 7seg from on to off & off-on 
Input Value: void
Return Value: void
*/
void SEG_Blink(void);
/**void SEG_GetValues(u8_t t)
Description: transform an interger from 0 to 99 to bcd and saves it to the default show array 
Input Value: an integer from 0 to 99
Return Value: void
*/
void SEG_GetValues(u8_t t);
/**void SEG_toogle (u8_t id)
Description:show the preloaded value on one 7SEG 
Input Value: void
Return Value: void*/
void SEG_toogle (u8_t id);
/**void SEG_ToogleVal (u8_t id ,u8_t val)
Description: show value (val) on a certain 7seg (id) 
Input Value: value to be shown from 0 to 99 , id of the 7segement to be shown on 
Return Value: void*/
void SEG_ToogleVal (u8_t id ,u8_t val);
/**void SEG_Stop(void)
Description: frees the 7SEG port from 7Seg operation and disables them
Input Value: void
Return Value: void*/
void SEG_Stop(void);
/**u8_t Cont_FAN(u8_t mode)
Description: gives signal to the fan to start or stop cooling , the signal is deleivred 
 * through precalcauted PWM, also enables the led
Input Value: mode on or off time as integer value
Return Value: void
Notes:this function redirects interrupt functionality for timer0ov*/
void Cont_FAN(u8_t mode);
/**u8_t Cont_Heater(u8_t mode)
Description: gives signal to the heater and the heater led pin to start or stop
Input Value: mode on or off time as integer value
Return Value: void*/
void Cont_Heater(u8_t mode);
/**void TempSensor_Init(void)
Description: enables the temperature  sensor and starts the adc , 
 * also writes the external eeprom for the first time
Input Value: void
Return Value: void*/
void TempSensor_Init(void);
/**void e2pex_InitDef(void)
Description: Initializes  the eeprom for the first time with temp data ,
 * writes first time init_flag on the eeprom
Input Value: void
Return Value: void
 * Notes: this function uses the i2c & adc modules & */
void e2pex_InitDef(void);
/**u8_t e2pext_r(u16_t addr)
Description: reads the word data (8bit) from a certain(16bit) eeprom address (addr)
Input Value: u16_t addr address of the data to be red on the eeprom
Return Value: 8 bit (word) of the value stored that addr
Notes:this function uses the I2C module and causes delay*/
u8_t e2pext_r(u16_t addr);
/**void e2pext_w(u16_t addr, u8_t val)
Description: writes the word data (8bit) (val) tp a certain(16bit) eeprom address (addr)
Input Value: u16_t addr address of the eeprom location to write the data on , u8_t val (byte) to be written
Return Value: void
Notes:this function uses the I2C module and causes delay*/
void e2pext_w(u16_t addr, u8_t val);
/**u8_t read_last_set()
Description: reads the last inputed temp setting from eeprom
Input Value:integear value from 0 to 99 reprerasinting last inputed temp 
Return Value: void
Notes:this function uses the I2C module and causes delay*/
u8_t read_last_set(void);
/**void write_last_set(u8_t t)
Description: update the last inputed temp setting on eeprom
Input Value:integear value from 0 to 99 reprerasinting last inputed temp 
Return Value: void
Notes:this function uses the I2C module and causes delay*/
void write_last_set(u8_t t);
/**u8_t read_last_temp()
Description: reads the last mesearued temp setting from eeprom
Input Value:void 
Return Value: integear value from 0 to 99 reprerasinting last measuerd temp
Notes:this function uses the I2C module and causes delay*/
u8_t read_last_temp(void);
/**u8_t get_averge_temp()
Description: reads the last 10  mesearued temp from eeprom and returns their average
Input Value:void 
Return Value: integear value from 0 to 99 reprerasinting the average
Notes:this function uses the I2C module and causes 10 read delay*/
u8_t get_averge_temp(void);
/**void update_temp_table(void)
Description: pushs the temp array in eeprom & reads the last temp
 *  from sensor then save it to the temp array in eeprom 
Input Value: void
Return Value: void
Notes:this function uses the I2C module and causes 128 read delay 47 ms*/
void update_temp_table(void);
/**void push_table(void)
Description: shifts the postion of each element in the array to the left freeing the last element
Input Value: void
Return Value: void
Notes:this function uses the I2C module and causes 20 read delay*/
void push_table(void);
/**void update_heater(void)
Description: a function containing the tasks that is repeated every 100 ms  :
 * updates tempreture table with a new read
 * if current state is on take a decision to control fan & heater 
 * the function spaces it's reads between 100 ms delay and changes the main control cycle of the program to 0.1 second
 * Input Value: void
Return Value: void
Notes:this function is called by an interruput handler*/
void update_heater(void);
/**void second_update(void)
Description: a function containing the tasks that is repeated every 1 second  :
 * if the heater is on the heater led blinks 
 * if current state is setting count to 5 second from each click & blink the 7seg
Input Value: void
Return Value: void*/
void second_update(void);
/**void Start_stop(void)
Description: a function containing the tasks that is called when INTP0 
 * reintializes the 7seg and systemclk , 
 * change the current state of the while loop to on
 * shuts down the 7seg display , 
 * changes the current state to off
Input Value: void
Return Value: void
Notes:this function is called by an interruput handler*/
void Start_stop(void);
u8_t return_CurrentTemp(void);
#endif // HAL_PICGENIOS_H_INCLUDED
