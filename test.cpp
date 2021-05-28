#include "test.h"

void test::set_grid(int cols, int rows)
{
	ROWS = rows;
	COLS = cols;
}
test::test()
{
	sAppName = "Test";
}
bool test::OnUserCreate()
{
	vBlockSize = {
		double(ScreenWidth()) / COLS,
		double(ScreenHeight()) / ROWS
	};

	blocks = std::make_unique<int[]>(ROWS * COLS);
	for (int y = 0 ; y < ROWS ; y++)
	{
		for (int x = 0 ; x < COLS ; x++)
		{
			if ((x + y) % 2 == 0) blocks[y * COLS + x] = 1;
			else blocks[y * COLS + x] = 0;
		}
	}
	return true;
}
bool test::OnUserUpdate(float fLastElapsed)
{

	if (GetKey(olc::Key::ESCAPE).bPressed) return false;

	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			switch (blocks[y * COLS + x])
			{
			case 0:
				FillRect(olc::vd2d(x, y) * vBlockSize, vBlockSize);
				break;
			case 1:
				FillRect(olc::vd2d(x, y) * vBlockSize, vBlockSize,olc::BLACK);
				break;
			}
		}
	}
	
	return true;
}
bool test::OnUserDestroy()
{
	return true;
}