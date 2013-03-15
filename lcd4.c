/*                          lcd4.h

   LCD ( 2x40,Port A:xxSRE & Port B:7654xxxx  )
*/

#include <pic.h>
#include "lcd4.h"

#define ENABLE    RA0
#define LCD_RW    RA1
#define LCD_RS    RA2

#define LCD_BUS   PORTB
                            //( PORTA & 0x18 ) -> RA4, RA3 livre para ser entrada
#define Send_cmd    PORTA = ( PORTA & 0x18 ) | 0x01
#define Ack_cmd     PORTA = ( PORTA & 0x18 ) | 0x00
#define Send_char   PORTA = ( PORTA & 0x18 ) | 0x05
#define Ack_char    PORTA = ( PORTA & 0x18 ) | 0x04

void lcd_daley(){
   int i = 50;
   while (i--);
}



void put_cmd(unsigned char cmd);

void lcd_init()
{
   unsigned char j;

   // lcd_daley de 15 ms
   for (j=0; j < 255; j++) lcd_daley();
   for (j=0; j < 50; j++) lcd_daley();
/*
  	             0x03            ' wait > 4.1 mS, 3<<4 = 48   B7-4
;               0x03            ' wait > .1 mS
;               0x03            ' wait > .1 mS
;               0x02            ' wait > .1 mS   2 << 4 = 32
 */
   Send_cmd;  // S=0 R=0 E=1
   LCD_BUS= (PORTB & 0x0F) | 0x30;  // PORTB = 48
   Ack_cmd;  // S=0 R=0 E=0
   for (j=0; j < 90; j++) lcd_daley();

   Send_cmd;
   LCD_BUS= (PORTB & 0x0F) | 0x30;  // PORTB = 48
   Ack_cmd;
   for (j=0; j < 5; j++) lcd_daley();

   Send_cmd;
   LCD_BUS= (PORTB & 0x0F) | 0x30;  // PORTB = 48
   Ack_cmd;
   for (j=0; j < 5; j++) lcd_daley();

// Configura: Interface = 4bits DL=0, Numero de linhas = 1 N=0, Fonte = 5x7 F=0
   Send_cmd;
   LCD_BUS= (PORTB & 0x0F) | 0x20;  // PORTB = 32
   Ack_cmd;
   for (j=0; j < 5; j++) lcd_daley();

/*
                0x02 0x08  ' Set 4-bit mode, 2 line display
;               0x00 0x08  ' Display "ON"   ?? 0C ?? display ON
;               0x00 0x01  ' Clear Display
;               0x00 0x06  ' Entry mode, auto increment cursor

*/
   put_cmd(40); // Numero de linhas = 2
   put_cmd(12); // display = off cursor = off cursor_blink = off
   put_cmd(1);  // Limpa display e posiciona cursor posicao inicial
   put_cmd(6);  // Desloca cursor pra direita sem mover o display
}

// enviar comando 8bits,
void put_cmd(unsigned char cmd)
{
	Send_cmd;
	LCD_BUS= (PORTB & 0x0F) | (cmd & 0xF0); // envia os 4 mais significativos
	Ack_cmd;
   lcd_daley();

	Send_cmd;
	LCD_BUS=(PORTB & 0x0F ) | cmd<<4; // desloca o numero pra enviar os 4 menos significativos
	Ack_cmd;
   lcd_daley();
}

// escreve dado S=1 R=0
void write_lcd_char(const unsigned char c)
{
	Send_char;
	LCD_BUS= (PORTB & 0x0F) | ((int)c & 0xF0);
	Ack_char;
   lcd_daley();

	Send_char;
	LCD_BUS= (PORTB & 0x0F ) | ((int)c)<<4;
	Ack_char;
	lcd_daley();
}

//escreve palavra no lcd
void write_lcd(const char msg[])
{
	int i=0;

	while(msg[i]!='\0')
	{

		write_lcd_char(msg[i]); // escreve caracter por caracter
		i++;
	}
}

// movimenta o cursor
void adress_ddram(int adress) {
	if( (adress>=0x80) && (adress<0xFF) ) {
		put_cmd(adress);
  	}

}

//limpa display
void lcd_clear()
{
   put_cmd(1);  // Limpa display e posiciona cursor posicao inicial
   put_cmd(2);
}