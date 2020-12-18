/*
 * commands.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This processes commands passed from input.c file.
 *
 */

#include "commands.h"
#include <stdio.h>
#include <string.h>
#define EQUAL 0
#define MAX_COMMANDS 16

volatile struct command commands[MAX_COMMANDS];
volatile int commandAmount = 0;

// Each command register themselves to the system through here
inline void register_command(struct command cmd)
{
    if (commandAmount < MAX_COMMANDS)
    {
        commands[commandAmount] = cmd;
        commandAmount++;
    }
}

// Try to execute given command string
inline void execute_command(char *command)
{
    // Make received command string uppercased
    command = strupr(command);
    
    // Split given command
    char* ptr = strtok(command, " ");
    
    // Processing the first word of the command string
    if (ptr != NULL)
    {
        // Help command
        if (strcmp(ptr, "HELP") == EQUAL)
        {
            // Get the next word (possibly first parameter)
            ptr = strtok(NULL, " ");
            help_command(ptr);
        }
        
        // Search the command from command register
        else
        {
            // Search for a right command
            for (int i = 0; i < commandAmount; i++)
            {
                if (strcmp(ptr, commands[i].name) == EQUAL)
                {
                    // Get the next word (possibly first parameter)
                    ptr = strtok(NULL, " ");
                    commands[i].execute(ptr);

                    return;
                }
            }

            printf("\r\nInvalid command %s\r\n", ptr);
        }
    }
}

inline void help_command(char *command)
{
    // NULL parameter, print a list of commands
    if (command == NULL)
    {
        printf("Available commands:\r\n");
        for(int i = 0; i < commandAmount; i++)
        {
            printf("\t%s \t%s\r\n", commands[i].name, commands[i].description);
        }
    }
    
    else
    {
        // Search for a right command
        for (int i = 0; i < commandAmount; i++)
        {
            if (strcmp(command, commands[i].name) == EQUAL)
            {
                commands[i].help();

                return;
            }
        }
        
        printf("%s is not a valid command, check HELP for a list of valid "
                "commands\r\n", command);
    }
}