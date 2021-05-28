#include "tetris.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <sys/stat.h>
#include <random>
#pragma comment(lib, "Winmm.lib")
void tetris::myDraw() 
{
	Clear(olc::WHITE);
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			DrawRect(olc::vi2d(x, y)*scale, olc::vi2d(int(scale), int(scale)), olc::GREY);
		}
	}
	
}

bool tetris::check()
{
	for (int i = 0 ; i<4;i++)
	{
		for (int i = 0; i < 4; i++)
			if (a[i].x < 0 || a[i].x >= 10 || a[i].y >= 20) return 0;
			else if (field[a[i].y][a[i].x]) return 0;
		return 1;
	}
}
tetris::tetris()
{
	sAppName = "Tetris";
}
bool tetris::OnUserCreate()
{
	sndPlaySound(TEXT("resources/tetris.wav"), SND_FILENAME | SND_ASYNC | SND_LOOP);
	s = std::make_unique<olc::Sprite>("resources/tiles.png");
	myDraw();
	sX = ScreenWidth();
	sY = ScreenHeight();

	std::mt19937 gen(static_cast<long unsigned int>(time(0)));
	std::uniform_int_distribution<> dis(0, 6);
	int n = dis(gen);
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2 + 4;
		a[i].y = figures[n][i] / 2;
	}
	return true;
}
void tetris::handle_input(float fElapsedTime)
{
	if (GetKey(olc::Key::UP).bPressed) rotate = true;
	
	else if ( GetKey(olc::Key::LEFT).bPressed)
	{
		dx = -1;	
	}
	else if ( GetKey(olc::Key::RIGHT).bPressed)
	{
		dx = 1;
	}
	if (GetKey(olc::Key::DOWN).bHeld) delay = 0.05f;
	
	if (GetKey(olc::Key::LEFT).bHeld)
		heldL += fElapsedTime;
	if (GetKey(olc::Key::RIGHT).bHeld)
		heldR += fElapsedTime;

	if (heldL >= 0.1f)
	{
		heldL -= fTargetFrameTime;
		dx = -1;
	}
	if (heldR >= 0.1f)
	{
		heldR -= fTargetFrameTime;
		dx = 1;
	}
}

bool tetris::OnUserUpdate(float fElapsedTime)
{
	
	if (gameOver) return false;
	handle_input(fElapsedTime);

	timer += fElapsedTime;
	fAccumulatedTime += fElapsedTime;

	if (fAccumulatedTime >= fTargetFrameTime)
	{
		fAccumulatedTime -= fTargetFrameTime;
		fElapsedTime = fTargetFrameTime;
	}
	else
		return true; // Don't do anything this frame
	
	

	for (int i = 0; i < 4; i++) 
	{
		b[i] = a[i]; 
		a[i].x += dx;
	}
	if (!check()) for (int i = 0; i <= 4; i++) a[i] = b[i];
	on_side = false;
	
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x == 0 || a[i].x == 9) on_side = true;
	}
	if (rotate && !on_side) 
	{
		Point p = a[1];
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
	}

	std::mt19937 gen(static_cast<long unsigned int>(time(0)));
	std::uniform_int_distribution<> dis(0,6);

	if (timer > delay)
	{
		for (int i = 0; i < 4; i++) 
		{ 
			b[i] = a[i]; 
			a[i].y += 1;
		}

		if (!check())
		{
			for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = numColor;
			
			
			numColor = dis(gen) + 1;
			int n = dis(gen);

			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2 + 4;
				a[i].y = figures[n][i] / 2;
			}
		}
		
		timer = 0.0f;
	}

	for (int i = 0; i < 10; i++)
	{
		if (field[1][i] != 0) gameOver = true;
	}
	int k = 19;
	int tetris = 0;
	for (int i = 19; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < 10; j++)
		{
			if (field[i][j]) count++;
			field[k][j] = field[i][j];
		}
		if (count == 10) tetris++;
		if (count < 10) k--;
	}
	if (tetris == 4) std::cout << "!TETRIS";
	myDraw();
	
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			if (field[i][j] == 0) continue;
			DrawPartialSprite(olc::vi2d(j, i) * vScale, s.get(), olc::vi2d(field[i][j], 0)*vScale, vScale);
		}
	for (int i = 0; i < 4; i++)
	{
		
		DrawPartialSprite(olc::vi2d(a[i].x, a[i].y) * vScale, s.get(), olc::vi2d(numColor, 0)*vScale , vScale);

	}

	dx = 0; rotate = 0; delay = 0.3f; 
	return true;
}



bool tetris::OnUserDestroy()
{
	return true;
}
void tetris::setFPS(float x)
{
	fTargetFrameTime = 1 / x;
}