/*
 * cmd_reset.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This command restarts the MCU.
 *
 */

#include "cmd_reset.h"
#include "commands.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#define EQUAL 0

void cmd_reset_init(void)
{
    // Declare and register the command
    struct command RESET;
    
    // Set the name of the command that is called by
    strcpy(RESET.name, "RESET");
    
    // Set the command's description
    strcpy(RESET.description, "Reset the microcontroller");
    
    // Set the pass function which resolves given parameters
    RESET.execute = cmd_reset_execute;
    
    // Set the help print function
    RESET.help = cmd_reset_help;
    
    register_command(RESET);
}

//Execute a RESET command determined by parameters
inline void cmd_reset_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_reset_restart();
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, RESET doesn't take any.\r\n", params);
    }
}

inline void cmd_reset_restart(void)
{
    // Allow to write to Configuration Change Protection register to reset
    CCP = CCP_IOREG_gc;
    RSTCTRL.SWRR = RSTCTRL_SWRE_bm;
}

inline void cmd_reset_help(void)
{
    printf("Reset the microcontroller. Takes no parameters.\r\n");
}
