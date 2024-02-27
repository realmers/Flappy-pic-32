#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"
// by Luis
#define TMR2PERIOD ((80000000 / 256) / 45) /*45fps => 22,222...ms*/

float delayValue = 80;
int referenceX = 15;
int referenceY = 10;
int obstaclesX[] = {100, 140, 200, 250};
int obstacleY[] = {31, 0, 0, 31};
int ceiling = 0;
int floor = 0;
int gameStateBool = 1;
volatile int *portE = (volatile int *)0xbf886110;

void user_isr(void)
{
    IFSCLR(0) = 0x100;
    interruptBool = 1;
}

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
// by Alexander
int checkCollision() {
    	if (((referenceY - 1) >= obstacleY[0] - 18 && (referenceX + 5) == obstaclesX[0]) ||
            (((referenceY - 1) <= (obstacleY[1] + 12)) && ((referenceX + 5) == obstaclesX[1])) ||
            (((referenceY - 1) <= obstacleY[2] + 20) && ((referenceX + 5) == obstaclesX[2])) ||
            (((referenceY - 1) >= (obstacleY[3] - 16)) && ((referenceX + 5) == obstaclesX[3]))) {
            return 1;
        }
    
    return 0;
}

void restartGame() {
    delayValue = 80;
    *portE = 0;
    referenceY = 15;
    referenceX = 10;
    obstaclesX[0] = 100;
    obstaclesX[1] = 140;
    obstaclesX[2] = 200;
    obstaclesX[3] = 250;
}

void labwork(void)
{
    while (gameStateBool)
    {
        display_string(0, "FLAPPY SQUARE");
        display_string(2, "BTN4 TO START");
        display_string(3, "BTN3 TO JUMP");

        display_update();
        if (getbtns() == 4)
        {
            gameStateBool = 0;
        }
    }
	// by Alexander
    display_string(0, "");
    DrawBoxCharacter(referenceX, referenceY);

    MarkObstacles(obstaclesX[0], obstacleY[0], 18, -1);  // Downwards
    MarkObstacles(obstaclesX[1], obstacleY[1], 12, 1);   // Upwards
    MarkObstacles(obstaclesX[2], obstacleY[2], 20, 1);   // Upwards
    MarkObstacles(obstaclesX[3], obstacleY[3], 16, -1); // Downwards

    DrawCeilingAndFloor(ceiling, floor);
    display_image(0, icon);
    MakeScreenBlack();

    referenceY = referenceY + 1;

    delay(delayValue);
    delayValue -= 0.1;

    obstaclesX[0] = obstaclesX[0] - 1;
    obstaclesX[1] = obstaclesX[1] - 1;
    obstaclesX[2] = obstaclesX[2] - 1;
    obstaclesX[3] = obstaclesX[3] - 1;

    if (obstaclesX[0] <= -128) obstaclesX[0] = 128;
    if (obstaclesX[1] <= -128) obstaclesX[1] = 128;
    if (obstaclesX[2] <= -128) obstaclesX[2] = 128;
    if (obstaclesX[3] <= -128) obstaclesX[3] = 128;

    if (referenceX == obstaclesX[0] || referenceX == obstaclesX[1] ||
        referenceX == obstaclesX[2] || referenceX == obstaclesX[3]) {
        *portE += 1;
    }
    if (getbtns() == 2) {
        referenceY = referenceY - 3;
    }
    if (referenceY >= 30 || referenceY <= 0 || checkCollision()) {
        while (1) {
            display_string(0, "GAME OVER");
			display_string(2, "BTN3 TO JUMP");
            display_string(3, "BTN4 TO RESTART");
            display_string(1, "");
            display_update();
            if (getbtns() == 4) {
                restartGame();
                break;
            }
        }
    }
}
