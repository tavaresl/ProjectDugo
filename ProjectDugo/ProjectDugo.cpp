#include <stdio.h>
#include "olcConsoleGameEngine.h"

class NeighboursData
{
public:
    int left = 1;
    int right = 1;
    int up = 1;
    int down = 1;

};

class GameData
{
private:
    int nNeighboursForMinimumDeath = 1;
    int nNeighboursForMaximumDeath = 4;
    int nNeighboursForCreation = 3;
    NeighboursData neighboursDistance;

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

class ProjectDugo_Game : public olcConsoleGameEngine {
public:
    ProjectDugo_Game(int _lifeArea)
    {
        m_sAppName = L"OLCJAM 2020 - GAME OF LIFE";
        lifeArea = _lifeArea;
    }

    virtual bool OnUserCreate() {

        m_output = new int[ScreenWidth() * ScreenHeight()];
        m_state = new int[ScreenWidth() * ScreenHeight()];
        memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
        memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

        SetInitialData();

        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {

        CheckInputForGameState();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto cell = [&](int x, int y)
        {
            return m_output[y * ScreenWidth() + x];
        };

        for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
            m_output[i] = m_state[i];

        for (int x = gamedata.GetNumberNeighbourDistance().left; x < ScreenWidth() - gamedata.GetNumberNeighbourDistance().right; x++)
            for (int y = gamedata.GetNumberNeighbourDistance().up; y < ScreenHeight() - gamedata.GetNumberNeighbourDistance().down; y++)
            {
                int nNeighbours = GetNumberOfNeighboursActive(x, y);

                if (cell(x, y) == 1)
                    m_state[y * ScreenWidth() + x] = (nNeighbours > gamedata.GetNumberMinNeighbours() && nNeighbours < gamedata.GetNumberMaxNeighbours());
                else
                    m_state[y * ScreenWidth() + x] = nNeighbours == gamedata.GetNumberNeighboursToCreateLife();

                if (cell(x, y) == 1) Draw(x, y, PIXEL_SOLID, FG_WHITE);
                else Draw(x, y, PIXEL_SOLID, FG_BLACK);

            }


        DrawString(0, 0, L"Game Of Life");


        return true;
    }
private:
    GameData gamedata;

    int* m_output;
    int* m_state;
    int lifeArea;


    bool IsInLifeArea(int x, int y, int space)
    {
        int screenMiddleX = (ScreenWidth() / 2);
        int screenMiddleY = (ScreenWidth() / 2);
        return
            (x > screenMiddleX - space && x < screenMiddleX + space &&
                y > screenMiddleY - space && y < screenMiddleY + space);
    }

    void CheckInputForGameState()
    {
        if (/*S*/ m_keys[0x53].bHeld) gamedata.ChangeNumbersToOriginal();
        if (/*Q*/ m_keys[0x51].bHeld) gamedata.ChangeNumberNeighborToCreateLife();
        if (/*E*/ m_keys[0x45].bHeld) gamedata.ChangeNumberNeighborToMinimumDeath();
        if (/*Z*/ m_keys[0x5A].bHeld) gamedata.ChangeNumberNeighborToMaximumDeath();
        if (/*C*/ m_keys[0x43].bHeld) gamedata.ChangeNumberNeighborDistance();

        if (/*W*/ m_keys[0x57].bHeld) gamedata.ChangeDirectionToUp();
        if (/*A*/ m_keys[0x41].bHeld) gamedata.ChangeDirectionToLeft();
        if (/*D*/ m_keys[0x44].bHeld) gamedata.ChangeDirectionToRight();
        if (/*X*/ m_keys[0x58].bHeld) gamedata.ChangeDirectionToDown();

        if (m_keys[VK_SPACE].bHeld) SetInitialData();
    }

    void SetInitialData()
    {
        for (int x = 1; x < ScreenWidth() - 1; x++)
            for (int y = 1; y < ScreenHeight() - 1; y++)
                if (IsInLifeArea(x, y, lifeArea))
                    m_state[y * ScreenWidth() + x] = rand() % 2;

    }

    int GetNumberOfNeighboursActive(int _x, int _y)
    {
        auto cell = [&](int x, int y)
        {
            return m_output[y * ScreenWidth() + x];
        };

        int cellCount = 0;

        for (int x = -gamedata.GetNumberNeighbourDistance().left; x <= gamedata.GetNumberNeighbourDistance().right; x++)
            for (int y = -gamedata.GetNumberNeighbourDistance().up; y <= gamedata.GetNumberNeighbourDistance().down; y++)
                if(x!=0 || y!=0)
                   cellCount += cell(_x - x, _y - y);


        return cellCount;
    }
};

int main()
{
    ProjectDugo_Game game(15);

    if(game.ConstructConsole(80, 60, 8, 8))
        game.Start();

    return 0;
}