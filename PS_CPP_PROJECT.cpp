#include <iostream>
#include <string>
#include "olcPixelGameEngine.h"
#include "Snake.h"
#include "tetris.h"
#include "test.h"
void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}
int main()
{
    int h, v;
    GetDesktopResolution(h, v);
    
    tetris game;
    std::string user,fps;
    std::cout << "Enter your username:\n";
    std::cin >> user;
    
    int ch = 1;
    while (ch != 0)
    {
        std::cout << "0. Exit\n1. Snake\n2. Tetris\n";
        std::cin >> ch;
        if (ch == 1)
        {
            Snake snek;
            snek.user = user;
            if (snek.Construct(35 * 16, 40 * 16, 2, 2, false, false))
                std::cout << "fps:\n";
            std::cin >> fps;
            snek.setFPS(10);
            if (fps.find_first_not_of("0123456789") == std::string::npos and 0 < stoi(fps) and stoi(fps) < 30)
                snek.setFPS(stoi(fps));
            snek.Start();
        }
        system("cls");
    }
    
   
    /*test g;
    if (g.Construct(h/2,v/2,1,1, false, false))
    {   
        g.set_grid(16,9);
        g.Start();
    }*/
    return 0;
}
