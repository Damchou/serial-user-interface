/* 
 * File:   cmd_vref.h
 */

#ifndef CMD_VREF_H
#define	CMD_VREF_H

/* Function prototypes */
void cmd_vref_init(void);
void cmd_vref_execute(char *params);
void cmd_vref_status(void);
void cmd_vref_set(char *params);
void cmd_vref_help(void);

#endif	/* CMD_VREF_H */

