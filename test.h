#pragma once
#include "olcPixelGameEngine.h"
class test : public olc::PixelGameEngine
{
public:
	test();
	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;
	bool OnUserDestroy() override;
	void set_grid(int, int);
private:
	int i = 0;
	int j = 0;
	olc::Pixel a = olc::RED;
	int ROWS=10;
	int COLS=10;
	olc::vd2d vBlockSize;
	std::unique_ptr<int[]> blocks;

};

