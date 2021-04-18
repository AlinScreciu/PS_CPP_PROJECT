#pragma once
#include "olcPixelGameEngine.h"
class Snake : public olc::PixelGameEngine {
public:
	Snake();
	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;
private:
	olc::vf2d fSnakeHeadStart = { 0.0f,0.0f };
	std::list<olc::vf2d> fSnakeBody;
	
	olc::vi2d vBlockSize = { 16,16 };
	std::unique_ptr<int[]> blocks;
	std::unique_ptr<olc::Sprite> sprTile;
};