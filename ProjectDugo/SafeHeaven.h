#pragma once
#include "olcConsoleGameEngine.h"

class SafeHeaven
{
private:
    int x;
    int y;
    int w;
    int h;
    olc::Pixel color;

public:
    SafeHeaven()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
        color = olc::GREEN;
    }

    SafeHeaven(int _x, int _y, int _w, int _h, olc::Pixel _color)
    {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        color = _color;
    }

    int GetX() { return x; }
    int GetY() { return y; }
    int GetW() { return w; }
    int GetH() { return h; }
    olc::Pixel GetColor() { return color; }

    bool Contains(int _x, int _y)
    {
        return _x >= x && _x <= x + w && _y >= y && _y <= y + h;
    }
};
