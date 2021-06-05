#include "tetris.h"
#include <Windows.h>
#include <sys/stat.h>
#include <random>
#pragma comment(lib, "Winmm.lib")

tetris::tetris()
{
	sAppName = "Tetris";
}
bool tetris::OnUserCreate()
{
	sndPlaySound(TEXT("resources/tetris.wav"), SND_FILENAME | SND_ASYNC | SND_LOOP);
	s = std::make_unique<olc::Sprite>("resources/tiles.png");
	//Create the tetronimos
	tetromino[0].append(L"..X...X...X...X."); // Tetronimos 4x4
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	//Create the board
	pField = new unsigned char[nFieldWidth * nFieldHeight];

	//Init all other variables
	OnResetGame();
	return true;
}
void tetris::Display()
{
	// Display ======================
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));

	// Draw Field
	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
			DrawTetrisSprite(x, y, pField[y * nFieldWidth + x]);

	// Draw Current Piece
	nGhostY = nCurrentY;
	while (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nGhostY + 1))
		nGhostY += 1; //Ghost piece calculation

	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
			{
				DrawTetrisSprite(nCurrentX + px, nCurrentY + py, nCurrentPiece + 1);
				DrawTetrisSprite(nCurrentX + px, nGhostY + py, 8);
			}

	// Draw Next Piece
	DrawString(13 * tileSize, 0.25 * tileSize, "Next:");
	for (int nx = 0; nx < 4; nx++)
		for (int ny = 0; ny < 4; ny++)
			if (tetromino[nNextPiece][Rotate(nx, ny, 0)] != L'.')
			{
				DrawTetrisSprite(12 + nx, 1 + ny, nNextPiece + 1);
			}

	// Draw Hold Piece
	DrawString(13 * tileSize, 9.25 * tileSize, "Hold:");
	for (int nx = 0; nx < 4; nx++)
		for (int ny = 0; ny < 4; ny++)
			if (tetromino[nHoldPiece][Rotate(nx, ny, 0)] != L'.')
			{
				DrawTetrisSprite(12 + nx, 10 + ny, nHoldPiece + 1);
			}

	// Draw Score
	DrawString(0, 18.25 * tileSize, "SCORE: " + std::to_string(nScore));

	// Animate Line Completion
	if (!vLines.empty() && !bDelayForLine)
	{
		// Display Frame (cheekily to draw lines)
		tickAcc = 0;

		for (auto& v : vLines)
			for (int px = 1; px < nFieldWidth - 1; px++)
			{
				for (int py = v; py > 0; py--)
					pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
				pField[px] = 0;
			}
		vLines.clear();
	}
}

