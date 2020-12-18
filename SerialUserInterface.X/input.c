/*
 * input.c - Serial User Interface / DTEK0068 (2020) Course Project
 * 
 *      Marko Niiranen <majnii@utu.fi>
 * 
 *  DESCRIPTION
 *      This processes input from the user using USART. Includes Receive 
 *      interrupt.
 *
 */

#include "input.h"
#include "commands.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define MAX_COMMAND_LEN 32
#define F_CPU 3333333
#define USART0_BAUD_RATE(BAUD_RATE) \
((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

void USART0_sendChar(char c);
int USART0_printChar(char c, FILE *stream);
void USART0_init(void);
char USART0_readChar(void);
volatile char c;
volatile char commandStr[MAX_COMMAND_LEN];
volatile int cmd_index = 0;

void USART0_sendChar(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
    {
        ;
    }
    USART0.TXDATAL = c;
}

int USART0_printChar(char c, FILE *stream)
{ 
    USART0_sendChar(c);
    return 0; 
}

FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

void USART0_init(void)
{
    PORTA.DIR &= ~PIN1_bm;
    PORTA.DIR |= PIN0_bm;
    
    USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600); 
    
    // Enable receiver and transmitter, also receive interrupt
    USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
    
    // Enable receive interrupt
    USART0.CTRLA |= USART_RXCIE_bm;
    
    stdout = &USART_stream;
}

char USART0_readChar(void)
{
    while (!(USART0.STATUS & USART_RXCIF_bm))
    {
        ;
    }
    return USART0.RXDATAL;
}

void input_init(void)
{
    // Init USART
    USART0_init();
}

// Input interruption - process user input
ISR(USART0_RXC_vect)
{
    cli();
    
    c = USART0_readChar();
    
    // ENTER or TAB pressed
    if(c == '\n' || c == '\t')
    {
        // Do nothing if user did not enter anything
        if (cmd_index != 0) 
        {
            commandStr[cmd_index] = '\0';
            cmd_index = 0;
            
            printf("\r\n");

            // Pass command string
            execute_command(commandStr);
            
            printf("\r\n> ");
        }
    }
    
    // BACKSPACE pressed (DELETE in PuTTY, ASCII value of 127)
    else if (c == '\b' || c == 127)
    {
        // Do nothing if we're at index 0
        if (cmd_index != 0)
        {
            commandStr[cmd_index] = '\0';
            cmd_index--;
            
            USART0_sendChar(c);
        }
    }
    
    // Character pressed
    else if(c != '\r')
    {
        commandStr[cmd_index++] = c;
        if(cmd_index > MAX_COMMAND_LEN)
        {
            cmd_index = 0;
        }

        // Send received character back to the user terminal
        USART0_sendChar(c);
    }
    
    sei();
}