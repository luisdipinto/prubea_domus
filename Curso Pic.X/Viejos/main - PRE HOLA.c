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


#define _XTAL_FREQ 16000000

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

OSCCON = 0xF0; //0xF0 para 16 MHz   -  0x20 para 500 KHz
TRISA = 0B00111111;
TRISB = 0B00000000;
TRISC = 0B00010000;
TRISD = 0B00001111;
TRISE = 0B00000000;
ANSEL = 0X00;
GIE = 1;
PEIE = 1;
PORTA = 0;
PORTB = 0;
PORTC = 0;
PORTD = 0;
PORTE = 0;

LATCbits.LATC5 = 1;
LATCbits.LATC6 = 1;
LATCbits.LATC7 = 1;
LATAbits.LATA6 = 1;

WriteTimer1(0xF05F);    // 1ms para 16Hmz de Fosc
OpenTimer1(T1_16BIT_RW & T1_PS_1_1 & T1_OSC1EN_OFF & T1_SOURCE_INT);

   TXSTA = 0X24;
   RCSTA = 0X90;

   BAUDCON = 0X08;
   SPBRGH = 0X01;
   SPBRG = 0XA0; //0x01A0  => 9600 BAUD

   PIE1bits.RCIE = 1;

Disp_Var = Var_1;
Disp_Modo = Dec_p;
Disp_ModoL = Led_n;

OpenTimer2(T2_PS_1_16 & T2_POST_1_1);
OpenPWM2(0x9C);     // Ciclo de 50 Hz

//Configuracion del modulo ADC
   ADCON0=0b00000001;       
   ADCON1=0b00000000;
   ADCON2=0b10001101;      //Right Justified  -  Tad=FOsc/16  -  Tacq = 2*Tad
   ANSEL=0X01;     //RA0 es ADC


LATCbits.LATC0 = 1;
LATCbits.LATC5 = 0;
    __delay_ms(40);
LATCbits.LATC0 = 0;
LATCbits.LATC5 = 1;

    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);

LATCbits.LATC0 = 1;
LATCbits.LATC6 = 0;
    __delay_ms(40);
    __delay_ms(40);
LATCbits.LATC0 = 0;
LATCbits.LATC6 = 1;

    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);

LATCbits.LATC0 = 1;
LATCbits.LATC7 = 0;
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
    __delay_ms(40);
LATCbits.LATC0 = 0;
LATCbits.LATC7 = 1;

PWMP = 50;

SSPSTAT = 0XC0;
SSPCON1 = 0X22;



while(1)
{
    SetDCPWM2(PWMP);
    Mostrar();
    Teclado();

    Disp_Var_2H= t_1m;
    Disp_Var_2L= t_1s;

    //Disp_Var_1 = a;
    Disp_Leds = a;

    if(FTT == 1)
    {
        if(TRMT == 1)
        {
            FTT  = 0;
            TXREG = a;
        }
    }

    NOP();

    }
}

void Chk_Botones()
{

    if(PORTAbits.RA1 == 1)
    {
#if 1

#endif
    }

    if(PORTAbits.RA2 == 1)
    {
#if 1

#endif
    }

    if(PORTAbits.RA3 == 1)
    {
#if 1

#endif
    }

    if(PORTAbits.RA4 == 1)
    {
#if 1

#endif
    }

    if(PORTAbits.RA5 == 1)
    {
#if 1

#endif
    }
}

void Tabla(uint8_t numero)
{
switch(numero){
case(0): PORTB=0b11011110; break;
case(1): PORTB=0b10001000; break;
case(2): PORTB=0b10110110; break;
case(3): PORTB=0b10111010; break;
case(4): PORTB=0b11101000; break;
case(5): PORTB=0b01111010; break;
case(6): PORTB=0b01111110; break;
case(7): PORTB=0b10011000; break;
case(8): PORTB=0b11111110; break;
case(9): PORTB=0b11111000; break;
case(10): PORTB=0b11111100; break;
case(11): PORTB=0b01101110; break;
case(12): PORTB=0b01010110; break;
case(13): PORTB=0b10101110; break;
case(14): PORTB=0b01110110; break;
case(15): PORTB=0b01110100; break;
default: PORTB=0b00000000;
}
}

