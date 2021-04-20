#include "Snake.h"

Snake::Snake() 
{
	sAppName = " Snake ";
}


bool Snake::OnUserCreate() 
{	
	blocks = std::make_unique<int[]>(31 * 31);
	for(int y = 0; y < 31 ; y++)
	{
		for (int x = 0; x  < 31; x++)
		{
			if ((x + y) % 2 == 0) blocks[y * 31 + x] = 0;
			else blocks[y * 31 + x] = 1;
		}
	}

	apple = { 15.0f,15.0f };
	speed = 5;
	fSnakeBody.reserve(100);
	fSnakeBody.push_back({ 10.0f,10.0f });

	return true;
}

bool Snake::OnUserUpdate(float fElapsedTime)
{  
	head = fSnakeBody[0];
	for (auto part = begin(fSnakeBody) + 1, e = end(fSnakeBody); part != e; ++part)
	{
		if (*part == head) return 0;
	}

	bool ate = FALSE;
	oldTAIL = fSnakeBody.back();
	for (int i = fSnakeBody.size() - 1; i >= 1; i--) 
	{
		fSnakeBody[i] = fSnakeBody[i - 1];
	}

	if (GetKey(olc::Key::LEFT).bHeld && direction.x != 1 && fSnakeBody.size()>1)
	{
		direction.x = -1;
		direction.y = 0;
	}
	else if (GetKey(olc::Key::LEFT).bHeld && fSnakeBody.size() == 1) 
	{
		direction.x = -1;
		direction.y = 0;
	}
	if (GetKey(olc::Key::RIGHT).bHeld && direction.x != -1 && fSnakeBody.size() > 1)
	{
		direction.x = 1;
		direction.y = 0;
	}
	else if (GetKey(olc::Key::RIGHT).bHeld && fSnakeBody.size() == 1) {
		direction.x = 1;
		direction.y = 0;
	}
	if (GetKey(olc::Key::UP).bHeld && direction.y != 1 && fSnakeBody.size() > 1)
	{
		direction.y = -1;
		direction.x = 0;
	}
	else if (GetKey(olc::Key::UP).bHeld && fSnakeBody.size() == 1) {
		direction.x = 0;
		direction.y = -1;
	}
	if (GetKey(olc::Key::DOWN).bHeld && direction.y != -1 && fSnakeBody.size()>1)
	{
		direction.x = 0;
		direction.y = 1;
	}
	else if (GetKey(olc::Key::DOWN).bHeld && fSnakeBody.size() == 1) 
	{
		direction.x = 0;
		direction.y = 1;
	}
	if (direction.y == -1)
	{
		Sleep(speed);
		fSnakeBody.front().y -= 1;
	}
	if (direction.y == 1) 
	{
		Sleep(speed);
		fSnakeBody.front().y += 1;
	}
	if (direction.x == -1) 
	{
		Sleep(speed);
		fSnakeBody.front().x -= 1;
	}
	if (direction.x == 1) 
	{
		Sleep(speed);
		fSnakeBody.front().x += 1;
	}
		
	if (fSnakeBody.front() == apple)
	{
		ate = true;
		apple = { rand() % 24 + 4.0f,rand() % 24 + 4.0f };
		fSnakeBody.push_back(oldTAIL);
	}

	if (fSnakeBody.front().x >= ScreenWidth() / vBlockSize.x || fSnakeBody.front().x < 0) return 0;
	if (fSnakeBody.front().y >= ScreenHeight() / vBlockSize.y || fSnakeBody.front().y < 4.0f) return 0;
	
	Clear(olc::BLACK);
	FillRect(apple * vBlockSize, vBlockSize, olc::RED);
	for (auto part = fSnakeBody.rbegin(); part != fSnakeBody.rend(); ++part) 
	{
	FillRect(*part * vBlockSize, vBlockSize, olc::GREEN);
	//DrawRect(*part * vBlockSize, vBlockSize, olc::WHITE);
}
	DrawLine(0 , 4.0f*16 , ScreenWidth(),4.0f*16, olc::WHITE);
	DrawLine(0, 4.0f * 16, 0, ScreenHeight(), olc::WHITE);
	DrawLine( 0, ScreenHeight()-1.0f, ScreenWidth()-1.0f, ScreenHeight()-1.0f,olc::WHITE);
	DrawLine( ScreenWidth() - 1.0f, ScreenHeight() - 1.0f ,ScreenWidth()-1.0f , 4.0f * 16,olc::WHITE);
	if (ate) score++;
	
	std::string s = "Score: ";
	s += std::to_string(score);
	DrawStringProp((1.0f, 1.0f) * vBlockSize,s, olc::RED, 5); 

	return true;
}

bool Snake::OnUserDestroy() 
{
	std::cout << "GOOD JOB!";
	return 1;
}