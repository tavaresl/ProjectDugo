#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define OLC_PGE_APPLICATION
#include "olcConsoleGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"
#include "SafeHeaven.h"
#include "GameData.h"
#include "Score.h"
#include "Countdown.h"
#include "StateController.h"

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 60;
const int SCALE = 8;

enum class EnumGameState { MainMenu, Gameplay, GameOver};

class ProjectDugo_Game : public olc::PixelGameEngine {
public:

    ProjectDugo_Game(int _lifeArea)
    {
        sAppName = "Conway's Container";
        lifeArea = _lifeArea;
        srand(time(NULL));
        randState = rand() % 100;
        gamestate = EnumGameState::MainMenu;

        score = new Score();
        countdown = new Countdown(gamedata.GetTimelimit(), gamedata.GetTickRate());
    }

    bool OnUserCreate() override
    {
        olc::SOUND::InitialiseAudio();
        bgMusicSample = olc::SOUND::LoadAudioSample("bg.wav");
        tickBeepSample = olc::SOUND::LoadAudioSample("beep-29.wav");
        pointBeepSample = olc::SOUND::LoadAudioSample("beep-22.wav");
        gamestartSample = olc::SOUND::LoadAudioSample("gamestart.wav");
        gameoverSample = olc::SOUND::LoadAudioSample("gameover.wav");
        ChangeState(gamestate);

        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override {

        switch (gamestate)
        {
        case EnumGameState::MainMenu:
            MainMenuStateUpdate();
            break;
        case EnumGameState::Gameplay:
            GameplayStateUpdate();
            break;
        case EnumGameState::GameOver:
            GameoverStateUpdate();
            break;
        default:
            break;
        }

        return true;
    }
    
    bool OnUserDestroy() override
    {
        olc::SOUND::DestroyAudio();
        return true;
    }

    void CreateSafeHeaven()
    {
        int safeHeavenSize = SCALE * RandomHeavenSize(16, 24); //SCALE * 32;
        int debugAreaOffset = gamedata.GetNumberOfDabugArea() * SCALE;
        int widthOffset = safeHeavenSize + debugAreaOffset;
        int width = ScreenWidth() - widthOffset;
        int height = ScreenHeight() - safeHeavenSize;
        int randomX = Rand() % width;
        int randomY = Rand() % height;

        safeHeaven = new SafeHeaven(randomX, randomY, safeHeavenSize, safeHeavenSize, olc::GREEN);
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
    Score* score;
    Countdown* countdown;
    EnumGameState gamestate;
    const int IS_ALIVE = 1;

    const int beepInterval = 10;
    int bgMusicSample;
    int pointBeepSample;
    int tickBeepSample;
    int gamestartSample;
    int gameoverSample;
    bool tickBeeped;


    int* m_output;
    int* m_state;
    int lifeArea;
    uint32_t gameTick = 0;
    uint32_t randState = 0;

    //GameStates

    void ChangeState(EnumGameState newState)
    {
        Clear(olc::BLACK);

        switch (newState)
        {
        case EnumGameState::MainMenu:
            MainMenuStateStart();
            break;
        case EnumGameState::Gameplay:
            GameplayStateStart();
            break;
        case EnumGameState::GameOver:
            GameoverStateStart();
            break;
        default:
            break;
        }

        gamestate = newState;
    }

    void MainMenuStateStart()
    {
        int initialInstructionsXPosition = 12;
        olc::SOUND::PlaySample(bgMusicSample, true);

        DrawString(
            GetDiffPos(14),
            GetDiffPos(10),
            "Conway's Container", olc::WHITE, 3U);

        DrawString(
            GetDiffPos(initialInstructionsXPosition),
            GetDiffPos(18),
            "> YOUR OBJECTIVE IS TO CONTAIN LIFE IN THE", olc::WHITE);


        DrawString(
            GetDiffPos(initialInstructionsXPosition+44),
            GetDiffPos(18),
            "GREEN SPACE", olc::GREEN);


        DrawString(
            GetDiffPos(initialInstructionsXPosition),
            GetDiffPos(20),
            "> YOU CAN MANIPULATE THE GAME RULES TO REACH YOUR GOAL", olc::WHITE);

        DrawString(
            GetDiffPos(initialInstructionsXPosition),
            GetDiffPos(22),
            "> HAVE FUN!!", olc::WHITE);

        
        DrawString(
            GetDiffPos(20),
            GetDiffPos(GetDiffScreenHeight() - 8),
            "PRESS SPACE TO BEGIN", olc::WHITE, 2U);

        DrawString(
            GetDiffPos(1),
            GetDiffPos(GetDiffScreenHeight() - 1),
            "FALME STREAMLESS", olc::GREY);

        DrawString(
            GetDiffPos(GetDiffScreenWidth() - 14),
            GetDiffPos(GetDiffScreenHeight() - 1),
            "LUCAS TAVARES", olc::GREY);
    }

    void MainMenuStateUpdate()
    {
        if (GetKey(olc::SPACE).bPressed) ChangeState(EnumGameState::Gameplay);
    }

    void GameplayStateStart()
    {

        m_output = new int[GetDiffScreenWidth() * GetDiffScreenHeight()];
        m_state = new int[GetDiffScreenWidth() * GetDiffScreenHeight()];

        if (score == NULL) score = new Score();
        //countdown = new Countdown(gamedata.GetTimelimit(), gamedata.GetTickRate());

        memset(m_output, 0, GetDiffScreenWidth() * GetDiffScreenHeight() * sizeof(int));
        memset(m_state, 0, GetDiffScreenWidth() * GetDiffScreenHeight() * sizeof(int));

        score->ResetCurrentScore();

        olc::SOUND::PlaySample(gamestartSample);
        SetInitialData();

        CreateSafeHeaven();
        DrawDebugBorders();
        DrawGameTexts();
    }

    void GameplayStateUpdate()
    {
        CheckInputForGameState();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        countdown->Decrement();

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
            olc::SOUND::PlaySample(pointBeepSample);
            score->IncrementCurrentScore();
            score->UpdateHighScore();
            CreateSafeHeaven();
        }

        if (countdown->Counter() == 0)
        {
            countdown->Reset();
            score->UpdateHighScore();
            CreateSafeHeaven();

            ChangeState(EnumGameState::GameOver);
        }

        if (countdown->Counter() % beepInterval != 0)
            tickBeeped = false;

        if (!tickBeeped && countdown->Counter() != gamedata.GetTimelimit() && countdown->Counter() % beepInterval == 0)
        {
            olc::SOUND::PlaySample(tickBeepSample, false);
            tickBeeped = true;
        }
    }


    void GameoverStateStart()
    {
        int initialInstructionsXPosition = 22;
        olc::SOUND::PlaySample(gameoverSample);

        DrawString(
            GetDiffPos(26),
            GetDiffPos(10),
            "GAME OVER", olc::WHITE, 3U);



        DrawString(
            GetDiffPos(initialInstructionsXPosition),
            GetDiffPos(20),
            "SCORE : " + std::to_string(score->CurrentScore()), olc::WHITE, 2U);

        DrawString(
            GetDiffPos(initialInstructionsXPosition),
            GetDiffPos(26),
            "HIGHSCORE : "+ std::to_string(score->Highscore()), olc::WHITE, 2U);


        DrawString(
            GetDiffPos(20),
            GetDiffPos(GetDiffScreenHeight() - 8),
            "PRESS SPACE TO BEGIN", olc::WHITE, 2U);
    }

    void GameoverStateUpdate()
    {
        if (GetKey(olc::SPACE).bPressed) ChangeState(EnumGameState::Gameplay);

    }

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
        int screenMiddleX = ((GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea()) / 2);
        int screenMiddleY = (GetDiffScreenHeight() / 2);
        return
            (x > screenMiddleX - space && x < screenMiddleX + space &&
                y > screenMiddleY - space && y < screenMiddleY + space);
    }

