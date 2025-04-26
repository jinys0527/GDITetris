#include "Cell.h"

void Cell::Clear()
{
	occupied = false;
	blockType = Tetromino::TYPE_NONE;
}

void Cell::Set(Tetromino::eBrickType type)
{
	occupied = true;
	blockType = type;
}

GameBoard::GameBoard(int x, int y, int size) : boardX(x), boardY(y), cellSize(size)
{
	InitializeBoard();
}

void GameBoard::InitializeBoard()
{
	for (int y = 0; y < 21; y++)
	{
		for (int x = 0; x < 12; x++)
		{
			grid[y][x].Clear();
		}
	}

	for (int y = 0; y < 21; y++) {
		grid[y][0].Set(Tetromino::TYPE_GRID);
		grid[y][11].Set(Tetromino::TYPE_GRID);
	}

	for (int x = 0; x < 12; x++) {
		grid[20][x].Set(Tetromino::TYPE_GRID);
	}

	CenterOnScreen(1024, 720);
}

bool GameBoard::IsOccupied(int x, int y) const
{
	if (x < 0 || x >= 12 || y < 0 || y >= 21) return true;
	return grid[y][x].occupied;
}

Tetromino::eBrickType GameBoard::GetBlockType(int x, int y) const
{
	return grid[y][x].blockType;
}

void GameBoard::SetCell(int x, int y, Tetromino::eBrickType type)
{
	if (x >= 0 && x < 12 && y >= 0 && y < 21) {
		grid[y][x].Set(type);
	}
}

void GameBoard::CenterOnScreen(int windowWidth, int windowHeight)
{
	int boardWidth = 12 * cellSize;
	int boardHeight = 21 * cellSize;

	boardX = (windowWidth - boardWidth) / 2;
	boardY = (windowHeight - boardHeight) / 2;
}

void GameBoard::Draw(HDC hdc, SpriteSheet* pSpriteSheet, Tetromino* activeTetromino)
{
	for (int y = 0; y < 21; y++)
	{
		for (int x = 0; x < 12; x++)
		{
			if (IsOccupied(x, y))
			{
				int drawX = boardX + (x * cellSize);
				int drawY = boardY + (y * cellSize);;
				DrawBlock(drawX, drawY, GetBlockType(x, y), hdc, pSpriteSheet);
			}
		}
	}

	if (activeTetromino)
	{
		int rotation = activeTetromino->GetRotation();
		int baseX = activeTetromino->GetX();
		int baseY = activeTetromino->GetY();

		for (int j = 0; j < 4; j++)     //y
		{
			for (int i = 0; i < 4; i++) //x
			{
				if (activeTetromino->GetBlock(rotation, j, i) != 0)
				{
					int x = baseX + i;
					int y = baseY + j;

					int drawX = boardX + (x * cellSize);
					int drawY = boardY + (y * cellSize);

					DrawBlock(drawX, drawY, activeTetromino->GetType(), hdc, pSpriteSheet);
				}
			}
		}
	}
}

void GameBoard::DrawBlock(int x, int y, Tetromino::eBrickType type, HDC hdc, SpriteSheet* pSpriteSheet)
{
	if (type < 0 || type >= Tetromino::TYPE_MAX || !pSpriteSheet)
		return;

	// 테트로미노 타입을 스프라이트 인덱스로 사용
	int spriteIndex = static_cast<int>(type);

	// 스프라이트 그리기
	pSpriteSheet->DrawSprite(hdc, spriteIndex, x, y, cellSize, cellSize);
}

void GameBoard::FixTetrominoToBoard(GameBoard& board, Tetromino* tetromino)
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (tetromino->GetBlock(tetromino->GetRotation(), j, i) != 0)
			{
				int boardX = tetromino->GetX() + i;
				int boardY = tetromino->GetY() + j;

				if (boardX >= 0 && boardX < 12 && boardY >= 0 && boardY < 21)
				{
					board.SetCell(boardX, boardY, tetromino->GetType());
				}
			}
		}
	}
}

bool GameBoard::CheckFullLine(int y) const
{
	for (int x = 1; x < 11; x++)
	{
		if (!IsOccupied(x, y))
		{
			return false;
		}
	}
	return true;
}

void GameBoard::ClearLine(int y)
{
	for (int x = 1; x < 11; x++)
	{
		grid[y][x].Clear();
	}

	for (int i = y; i > 1; i--)
	{
		for (int x = 1; x < 11; x++)
		{
			grid[i][x] = grid[i - 1][x];
		}
	}
}

void GameBoard::RemoveFullLines()
{
	for (int y = 0; y < 20; y++)
	{
		if (CheckFullLine(y))
		{
			ClearLine(y);
		}
	}
}

bool GameBoard::IsGameOver()
{
	for (int x = 1; x <= 10; x++)
	{
		if (IsOccupied(x, 2))
		{ 
			return true;
		}
	}
	return false;
}