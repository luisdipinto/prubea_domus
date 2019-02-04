#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <timers.h>
#include <adc.h>
#include "xlcd.h"
#include <plib/delays.h>
#define _XTAL_FREQ 16000000
#pragma config FOSC = INTIO67 //Oscilador Interno
#pragma config WDTEN = OFF //WDT off
#pragma config PBADEN = OFF //PortB como I/O digital
#pragma config MCLRE = OFF //MCLR off, RE3 on
#pragma config LVP =OFF // Single-Supply ICSP Enable bit

uint8_t leerTeclado();
uint8_t teclas[16];
void escribirLCD(char vector[32]);

void DelayFor18TCY(); //delay de 18 instrucciones
void DelayPORXLCD(); //delay de 15ms
void DelayXLCD(); //delay de 5ms

int main(int argc, char** argv) {

OSCCON = 0xF0; //0 salida, 1 entrada
PORTA = 0;
PORTB = 0;
PORTC = 0xE0;
PORTD = 0;
PORTE = 0;
PORTAbits.RA6 = 1;
TRISA = 0B00111111;
TRISB = 0B00000000;
TRISC = 0B00010000;
TRISD = 0B00001111;
TRISE = 0B00000000;

OpenXLCD(FOUR_BIT & LINES_5X7);
while(BusyXLCD());

WriteCmdXLCD(0x06);
while(BusyXLCD());

WriteCmdXLCD(0x0C);
while(BusyXLCD());

WriteCmdXLCD(SHIFT_DISP_LEFT);
while(BusyXLCD());
uint8_t i;
char vector[32];
for(i=0;i<32;i++) vector[i]=' ';
i=0;
while (1) {
uint8_t tecla = leerTeclado();
if (tecla != 0){
vector[i] = '0'+tecla%10;
i = (i+1)%32;
escribirLCD(vector);
}
}
return (EXIT_SUCCESS);
}
void escribirLCD(char vector[32]){

char vector1[16];
uint8_t i;
for (i=0;i<16;i++) vector1[i]=vector[i];
char vector2[16];
for (i=0;i<16;i++) vector2[i]=vector[i+16];
SetDDRamAddr(0X00);
while(BusyXLCD());

putrsXLCD(vector1);
while(BusyXLCD());

SetDDRamAddr(0X40);
while(BusyXLCD());

putrsXLCD(vector2);
while(BusyXLCD());

}
uint8_t leerTeclado() {
// El array guarda los estados anteriores de las teclas
// para poder detectar un flanco. Es decir cuando una
// tecla pasa de 0 a 1.
//static uint8_t teclas[16];
PORTD = 0b10000000;
if (PORTDbits.RD3 == 1 && !teclas[1]) {
teclas[1] = PORTDbits.RD3;
return 1;
}
teclas[1] = PORTDbits.RD3;
if (PORTDbits.RD2 == 1 && !teclas[2]) {
teclas[2] = PORTDbits.RD2;
return 2;
}
teclas[2] = PORTDbits.RD2;
if (PORTDbits.RD1 == 1 && !teclas[3]) {
teclas[3] = PORTDbits.RD1;
return 3;
}
teclas[3] = PORTDbits.RD1;
if (PORTDbits.RD0 == 1 && !teclas[11]) { // 11 -> A
teclas[11] = PORTDbits.RD0;
return 11;
}
teclas[11] = PORTDbits.RD0;
PORTD = 0b01000000;
if (PORTDbits.RD3 == 1 && !teclas[4]) {
teclas[4] = PORTDbits.RD3;
return 4;
}
teclas[4] = PORTDbits.RD3;
if (PORTDbits.RD2 == 1 && !teclas[5]) {
teclas[5] = PORTDbits.RD2;
return 5;
}
teclas[5] = PORTDbits.RD2;
if (PORTDbits.RD1 == 1 && !teclas[6]) {
teclas[6] = PORTDbits.RD1;
return 6;
}
teclas[6] = PORTDbits.RD1;
if (PORTDbits.RD0 == 1 && !teclas[12]) { // 12 -> B
teclas[12] = PORTDbits.RD0;
return 12;
}
teclas[12] = PORTDbits.RD0;
PORTD = 0b00100000;
if (PORTDbits.RD3 == 1 && !teclas[7]) {
teclas[7] = PORTDbits.RD3;
return 7;
}
teclas[7] = PORTDbits.RD3;
if (PORTDbits.RD2 == 1 && !teclas[8]) {
teclas[8] = PORTDbits.RD2;
return 8;
}
teclas[8] = PORTDbits.RD2;
if (PORTDbits.RD1 == 1 && !teclas[9]) {
teclas[9] = PORTDbits.RD1;
return 9;
}
teclas[9] = PORTDbits.RD1;
if (PORTDbits.RD0 == 1 && !teclas[13]) { // 13 -> C
teclas[13] = PORTDbits.RD0;
return 13;
}
teclas[13] = PORTDbits.RD0;
PORTD = 0b00010000;
if (PORTDbits.RD3 == 1 && !teclas[14]) {
teclas[14] = PORTDbits.RD3;
return 14;
}
teclas[14] = PORTDbits.RD3;
if (PORTDbits.RD2 == 1 && !teclas[10]) {
teclas[10] = PORTDbits.RD2;
return 10;
}
teclas[10] = PORTDbits.RD2;
if (PORTDbits.RD1 == 1 && !teclas[15]) {
teclas[15] = PORTDbits.RD1;
return 15;
}
teclas[15] = PORTDbits.RD1;
if (PORTDbits.RD0 == 1 && !teclas[0]) { // 16 -> D
teclas[0] = PORTDbits.RD0;
return 16;
}
teclas[0] = PORTDbits.RD0;
return 0;
}
void DelayFor18TCY(){
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
return;
}
void DelayPORXLCD(){
__delay_ms(15);
}
void DelayXLCD(){
__delay_ms(5);
return;
}