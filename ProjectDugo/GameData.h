#pragma once
#include "NeighboursData.h"

class GameData
{
private:
    NeighboursData neighboursDistance;
    int nNeighboursForMinimumDeath = 1;
    int nNeighboursForMaximumDeath = 4;
    int nNeighboursForCreation = 3;
    int areaOfDebugWindow = 18;
    int tickrate = 10;
    int timelimit = 20;

public:
    int GetNumberMinNeighbours()
    {
        return nNeighboursForMinimumDeath;
    }
    int GetNumberMaxNeighbours()
    {
        return nNeighboursForMaximumDeath;
    }
    int GetNumberNeighboursToCreateLife()
    {
        return nNeighboursForCreation;
    }

    int GetNumberOfDabugArea()
    {
        return areaOfDebugWindow;
    }

    int GetTickRate()
    {
        return tickrate;
    }

    int GetTimelimit()
    {
        return timelimit;
    }

    NeighboursData GetNumberNeighbourDistance()
    {
        return neighboursDistance;
    }

    void ChangeNumberNeighborToCreateLife()
    {
        nNeighboursForCreation = 2;
    }

    void ChangeNumberNeighborToMinimumDeath()
    {
        nNeighboursForMinimumDeath = 2;
    }

    void ChangeNumberNeighborToMaximumDeath()
    {
        nNeighboursForMaximumDeath = 3;
    }

    void ChangeNumberNeighborDistance()
    {
        neighboursDistance.left = 2;
        neighboursDistance.right = 2;
        neighboursDistance.up = 2;
        neighboursDistance.down = 2;
    }

    //Directions 

    void ChangeDirectionToUp()
    {
        ResetNeighboursDistance();
        neighboursDistance.down = 0;
    }

    void ChangeDirectionToDown()
    {
        ResetNeighboursDistance();
        neighboursDistance.up = 0;

    }

    void ChangeDirectionToLeft()
    {
        ResetNeighboursDistance();
        neighboursDistance.right = 0;

    }

    void ChangeDirectionToRight()
    {
        ResetNeighboursDistance();
        neighboursDistance.left = 0;
    }

    void ResetNeighboursDistance()
    {
        neighboursDistance.left = 1;
        neighboursDistance.right = 1;
        neighboursDistance.up = 1;
        neighboursDistance.down = 1;
    }

    //Reset

    void ChangeNumbersToOriginal()
    {
        nNeighboursForMinimumDeath = 1;
        nNeighboursForMaximumDeath = 4;
        nNeighboursForCreation = 3;
        ResetNeighboursDistance();
    }

};
