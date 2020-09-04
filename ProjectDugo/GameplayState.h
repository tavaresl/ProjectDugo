#pragma once

#include "GameState.h"
#include "GameoverState.h"
#include "ProjectDugo_Game.h"

class GameplayState : public GameState
{
public:
    GameplayState(ProjectDugo_Game* _game):GameState(_game)
    {

    }

    GameState* GoToNext() override
    {
        return new GameoverState(game);
    }

    void Run() override
    {

    }
};