/* 
 * File:   cmd_reset.h
 */

#ifndef CMD_RESET_H
#define	CMD_RESET_H

/* Function prototypes */
void cmd_reset_init(void);
void cmd_reset_execute(char *params);
void cmd_reset_restart(void);
void cmd_reset_help(void);

#endif	/* CMD_RESET_H */

