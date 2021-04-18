#pragma once
#include "olcPixelGameEngine.h"
class BreakOut : public olc::PixelGameEngine
{
public:
	BreakOut();
	bool OnUserCreate() override;
	bool OnUserUpdate(float ) override;


private:
	float fBatPos = 20.0f;
	float fBatWidth = 40.0f;
	float fBatSpeed = 250.0f;
	olc::vf2d vBall = { 200.0f, 200.0f };
	olc::vf2d vBallVel = { 200.0f, -100.0f };
	olc::vf2d vBallPos = { 0.0f, 0.0f };
	olc::vf2d vBallDir = { 0.0f, 0.0f };
	olc::vf2d vPotentialBallPos;
	olc::vf2d vTileBallRadialDims;
	float fBallSpeed = 20.0f;
	float fBallRadius = 5.0f;

	olc::vi2d vBlockSize = { 16, 16 };
	std::unique_ptr<int[]> blocks;

	std::unique_ptr<olc::Sprite> sprTile;
	std::unique_ptr<olc::Sprite> sprFragment;
	std::unique_ptr<olc::Decal> decFragment;

	struct sFragment
	{
		olc::vf2d pos;
		olc::vf2d vel;
		float fAngle;
		float fTime;
		olc::Pixel colour;
	};
	std::list<sFragment> listFragments;
};

