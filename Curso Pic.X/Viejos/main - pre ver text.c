#include <xc.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <GenericTypeDefs.h>
#include <timers.h>
#include <adc.h>
#include <pwm.h>
#include <i2c.h>
#include <xlcd.h>



#define _XTAL_FREQ 4000000

#pragma config FOSC = INTIO67 //Oscilador Interno
#pragma config WDTEN = OFF //WDT off
#pragma config PBADEN = OFF //PortB como I/O digital
#pragma config MCLRE = OFF //MCLR off, RE3 on
#pragma config LVP =OFF // Single-Supply ICSP Enable b

void Tabla(uint8_t);
void Tabla0(void);
void Armar(void);
void Mostrar (void);
void Teclado(void);
uint8_t SPI_Check(uint8_t CMD, uint8_t DATA);
uint16_t cont=0, b=0;
uint8_t Tecla=0, t_ADC = 0;
BIT     FBA = 0;
BIT     FTT = 0;
BIT     FRT = 0;
uint24_t a=0;
uint16_t PWMP;

uint8_t prueba;


#if 1
 //         ***** Variables de Tiempos ****
BIT         F_1ms;
BIT         F_10ms;
BIT         F_100ms;
BIT         F_1s;
BIT         F_1m;
BIT         F_1h;

BYTE        t_1ms;
BYTE        t_10ms;
BYTE        t_100ms;
BYTE        t_1s;
BYTE        t_1m;
BYTE        t_1h;


 //         ***** Variables de Displays ****

BYTE        t_mux;              // Tiempo de Multiplexacion
BYTE        o_mux;              // Orden de Multiplexado


uint16_t    Disp_Var_1=0;         // Variable a mostrar en el Display, 1 sola variable, 4 displays

uint8_t     Disp_Var_2H=0;        // Variable a mostrar en el Display, la mas significativa de 2 variables, de 2 displays cada una
uint8_t     Disp_Var_2L=0;        // Variable a mostrar en el Display, la menos significativa de 2 variables, de 2 displays cada una

uint8_t     Disp_Var_4M=0;        // Variable a mostrar en el Display, la de los Miles de 4 variables, de 1 display cada una
uint8_t     Disp_Var_4C=0;        // Variable a mostrar en el Display, la de los Cientos de 4 variables, de 1 display cada una
uint8_t     Disp_Var_4D=0;        // Variable a mostrar en el Display, la de las Decenas de 4 variables, de 1 display cada una
uint8_t     Disp_Var_4U=0;        // Variable a mostrar en el Display, la de las Unidades de 4 variables, de 1 display cada una

BYTE        Disp_Var = 0;       // Inidcara cuantas variables se usaran para el display, segun lo siguiente:
#define     Var_1   0x00        // Usar 1 variable - - > Por defecto
#define     Var_2   0x01        // Usar 2 variables
#define     Var_4   0x02        // Usar 4 variables


BYTE        Disp_Modo = 0;     // Inidcara que y como se muestra, segun lo siguiente:
#define     Dec_o   0x00       // Mostrar la variable(s) en Decimal y ceros a la izquierda: Off - > Por defecto
#define     Dec_p   0x01       // Mostrar la variable(s) en Decimal y ceros a la izquierda: .
#define     Dec_d   0x02       // Mostrar la variable(s) en Decimal y ceros a la izquierda: _
#define     Dec_m   0x03       // Mostrar la variable(s) en Decimal y ceros a la izquierda: -
#define     Dec_u   0x04       // Mostrar la variable(s) en Decimal y ceros a la izquierda: ¯
#define     Hex_o   0x10       // Mostrar la variable(s) en Hexadecimal y ceros a la izquierda: Off
#define     Hex_p   0x11       // Mostrar la variable(s) en Hexadecimal y ceros a la izquierda: .
#define     Hex_d   0x12       // Mostrar la variable(s) en Hexadecimal y ceros a la izquierda: _
#define     Hex_m   0x13       // Mostrar la variable(s) en Hexadecimal y ceros a la izquierda: -
#define     Hex_u   0x14       // Mostrar la variable(s) en Hexadecimal y ceros a la izquierda: ¯



BYTE        Disp_ModoL = 0;     // Inidcara como se usaran los Leds, segun lo siguiente:
#define     Led_b   0x00        // Usar las 8 variables de bits de los leds - > Por defecto
#define     Led_n   0x01        // Mostrar el numero en Binario en los leds

uint8_t     Disp_Leds = 0;      // Variable a mostrar de manera binaria en los leds, multiplexado junto a los Displays (por defecto = 0)

BIT         Led_RB0;            // Led RB0 multiplexado junto a los Displays
BIT         Led_RB1;            // Led RB1 multiplexado junto a los Displays
BIT         Led_RB2;            // Led RB2 multiplexado junto a los Displays
BIT         Led_RB3;            // Led RB3 multiplexado junto a los Displays
BIT         Led_RB4;            // Led RB4 multiplexado junto a los Displays
BIT         Led_RB5;            // Led RB5 multiplexado junto a los Displays
BIT         Led_RB6;            // Led RB6 multiplexado junto a los Displays
BIT         Led_RB7;            // Led RB7 multiplexado junto a los Displays

BYTE        Disp_M=0;             // Digito de los Miles
BYTE        Disp_C=0;             // Digito de las Centenas
BYTE        Disp_D=0;             // Digito de las Decenas
BYTE        Disp_U=0;             // Digito de las Unidades



 //         ***** Variables del Teclado ****

BYTE        o_tec = 1;      // Orden de Multiplexacion del teclado
BYTE        t_tec = 0;


BIT         FCC = 0;


#endif  // < ------ Variables



void main() {

OSCCON = 0x50; //0xF0 para 16 MHz   -  0x20 para 500 KHz
TRISA = 0B00111111;
TRISB = 0B00000000;
TRISC = 0B00010000;
TRISD = 0B00000000;
TRISE = 0B00000000;





	// Colocamos un retardo inicial para dar tiempo a que todos
	// los sistemas se estabilicen despues del encendido
	Delay1KTCYx(5);		//Retardo para iniciar el LCD

	// Inicializamos el LCD con un bus de datos de 8-bits y le
	// indicamos que el LCD contiene varias lineas(en este caso
	// contiene 2) y los caracteres estan definidos en una matriz
	// de 5x7
	OpenXLCD( FOUR_BIT & LINES_5X7);

	WriteCmdXLCD(0); 	// Nos posicionamos en la dereccion 0
	putrsXLCD("LCD: PIC18F4620"); 	// Enviamos un mensaje
	WriteCmdXLCD(0 + 3);  	// Cambiamos de linea y nos
								  	// colocamos en la posicion 3
	putrsXLCD("Por ");				// Colocar aqui su nombre

	// Una vez escrito el mensaje, borramos el cursor y el
	// parpadeo del mismo
	WriteCmdXLCD(CURSOR_OFF & BLINK_OFF);
}

void DelayFor18TCY( void )
{
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
}

void DelayPORXLCD (void)
{
  Delay1KTCYx(15); // Delay of 15ms
                   // Cycles = (TimeDelay * Fosc) / 4
                   // Cycles = (15ms * 4MHz) / 4
                   // Cycles = 15000
  return;
}
void DelayXLCD (void)
{
  Delay1KTCYx(5); // Delay of 5ms
                   // Cycles = (TimeDelay * Fosc) / 4
                   // Cycles = (5ms * 4MHz) / 4
                   // Cycles = 5,000
  return;
}
