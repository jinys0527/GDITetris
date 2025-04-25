#pragma once
#include "Tetromino.h"
#include <wtypes.h>
#include "RenderHelp.h"

namespace renderHelp
{
	class SpriteSheet;
}

class Cell
{
public:
	bool occupied;
	Tetromino::eBrickType blockType;

	Cell() : occupied(false), blockType(Tetromino::TYPE_GRID) {}

	void Clear()
	{
		occupied = false;
		blockType = Tetromino::TYPE_GRID;
	}

	void Set(Tetromino::eBrickType type)
	{
		occupied = true;
		blockType = type;
	}
};

class GameBoard
{
	using SpriteSheet = renderHelp::SpriteSheet;
private:
	Cell grid[22][12];
	int boardX;
	int boardY;
	int cellSize;
public:
	GameBoard(int x, int y, int size) : boardX(x), boardY(y), cellSize(size)
	{
		InitializeBoard();
	}

	void InitializeBoard()
	{
		for (int y = 0; y < 22; y++)
		{
			for (int x = 0; x < 12; x++)
			{
				grid[y][x].Clear();
			}
		}

		for (int y = 0; y < 22; y++) {
			grid[y][0].Set(Tetromino::TYPE_GRID);
			grid[y][11].Set(Tetromino::TYPE_GRID);
		}

		for (int x = 0; x < 12; x++) {
			grid[21][x].Set(Tetromino::TYPE_GRID);
		}
	}

	bool IsOccupied(int x, int y) const {
		if (x <= 0 || x >= 11 || y <= 0 || y >= 21) return true;
		return grid[y][x].occupied;
	}

	Tetromino::eBrickType GetBlockType(int x, int y) const {
		if (x <= 0 || x >= 11 || y <= 0 || y >= 21) return Tetromino::TYPE_GRID;
		return grid[y][x].blockType;
	}

	void SetCell(int x, int y, Tetromino::eBrickType type) {
		if (x >= 0 && x < 12 && y >= 0 && y < 22) {
			grid[y][x].Set(type);
		}
	}

	void CenterOnScreen(int windowWidth, int windowHeight)
	{
		int boardWidth = 12 * cellSize;
		int boardHeight = 22 * cellSize;

		boardX = (windowWidth - boardWidth) / 2;
		boardY = (windowHeight - boardHeight) / 2;
	}

	void Draw(HDC hdc, SpriteSheet* pSpriteSheet)
	{
		for (int y = 0; y < 22; y++)
		{
			for (int x = 0; x < 12; x++)
			{
				int drawX = boardX + (x * cellSize);
				int drawY = boardY + (y * cellSize);;

				Tetromino::eBrickType type = GetBlockType(x, y);
				if (IsOccupied(x, y))
				{
					DrawBlock(drawX, drawY, type, hdc, pSpriteSheet);
				}
			}
		}
	}

	void DrawBlock(int x, int y, Tetromino::eBrickType type, HDC hdc, SpriteSheet* pSpriteSheet)
	{
		if (type < 0 || type >= Tetromino::TYPE_MAX || !pSpriteSheet)
			return;

		// 테트로미노 타입을 스프라이트 인덱스로 사용
		int spriteIndex = static_cast<int>(type);

		// 스프라이트 그리기
		pSpriteSheet->DrawSprite(hdc, spriteIndex, x, y, cellSize, cellSize);
	}
};