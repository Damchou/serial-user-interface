/*
 * main.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This is an implementation of a Serial User Interface -course project.
 *      It utilises multiple source files, a main file and command files. This
 *      way new commands can be added modularily by adding new files and include
 *      their headers in this file. See cmd_example.c for a template or any 
 *      command file for reference.
 * 
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "input.h"
#include "commands.h"
#include "cmd_led.h"
#include "cmd_button.h"
#include "cmd_temp.h"
#include "cmd_adc.h"
#include "cmd_vref.h"
#include "cmd_reset.h"

int main(void) 
{   
    // Sets sleep mode to IDLE
    set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
    
    input_init();
    
    // Init every known command
    cmd_led_init();
    cmd_button_init();
    cmd_temp_init();
    cmd_adc_init();
    cmd_vref_init();
    cmd_reset_init();
    
    
    printf("\r\nWelcome!\r\n\nTo get started, type HELP and press ENTER or TAB "
            "to execute a command.\r\n(If you are using unmodified PuTTY, "
            "only TAB is available.)\r\n");
    
    printf("\r\n> ");
    
    // Enable interrupts
    sei();
    
    while (1) 
    {
        // Go to idle -sleep mode until interrupted by user input
        sleep_mode();
    }
}