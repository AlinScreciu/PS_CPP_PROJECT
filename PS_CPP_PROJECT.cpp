#include <iostream>
#include <string>
#include "olcPixelGameEngine.h"
#include "Snake.h"
#include "BreakOut.h"
#include "tetris.h"
int main()
{
    /*Snake snek;
    if (snek.Construct(35 * 16,40 * 16, 2, 2, false, false))
         std::cout << "fps:\n";
         std::string fps;
         std::cin >> fps;
         snek.setFPS(10);
         if (fps.find_first_not_of("0123456789") == std::string::npos and 0 < stoi(fps) and stoi(fps) < 30)
             snek.setFPS(stoi(fps));
         snek.Start();*/

   /*BreakOut d;
    if (d.Construct(512, 480, 2, 2)) {
        d.Start();
    }*/
   /* test t;
    t.set_grid(20, 20);
    if (t.Construct(100, 100, 10, 10,false, false))
        t.Start();*/
    tetris game;
    if (game.Construct(400 ,400  ,2, 2, false, false))
    {
        game.Start();
    }
    return 0;
}
