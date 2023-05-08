#include <stdint.h>
#include <stdbool.h>
#include "C:\ti\TivaWare_C_Series-2.2.0.295\inc\tm4c123gh6pm.h" //Definitions for the interrupt and register assignments on the Tiva C
                                                                //Series device on the LaunchPad board

#include "C:\ti\TivaWare_C_Series-2.2.0.295\inc\hw_memmap.h"
#include "C:\ti\TivaWare_C_Series-2.2.0.295\inc\hw_types.h"
#include "C:\ti\TivaWare_C_Series-2.2.0.295\driverlib\sysctl.h"
#include "C:\ti\TivaWare_C_Series-2.2.0.295\driverlib\interrupt.h"//Defines and macros for NVIC Controller (Interrupt) API of
                                                                  //driverLib. This includes API functions such as IntEnable and
                                                                  //IntPrioritySet.

#include "C:\ti\TivaWare_C_Series-2.2.0.295\driverlib\gpio.h"

#include "C:\ti\TivaWare_C_Series-2.2.0.295\driverlib\timer.h"    //Defines and macros for Timer API of driverLib. This includes API
                                                                  //functions such as TimerConfigure and TimerLoadSet.

uint8_t ui8PinData=2;

int main(void){
    uint32_t ui32Period; //Setup clock with unsigned 32-bit interger

    //Clock setup
    SysCtlClockSet (SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); //Setting the system clock to run 40MHz

    //GPIO Config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Enable the GPIO port F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //Enable The GPIO port F1,F2,F3

    //Timer Config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    //Calculate Delay
    ui32Period = (SysCtlClockGet()/ 10) / 2; //-> Sysclock get the ctl clock and divide for the clock required(10Hz) and after divide for 2 to get 50% of duty cycle
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1); //This calculated period is then loaded into the Timer’s Interval Load register using the  TimerLoadSet function of the driverLib Timer API, in the third value is subtract one from timer period because the timer period starts at zero

    //Interrupt Enable
    IntEnable(INT_TIMER0A); //Enable the vector associated with Timer0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //Enable specific event within the timer to generate an interrupt
    IntMasterEnable(); //Enable the master interrupt for all interrupts

    //Timer Enable
    TimerEnable(TIMER0_BASE, TIMER_A);//This will start the timer and interrupts and will trigger all the timeouts

    while(1){
    }
}

void Timer0IntHandler(void){
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and write back the opposite state
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
}
