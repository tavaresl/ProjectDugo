#include <stdio.h>
#define OLC_PGE_APPLICATION
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
    int areaOfDebugWindow = 18;
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

    int GetNumberOfDabugArea()
    {
        return areaOfDebugWindow;
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

class ProjectDugo_Game : public olc::PixelGameEngine {
public:
    ProjectDugo_Game(int _lifeArea)
    {
        sAppName = "OLCJAM 2020 - GAME OF LIFE";
        lifeArea = _lifeArea;
    }

    bool OnUserCreate() override {

        m_output = new int[GetDiffScreenWidth() * GetDiffScreenHeight()];
        m_state = new int[GetDiffScreenWidth() * GetDiffScreenHeight()];
        memset(m_output, 0, GetDiffScreenWidth() * GetDiffScreenHeight() * sizeof(int));
        memset(m_state, 0, GetDiffScreenWidth() * GetDiffScreenHeight() * sizeof(int));

        SetInitialData();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {

        CheckInputForGameState();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto cell = [&](int x, int y)
        {
            return m_output[y * GetDiffScreenWidth() + x];
        };

        for (int i = 0; i < GetDiffScreenWidth() * GetDiffScreenHeight(); i++)
            m_output[i] = m_state[i];

        for (int x = gamedata.GetNumberNeighbourDistance().left; x < GetDiffScreenWidth() - gamedata.GetNumberNeighbourDistance().right - gamedata.GetNumberOfDabugArea(); x++)
            for (int y = gamedata.GetNumberNeighbourDistance().up; y < GetDiffScreenHeight() - gamedata.GetNumberNeighbourDistance().down; y++)
            {
                int nNeighbours = GetNumberOfNeighboursActive(x, y);

                if (cell(x, y) == 1)
                    m_state[y * GetDiffScreenWidth() + x] = (nNeighbours > gamedata.GetNumberMinNeighbours() && nNeighbours < gamedata.GetNumberMaxNeighbours());
                else
                    m_state[y * GetDiffScreenWidth() + x] = nNeighbours == gamedata.GetNumberNeighboursToCreateLife();

                if (cell(x, y) == 1) FillRect(GetDiffPos(x), GetDiffPos(y), 8,8,olc::WHITE);
                else FillRect(GetDiffPos(x), GetDiffPos(y), 8, 8, olc::BLACK);

            }

        DrawGameBorders();
        DrawDebugBorders();
        DrawGameTexts();


        return true;
    }
private:
    GameData gamedata;

    int* m_output;
    int* m_state;
    int lifeArea;


    bool IsInLifeArea(int x, int y, int space)
    {
        int screenMiddleX = (GetDiffScreenWidth() / 2);
        int screenMiddleY = (GetDiffScreenHeight() / 2);
        return
            (x > screenMiddleX - space && x < screenMiddleX + space &&
                y > screenMiddleY - space && y < screenMiddleY + space);
    }

    void CheckInputForGameState()
    {
        if (GetKey(olc::S).bHeld) gamedata.ChangeNumbersToOriginal();
        if (GetKey(olc::Q).bHeld) gamedata.ChangeNumberNeighborToCreateLife();
        if (GetKey(olc::E).bHeld) gamedata.ChangeNumberNeighborToMinimumDeath();
        if (GetKey(olc::Z).bHeld) gamedata.ChangeNumberNeighborToMaximumDeath();
        if (GetKey(olc::C).bHeld) gamedata.ChangeNumberNeighborDistance();

        if (GetKey(olc::W).bHeld) gamedata.ChangeDirectionToUp();
        if (GetKey(olc::A).bHeld) gamedata.ChangeDirectionToLeft();
        if (GetKey(olc::D).bHeld) gamedata.ChangeDirectionToRight();
        if (GetKey(olc::X).bHeld) gamedata.ChangeDirectionToDown();

        if (GetKey(olc::SPACE).bHeld) SetInitialData();
    }

    void SetInitialData()
    {
        for (int x = 1; x < GetDiffScreenWidth() - 1; x++)
            for (int y = 1; y < GetDiffScreenHeight() - 1; y++)
                if (IsInLifeArea(x, y, lifeArea))
                    m_state[y * GetDiffScreenWidth() + x] = rand() % 2;

    }

    int GetNumberOfNeighboursActive(int _x, int _y)
    {
        auto cell = [&](int x, int y)
        {
            return m_output[y * GetDiffScreenWidth() + x];
        };

        int cellCount = 0;

        for (int x = -gamedata.GetNumberNeighbourDistance().left; x <= gamedata.GetNumberNeighbourDistance().right; x++)
            for (int y = -gamedata.GetNumberNeighbourDistance().up; y <= gamedata.GetNumberNeighbourDistance().down; y++)
                if(x!=0 || y!=0)
                   cellCount += cell(_x - x, _y - y);


        return cellCount;
    }

    void DrawGameBorders()
    {
        for (int x = 0; x < GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea(); x++)
        {
            FillRect(GetDiffPos(x), 0, 8,8, olc::WHITE);
            FillRect(GetDiffPos(x), GetDiffPos(GetDiffScreenHeight() - 1), 8, 8, olc::WHITE);
        }

        for (int y = 0; y < GetDiffScreenHeight(); y++) {
            FillRect(0, GetDiffPos(y), 8, 8, olc::WHITE);
            FillRect(GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() -1), GetDiffPos(y), 8, 8, olc::WHITE);
        }
    }


    void DrawDebugBorders()
    {
        for (int x = GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea(); x < GetDiffScreenWidth(); x++)
        {
            FillRect(GetDiffPos(x), 0, 8, 8, olc::CYAN);
            FillRect(GetDiffPos(x), GetDiffPos(GetDiffScreenHeight() - 1), 8, 8, olc::CYAN);
        }

        for (int y = 0; y < GetDiffScreenHeight(); y++) {
            FillRect(GetDiffPos(GetDiffScreenWidth() -1), GetDiffPos(y), 8, 8, olc::CYAN);
            FillRect(GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea()), GetDiffPos(y), 8, 8, olc::CYAN);
        }
    }

    void DrawGameTexts()
    {
        

        DrawString(
            GetDiffPos(1),
            GetDiffPos(0),
            "Game Of Life", olc::BLACK);

        DrawString(
            GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() + 4),
            GetDiffPos(0),
            "COMMANDS", olc::BLACK);

        DrawDebugText(2, 2, "> SPACE <");
        DrawDebugText(2, 3, "Create Life");

        DrawDebugText(2, 6, "> Q <");
        DrawDebugText(2, 7, "Less Deaths");

        DrawDebugText(2, 10, "> E <");
        DrawDebugText(1, 11, "Change Minimum");
        DrawDebugText(1, 12, "Neighbour Death");

        DrawDebugText(2, 15, "> Z <");
        DrawDebugText(1, 16, "Change Maximum");
        DrawDebugText(1, 17, "Neighbour Death");

        DrawDebugText(2, 20, "> C <");
        DrawDebugText(1, 21, "Change Distance");

        DrawDebugText(2, 24, "> W <");
        DrawDebugText(1, 25, "Go Up");

        DrawDebugText(2, 28, "> A <");
        DrawDebugText(1, 29, "Go Left");

        DrawDebugText(2, 32, "> D <");
        DrawDebugText(1, 33, "Go Right");

        DrawDebugText(2, 36, "> X <");
        DrawDebugText(1, 37, "Go Down");

        DrawDebugText(2, 40, "> S <");
        DrawDebugText(1, 41, "Reset Stats");
    }

    void DrawDebugText(int x, int y, std::string text)
    {
        DrawString(
            GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() + x),
            GetDiffPos(y),
            text);
    }

    int GetDiffScreenWidth()
    {
        return ScreenWidth() / 8;
    }

    int GetDiffScreenHeight()
    {
        return ScreenHeight() / 8;
    }

    int GetDiffPos(int a)
    {
        return a * 8;
    }
};

int main()
{
    ProjectDugo_Game game(15);

    if(game.Construct(80*8, 60*8, 1, 1))
        game.Start();

    return 0;
}