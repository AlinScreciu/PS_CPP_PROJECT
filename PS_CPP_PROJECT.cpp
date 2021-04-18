

#include <iostream>
#include "olcPixelGameEngine.h"
#include "Snake.h"
//#include "BreakOut.h"
int main()
{
    /*BreakOut demo;
    if (demo.Construct(512, 480, 2, 2))
        demo.Start();*/
    Snake snek;
    if (snek.Construct(31*16, 31*16, 2,2))
        snek.Start();
    return 0;
}




