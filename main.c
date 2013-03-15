/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "lcd4.c"

long current_value = 0;

main() {
  char c;
  long current_value_aux;
  int i;

  // ******* Input/Output Configuration ********
  TRISA = 0x00; // PORT A - Output
  TRISB = 0x08; // PORT B: input B3, Output B0-B2,B4-B7

  TMR1CS = 0;
  TMR1IF = 0;
  TMR1H  = 0x00;  TMR1L = 0x00;
  T1CKPS1 = 0;  T1CKPS0 = 0;
  nT1SYNC = 0;
  TMR1IE = 1;
  TMR1ON = 1;

  CCP1M3 = 0; CCP1M2 = 1; CCP1M1 = 0; CCP1M0 = 1;
  CCP1IE = 1;

   GIE = 1;  // Interrupt Enable
  PEIE = 1;


  // initialize the LC Display
  lcd_init();

  // loop forever....
  while(1) {
    // LC cursor left to right moving
    put_cmd(4);

    // put LC cursor at column 10, first line
    adress_ddram(0x89);

    current_value_aux = current_value;
    // Send the sample to LCD by converting into 3 digit BCD value
    for ( i = 0; i < 3; i++ ) {
      c = (current_value_aux % 10) + 48;
      write_lcd_char(c);
      current_value_aux = current_value_aux/10;
    }
  }  // end loop forever

} // end main


/*
 * interrupt - routine
 */
static void interrupt isr(void) {

  // if capture/compare interrupt: update 'current_value' from CCP value
  if ( CCP1IF ) {
    current_value = (CCPR1L & 0x00ff);
    CCP1IF = 0;
  }

  // if timer interrupt: reset timer and toggle Port B.2
  if ( TMR1IF ) {
    RB2 = !RB2;
    TMR1H = 0x00;  TMR1L = 0x00;
    TMR1IF = 0;
  }
}