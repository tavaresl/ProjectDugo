/*#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define OLC_PGE_APPLICATION
#include "olcConsoleGameEngine.h"
#include "SafeHeaven.h"
#include "GameData.h"
#include "Score.h"
#include "Countdown.h"
#include "StateController.h"*/

#include "ProjectDugo_Game.h"


int main()
{
    ProjectDugo_Game game(15);

    if(game.Construct(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, 1, 1))
        game.Start();

    return 0;
}
