#pragma once
#include "GameState.h"
#include "GameplayState.h"
#include "ProjectDugo_Game.h"



class GameoverState : public GameState
{
public:
    GameoverState(ProjectDugo_Game* _game) :GameState(_game)
    {
    }

    GameState* GoToNext() override
    {
        return new GameplayState(game);
    }
};