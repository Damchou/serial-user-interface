/* 
 * File:   cmd_adc.h
 */

#ifndef CMD_ADC_H
#define	CMD_ADC_H

/* Function prototypes */
void cmd_adc_init(void);
void cmd_adc_execute(char *params);
void cmd_adc_convert(void);
void cmd_adc_channel(char *params);
void cmd_adc_help(void);

#endif	/* CMD_ADC_H */

