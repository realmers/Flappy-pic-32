#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h"  

// omgjord

int getsw(void){
  return (PORTD >> 8) & 0xF;
}

int getbtns(void){
  return (PORTD >> 5) & 0x7;
}
