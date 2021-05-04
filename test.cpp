#include "test.h"

void test::set_grid(int rows, int cols)
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
	vBlockSize = { double(ScreenHeight()) / ROWS ,double( ScreenHeight() )/ COLS};
	blocks = std::make_unique<int[]>(ROWS * COLS);
	for (int y = 0 ; y < ROWS ; y++)
	{
		for (int x = 0 ; x < COLS ; x++)
		{
			if ((x + y) % 2 == 0) blocks[y * COLS + x] = 1;
			else blocks[y * COLS + x] = 0;
		}
	}
	A = { (ScreenWidth() / 2) - 5,(ScreenHeight() / 2) - 5 };
	B = { (ScreenWidth() / 2) - 5 + 10,(ScreenHeight() / 2) - 5 };
	C = { (ScreenWidth() / 2) - 5,(ScreenHeight() / 2) - 5 + 10 };
	D = { (ScreenWidth() / 2) - 5 + 10,(ScreenHeight() / 2) - 5 + 10 };
	return true;
}
bool test::OnUserUpdate(float fLastElapsed)
{

	if (GetKey(olc::Key::ESCAPE).bPressed) return false;

	/*for (int y = 0; y < ROWS; y++)
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
	}*/
	Clear(olc::BLACK);
	DrawLine(A, B);
	DrawLine(B, D);
	DrawLine(D, C);
	DrawLine(C, A);
	Draw(A, olc::RED); // A
	Draw(B, olc::RED); // B
	Draw(C, olc::RED); // C
	Draw(D, olc::RED); // D
	Sleep(100);
	DrawLine({ ScreenHeight() / 2,ScreenWidth() / 2 }, { i * 10,int(std::cos(i) * 10) },olc::RED);
	DrawLine({ ScreenHeight() / 2,ScreenWidth() / 2 }, { i * 10,int(std::sin(i) * 10) }, olc::BLUE);
	if (i == 360) i = 0;
	i++;
	return true;
}
bool test::OnUserDestroy()
{
	return true;
}