#pragma once
#include "olcPixelGameEngine.h"
class Snake : public olc::PixelGameEngine {
public:
	Snake();
	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;
	bool OnUserDestroy() override;
private:
	float fTargetFrameTime = 1.0f / 10.0f; // Virtual FPS of 20fps
	float fAccumulatedTime = 0.0f;
	std::vector<olc::vf2d> fSnakeBody;
	olc::vf2d apple;
	olc::vf2d direction = { 0,0 };
	olc::vi2d vBlockSize = { 16,16 };
	std::unique_ptr<int[]> blocks;
	std::unique_ptr<olc::Sprite> sprTile;
	olc::vf2d oldTAIL;
	olc::vf2d head;
	int score;
	float speed;
	bool movement = false;
};