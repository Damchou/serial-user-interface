/*
 * cmd_temp.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This command prints the internal temperature.
 * 
 *      Currently does not work properly.
 *
 */

#include "cmd_temp.h"
#include "commands.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#define EQUAL 0

void cmd_temp_init(void)
{   
    // Configure resolution
    ADC0.CTRLA |= ADC_RESSEL_bm;
    
    // Configure 1.1V internal voltage reference
    VREF_CTRLA = 0x1;
    ADC0.CTRLC |= ADC_REFSEL_gp;
    
    // Set ADC Temperature sensor channel
    ADC0.MUXPOS |= ADC_MUXPOS_TEMPSENSE_gc;
    
    // Select Initialization Delay
    ADC0.CTRLD |= ADC_INITDLY_DLY32_gc;
          
    // Select SAMPCAP = 1
    ADC0.CTRLC |= ADC_SAMPCAP_bm;
    
    // Enable ADC
    ADC0.CTRLA |= ADC_ENABLE_bm;
    
    
    // Declare and register the command
    struct command temp;
    
    // Set the name of the command that is called by
    strcpy(temp.name, "TEMP");
    
    // Set the command's description
    strcpy(temp.description, "Prints internal temperature in Celsius");
    
    // Set the pass function which resolves given parameters
    temp.execute = cmd_temp_execute;
    
    // Set the help print function
    temp.help = cmd_temp_help;
    
    register_command(temp);
}

//Execute a TEMP command determined by parameters
inline void cmd_temp_execute(char *params)
{
    // NULL parameter
    if (params == NULL)
    {
        cmd_temp_print();
    }
    
    // Invalid parameter
    else
    {
        printf("Invalid parameter %s, TEMP doesn't take any.\r\n", params);
    }
}

inline void cmd_temp_print(void)
{
    cli();
    
    // Start conversion
    ADC0_COMMAND |= ADC_STCONV_bp;
    
    // Wait for conversion to complete
    while ((ADC0_COMMAND & ADC_STCONV_bp) != 0)
    {
        ;
    };
    
    unsigned int Ctemp = ((((ADC0.RESH << 8) | ADC0.RESL) - SIGROW.TEMPSENSE1) \
                         * SIGROW.TEMPSENSE0) >> 8;
    Ctemp -= 273.15;
    
    printf("Internal temperature is %d C.\r\n", Ctemp);
    
    sei();
}

inline void cmd_temp_help(void)
{
    printf("Prints internal temperature in Celsius. Takes no parameters.\r\n");
}