void Tabla0()
{
switch((Disp_Modo)&0x0F)
{
    case(1): PORTB=0b00000001; break;   // .
    case(2): PORTB=0b00000010; break;   // _
    case(3): PORTB=0b00100000; break;   // -
    case(4): PORTB=0b00010000; break;   // ¯
    default: PORTB=0b00000000; break;   // Off
}
}

void Mostrar()
{
    if(F_1ms == 1)      // pregunto si paso un seg
    {
        Armar();
        F_1ms = 0;      // indico que ya contabilicé este ms
        t_mux++;
        if(t_mux == 4)  // cuando pase esta cantidad de ms (6 serian 33 fps)
        {
            t_mux = 0;  // llego el momento de cambiar, reiniciamos la cuenta

            switch (o_mux)
            {
                case(1):
                    o_mux = 2;
                    LATCbits.LATC4 = 0;      // Apagamos los leds
                    Tabla(Disp_U);
                    LATAbits.LATA7 = 1;      // Prendemos el Display de la Unidades
                break;

                case(2):
                    o_mux = 3;
                    LATAbits.LATA7 = 0;      // Apagamos el Display de la Unidades
                    if((Disp_D == 0)&(Disp_C == 0)&(Disp_M == 0))
                    {
                        Tabla0();
                    }
                    else
                    {
                        Tabla(Disp_D);
                    }
                    PORTE = 0b00000100;     // Prendemos el Display de la Decenas
                break;

                case(3):
                    o_mux = 4;
                    PORTE = 0b00000000;     // Apagamos el Display de la Decenas
                    if((Disp_C == 0)&(Disp_M == 0))
                    {
                        Tabla0();
                    }
                    else
                    {
                        Tabla(Disp_C);
                    }
                    PORTE = 0b00000010;     // Prendemos el Display de la Centenas
                break;

                case(4):
                    o_mux = 5;
                    PORTE = 0b00000000;     // Apagamos el Display de la Centenas
                    if(Disp_M == 0)
                    {
                        Tabla0();
                    }
                    else
                    {
                        Tabla(Disp_M);
                    }
                    PORTE = 0b00000001;     // Prendemos el Display de los Miles
                break;

                case(5):
                    o_mux = 1;
                    PORTE = 0b00000000;     // Apagamos el Display de los Miles
                    if(Disp_ModoL == 1)
                    {
                        PORTB = Disp_Leds;
                    }
                    else
                    {
                        LATBbits.LATB0 = Led_RB0;
                        LATBbits.LATB1 = Led_RB1;
                        LATBbits.LATB2 = Led_RB2;
                        LATBbits.LATB3 = Led_RB3;
                        LATBbits.LATB4 = Led_RB4;
                        LATBbits.LATB5 = Led_RB5;
                        LATBbits.LATB6 = Led_RB6;
                        LATBbits.LATB7 = Led_RB7;
                    }
                    LATCbits.LATC4 = 1;      // Prendemos los leds
                break;

                default:
                    o_mux = 1;
                break;
            }
        }

    }

}

