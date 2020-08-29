// ProjectDugo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "olcConsoleGameEngine.h"

class ProjectDugo_Game : public olcConsoleGameEngine {
public:

    ProjectDugo_Game()
    {
        m_sAppName = L"OLCJAM 2020 - GAME OF LIFE";
    }

    virtual bool OnUserCreate() {

        m_output = new int[ScreenWidth() * ScreenHeight()];
        m_state = new int[ScreenWidth() * ScreenHeight()];
        memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
        memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

        for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
            m_state[i] = rand() % 2;

        return true;
    }

    virtual bool OnUserUpdate(float fElapsedTime) {

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto cell = [&](int x, int y)
        {
            return m_output[y * ScreenWidth() + x];
        };

        for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
            m_output[i] = m_state[i];

        for(int x=1; x<ScreenWidth() -1; x++)
            for (int y = 1; y < ScreenHeight() - 1; y++)
            {
                int nNeighbours =   cell(x - 1, y - 1) + cell(x, y - 1) + cell(x + 1, y - 1) +
                                    cell(x - 1, y) +        0           + cell(x + 1, y) +
                                    cell(x - 1, y + 1) + cell(x, y + 1) + cell(x + 1, y + 1);

                if (cell(x, y) == 1)
                    m_state[y * ScreenWidth() + x] = nNeighbours == 2 || nNeighbours == 3;
                else
                    m_state[y * ScreenWidth() + x] = nNeighbours == 3;

                if (cell(x, y) == 1) Draw(x, y, PIXEL_SOLID, FG_WHITE);
                else Draw(x, y, PIXEL_SOLID, FG_BLACK);

            }


        return true;
    }
private:
    int* m_output;
    int* m_state;
};

int main()
{
    ProjectDugo_Game game;

    game.ConstructConsole(80, 60, 8, 8);
    game.Start();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
