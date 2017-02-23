char              rx_char; //UART Example for inclass coding
//Roger Traylor 12.4.12
//Connect two mega128 boards via rs232 and they will send to each
//other a message and a sequence number.


#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "uart_functions.h"
#include "hd44780.h"
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t           i;
volatile uint8_t  rcv_rdy;
char              rx_char; 
char              lcd_str_array[16];  //holds string to send to lcd
uint8_t           send_seq=0;         //transmit sequence number
char              lcd_string[3];      //holds value of sequence number

void spi_init(void){
  DDRB   = DDRB | 0x07;           //Turn on SS, MOSI, SCLK pins
  SPCR  |= (1<<SPE) | (1<<MSTR);  //set up SPI mode
  SPSR  |= (1<<SPI2X);            //run at double speed 
}//spi_init    

int main(){
      DDRF |= 0x08; //lcd strobe bit
      uart_init();  
      spi_init();
      lcd_init();
      clear_display();
      cursor_home();

      sei();
      while(1){
    //**************  start rcv portion ***************
          if(rcv_rdy==1){
            string2lcd(lcd_str_array);  //write out string if its ready
            rcv_rdy=0;
            cursor_home(); //? equivalent in LCDDriver?
        }//if 
    //**************  end rcv portion ***************

    //**************  start tx portion ***************
        uart_puts("Hi! Dilbert: ");
        itoa(send_seq,lcd_string,10);
        uart_puts(lcd_string);
        uart_putc('\0');
        for(i=0;i<=9;i++){_delay_ms(100);}
        send_seq++;
        send_seq=(send_seq%20);
    //**************  end tx portion ***************
      }//while
}//main

ISR(USART0_RX_vect){
//USART0 RX complete interrupt
static  uint8_t  i;
  rx_char = UDR0;              //get character
  lcd_str_array[i++]=rx_char;  //store in array 
 //if entire string has arrived, set flag, reset index
  if(rx_char == '\0'){
    rcv_rdy=1; 
    lcd_str_array[--i]  = (' ');     //clear the count field
    lcd_str_array[i+1]  = (' ');
    lcd_str_array[i+2]  = (' ');
    i=0;  
  }
}
//************************************//

