#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define OLC_PGE_APPLICATION
#include "olcConsoleGameEngine.h"
#include "SafeHeaven.h"
#include "GameData.h"

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 60;
const int SCALE = 8;

class ProjectDugo_Game : public olc::PixelGameEngine {
public:
    ProjectDugo_Game(int _lifeArea)
    {
        sAppName = "OLCJAM 2020 - GAME OF LIFE";
        lifeArea = _lifeArea;
        srand(time(NULL));
        randState = rand() % 100;
    }

    bool OnUserCreate() override {

        m_output = new int[GetDiffScreenWidth() * GetDiffScreenHeight()];
        m_state = new int[GetDiffScreenWidth() * GetDiffScreenHeight()];

        memset(m_output, 0, GetDiffScreenWidth() * GetDiffScreenHeight() * sizeof(int));
        memset(m_state, 0, GetDiffScreenWidth() * GetDiffScreenHeight() * sizeof(int));

        SetInitialData();

        CreateSafeHeaven();
        DrawDebugBorders();
        DrawGameTexts();

        return true;
    }

    void CreateSafeHeaven()
    {
        //SeedRandomIfFirstRun();

        int safeHeavenSize = SCALE * 32;
        int debugAreaOffset = gamedata.GetNumberOfDabugArea() * SCALE;
        int widthOffset = safeHeavenSize + debugAreaOffset;
        int width = ScreenWidth() - widthOffset;
        int height = ScreenHeight() - safeHeavenSize;
        int randomX = Rand() % width;
        int randomY = Rand() % height;
        
        safeHeaven = new SafeHeaven(randomX, randomY, safeHeavenSize, safeHeavenSize, olc::GREEN);
    }
    bool OnUserUpdate(float fElapsedTime) override {

        CheckInputForGameState();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        bool isAnyCellOutsideSafeHeaven = false;
        bool isAnyCellAlive = false;

        auto cellAt = [&](int x, int y)
        {
            return m_output[y * GetDiffScreenWidth() + x];
        };

        for (int i = 0; i < GetDiffScreenWidth() * GetDiffScreenHeight(); i++)
            m_output[i] = m_state[i];

        for (int x = gamedata.GetNumberNeighbourDistance().left; x < GetDiffScreenWidth() - gamedata.GetNumberNeighbourDistance().right - gamedata.GetNumberOfDabugArea(); x++)
            for (int y = gamedata.GetNumberNeighbourDistance().up; y < GetDiffScreenHeight() - gamedata.GetNumberNeighbourDistance().down; y++)
            {
                int nNeighbours = GetNumberOfNeighboursActive(x, y);

                if (cellAt(x, y) == IS_ALIVE)
                    m_state[y * GetDiffScreenWidth() + x] = (nNeighbours > gamedata.GetNumberMinNeighbours() && nNeighbours < gamedata.GetNumberMaxNeighbours());
                else
                    m_state[y * GetDiffScreenWidth() + x] = nNeighbours == gamedata.GetNumberNeighboursToCreateLife();

                if (cellAt(x, y) == IS_ALIVE)
                {
                    isAnyCellAlive = true;

                    if (safeHeaven->Contains(GetDiffPos(x), GetDiffPos(y)))
                    {
                        FillRect(GetDiffPos(x), GetDiffPos(y), SCALE, SCALE, olc::CYAN);
                    }
                    else
                    {
                        isAnyCellOutsideSafeHeaven = true;
                        FillRect(GetDiffPos(x), GetDiffPos(y), SCALE, SCALE, olc::WHITE);
                    }
                }
                else
                {
                    FillRect(GetDiffPos(x), GetDiffPos(y), SCALE, SCALE, olc::BLACK);
                }

            }

        DrawSafeHeaven();
        DrawGameBorders();
        DrawGameStats();

        if (isAnyCellAlive && !isAnyCellOutsideSafeHeaven)
        {
            CreateSafeHeaven();
        }

        return true;
    }
    void DrawSafeHeaven()
    {
        DrawRect(
            safeHeaven->GetX(),
            safeHeaven->GetY(),
            safeHeaven->GetW(),
            safeHeaven->GetH(),
            safeHeaven->GetColor()
        );
    }
private:
    GameData gamedata;
    SafeHeaven* safeHeaven;
    const int IS_ALIVE = 1;

