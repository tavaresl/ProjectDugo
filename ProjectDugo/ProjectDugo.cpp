#include <stdio.h>
#include "olcConsoleGameEngine.h"

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

        for (int x = 1; x < ScreenWidth() - 1; x++)
            for (int y = 1; y < ScreenHeight() - 1; y++)
                if(IsInLifeArea(x,y,lifeArea))
                    m_state[y * ScreenWidth() + x] = rand() % 2;

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

        for (int x = 1; x < ScreenWidth() - 1; x++)
            for (int y = 1; y < ScreenHeight() - 1; y++)
            {
                int nNeighbours = cell(x - 1, y - 1) + cell(x, y - 1) + cell(x + 1, y - 1) +
                    cell(x - 1, y) + 0 + cell(x + 1, y) +
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
    int lifeArea;

    bool IsInLifeArea(int x, int y, int space)
    {
        int screenMiddleX = (ScreenWidth() / 2);
        int screenMiddleY = (ScreenWidth() / 2);
        return
            (x > screenMiddleX - space && x < screenMiddleX + space &&
                y > screenMiddleY - space && y < screenMiddleY + space);
    }
};

int main()
{
    ProjectDugo_Game game(10);

    if(game.ConstructConsole(80, 60, 8, 8))
        game.Start();

    return 0;
}