

#include <iostream>
#include "olcPixelGameEngine.h"
#include "Snake.h"
#include "BreakOut.h"
int main()
{ 
    Snake snek;
    if (snek.Construct(31*16, 35*16, 2,2,false,false))
        snek.Start();
   /* BreakOut d;
    if (d.Construct(512, 480, 2, 2)) {
        d.Start();
    }*/
    return 0;
}

