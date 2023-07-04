// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS     // Oscillator Selection bits (RC oscillator)
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
#define _XTAL_FREQ 20000000  //clock signal : 6MHz

void init();
void Lcd_Command(unsigned char);
void Lcd_Data(unsigned char);
void LcdOutput(unsigned int);
void delay(unsigned int);
unsigned int tran_val, rec_val;
unsigned char rec[20]={"RECEIVED = "};
unsigned char tran[20]={"TRANSMITTED = "};

void main(){
    init();
    while(1){
        if (PIR1&0x20){ //If data is received then
            rec_val=RCREG;
            Lcd_Command(0x8B);
            Lcd_Data(rec_val);
            switch(rec_val){
                case 'A' : 
                    TXREG='a';
                break;
                case 'B' : 
                    TXREG='b';
                break;
                case 'C' : 
                    TXREG='c';
                break;
                case 'D' : 
                    TXREG='d';
                break;
            }
            tran_val=TXREG;
            Lcd_Command(0xCE);
            Lcd_Data(tran_val);
        }
        __delay_ms(100);
    }
}

void init(){
    TRISC=0x80; //RC7=1(Receiver Bit set as input), RC6=0(Transmitter bit set as output)
    TRISD=0x00; //RD0 to RD7 are set as output (For LCD Connection)
    //Initializations for Data Transmission
    TXSTA=0x20; //0010 0000 - Transmit status control register 
    //Initializations for Data Receiver
    RCSTA=0x90; //1001 0000 - Receive status control register
    //Baud rate generation : 9600 bits/sec
    SPBRG=0x1F; //X=31
    //data sheet protocol followed
    Lcd_Command(0x38); //0011 xxxx
    __delay_ms(100);
    Lcd_Command(0x38); //0011 xxxx
    __delay_ms(100);
    Lcd_Command(0x38); //0011 xxxx
    __delay_ms(100);
    Lcd_Command(0x38); //0011 1000 : N=1(2 line display), F=0(5x7) Function set
    __delay_ms(100);
    Lcd_Command(0x08); //0000 1000 : Display OFF
    __delay_ms(100);
    Lcd_Command(0x01); //0000 0001 : Clear Display
    __delay_ms(100);
    Lcd_Command(0x0C); //0000 1100 : Display ON, Cursor OFF, Blink OFF
    __delay_ms(100);
    Lcd_Command(0x80);
    for (int i=0; i<11; i++){
        Lcd_Data(rec[i]);
    }
    Lcd_Command(0xC0);
    for (int i=0; i<14; i++){
        Lcd_Data(tran[i]);
    }
}

void Lcd_Command(unsigned char i)
{
    PORTC&=0xF7;;
    PORTD=i;
    PORTC|=0x01;
    PORTC&=~0x01;
    delay(100);
}

void Lcd_Data(unsigned char i)
{
    PORTC|=0x08;
    PORTD=i;
    PORTC|=0x01;
    PORTC&=~0x01;
    delay(100);
}

void delay(unsigned int delaycount)
{
    while(--delaycount);
}
