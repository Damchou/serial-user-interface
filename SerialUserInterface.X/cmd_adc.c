/*
 * cmd_adc.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This command performs ADC conversion.
 *
 *      Currently does not work properly.
 */

#include "cmd_adc.h"
#include "commands.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#define EQUAL 0

void cmd_adc_init(void)
{
    // Init ADC
    ADC0.CTRLA = ADC_ENABLE_bm;
    
    
    // Declare and register the command
    struct command adc;
    
    // Set the name of the command that is called by
    strcpy(adc.name, "ADC");
    
    // Set the command's description
    strcpy(adc.description, "Performs ADC conversion (HELP ADC for details)");
    
    // Set the pass function which resolves given parameters
    adc.execute = cmd_adc_execute;
    
    // Set the help print function
    adc.help = cmd_adc_help;
    
    register_command(adc);
}

//Execute a ADC command determined by parameters
inline void cmd_adc_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_adc_convert();
    }
    
    // SET parameter
    else if (strcmp(params, "SET") == EQUAL)
    {
        params = strtok(NULL, " ");
        cmd_adc_channel(params);
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, check HELP ADC.\r\n", params);
    }
}

inline void cmd_adc_convert(void)
{
    cli();
    
    // Start conversion
    ADC0_COMMAND |= ADC_STCONV_bp;
    
    // Wait for conversion to complete
    while ((ADC0_COMMAND & ADC_STCONV_bp) != 0)
    {
        ;
    };
    
    unsigned int result = ADC0.RES;
    
    printf("ADC conversion result is %d.\r\n", result);
    
    sei();
}

inline void cmd_adc_channel(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        printf("ADC SET takes a parameter between AN0 and AN8.\r\n");
    }
    
    else if (params[0] == 'A' && params[1] == 'N' && strlen(params) == 3)
    {
        unsigned int newChannel = params[2] - '0';
    
        // Out-of-range parameter
        if(newChannel < 0 || newChannel > 8)
        {
            printf("You must give channel value between AN0 to AN8.\r\n");
        }

        // Proceed to set the new ADC channel
        else 
        {
            ADC0_MUXPOS = newChannel;
            printf("Set ADC channel to %d.\r\n", newChannel);
        }
    }
    
    else
    {
        printf("Invalid parameter %s for ADC SET. Check HELP ADC.\r\n", params);
    }
}

inline void cmd_adc_help(void)
{
    printf("Available ADC commands:\r\n"
            "\tADC \t\tADC convert and print value\r\n"
            "\tADC SET AN<n> \tSet input channel (0 <= n <= 8)\r\n");
}