    void CheckInputForGameState()
    {
        if (GetKey(olc::SPACE).bHeld) SetInitialData();

        if (GetKey(olc::Q).bHeld) gamedata.ChangeNumberNeighborToMinimumDeath();
        if (GetKey(olc::E).bHeld) gamedata.ChangeNumberNeighborToMaximumDeath();

        if (GetKey(olc::J).bHeld) gamedata.ChangeNumberNeighborToCreateLife();
        if (GetKey(olc::K).bHeld) gamedata.ChangeNumberNeighborDistance();

        if (GetKey(olc::W).bHeld) gamedata.ChangeDirectionToUp();
        if (GetKey(olc::A).bHeld) gamedata.ChangeDirectionToLeft();
        if (GetKey(olc::D).bHeld) gamedata.ChangeDirectionToRight();
        if (GetKey(olc::S).bHeld) gamedata.ChangeDirectionToDown();

        if (GetKey(olc::BACK).bHeld) gamedata.ChangeNumbersToOriginal();

        // DEBUG
        //if (GetKey(olc::K).bHeld) CreateSafeHeaven();

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
                if (x != 0 || y != 0)
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
            FillRect(GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() - 1), GetDiffPos(y), SCALE, SCALE, olc::WHITE);
        }

        //Titles
        DrawString(
            GetDiffPos(1),
            GetDiffPos(0),
            "Game Of Life", olc::BLACK);
    }


    void DrawDebugBorders()
    {
        for (int x = GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea(); x < GetDiffScreenWidth(); x++)
        {
            FillRect(GetDiffPos(x), 0, SCALE, SCALE, olc::CYAN);
            FillRect(GetDiffPos(x), GetDiffPos(43), SCALE, SCALE, olc::CYAN);
            FillRect(GetDiffPos(x), GetDiffPos(GetDiffScreenHeight() - 1), SCALE, SCALE, olc::CYAN);
        }

        for (int y = 0; y < GetDiffScreenHeight(); y++) {
            FillRect(GetDiffPos(GetDiffScreenWidth() - 1), GetDiffPos(y), SCALE, SCALE, olc::CYAN);
            FillRect(GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea()), GetDiffPos(y), SCALE, SCALE, olc::CYAN);
        }
    }

    void DrawGameTexts()
    {


        DrawString(
            GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() + 4),
            GetDiffPos(0),
            "COMMANDS", olc::BLACK);

        //Commands

        DrawDebugText(2, 2, "> SPACE <");
        DrawDebugText(1, 4, "Create Life");
        
        DrawDebugText(2, 7, "> W A S D <");
        DrawDebugText(1, 9, "Move Life");

        DrawDebugText(2, 13, "> Q <");
        DrawDebugText(1, 15, "Change Minimum");
        DrawDebugText(1, 16, "Neighbour Death");

        DrawDebugText(2, 20, "> E <");
        DrawDebugText(1, 22, "Change Maximum");
        DrawDebugText(1, 23, "Neighbour Death");

        DrawDebugText(2, 27, "> J <");
        DrawDebugText(1, 29, "Less Deaths");

        DrawDebugText(2, 33, "> K <");
        DrawDebugText(1, 35, "Change Distance");

        DrawDebugText(2, 39, "> Backspace <");
        DrawDebugText(1, 41, "Reset Rules");


    }

    void DrawGameStats()
    {
        FillRect(
            GetDiffPos(GetDiffScreenWidth() - gamedata.GetNumberOfDabugArea() + 1),
            GetDiffPos(45),
            SCALE * (gamedata.GetNumberOfDabugArea() - 2),
            SCALE * 14,
            olc::BLACK);

        //Status

        DrawDebugText(1, 45, "N to Create : " + std::to_string(gamedata.GetNumberNeighboursToCreateLife()));
        DrawDebugText(1, 47, "MinNeighbour : " + std::to_string(gamedata.GetNumberMinNeighbours()));
        DrawDebugText(1, 49, "MaxNeighbour : " + std::to_string(gamedata.GetNumberMaxNeighbours()));
        DrawDebugText(1, 51, "Distance : " + std::to_string(gamedata.GetNumberNeighbourDistance().right));
        DrawDebugText(1, 53, "Score : " + std::to_string(score->CurrentScore()));
        DrawDebugText(1, 55, "Highscore : " + std::to_string(score->Highscore()));

        std::string direction = "";

        if (gamedata.GetNumberNeighbourDistance().up == 0) direction = "S";
        else if (gamedata.GetNumberNeighbourDistance().down == 0) direction = "N";
        else if (gamedata.GetNumberNeighbourDistance().left == 0) direction = "E";
        else if (gamedata.GetNumberNeighbourDistance().right == 0) direction = "W";

        DrawDebugText(1, 57, "Time : " + std::to_string(countdown->Counter()));
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

    int RandomHeavenSize(int min, int max)
    {
        return Rand() % max + min;
    }
};