void Armar()
{
    uint16_t Aux;

    switch(Disp_Var)
    {
        case(1):    // 2 variables de 2 dispays cada una
        {
            if((Disp_Modo&0xF0) == 0)       // Mostrar Decimales
            {
                if(Disp_Var_2H > 99)        // el numero supera el 99, no se muestra nada
                {
                    Disp_M=0;
                    Disp_C=0;
                }
                else
                {
                    Disp_M=Disp_Var_2H/10;
                    Disp_C=Disp_Var_2H%10;
                }

                if(Disp_Var_2L > 99)        // el numero supera el 99, no se muestra nada
                {
                    Disp_D=0;
                    Disp_U=0;
                }
                else
                {
                    Disp_D=Disp_Var_2L/10;
                    Disp_U=Disp_Var_2L%10;
                }
            }
            else                // Mostar Hexadecimal
            {
                Disp_M = Disp_Var_2H & 0xF0;
                Disp_M = Disp_M >> 4;
                Disp_C = Disp_Var_2H & 0x0F;

                Disp_D = Disp_Var_2L & 0xF0;
                Disp_D = Disp_D >> 4;
                Disp_U = Disp_Var_2L & 0x0F;
            }
            break;
        }

        case(2):    // 4 variables de 1 dispay cada una
        {
            if((Disp_Modo&0xF0) == 0)       // Mostrar Decimales
            {
                if(Disp_Var_4M > 9)        // el numero supera el 9, no se muestra nada
                {
                    Disp_M=0;
                }
                else
                {
                    Disp_M=Disp_Var_4M;
                }
                if(Disp_Var_4C > 9)        // el numero supera el 9, no se muestra nada
                {
                    Disp_C=0;
                }
                else
                {
                    Disp_C=Disp_Var_4C;
                }
                if(Disp_Var_4D > 9)        // el numero supera el 9, no se muestra nada
                {
                    Disp_D=0;
                }
                else
                {
                    Disp_D=Disp_Var_4D;
                }
                if(Disp_Var_4U > 9)        // el numero supera el 9, no se muestra nada
                {
                    Disp_U=0;
                }
                else
                {
                    Disp_U=Disp_Var_4U;
                }
            }
            else                // Mostar Hexadecimal
            {
                Disp_M=Disp_Var_4M;
                Disp_C=Disp_Var_4C;
                Disp_D=Disp_Var_4D;
                Disp_U=Disp_Var_4U;
            }
            break;
        }

        default:    // 1 variables de 4 dispays
        {
            if((Disp_Modo&0xF0) == 0)       // Mostrar Decimales
            {
                if(Disp_Var_1<9999)
                {
                    Aux = Disp_Var_1;
                    Disp_U = Aux % 10;
                    Aux = Aux / 10;

                    Disp_D = Aux % 10;
                    Aux = Aux / 10;

                    Disp_C = Aux % 10;
                    Aux = Aux / 10;

                    Disp_M = Aux;
                    Aux = Aux / 10;
                }

            }
            else                // Mostar Hexadecimal
            {
                Aux = Disp_Var_1;
                Disp_U = Aux & 0x000F;
                Aux = Aux >> 4;
                Disp_D = Aux & 0x000F;
                Aux = Aux >> 4;
                Disp_C = Aux & 0x000F;
                Aux = Aux >> 4;
                Disp_M = Aux & 0x000F;
            }
            break;
        }

    }

}

uint8_t SPI_Check(uint8_t CMD, uint8_t DATA)
{
    uint8_t i, RDATA;
    SSPCON1 = 0x22;
    PIR1bits.SSPIF = 0;
    //clear SSP1BUF
    i = SSPBUF;
    LATAbits.LATA6 = 0;

    SSPBUF = CMD;
    while( PIR1bits.SSPIF == 0 );
    PIR1bits.SSPIF = 0;

    __delay_us(50);

    i = SSPBUF;

    SSPBUF = DATA;
    while( PIR1bits.SSPIF == 0 );
    PIR1bits.SSPIF = 0;
    RDATA = SSPBUF;
    LATAbits.LATA6 = 1;

    SSPCON1 = 0x21;

    return RDATA;
}

