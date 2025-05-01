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
	bool m_occupied;
	Tetromino::eBrickType m_blockType;

	Cell() : m_occupied(false), m_blockType(Tetromino::TYPE_GRID) {}

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
	void DrawGhostPiece(HDC hdc, SpriteSheet* pSpriteSheet, Tetromino* activeTetromino);
	void DrawGhostBlock(int x, int y, Tetromino::eBrickType type, HDC hdc, SpriteSheet* pSpriteSheet);

	void PositioningHoldBlock(int& x, int& y, Tetromino* holdTetromino);
	void PositioningNextBlock(int& x, int& y, int index, Tetromino* nextTetromino);

	void FixTetrominoToBoard(GameBoard& board, Tetromino* tetromino);

	bool CheckTSpin(Tetromino* tetromino);
	bool CheckPerfectClear();

	bool CheckFullLine(int y) const;
	int RemoveFullLines();

	bool IsGameOver();

	int GetMaxXIndex() const;
	int GetMaxYIndex() const;

private:
	Cell m_grid[23][12];
	int m_boardX;
	int m_boardY;
	int m_cellSize;

	int m_maxXIndex = 11;
	int m_maxYIndex = 22;
};