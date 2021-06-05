#pragma once
#include "olcPixelGameEngine.h"
#include <vector>

class tetris : public olc::PixelGameEngine
{
public:
	void HandleInput(float);
	void Display();
	tetris();
	bool OnUserCreate() override;
	bool OnUserUpdate(float) override;
	bool OnUserDestroy() override;
	void setFPS(float);
	float fAccumulatedTime = 0;
	float fTargetFrameTime = 1/60;
	int tileSize = 18;
	int ngScreenWidth = 16 * tileSize;			// Console Screen Size X (columns)
	int ngScreenHeight = 19 * tileSize;
	std::wstring tetromino[7];
	int nFieldWidth = 12;
	int nFieldHeight = 18;
	unsigned char* pField = nullptr;
	void OnResetGame();
	bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY);
	bool DrawTetrisSprite(int x, int y, int c);
	int Rotate(int px, int py, int r);
	// Game Logic
	bool bKey[4];
	bool bKeyNewState[4];
	float tickAcc;

	int nCurrentPiece;
	int nNextPiece;
	int nHoldPiece;
	int nCurrentRotation;
	int nCurrentX;
	int nCurrentY;
	int nGhostY;
	int nSpeed;
	int nSpeedCount;
	int nFade;
	bool bForceDown;
	bool bRotateHold;
	bool bDownHold;
	bool bDelayForLine;
	bool bCanSwap;
	int nPieceCount;
	int nScore;
	float heldL, heldR, heldD, tickrate =0.05;
	std::vector<int> vLines;
	bool bGameOver = false;
	std::unique_ptr<olc::Sprite> s;
};

