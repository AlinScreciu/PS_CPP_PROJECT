#include "Snake.h"
#include <Windows.h>
#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")
Snake::Snake()
{
	sAppName = " Snake ";
}

void Snake::movement()

{
	moved = false;
	if (GetKey(olc::Key::UP).bPressed && direction.y != 1 && fSnakeBody.size() > 1 && !moved)
	{
		direction.x = 0;
		direction.y = -1;
		moved = true;
	}
	else if ((GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed) && fSnakeBody.size() == 1) {
		direction.x = 0;
		direction.y = -1;
		moved = true;
	}
	if (GetKey(olc::Key::DOWN).bPressed && direction.y != -1 && fSnakeBody.size() > 1 && !moved)
	{
		direction.x = 0;
		direction.y = 1;
		moved = true;
	}
	else if (GetKey(olc::Key::DOWN).bPressed && fSnakeBody.size() == 1)
	{
		direction.x = 0;
		direction.y = 1;
		moved = true;
	}
	if (GetKey(olc::Key::LEFT).bPressed && direction.x != 1 && fSnakeBody.size() > 1 && !moved)
	{
		direction = { -1,0 };
		moved = true;
	}
	else if (GetKey(olc::Key::LEFT).bPressed && fSnakeBody.size() == 1)
	{
		direction.x = -1;
		direction.y = 0;
		moved = true;
	}
	if (GetKey(olc::Key::RIGHT).bPressed && direction.x != -1 && fSnakeBody.size() > 1 && !moved)
	{
		direction.x = 1;
		direction.y = 0;
		moved = true;
	}
	else if (GetKey(olc::Key::RIGHT).bPressed && fSnakeBody.size() == 1) {
		direction.x = 1;
		direction.y = 0;
		moved = true;
	}
}
void Snake::update_snake(){
	if (direction.y == -1)
	{
		fSnakeBody.front().y -= 1;
	}
	if (direction.y == 1)
	{
		fSnakeBody.front().y += 1;
	}
	if (direction.x == -1)
	{
		fSnakeBody.front().x -= 1;
	}
	if (direction.x == 1)
	{
		fSnakeBody.front().x += 1;
	}
}
bool Snake::OnUserCreate()
{


	apple = { 15.0f,15.0f };
	speed = 80;
	fSnakeBody.reserve(100);
	fSnakeBody.push_back({ 10.0f,10.0f });
	std::fstream alabala ("ceva.txt", std::ios::in && std::ios::out && std::fstream::app);
	if (alabala.peek() == -1) {
		alabala << score;
	}
	alabala.close();
	sprHead = std::make_unique<olc::Sprite>("capuPedro.png");
	return true;
}

bool Snake::OnUserUpdate(float fElapsedTime)
{
	movement();
	fAccumulatedTime += fElapsedTime;
	if (fAccumulatedTime >= fTargetFrameTime)
	{
		fAccumulatedTime -= fTargetFrameTime;
		fElapsedTime = fTargetFrameTime;
	}
	else
		return true; // Don't do anything this frame
	bool ate = FALSE;

	// Save the old tail and then update the position of the snake
	// You copy the position of the part in front of you
	// The head will be updated afterwards by the direction

	oldTAIL = fSnakeBody.back();
	for (int i = fSnakeBody.size() - 1; i >= 1; i--)
	{
		fSnakeBody[i] = fSnakeBody[i - 1];
	}


	// Movment


	
	update_snake();

	// Checking boundaries

	if (fSnakeBody.front().x >= ScreenWidth() / vBlockSize.x || fSnakeBody.front().x < 0) return false;
	if (fSnakeBody.front().y >= ScreenHeight() / vBlockSize.y || fSnakeBody.front().y < 4.0f) return false;

	// Check if head overlaps any part of the body

	head = fSnakeBody[0];
	for (auto part = begin(fSnakeBody) + 1, e = end(fSnakeBody); part != e; ++part)
	{
		if (*part == head) return 0;
	}
	// Eating and doing something naughty

	if (fSnakeBody.front() == apple)
	{
		ate = true;
		apple = { rand() % 24 + 4.0f,rand() % 24 + 4.0f };
		fSnakeBody.push_back(oldTAIL);
		if ((rand() % 100 ) % 2 == 0)
		{
			PlaySound(TEXT("resources/moan.wav"), GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
		}
		else 
		{ 
			PlaySound(TEXT("resources/moan2.wav"), GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC); 
		}
	}
	if (ate) 
	{
		bool good = false;
		while (!good) 
		{
			for (auto& part : fSnakeBody) 
			{
				if (apple == part) good = false;
				else good = true;
			}
		}
	}


	// DRAWING
	///
	Clear(olc::BLACK);


	// Drawing the apple first so the snake will draw over the apple

	FillRect(apple * vBlockSize, vBlockSize, olc::RED);
	

	// Drawing the snake

	for (auto part = fSnakeBody.rbegin(); part != fSnakeBody.rend(); ++part)
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

	DrawLine(0 , 4.0f * vBlockSize.x , ScreenWidth(),4.0f* vBlockSize.x, olc::WHITE);
	DrawLine(0, 4.0f * vBlockSize.x, 0, ScreenHeight(), olc::WHITE);
	DrawLine( 0, ScreenHeight()-1.0f, ScreenWidth()-1.0f, ScreenHeight()-1.0f,olc::WHITE);
	DrawLine( ScreenWidth() - 1.0f, ScreenHeight() - 1.0f ,ScreenWidth()-1.0f , 4.0f * vBlockSize.x,olc::WHITE);


	// Increasing score if on this frame you have "ate"

	if (ate) score++;


	// Displaying the current score

	std::string s = "Score: ";
	s += std::to_string(score);
	DrawStringProp((1.0f, 1.0f) * vBlockSize,s, olc::RED, 5);
	return true;
}
bool Snake::OnUserDestroy() {
	std::fstream alabala("ceva.txt", std::ios::in && std::ios::out && std::fstream::trunc);
	int hi;
	alabala >> hi;
	if (score > hi) {
		alabala << score;
	}
	return true;
}
