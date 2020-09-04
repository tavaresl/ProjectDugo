#pragma once

#include "GameState.h"
#include "MainMenuState.h"
#include "ProjectDugo_Game.h"

class StateController
{
public:
    StateController(ProjectDugo_Game* game)
    {
        currentState = new MainMenuState(game);
    }

    void Start()
    {
        currentState->Run();
    }

    void Update()
    {
        if (currentState->IsOver())
        {
            currentState = currentState->GoToNext();
            currentState->Run();
        }
    }

private:
    GameState* currentState;
};