void Teclado()
{
    if(F_10ms == 1)      // pregunto si paso un seg
    {
        F_10ms = 0;      // indico que ya contabilicé este ms
        t_tec ++;
        if(t_tec == 4)
        {
            t_tec = 0;
            switch (o_tec)
            {
                case(1):
                    o_tec = 2;
                    if(PORTDbits.RD3 == 1 ){ Tecla = 1; FBA = 1; }
                    if(PORTDbits.RD2 == 1 ){ Tecla = 2; FBA = 1; }
                    if(PORTDbits.RD1 == 1 ){ Tecla = 3; FBA = 1; }
                    if(PORTDbits.RD0 == 1 ){ Tecla = 0x0A; FBA = 1; }
                    LATDbits.LATD7 = 0;
                    LATDbits.LATD6 = 1;
                break;

                case(2):
                    o_tec = 3;
                    if(PORTDbits.RD3 == 1 ){ Tecla = 4; FBA = 1;}
                    if(PORTDbits.RD2 == 1 ){ Tecla = 5; FBA = 1;}
                    if(PORTDbits.RD1 == 1 ){ Tecla = 6; FBA = 1;}
                    if(PORTDbits.RD0 == 1 ){ Tecla = 0x0B; FBA = 1;}
                    LATDbits.LATD6 = 0;
                    LATDbits.LATD5 = 1;
                break;

                case(3):
                    o_tec = 4;
                    if(PORTDbits.RD3 == 1 ){ Tecla = 7; FBA = 1;}
                    if(PORTDbits.RD2 == 1 ){ Tecla = 8; FBA = 1;}
                    if(PORTDbits.RD1 == 1 ){ Tecla = 9; FBA = 1;}
                    if(PORTDbits.RD0 == 1 ){ Tecla = 0x0C; FBA = 1;}
                    LATDbits.LATD5 = 0;
                    LATDbits.LATD4 = 1;
                break;

                case(4):
                    o_tec = 1;
                    if(PORTDbits.RD3 == 1 ){ Tecla = 0x0F; FBA = 1;}
                    if(PORTDbits.RD2 == 1 ){ Tecla = 0; FBA = 1;}
                    if(PORTDbits.RD1 == 1 ){ Tecla = 0x0E; FBA = 1;}
                    if(PORTDbits.RD0 == 1 ){ Tecla = 0x0D; FBA = 1;}
                    LATDbits.LATD4 = 0;
                    LATDbits.LATD7 = 1;
                break;


                default:
                    o_tec = 1;
                break;
            }
            if (FBA == 1){b = 255; FBA = 0; a = Tecla; FTT = 1; TXREG = 1;}
        }

    }
}

void interrupt Interrupciones()
{

#if 1
    if(TMR1IE==1 && TMR1IF==1)
    {

        TMR1IF = 0;
        WriteTimer1(0xF05F);    // 1ms para 16Hmz de Fosc

        F_1ms = 1;
        t_1ms ++;
        if(t_1ms == 10)
        {
            t_1ms = 0;
            F_10ms = 1;
            t_10ms ++;
            if(t_10ms == 10)
            {
                t_10ms = 0;
                F_100ms = 1;
                t_100ms ++;
                if(t_100ms == 10)
                {
                    t_100ms = 0;
                    F_1s = 1;
                    t_1s ++;
                    if(t_1s == 60)
                    {
                        t_1s = 0;
                        F_1m = 1;
                        t_1m ++;
                        if(t_1m == 60)
                        {
                            t_1m = 0;
                            F_1h = 1;
                            t_1h ++;
                            if(t_1h == 24)
                            {
                                t_1h = 0;
                            }
                        }
                    }
                }
            }
        }
    }
#endif // Timer 1 - Calculo de tiempos x1ms x10ms x100ms x1seg x1min x1hora

#if 1

    if (PIR1bits.RCIF == 1) // USART/RX
    {
        if(FRT == 0)
        {
            if (RCREG == 1)
            {
                FRT = 1;        // indico que ya recibi el 1
            }
        }
        else
        {
            Disp_Var_1 = RCREG;
        }
    }


#endif // Usart RX

#if 1
    if(TMR2IE==1 && TMR2IF==1)
    {
        TMR2IF=0;
    }
#endif// Timer 2

}

