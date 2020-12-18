/*
 * cmd_led.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This is a command that prints LED status, sets brightness and on/off 
 *      state.
 *
 */

#include "cmd_led.h"
#include "commands.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define EQUAL 0
#define TCB_CMP_VALUE   (0x126)

void tcb_init(void);
volatile enum ledState { On, Off };
volatile enum ledState currentLedState;
const unsigned int PERIOD = 255;
volatile unsigned int duty;
volatile unsigned int currentDuty;

void cmd_led_init(void)
{
    tcb_init();
    
    // Set PF5 (PORTF pin 5) as output (it will drive the LED)
    PORTF.DIRSET = PIN5_bm;
    
    // LED start as off
    cmd_led_off();
    
    // Set initial brightness to max
    duty = PERIOD;
    
    
    // Declare and register the command
    struct command led;
    
    // Set the name of the command that is called by
    strcpy(led.name, "LED");
    
    // Set the command's description
    strcpy(led.description, "LED settings (HELP LED for details)");
    
    // Set the pass function which resolves given parameters
    led.execute = cmd_led_execute;
    
    // Set the help print function
    led.help = cmd_led_help;
    
    register_command(led);
}

void tcb_init(void)
{
    /* Disable CLK_PER Prescaler */
    CLKCTRL.MCLKCTRLB = 0 << CLKCTRL_PEN_bp; 
    
    /* Select 32KHz Internal Ultra Low Power Oscillator (OSCULP32K) */
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
    
    /* Wait for system oscillator changing to finish */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        ;
    }
    
    /* Load the Compare or Capture register with the timeout value*/
    TCB0.CCMP = TCB_CMP_VALUE;
   
    /* Enable TCB and set CLK_PER divider to 1 (No Prescaling) */
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm | TCB_RUNSTDBY_bm;
    
    /* Enable Capture or Timeout interrupt */
    TCB0.INTCTRL = TCB_CAPT_bm;
}

// Execute a LED command determined by parameters
inline void cmd_led_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_led_status();
    }
    
    // ON parameter
    else if (strcmp(params, "ON") == EQUAL)
    {
        cmd_led_on();
    }
    
    // OFF parameter
    else if (strcmp(params, "OFF") == EQUAL)
    {
        cmd_led_off();
    }
    
    // SET parameter
    else if (strcmp(params, "SET") == EQUAL)
    {
        params = strtok(NULL, " ");
        cmd_led_brightness(params);
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, check HELP LED.\r\n", params);
    }
}

inline void cmd_led_status(void)
{
    // Print ON/OFF, brightness
    
    printf("LED is currently ");
    
    if (PORTF.OUT & PIN5_bm) 
    {
        printf("off ");
    }
    else
    {
        printf("on ");
    }
    
    printf("and the brightness is set to %d.\r\n", duty);
}

inline void cmd_led_on(void)
{
    currentLedState = On;
    
    // Set brightness interrupts
    TCB0.CTRLA = TCB_ENABLE_bm;
    
    PORTF.OUTCLR = PIN5_bm;
    
    printf("LED set ON.\r\n");
}

inline void cmd_led_off(void)
{
    currentLedState = Off;
    
    // Cancel brightness interrupts
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
    
    PORTF.OUTSET = PIN5_bm;
    
    printf("LED set OFF.\r\n");
}

inline void cmd_led_brightness(char *value)
{
    unsigned int newBrightness = atoi(value);
    
    // NULL or out-of-range parameter
    if(value == NULL || (newBrightness < 0 || newBrightness > 255))
    {
        printf("You must give brightness value between 0 to 255.\r\n");
    }
    
    // Proceed to set the LED's brightness
    else 
    {
        duty = newBrightness;
        printf("Set LED brightness to %d.\r\n", newBrightness);
    }
}

// Timed interruption for led brightness
ISR(TCB0_INT_vect)
{
    TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
    
    if (currentLedState == On)
    {
        if (currentDuty < PERIOD)
        {
            currentDuty++;
        }

        else
        {
            currentDuty = 0;
        }

        // Set LED on (LOW)
        if (0 < duty && duty >= currentDuty)
        {
            PORTF.OUTCLR = PIN5_bm;
        }

        // Set LED off (HIGH)
        else
        {
            PORTF.OUTSET = PIN5_bm;
        }
    }
}

// Prints information about this command when HELP is called on this
inline void cmd_led_help(void)
{
    printf("Available LED commands:\r\n"
            "\tLED \t\t\tQuery LED brightness and ON/OFF state\r\n"
            "\tLED [ON|OFF] \t\tTurn LED on or off\r\n"
            "\tLED SET <n> \t\tSet LED brightness (0 <= n <= 255)\r\n");
}