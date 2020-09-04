#pragma once

#include "ProjectDugo_Game.h"

class GameState
{
public:
    GameState(ProjectDugo_Game _game)
    {
        game = &_game;
    }

    virtual GameState* GoToNext();
    virtual bool IsOver();
    virtual void Run();
protected:
    ProjectDugo_Game* game;
};