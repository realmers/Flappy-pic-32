#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

// omgjord

int getsw(void){
  return (PORTD >> 8) & 0xF;
}

int getbtns(void){
  return (PORTD >> 5) & 0x7;
}
