#include "Snake.h"

Snake::Snake() 
{
	sAppName = " Tutorial PGME";
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
	fSnakeBody.push_front(fSnakeHeadStart);
	return true;
}
bool Snake::OnUserUpdate(float fElapsedTime) 
{	
	bool ate  = FALSE;
	olc::vf2d apple = { 15.0f,15.0f };
	if (apple == fSnakeHeadStart)
	{	
		ate = TRUE;
		apple = { -10.0f,0.0f };
	}
	if (GetKey(olc::Key::LEFT).bPressed) fSnakeHeadStart.x -= 1.0f;
	if (GetKey(olc::Key::RIGHT).bPressed) fSnakeHeadStart.x += 1.0f;
	if (GetKey(olc::Key::UP).bPressed) fSnakeHeadStart.y -= 1.0f;
	if (GetKey(olc::Key::DOWN).bPressed) fSnakeHeadStart.y += 1.0f;

	if (fSnakeHeadStart.x >= ScreenWidth() / vBlockSize.x || fSnakeHeadStart.x < 0) fSnakeHeadStart = {15,15 };
	if (fSnakeHeadStart.y >= ScreenHeight() / vBlockSize.y || fSnakeHeadStart.y < 0) fSnakeHeadStart = { 15,15 };


	Clear(olc::BLACK); 
	FillRect(apple * vBlockSize, vBlockSize, olc::RED);
	FillRect(fSnakeHeadStart * vBlockSize, vBlockSize, olc::GREEN);
	//
	DrawLine(0 , 0 , ScreenWidth(),0, olc::WHITE);
	DrawLine(0, 0, 0, ScreenHeight(), olc::WHITE);
	DrawLine( 0, ScreenHeight()-1.0f, ScreenWidth()-1.0f, ScreenHeight()-1.0f,olc::WHITE);
	DrawLine( ScreenWidth() - 1.0f, ScreenHeight() - 1.0f ,ScreenWidth()-1.0f , 0,olc::WHITE);
	//const std::string a = "Test";
	if(ate) DrawStringProp((1.0f, 1.0f) * vBlockSize, "You ate\nan apple.", olc::RED, 5);
	
	
	// | = |
	SetPixelMode(olc::Pixel::NORMAL);
	
	return true;
}