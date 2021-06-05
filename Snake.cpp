#include "Snake.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <sys/stat.h>
#pragma comment(lib, "Winmm.lib")
void Snake::setHi(int newHi)
{
	hi = newHi;
}
Snake::Snake()
{
	sAppName = " Snake ";
}
void Snake::eating()
{

	if (fSnakeBody.front() == apple)
	{
		ate = true;
		fSnakeBody.push_back(oldTail);
	}
	if (ate)
	{
		apple = { rand() % 24 + 4.0f,rand() % 24 + yOffset };

		bool good = false;

		while (!good)
		{
			for (auto part : fSnakeBody)
			{
				good = true;

				if (apple == part)
				{
					good = false;
					apple = { rand() % 24 + 4.0f,rand() % 24 + yOffset };
				}
			}
		}
	}

}
void Snake::movement_input()
{
	moved = false;

	if ( GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed) up = true;
	if ( GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed) down = true;
	if ( GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::A).bPressed) left = true;
	if ( GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::D).bPressed) right = true;

}
void Snake::movement_set_dir()
{
	

	if ((up && (direction != olc::vf2d(0,1))) && !moved ) 
	{
		direction = { 0,-1 };
		moved = true;
	}
	if ((left && (direction != olc::vf2d(1, 0))) && !moved )
	{
		direction = { -1,0 };
		moved = true;
	}
	if ((down && (direction != olc::vf2d(0, -1))) && !moved )
	{
		direction = { 0,1 };
		moved = true;
	}
	if ((right && (direction != olc::vf2d(-1, 0))) && !moved )
	{
		direction = { 1,0 };
		moved = true;
	}
	up = false;
	down = false;
	left = false;
	right = false;
}
void Snake::update_snake(){
	
	if (direction != olc::vf2d(0, 0))
	{
		for (int i = fSnakeBody.size() - 1; i >= 1; i--)
		{
			fSnakeBody[i] = fSnakeBody[i - 1];
		}
	}
	oldTail = fSnakeBody.back();

	if (direction == olc::vf2d(0, -1))
	{
		fSnakeBody.front().y -= 1;
		return;
	}

	if (direction == olc::vf2d(0, 1))
	{
		fSnakeBody.front().y += 1;
		return;
	}

	if (direction == olc::vf2d(-1, 0))
	{
		fSnakeBody.front().x -= 1;
		return;
	}

	if (direction == olc::vf2d(1, 0))
	{
		fSnakeBody.front().x += 1;
		return;
	}
	return;
}
bool Snake::is_game_over()
{
	if (fSnakeBody.front().x >= ScreenWidth() / vBlockSize.x || fSnakeBody.front().x < 0) return true;
	if (fSnakeBody.front().y >= ScreenHeight() / vBlockSize.y || fSnakeBody.front().y < yOffset) return true;

	// Check if head overlaps any part of the body

	head = fSnakeBody[0];

	for (auto part = begin(fSnakeBody) + 1, e = end(fSnakeBody); part != e; ++part)
	{
		if (*part == head) return true;
	}
	return false;
}
void Snake::setFPS(float x)
{
	fTargetFrameTime = 1 / x;
}
bool Snake::OnUserCreate()
{	
	std::cout << hi;
	score = 0;
	apple = { 15.0f,15.0f +yOffset};
	speed = 80;
	fSnakeBody.reserve(100);
	fSnakeBody.push_back({ 10.0f,10.0f + yOffset});
	fSnakeBody.push_back({ 11.0f,10.0f + yOffset});
	sprHead = std::make_unique<olc::Sprite>("capuPedro.png");
	std::fstream fs;
	return true;
}
bool Snake::OnUserUpdate(float fElapsedTime)
{
	if (score > hi) hi = score;
	
	movement_input();
	
	fAccumulatedTime += fElapsedTime;

	if (fAccumulatedTime >= fTargetFrameTime)
	{
		fAccumulatedTime -= fTargetFrameTime;
		fElapsedTime = fTargetFrameTime;
	}
	else
		return true; // Don't do anything this frame
	
	ate = FALSE;

	movement_set_dir();

	update_snake();

	// Checking boundaries
	if (is_game_over()) return false;

	eating();
	// DRAWING
	///
	Clear(olc::BLACK);


	// Drawing the apple first so the snake will draw over the apple

	FillRect(apple * vBlockSize, vBlockSize, olc::RED);
	

	// Drawing the snake

	for (auto part = fSnakeBody.begin(); part != fSnakeBody.end(); ++part)
	{
		if (*part == fSnakeBody.front())
		{
			DrawSprite(fSnakeBody.front()*vBlockSize.x, sprHead.get());
		}
		else
		{
			FillRect(*part * vBlockSize, vBlockSize, olc::GREEN);
		}
	}

	// Drawing the boundaries

	DrawLine(0 , yOffset * vBlockSize.x , ScreenWidth(),yOffset* vBlockSize.x, olc::WHITE);
	DrawLine(0, yOffset * vBlockSize.x, 0, ScreenHeight(), olc::WHITE);
	DrawLine( 0, ScreenHeight()-1.0f, ScreenWidth()-1.0f, ScreenHeight()-1.0f,olc::WHITE);
	DrawLine( ScreenWidth() - 1.0f, ScreenHeight() - 1.0f ,ScreenWidth()-1.0f , yOffset * vBlockSize.x,olc::WHITE);


	// Increasing score if on this frame you have "ate"
	if (ate) score++;

	// Displaying the current score

	std::string scores = "Score:";
	scores += std::to_string(score);
	scores += " Hi:";
	scores += std::to_string(hi);
	std::string player = "Player";
	
	DrawStringProp(vBlockSize.x, vBlockSize.y, player, olc::RED, 5);
	DrawStringProp(vBlockSize.x, 4 * vBlockSize.y, user, olc::RED, 5);
	DrawStringProp(vBlockSize.x, 8 * vBlockSize.y, scores, olc::RED, 5);
	return true;
}


