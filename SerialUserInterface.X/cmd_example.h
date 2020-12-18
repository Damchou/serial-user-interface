/* 
 * File:   cmd_example.h
 */

#ifndef CMD_EXAMPLE_H
#define	CMD_EXAMPLE_H

/* Function prototypes */
void cmd_example_init(void);
void cmd_example_execute(char *params);
void cmd_example_status(void);
void cmd_example_on(void);
void cmd_example_off(void);
void cmd_example_set(char *value);
void cmd_example_help(void);

#endif	/* CMD_EXAMPLE_H */

