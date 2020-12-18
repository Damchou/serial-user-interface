/*
 * cmd_vref.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This command queries or sets ADC internal reference voltage.
 *
 */

#include "cmd_vref.h"
#include "commands.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#define EQUAL 0

void cmd_vref_init(void)
{
    // Declare and register the command
    struct command vref;
    
    // Set the name of the command that is called by
    strcpy(vref.name, "VREF");
    
    // Set the command's description
    strcpy(vref.description, "Internal reference voltage settings "
                                "(HELP VREF for details)");
    
    // Set the pass function which resolves given parameters
    vref.execute = cmd_vref_execute;
    
    // Set the help print function
    vref.help = cmd_vref_help;
    
    register_command(vref);
}

//Execute a VREF command determined by parameters
inline void cmd_vref_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_vref_status();
    }
    
    // SET parameter
    else if (strcmp(params, "SET") == EQUAL)
    {
        params = strtok(NULL, " ");
        cmd_vref_set(params);
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, check HELP VREF.\r\n", params);
    }
}

inline void cmd_vref_status(void)
{
    printf("Internal reference voltage is currently ");
    
    if (VREF.CTRLA & 0x1)
    {
        printf("1V1.\r\n");
    }
    
    else if (VREF.CTRLA & 0x2)
    {
        printf("2V5.\r\n");
    }
    
    else if (VREF.CTRLA & 0x3)
    {
        printf("4V3.\r\n");
    }
    
    else if (VREF.CTRLA & 0x4)
    {
        printf("1V5.\r\n");
    }
    
    else
    {
        printf("0V55.\r\n");
    }
}

inline void cmd_vref_set(char *params)
{
    // NULL parameter
    if(params == NULL)
    {
        printf("You must give voltage value of 0V55, 1V1, 1V5 or 2V5.\r\n");
    }
    
    else if (strcmp(params, "0V55") == EQUAL)
    {
        VREF.CTRLA = 0x0;
        printf("Set reference voltage to %s.\r\n", params);
    }
    
    else if (strcmp(params, "1V1") == EQUAL)
    {
        VREF.CTRLA = 0x1;
        printf("Set reference voltage to %s.\r\n", params);
    }
    
    else if (strcmp(params, "1V5") == EQUAL)
    {
        VREF.CTRLA = 0x4;
        printf("Set reference voltage to %s.\r\n", params);
    }
    
    else if (strcmp(params, "2V5") == EQUAL)
    {
        VREF.CTRLA = 0x2;
        printf("Set reference voltage to %s.\r\n", params);
    }
    
    else
    {
        printf("Invalid parameter %s for VREF SET, check HELP VREF.", params);
    }
}

inline void cmd_vref_help(void)
{
    printf("Available VREF commands:\r\n"
            "\tVREF \t\t\t\tQuery internal reference voltage\r\n"
            "\tVREF SET [0V55|1V1|1V5|2V5] \tSet reference voltage\r\n");
}

