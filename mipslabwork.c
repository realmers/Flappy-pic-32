/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   Modified 2024-02-28 by Alexander Danho and Luis Gabriel Patio Mirador

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>
#include <stdio.h>
#define   TMR2PERIOD ((80000000 / 256) / 25 )  // = 31250 < 65 536 (=2^16)
int TacoX = 15;
int TacoY = 5;


// Initieras globalt
volatile int* portE = (volatile int*) 0xbf886110;


// make install TTYDEV=/dev/cu.usbserial-AJV9JY30

/* Interrupt Service Routine */
void user_isr( void )
{
  IFSCLR(0) = 0x100;  
  interuptFlag25fps = 1;
}

/* Lab-specific initialization goes here */
void labinit( void )
{


// Initiera TRISE så att det är output.
volatile int* trise = (volatile int*) 0xbf886100;
*trise = ~0xff;
// PORTE sätts till 0 vid initiering.
*portE = 0;
//Initiera TRISE så att det är input.
TRISD = 0xfe0;

T2CON = 0x70;          // 0111 0000 Vi sätter bit 6:4 till 111 prescale 1:256
                      // set prescaler at 1:1, internal clock source
TMR2 = 0x0;           // Clear timer register
PR2 = TMR2PERIOD;         // Load period register
T2CONSET = 0x8000;         // Start timer

  int *IEC = 0xbf881060;
  int *IPC2 = 0xbf8810b0;

  *IEC = 0x100;     //Vi sätter bit #9 (index 8) se bild sid 90.
  // Interupt Priority Control
  // http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf Sid 90.
  // Bitar 4:2 sätter priotitet 0-7 där 0-3 är Subpriority.

  *IPC2 = *IPC2 | 0x10;   //Vi sätter prioriteret 4 genom att endast bit #5 (index 4)

  enable_interrupt(); // Kör funktion i Vectors.S som slår på ie

  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  int i;
  int j;

  gameScore();

  if (gameState == 1){                    //1 = Game over

    drawGameOver(TacoX, TacoY);
    delay(2000);
    updateGameScore();        // Skriver Gamescore till Highscore array
    gameScoreSort();          // sorterar Highscore
    display_string(0, "  High score:");
    display_score(1, s);      // Skriv ut Highscore
    display_update();

    delay(2000);

    //while( getbtns() != 4 ){}   // Väntar tills användaren trycker på knapp
    display_string_clear();   // Rensar text från skärmen
    StartCountDown();         // Animation som räknar ner.

// Återställer alla objekt på banan
    for(i=0; i<64*2; i++){
      objectPosLevel1[i]=objectPosLevel1Reset[i];
    }

    resetGameField();             //Ändrar tillbaka Taco och Tube x- och y-positioner
    clearScreenMemory();
    display_image(0, icon);
    display_string_clear();
    display_update();
    gameScoreZero();            // Nollställer gamescore
    gameState = 0;              // Gamestate 0 = kör spelet

}


if (gameState == 2){                //2 = Main Menu/Start Screen
  while(gameState == 2){
      countStart++;
      display_string_clear();
      delay(10);
      display_string(2, "  Flappy Taco");
      display_update();


      if(getbtns() == 4){
        MainMenuFade();           //Animation till höger av Flappy Taco
        StartCountDown();         //Kör 3,2,1,go på skärmen

      }
    }
}

clearScreenMemory();
drawTopLine ();
drawBottomLine();

                                                                                  //START: Denna del kodades endast av Gustaf
// Flyttar alla tuber på skärmen
for(i =0 ; i<= 64; i = i + 2){

  drawObjectTube(objectPosLevel1[i], objectPosLevel1[i+1]);   //i+1  = y-värdet
  objectPosLevel1[i]= objectPosLevel1[i]-1;     //Flyttar rören åt vänster

  if (getsw() == 8){    //Om switch längst till vänster är nertryckt
  objectPosLevel1[i+1] = objectPosLevel1[i+1]-1;
  }

  if (getsw() == 4){    //Om switch nästlängst till vänster är nertryckt
  objectPosLevel1[i+1] = objectPosLevel1[i+1]+1;
  }
  if (objectPosLevel1[i+1] == -80) {          //Om tuber
    objectPosLevel1[i+1] = objectPosLevel1[i+1]+120;
  }


  if (objectPosLevel1[i+1] < -80) {
    objectPosLevel1[i+1] = objectPosLevel1[i+1]+1;
  }

}
                                                                                  //SLUT:
// Flyttar taco nedåt (Gravitation)

TacoY += 1;

drawTaco(TacoX, TacoY);
display_image(0, icon);


// Kollar om någon knapp är nedtryckt.

    if ( getbtns() == 1){     // Startar om spelet
      gameState = 2;
    }

    if ( getbtns() == 2 ){    // Taco rör sig uppåt.
      TacoY -= 3;
    }

    if ( getbtns() == 4 ){
    }

// Styr Led-lampor
  *portE += 1;
}
