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

	void Clear();

	void Set(Tetromino::eBrickType type);
};

class GameBoard
{
	using SpriteSheet = renderHelp::SpriteSheet;
public:
	GameBoard(int x, int y, int size);

	void InitializeBoard();

	bool IsOccupied(int x, int y) const;

	Tetromino::eBrickType GetBlockType(int x, int y) const;

	void SetCell(int x, int y, Tetromino::eBrickType type);

	void PositioningOnScreen(int windowWidth, int windowHeight);

	void Draw(HDC hdc, SpriteSheet* pSpriteSheet, Tetromino* activeTetromino, Tetromino* holdTetromino, Tetromino** nextTetrominos, int nextTetrominosSize);
	void DrawBlock(int x, int y, Tetromino::eBrickType type, HDC hdc, SpriteSheet* pSpriteSheet);

	void FixTetrominoToBoard(GameBoard& board, Tetromino* tetromino);

	bool CheckFullLine(int y) const;
	void ClearLine(int y);
	void RemoveFullLines();

	bool IsGameOver();

	int GetLevel() const;
	void UpdateLevel();

	int GetLinesCleared() const;

	void AddScore(int points);
	int GetScore() const;
private:
	Cell grid[21][12];
	int boardX;
	int boardY;
	int cellSize;

	int level = 1;
	int linesCleared = 0;
	int score = 0;
};