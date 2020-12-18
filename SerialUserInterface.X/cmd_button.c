/*
 * cmd_button.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This is a command that configures button's invert and pull-up resistor.
 *
 */

#include "cmd_button.h"
#include "commands.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#define EQUAL 0

void cmd_button_init(void)
{
    // Set PF6 as input (the pin is connected to the PCB push-button)
    PORTF.DIRCLR = PIN6_bm;
    
    
    // Declare and register the command
    struct command button;
    
    // Set the name of the command that is called by
    strcpy(button.name, "BUTTON");
    
    // Set the command's description
    strcpy(button.description, "Button commands (HELP BUTTON for details)");
    
    // Set the pass function which resolves given parameters
    button.execute = cmd_button_execute;
    
    // Set the help print function
    button.help = cmd_button_help;
    
    register_command(button);
}

// Execute a BUTTON command determined by parameters
inline void cmd_button_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_button_status();
    }
    
    // INV parameter
    else if (strcmp(params, "INV") == EQUAL)
    {
        params = strtok(NULL, " ");
        cmd_button_inv(params);
    }
    
    // PUP parameter
    else if (strcmp(params, "PUP") == EQUAL)
    {
        params = strtok(NULL, " ");
        cmd_button_pup(params);
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, check HELP BUTTON.\r\n", params);
    }
}

inline void cmd_button_status(void)
{
    // Print button HIGH/LOW state, invert and pull-up resistor states
    
    printf("Button is in ");
    
    if (PORTF.IN & PIN6_bm)
    {
        printf("HIGH state, ");
    }
    else
    {
        printf("LOW state, ");
    }
    
    printf("and has currently ");
    
    if (PORTF.PIN6CTRL & PORT_INVEN_bm) 
    {
        printf("invert ON ");
    }
    else
    {
        printf("invert OFF ");
    }
    
    if (PORTF.PIN6CTRL & PORT_PULLUPEN_bm)
    {
        printf("and pull-up resistor ON.\r\n");
    }
    else
    {
        printf("and pull-up resistor OFF.\r\n");
    }
}

inline void cmd_button_inv(char *params)
{
    // Enable invert
    if (strcmp(params, "ON") == EQUAL)
    {
        printf("Set invert to ON\r\n");
        PORTF.PIN6CTRL |= PORT_INVEN_bm;
    }
    
    // Disable invert
    else if (strcmp(params, "OFF") == EQUAL)
    {
        printf("Set invert to OFF\r\n");
        PORTF.PIN6CTRL &= ~PORT_INVEN_bm;
    }
    
    // Invalid parameter
    else
    {
        printf("You must give ON or OFF parameter for invert.\r\n");
    }
}

inline void cmd_button_pup(char *params)
{
    // Enable pull-up resistor
    if (strcmp(params, "ON") == EQUAL)
    {
        printf("Set pull up to ON\r\n");
        PORTF.PIN6CTRL |= PORT_PULLUPEN_bm;
    }
    
    // Disable pull-up resistor
    else if (strcmp(params, "OFF") == EQUAL)
    {
        printf("Set pull up to OFF\r\n");
        PORTF.PIN6CTRL &= ~PORT_PULLUPEN_bm;
    }
    
    // Invalid parameter
    else
    {
        printf("You must give ON or OFF parameter for pull-up resistor.\r\n");
    }
}

// Prints information about this command when HELP is called on this
inline void cmd_button_help(void)
{
    printf("Available BUTTON commands:\r\n"
            "\tBUTTON \t\t\tQuery BUTTON invert and pull-up resistor state\r\n"
            "\tBUTTON INV [ON|OFF] \tConfigure state invert\r\n"
            "\tBUTTON PUP [ON|OFF] \tConfigure pull-up resistor\r\n");
}