/*
 * cmd_example.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This is an example template of a command that can be used to create more
 *      commands for the system.
 * 
 *      Simply create a new c file in the source direction, copy this file's
 *      code to it and implement desired functionality. You must also include
 *      header file and call the command's init -function inside the main file.
 * 
 *      See other implemented commands for reference if needed.
 * 
 */

#include "cmd_example.h"
#include "commands.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#define EQUAL 0

void cmd_example_init(void)
{
    /*
     *  Include any necessary initialization here, create a new command struct
     *  and register the command for the system.
     */
    
    // Declare and register the command
    struct command example;
    
    // Set the name of the command that is called by
    strcpy(example.name, "EXAMPLE");
    
    // Set the command's description (check commands.h for description length!)
    strcpy(example.description, "Example command (HELP EXAMPLE for details)");
    
    // Set the pass function which resolves given parameters
    example.execute = cmd_example_execute;
    
    // Set the help print function
    example.help = cmd_example_help;
    
    register_command(example);
}

/*  
 *  Execute a EXAMPLE command determined by parameters 
 *  Here you can add every possible parameters this command takes.
 */
void cmd_example_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_example_status();
    }
    
    // ON parameter
    else if (strcmp(params, "ON") == EQUAL)
    {
        cmd_example_on();
    }
    
    // OFF parameter
    else if (strcmp(params, "OFF") == EQUAL)
    {
        cmd_example_off();
    }
    
    // SET parameter with an additional parameter
    else if (strcmp(params, "SET") == EQUAL)
    {
        params = strtok(NULL, " ");
        cmd_example_set(params);
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, check HELP EXAMPLE.\r\n", params);
    }
}

void cmd_example_status(void)
{
    /* For this example we could print status when given no parameters */
}

void cmd_example_on(void)
{
    /* With a single parameter ON we could set something on */
}

void cmd_example_off(void)
{
    /* With a single parameter OFF we could set something off */
}

void cmd_example_set(char *params)
{
    /* With multiple parameters we could set values of something */
}

void cmd_example_help(void)
{
    /* Here comes the help print when user calls HELP for this command */
    
    printf("Available EXAMPLE commands:\r\n"
            "\tEXAMPLE \t\t\tQuery the status of EXAMPLE\r\n"
            "\tEXAMPLE [ON|OFF] \t\tTurn something on or off\r\n"
            "\tEXAMPLE SET <n> \t\tSet some value (0 <= n <= 1)\r\n");
}
