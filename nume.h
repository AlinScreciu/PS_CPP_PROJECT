#pragma once
#include "olcPixelGameEngine.h"
class nume :
    public olc::PixelGameEngine
{
	nume();
	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;
	bool OnUserDestroy() override;

};

