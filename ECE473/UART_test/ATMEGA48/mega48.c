//
//  mega48.c
//  Created by Brian Sia on 11/30/16.
//
//  Source code for UART temperature sensing.
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "twi_master.c"
#include "usart.c"
#include "lm73_functions_skel.c"

#define BAUD 9600

void readTemperature();

uint8_t           i;
volatile uint8_t  rcv_rdy;
char              rx_char;
char              lcd_str_array[16];  //holds string to send to lcd
uint8_t           send_seq=0;         //transmit sequence number
char              lcd_string[3];      //holds value of sequence number
uint16_t          lm73_temp; //a variable to assemble the temperature from the lm73

void spi_init(void){
    DDRB   = DDRB | 0x07;           //Turn on SS, MOSI, SCLK pins
    SPCR  |= (1<<SPE) | (1<<MSTR);  //set up SPI mode
    SPSR  |= (1<<SPI2X);            //run at double speed
}//spi_init

/****************************************************************
                readTemperature
 reads the temperature with the Atmega48 chip, and then sends it to the ATMEGA128
 ****************************************************************/
void readTemperature(){

    twi_start_rd(LM73_ADDRESS, lm73_rd_buf, 2); //read temperature data from LM73 (2 bytes)  (twi_start_rd())
    _delay_ms(2); //wait for it to finish
    
    //now assemble the two bytes read back into one 16-bit value
    lm73_temp = lm73_rd_buf[0]; //save high temperature byte into lm73_temp
    lm73_temp = lm73_temp << 8; //shift it into upper byte
    lm73_temp |= lm73_rd_buf[1];//"OR" in the low temp byte to lm73_temp
    lm73_temp = lm73_temp >> 7; //getting rid of the fractional parts of the number (first 7 bits)
    itoa(lm73_temp, lcd_str_array, 10); //convert to string in array with itoa() from avr-libc

}

int main(){

    //UART, SPI, and interrupt initializations
    USART0_init(9600);
    spi_init();
    sei();

    while(1){
    
        readTemperature();
        
        //start TX part
        USART_send_string(lcd_str_array);
        USART_transmit('\0');
        
    }//while

}//for