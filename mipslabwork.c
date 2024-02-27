/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

//By Luis, 1000/45
#define TMR2PERIOD ((80000000 / 256) / 45) /*45fps => 22,222...ms*/
float delayValue = 80;

// by Alexander
int referenceX = 15;
int referenceY = 10;

int obstaclesX[] = {128, 180, 240, 300};
int obsy[] = {31, 0, 0, 31};

int ceiling = 0;
int floor = 0;
// by Luis
int gameStateBool = 1;

volatile int *portE = (volatile int *)0xbf886110;

void user_isr(void)
{
	IFSCLR(0) = 0x100;
	interruptBool = 1;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
	volatile int *trisE = (volatile int *)0xbf886100;

	*trisE = *trisE & 0xFF00;

	TRISD = TRISD & 0x0fe0;

	PR2 = TMR2PERIOD;
	TMR2 = 0;		   
	T2CONSET = 0X8070; 

	IEC(0) = 0x100; 

	IPC(2) = 0x1f; 

	enable_interrupt();
	return;
}

/* This function is called repetitively from the main program */
void labwork(void)
{

	while (gameStateBool)
	{

		display_string(0, "Flappy square");
		display_string(2, "BTN4 to gameStateBool");
		display_string(3, "BTN3 to jump ");

		display_update();

		if (getbtns() == 4)
		{
			gameStateBool = 0;
		}
	}
	display_string(0, "");

	// TODO lägg till ghost mode/hard mode ifall man pressar en knapp i gameStateBool screen

	DrawBoxCharacter(referenceX, referenceY);

	// By Alexander
	MarkObstacles(obstaclesX[0], obsy[0], 14, -1);	// Downwards
	MarkObstacles(obstaclesX[1], obsy[1], 10, 1);	// Upwards
	MarkObstacles(obstaclesX[2], obsy[2], 19, 1);	// Upwards
	MarkObstacles(obstaclesX[3], obsy[3], 18, -1); // Downwards

	DrawCeilingAndFloor(ceiling, floor);
	
	display_image(0, icon);
	
	MakeScreenBlack();

	referenceY = referenceY + 1;

	// by Luis
	
	delay(delayValue);
	delayValue -= 0.1;

	obstaclesX[0] = obstaclesX[0] - 1;
	obstaclesX[1] = obstaclesX[1] - 1;
	obstaclesX[2] = obstaclesX[2] - 1;
	obstaclesX[3] = obstaclesX[3] - 1;

	if (obstaclesX[0] <= -128)
	{
		obstaclesX[0] = 128;
	}

	if (obstaclesX[1] <= -128)
	{
		obstaclesX[1] = 128;
	}

	if (obstaclesX[2] <= -128)
	{
		obstaclesX[2] = 128;
	}

	if (obstaclesX[3] <= -128)
	{
		obstaclesX[3] = 128;
	}
	

	if (referenceX == obstaclesX[0])
	{
		*portE = *portE + 1;
	}
	if (referenceX == obstaclesX[1])
	{
		*portE = *portE + 1;
	}
	if (referenceX == obstaclesX[2])
	{
		*portE = *portE + 1;
	}
	if (referenceX == obstaclesX[3])
	{
		*portE = *portE + 1;
	}

	if (getbtns() == 2)
	{
		referenceY = referenceY - 3;
	}

	int whileloop = 1;
	if (referenceY >= 30 || referenceY <= 0 || ((referenceY - 1) >= obsy[0] - 14 && (referenceX + 5) == obstaclesX[0]) ||
		(((referenceY - 1) <= (obsy[1] + 10)) && ((referenceX + 5) == obstaclesX[1])) || (((referenceY - 1) <= obsy[2] + 19) && ((referenceX + 5) == obstaclesX[2])) || (((referenceY - 1) >= (obsy[3] - 18)) && ((referenceX + 5) == obstaclesX[3])))
	{
		// ifall vi har någon kollision in i loopen pga 1
		while (whileloop)
		{
			display_string(0, "Game over");
			display_string(2, "BTN4 to restart");
			display_string(1, "");
			// display update, annars kommer inte att displayed uppdateras och vi ser all text
			display_update();
			// reseta allt till deras originella positioner, och ut ur loop
			//  btn 4
			if (getbtns() == 4)
			{
				delayValue = 80;
				*portE = 0;

				whileloop = 0;
				referenceY = 15;
				referenceX = 10;
				obstaclesX[0] = 128;
				obstaclesX[1] = 180;
				obstaclesX[2] = 240;
				obstaclesX[3] = 300;
			}
		}
	}
}
