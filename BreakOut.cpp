#include "BreakOut.h"

BreakOut::BreakOut() {
	sAppName = " Tutorial PGME";
}

bool BreakOut::OnUserCreate() {
	blocks = std::make_unique<int[]>(24 * 30);
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 24; x++)
		{
			if (x == 0 || y == 0 || x == 23)
				blocks[y * 24 + x] = 10;
			else
				blocks[y * 24 + x] = 0;

			if (x > 2 && x <= 20 && y > 3 && y <= 5)
				blocks[y * 24 + x] = 1;
			if (x > 2 && x <= 20 && y > 5 && y <= 7)
				blocks[y * 24 + x] = 2;
			if (x > 2 && x <= 20 && y > 7 && y <= 9)
				blocks[y * 24 + x] = 3;
		}
	}
	sprTile = std::make_unique<olc::Sprite>("tut_tiles.png");

	sprFragment = std::make_unique<olc::Sprite>("tut_fragment.png");
	decFragment = std::make_unique<olc::Decal>(sprFragment.get());

	float fAngle = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f;
	fAngle = -0.4f;
	vBallDir = { cos(fAngle), sin(fAngle) };
	vBallPos = { 12.5f, 15.5f };
	return true;
}

bool BreakOut::OnUserUpdate(float fElapsedTime) {
	// A better collision detection
		// Calculate where ball should be, if no collision
	olc::vf2d vPotentialBallPos = vBallPos + vBallDir * fBallSpeed * fElapsedTime;

	// Test for hits 4 points around ball
	olc::vf2d vTileBallRadialDims = { fBallRadius / vBlockSize.x, fBallRadius / vBlockSize.y };

	auto TestResolveCollisionPoint = [&](const olc::vf2d& point, olc::vf2d& hitpos, int& id)
	{
		olc::vi2d vTestPoint = vPotentialBallPos + vTileBallRadialDims * point;

		auto& tile = blocks[vTestPoint.y * 24 + vTestPoint.x];
		if (tile == 0)
		{
			// Do Nothing, no collision
			return false;
		}
		else
		{
			// Ball has collided with a tile
			bool bTileHit = tile < 10;
			if (bTileHit)
			{
				id = tile;
				hitpos = { float(vTestPoint.x), float(vTestPoint.y) };
				tile--;
			}

			// Collision response
			if (point.x == 0.0f) vBallDir.y *= -1.0f;
			if (point.y == 0.0f) vBallDir.x *= -1.0f;
			return bTileHit;
		}
	};

	bool bHasHitTile = false;
	olc::vf2d hitpos;
	int hitid = 0;
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, -1), hitpos, hitid);
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(0, +1), hitpos, hitid);
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(-1, 0), hitpos, hitid);
	bHasHitTile |= TestResolveCollisionPoint(olc::vf2d(+1, 0), hitpos, hitid);

	if (bHasHitTile)
	{
		for (int i = 0; i < 100; i++)
		{
			sFragment f;
			f.pos = { hitpos.x + 0.5f, hitpos.y + 0.5f };
			float fAngle = float(rand()) / float(RAND_MAX) * 2.0f * 3.14159f;
			float fVelocity = float(rand()) / float(RAND_MAX) * 10.0f;
			f.vel = { fVelocity * cos(fAngle), fVelocity * sin(fAngle) };
			f.fAngle = fAngle;
			f.fTime = 2.0f;
			if (hitid == 1)	f.colour = olc::RED;
			if (hitid == 2)	f.colour = olc::GREEN;
			if (hitid == 3)	f.colour = olc::YELLOW;
			listFragments.push_back(f);
		}
	}

	// Fake Floor
	if (vBallPos.y > 20.0f) vBallDir.y *= -1.0f;

	// Actually update ball position with modified direction
	vBallPos += vBallDir * fBallSpeed * fElapsedTime;


	// Update fragments
	for (auto& f : listFragments)
	{
		f.vel += olc::vf2d(0.0f, 20.0f) * fElapsedTime;
		f.pos += f.vel * fElapsedTime;
		f.fAngle += 5.0f * fElapsedTime;
		f.fTime -= fElapsedTime;
		f.colour.a = (f.fTime / 3.0f) * 255;
	}

	// Remove dead fragments
	listFragments.erase(
		std::remove_if(listFragments.begin(), listFragments.end(), [](const sFragment& f) { return f.fTime < 0.0f; }),
		listFragments.end());

	// Draw Screen
	Clear(olc::DARK_BLUE);
	SetPixelMode(olc::Pixel::MASK); // Dont draw pixels which have any transparency
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 24; x++)
		{
			switch (blocks[y * 24 + x])
			{
			case 0: // Do nothing
				break;
			case 10: // Draw Boundary
				DrawPartialSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get(), olc::vi2d(0, 0) * vBlockSize, vBlockSize);
				break;
			case 1: // Draw Red Block
				DrawPartialSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get(), olc::vi2d(1, 0) * vBlockSize, vBlockSize);
				break;
			case 2: // Draw Green Block
				DrawPartialSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get(), olc::vi2d(2, 0) * vBlockSize, vBlockSize);
				break;
			case 3: // Draw Yellow Block
				DrawPartialSprite(olc::vi2d(x, y) * vBlockSize, sprTile.get(), olc::vi2d(3, 0) * vBlockSize, vBlockSize);
				break;
			}
		}
	}
	SetPixelMode(olc::Pixel::NORMAL); // Draw all pixels

	// Draw Ball
	FillCircle(vBallPos * vBlockSize, int(fBallRadius), olc::CYAN);

	// Draw Fragments
	for (auto& f : listFragments)
		DrawRotatedDecal(f.pos * vBlockSize, decFragment.get(), f.fAngle, { 4, 4 }, { 1, 1 }, f.colour);


	return true;
}