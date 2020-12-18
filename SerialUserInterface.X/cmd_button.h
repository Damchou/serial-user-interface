/* 
 * File:   cmd_button.h
 */

#ifndef CMD_BUTTON_H
#define	CMD_BUTTON_H

/* Function prototypes */
void cmd_button_init(void);
void cmd_button_execute(char *params);
void cmd_button_status(void);
void cmd_button_inv(char *params);
void cmd_button_pup(char *params);
void cmd_button_help(void);

#endif	/* CMD_BUTTON_H */

