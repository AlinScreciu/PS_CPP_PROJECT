#pragma once
#include "olcPixelGameEngine.h"
class Snake : public olc::PixelGameEngine 
{
public:
	Snake();
	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;
	bool OnUserDestroy() override;
	void setFPS(float);
private:
	void eating();
	void update_snake();
	void movement_input(); 
	void movement_set_dir();
	bool is_game_over();
	float fTargetFrameTime; //  1 / x = > Virtual FPS of x fps
	float fAccumulatedTime = 0.0f;
	std::vector<olc::vf2d> fSnakeBody;
	olc::vf2d apple;
	olc::vf2d direction = { 0,0 };
	olc::vi2d vBlockSize = { 16, 16 };
	std::unique_ptr<olc::Sprite> sprHead;
	olc::vf2d oldTail;
	olc::vf2d oldHead;
	olc::vf2d head;
	int hi;
	int score;
	float speed;
	float yOffset = 8.0f; // 2 x string size => 2 lines
	bool moved;
	bool ate = false;
	bool up = false, down = false, left = false, right = false;
};
