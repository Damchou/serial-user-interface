/* 
 * File:   cmd_led.h
 */

#ifndef CMD_LED_H
#define	CMD_LED_H

/* Function prototypes */
void cmd_led_init(void);
void cmd_led_execute(char *params);
void cmd_led_status(void);
void cmd_led_on(void);
void cmd_led_off(void);
void cmd_led_brightness(char *value);
void cmd_led_help(void);

#endif	/* CMD_LED_H */

