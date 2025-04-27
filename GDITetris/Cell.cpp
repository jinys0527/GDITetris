#include "Cell.h"
#include "Utillity.h"
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

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
	int screenWidth = 0;
	int screenHeight = 0;
	learning::GetScreenSize(screenWidth, screenHeight);

	PositioningOnScreen(screenWidth, screenHeight);
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

void GameBoard::PositioningOnScreen(int windowWidth, int windowHeight)
{
	int boardWidth = 12 * cellSize;
	int boardHeight = 21 * cellSize;

	boardX = (windowWidth - boardWidth) / 2;
	boardY = (windowHeight - boardHeight) / 2;

	boardY += 110;
}

void GameBoard::Draw(HDC hdc, SpriteSheet* pSpriteSheet, Tetromino* activeTetromino, Tetromino* holdTetromino, Tetromino** nextTetrominos, int nextTetrominosSize)
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

	if (holdTetromino)
	{
		int holdX;
		int holdY;
		if (holdTetromino->GetType() == Tetromino::TYPE_I)
		{
			holdX = 200;
			holdY = 275;
		}
		else if (holdTetromino->GetType() == Tetromino::TYPE_O)
		{
			holdX = 170;
			holdY = 295;
		}
		else
		{
			holdX = 150;
			holdY = 295;
		}
		

		int rotation = holdTetromino->GetRotation();

		int minX = 4, maxX = -1, minY = 4, maxY = -1;

		for (int j = 0; j < 4; j++)		 //y
		{
			for (int i = 0; i < 4; i++)  //x
			{
				if (holdTetromino->GetBlock(rotation, j, i) != 0)
				{
					minX = min(minX, i);
					maxX = max(maxX, i);
					minY = min(minY, j);
					maxY = max(maxY, j);
				}
			}
		}

		int offsetX = (maxX - minX + 1) / 2;
		int offsetY = (maxY - minY + 1) / 2;

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (holdTetromino->GetBlock(rotation, j, i) != 0)
				{
					int x = i - offsetX;
					int y = j - offsetY;

					int drawX = holdX + (x * cellSize);
					int drawY = holdY + (y * cellSize);

					DrawBlock(drawX, drawY, holdTetromino->GetType(), hdc, pSpriteSheet);
				}
			}
		}
	}

	if (nextTetrominos)
	{
		for (int i = 0; i < nextTetrominosSize; i++)
		{
			if (nextTetrominos[i])
			{
				int nextX, nextY;

				if (nextTetrominos[i]->GetType() == Tetromino::TYPE_I)
				{
					nextX = 1080;
					nextY = 185 + i * 120; // i번째에 따라 y축 조금씩 띄우기
				}
				else if (nextTetrominos[i]->GetType() == Tetromino::TYPE_O)
				{
					nextX = 1050;
					nextY = 205 + i * 120; 
				}
				else
				{
					nextX = 1030;
					nextY = 205 + i * 120;
				}

				int rotation = nextTetrominos[i]->GetRotation();

				int minX = 4, maxX = -1, minY = 4, maxY = -1;

				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						if (nextTetrominos[i]->GetBlock(rotation, j, k) != 0)
						{
							minX = min(minX, k);
							maxX = max(maxX, k);
							minY = min(minY, j);
							maxY = max(maxY, j);
						}
					}
				}

				int offsetX = (maxX - minX + 1) / 2;
				int offsetY = (maxY - minY + 1) / 2;

				for (int j = 0; j < 4; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						if (nextTetrominos[i]->GetBlock(rotation, j, k) != 0)
						{
							int x = k - offsetX;
							int y = j - offsetY;

							int drawX = nextX + (x * cellSize);
							int drawY = nextY + (y * cellSize);

							DrawBlock(drawX, drawY, nextTetrominos[i]->GetType(), hdc, pSpriteSheet);
						}
					}
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
	int clearedLines = 0;
	for (int y = 0; y < 20; y++)
	{
		if (CheckFullLine(y))
		{
			clearedLines++;
			ClearLine(y);
		}
	}

	if (clearedLines > 0)
	{
		int points = 0;
		switch (clearedLines)
		{
		case 1:
			points = 100;
			break;
		case 2:
			points = 300;
			break;
		case 3:
			points = 500;
			break;
		case 4:
			points = 800;
			break;
		}
		linesCleared += clearedLines;
		UpdateLevel();
		AddScore(points);
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

int GameBoard::GetLevel() const
{
	return level;
}

void GameBoard::UpdateLevel()
{
	level = (linesCleared / 10) + 1;
}

int GameBoard::GetLinesCleared() const
{
	return linesCleared;
}

void GameBoard::AddScore(int points)
{
	score += points;
}

int GameBoard::GetScore() const
{
	return score;
}
