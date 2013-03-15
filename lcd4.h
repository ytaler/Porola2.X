/* 
 * File:   lcd4.h
 * Author: ytaler
 *
 * Created on 14 de marzo de 2013, 19:16
 */

#ifndef LCD4_H
#define	LCD4_H

void lcd_daley();
void lcd_init();
void put_cmd(unsigned char cmd);
void write_lcd_char(const unsigned char c);
void write_lcd(const char msg[]);
void adress_ddram(int adress);
void lcd_clear();


#endif	/* LCD4_H */

