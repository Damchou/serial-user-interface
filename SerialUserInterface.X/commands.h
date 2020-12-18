/* 
 * File:   commands.h
 * Author: dtek0068
 *
 * Created on 14 December 2020, 21:08
 */

#ifndef COMMANDS_H
#define	COMMANDS_H
#define MAX_NAME_LENGTH 25
#define MAX_DESC_LENGTH 60

/* Structures for commands */

struct command
{
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    void (*execute) (char *params);
    void (*help) (void);
};

/* Function prototypes */
void register_command(struct command);
void execute_command(char *command);
void help_command(char *command);

#endif	/* COMMANDS_H */