    int* m_output;
    int* m_state;
    int lifeArea;
    uint32_t randState = 0;

    // Lehmer’s random number generator
    uint32_t Rand()
    {
        randState += 0xe120fc15;
        uint64_t tmp;
        tmp = (uint64_t)randState * 0x4a39b70d;
        uint32_t m1 = (tmp >> 32) ^ tmp;
        tmp = (uint64_t)m1 * 0x12fad5c9;
        uint32_t m2 = (tmp >> 32) ^ tmp;
        return m2;
    }


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

        // DEBUG
        if (GetKey(olc::K).bHeld) CreateSafeHeaven();

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
            FillRect(GetDiffPos(x), 0, SCALE, SCALE, olc::WHITE);
            FillRect(GetDiffPos(x), GetDiffPos(GetDiffScreenHeight() - 1), SCALE, SCALE, olc::WHITE);
        }

        for (int y = 0; y < GetDiffScreenHeight(); y++)
        {
            FillRect(0, GetDiffPos(y), SCALE, SCALE, olc::WHITE);
            FillRect(GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() -1), GetDiffPos(y), SCALE, SCALE, olc::WHITE);
        }
    }


    void DrawDebugBorders()
    {
        for (int x = GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea(); x < GetDiffScreenWidth(); x++)
        {
            FillRect(GetDiffPos(x), 0, SCALE, SCALE , olc::CYAN);
            FillRect(GetDiffPos(x), GetDiffPos(43), SCALE, SCALE, olc::CYAN);
            FillRect(GetDiffPos(x), GetDiffPos(GetDiffScreenHeight() - 1), SCALE, SCALE, olc::CYAN);
        }

        for (int y = 0; y < GetDiffScreenHeight(); y++) {
            FillRect(GetDiffPos(GetDiffScreenWidth() -1), GetDiffPos(y), SCALE, SCALE, olc::CYAN);
            FillRect(GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea()), GetDiffPos(y), SCALE, SCALE, olc::CYAN);
        }
    }

    void DrawGameTexts()
    {
        
        //Titles
        DrawString(
            GetDiffPos(1),
            GetDiffPos(0),
            "Game Of Life", olc::BLACK);

        DrawString(
            GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() + 4),
            GetDiffPos(0),
            "COMMANDS", olc::BLACK);

        //Commands

        DrawDebugText(2, 2, "> SPACE <");
        DrawDebugText(1, 3, "Create Life");

        DrawDebugText(2, 6, "> Q <");
        DrawDebugText(1, 7, "Less Deaths");

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

    void DrawGameStats()
    {
        FillRect(
            GetDiffPos(GetDiffScreenWidth()-gamedata.GetNumberOfDabugArea()+1), 
            GetDiffPos(46), 
            SCALE * (gamedata.GetNumberOfDabugArea()-2), 
            SCALE * 10, 
            olc::BLACK);

        //Status

        DrawDebugText(1, 46, "N to Create : " + std::to_string(gamedata.GetNumberNeighboursToCreateLife()));
        DrawDebugText(1, 48, "MinNeighbour : " + std::to_string(gamedata.GetNumberMinNeighbours()));
        DrawDebugText(1, 50, "MaxNeighbour : " + std::to_string(gamedata.GetNumberMaxNeighbours()));
        DrawDebugText(1, 52, "Distance : " + std::to_string(gamedata.GetNumberNeighbourDistance().right));

        std::string direction = "";

        if (gamedata.GetNumberNeighbourDistance().up == 0) direction = "S";
        else if (gamedata.GetNumberNeighbourDistance().down == 0) direction = "N";
        else if (gamedata.GetNumberNeighbourDistance().left == 0) direction = "E";
        else if (gamedata.GetNumberNeighbourDistance().right == 0) direction = "W";

        DrawDebugText(1, 54, "Direction : " + direction);
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
        return ScreenWidth() / SCALE;
    }

    int GetDiffScreenHeight()
    {
        return ScreenHeight() / SCALE;
    }

    int GetDiffPos(int a)
    {
        return a * SCALE;
    }
};

int main()
{
    ProjectDugo_Game game(15);

    if(game.Construct(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, 1, 1))
        game.Start();

    return 0;
}