void tetris::HandleInput(float et)
{
	tickrate = 0.05f;

	nCurrentX += (GetKey(olc::RIGHT).bPressed && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
	nCurrentX -= (GetKey(olc::LEFT).bPressed && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;

	if (GetKey(olc::DOWN).bHeld && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) tickrate = 0.0000001f;

	nCurrentRotation += (GetKey(olc::UP).bPressed && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;

	if (GetKey(olc::SPACE).bPressed)
	{
		while (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			nCurrentY += 1;

		tickAcc = 0;
		bForceDown = true;
	}

	if (GetKey(olc::C).bPressed && bCanSwap)
	{
		int temp = nCurrentPiece;
		nCurrentPiece = nHoldPiece;
		nHoldPiece = temp;
		nCurrentY = -1;
		bCanSwap = false;
	}

}
bool tetris::OnUserUpdate(float fElapsedTime)
{
	
	//fAccumulatedTime += fElapsedTime;

	//if (fAccumulatedTime >= fTargetFrameTime)
	//{
	//	fAccumulatedTime -= fTargetFrameTime;
	//	fElapsedTime = fTargetFrameTime;
	//}
	//else
	//	return true; // Don't do anything this frame
	if (bGameOver)
	{
		sndPlaySound(NULL, NULL);
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::Pixel(0, 0, 0));
		DrawString(0, 0, "SCORE: " + std::to_string(nScore));

		if (GetKey(olc::ENTER).bPressed)
		{
			bGameOver = false;
			OnResetGame();
		}
		if (GetKey(olc::ESCAPE).bPressed) return false;

		return true;
	}

	// Timing =======================

	bForceDown = false;
	tickAcc += fElapsedTime;

	if (tickAcc >= tickrate)
	{
		tickAcc = 0;

		if (bDelayForLine)
		{
			nFade += 35;
			if (nFade >= 250)
			{
				bDelayForLine = false;
				nFade = 0;
			}

			//Draw the field again;
			for (int x = 0; x < nFieldWidth; x++)
				for (int y = 0; y < nFieldHeight; y++)
					DrawTetrisSprite(x, y, pField[y * nFieldWidth + x]);
		}
		else
		{
			nSpeedCount++;
			bForceDown = (nSpeedCount == nSpeed);
		}
	}

	

	// Input ========================
	// Handle player movement
	HandleInput(fElapsedTime);
	

	// Game Logic ===================
	// Force the piece down the playfield if it's time
	if (bForceDown)
	{
		// Update difficulty every 50 pieces
		nSpeedCount = 0;
		nPieceCount++;
		if (nPieceCount % 50 == 0)
			if (nSpeed >= 10) nSpeed--;

		// Test if piece can be moved down
		if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			nCurrentY++; // It can, so do it!
		else
		{
			// It can't! Lock the piece in place
			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
					if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
						pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

			// Check for lines
			for (int py = 0; py < 4; py++)
				if (nCurrentY + py < nFieldHeight - 1)
				{
					bool bLine = true;
					for (int px = 1; px < nFieldWidth - 1; px++)
						bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

					if (bLine)
					{

						// Remove Line, set to
						for (int px = 1; px < nFieldWidth - 1; px++)
							pField[(nCurrentY + py) * nFieldWidth + px] = 10;
						vLines.push_back(nCurrentY + py);
						
					}
				}

			nScore += 25;
			if (!vLines.empty())	nScore += (1 << vLines.size()) * 100;

			// Pick New Piece
			nCurrentX = nFieldWidth / 2 - 2;
			nCurrentY = 0;
			nCurrentRotation = 0;
			nCurrentPiece = nNextPiece;
			std::mt19937 gen(static_cast<long unsigned int>(time(0)));
			std::uniform_int_distribution<> dis(0, 6);
			nNextPiece = dis(gen);
			bCanSwap = true;

			// If piece does not fit straight away, game over!
			bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
		}
	}

	Display();
	

	return true;
}
void tetris::OnResetGame()
{
	sndPlaySound(TEXT("resources/tetris.wav"), SND_FILENAME | SND_ASYNC | SND_LOOP);
	nCurrentPiece = 0;
	std::mt19937 gen(static_cast<long unsigned int>(time(0)));
	std::uniform_int_distribution<> dis(0, 6);
	nNextPiece = dis(gen);
	nHoldPiece = dis(gen);
	nCurrentRotation = 0;
	nCurrentX = nFieldWidth / 2;
	nCurrentY = 0;
	nGhostY = 0;
	nSpeed = 20;
	nSpeedCount = 0;
	nFade = 0;
	bForceDown = false;
	bRotateHold = true;
	bDownHold = true;
	bDelayForLine = false;
	bCanSwap = true;
	nPieceCount = 0;
	nScore = 0;
	vLines.clear();
	bGameOver = false;
	tickAcc = 0;

	// Fill play field buffer
	for (int x = 0; x < nFieldWidth; x++) // Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
}

int tetris::Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}

bool tetris::DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}
		}

	return true;
}

bool tetris::DrawTetrisSprite(int x, int y, int c)
{
	int w = 18;
	int h = 18;
	int size = 18;
	olc::vi2d pos(x * size, y * size);
	olc::vi2d wh(w, h);
	switch (c)
	{
	case 0:
		break;
	case 1:
		DrawPartialSprite(pos, s.get(), olc::vi2d(0, 0), wh);
		break;
	case 2:
		DrawPartialSprite(pos, s.get(), olc::vi2d(w, 0), wh);
		break;
	case 3:
		DrawPartialSprite(pos, s.get(), olc::vi2d(2*w, 0), wh);
		break;
	case 4:
		DrawPartialSprite(pos, s.get(), olc::vi2d(3*w, 0), wh);
		break;
	case 5:
		DrawPartialSprite(pos, s.get(), olc::vi2d(4*w, 0), wh);
		break;
	case 6:
		DrawPartialSprite(pos, s.get(), olc::vi2d(5*w, 0), wh);
		break;
	case 7:
		DrawPartialSprite(pos, s.get(), olc::vi2d(6*w, 0), wh);
		break;
	case 9:
		DrawPartialSprite(pos, s.get(), olc::vi2d(7 * w, 0), wh);
		break;

	case 8:
		DrawRect(pos, wh);
	default:
		break;
	}

	return true;
}


bool tetris::OnUserDestroy()
{

	delete pField;
	sndPlaySound(NULL, NULL);
	return true;
}
void tetris::setFPS(float x)
{
	fTargetFrameTime = 1 / x;
}