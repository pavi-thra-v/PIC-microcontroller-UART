// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 6000000  //clock signal : 6MHz

void init();
unsigned int trans_val, rec_val;

void main(){
    init();
    while(1){
        trans_val=PORTB;
        switch(trans_val){
            case 0xE0 : TXREG='A';
            break;
            case 0xD0 : TXREG='B';
            break;
            case 0xB0 : TXREG='C';
            break;
            case 0x70 : TXREG='D';
            break;
        }
        if (PIR1&0x20){ //If data is received then
            rec_val=RCREG;
            switch(rec_val){
                case 'a' : PORTD=0x01;
                break;
                case 'b' : PORTD=0x02;
                break;
                case 'c' : PORTD=0x03;
                break;
                case 'd' : PORTD=0x00;
                break;
            }
        }
        __delay_ms(100);
    }
}

void init(){
    TRISC=0x80; //RC7=1(Receiver Bit set as input), RC6=0(Transmitter bit set as output)
    TRISD=0x00;
    TRISB=0xF0; //RB4 to RB7 set as input (Switches are connected to these pins)
    PORTB=0x00; //At Initial stage all pins for PORT B are set low
    PORTD=0X00;
    OPTION_REG&=0x7F; //RB7 is set low to enable Inbuilt pull up register for PORT B pins
    //Initializations for Data Transmission
    TXSTA=0x20; //0010 0000 - Transmit status control register 
    //Initializations for Data Receiver
    RCSTA=0x90; //1001 0000 - Receive status control register
    //Baud rate generation : 9600 bits/sec
    SPBRG=0x09; //X=9
}