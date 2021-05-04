#include <iostream>
#include "olcPixelGameEngine.h"
#include "Snake.h"
#include "BreakOut.h"
#include "test.h"
int main()
{
    /*Snake snek;
    if (snek.Construct(31 * 16, 35 * 16, 2, 2, false, false))
        snek.Start();*/

   /*BreakOut d;
    if (d.Construct(512, 480, 2, 2)) {
        d.Start();
    }*/
    test t;
    t.set_grid(20, 20);
    if (t.Construct(100, 100, 10, 10,false, false))
        t.Start();
    return 0;
}
