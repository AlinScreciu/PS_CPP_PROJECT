#pragma once
#include "olcPixelGameEngine.h"

class tetris : public olc::PixelGameEngine
{
public:
	
	tetris();
	bool OnUserCreate() override;
	void handle_input(float);
	bool OnUserUpdate(float) override;
	bool OnUserDestroy() override;
	void setFPS(float);
	void myDraw();
	bool check();
	float sX;
	float sY;
	float scale = 18.0f;
	olc::vf2d vScale = { scale, scale };
	float fTargetFrameTime = 1 / 15.0f; //  1 / x = > Virtual FPS of x fps
	float fAccumulatedTime = 0.0f;
	float timer = 0, delay = 0.3f;
	std::unique_ptr<olc::Sprite> s;
	int figures[7][4] =
	{
		1,3,5,7, // I
		2,4,5,7, // Z
		3,5,4,6, // S
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // O
	};
	bool gameOver = false;
	struct Point { int x, y; } a[4], b[4];
	int field[20][10] = { 0 };
	int dx = 0;
	bool rotate = 0;
	int numColor = 1;
	float heldL = 0.0f, heldR = 0.0f;
	bool on_side = false;
};

