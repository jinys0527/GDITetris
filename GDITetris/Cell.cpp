#include "Cell.h"
#include "Utillity.h"
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void Cell::Clear()
{
	m_occupied = false;
	m_blockType = Tetromino::TYPE_NONE;
}

void Cell::Set(Tetromino::eBrickType type)
{
	m_occupied = true;
	m_blockType = type;
}

GameBoard::GameBoard(int x, int y, int size) : m_boardX(x), m_boardY(y), m_cellSize(size)
{
	InitializeBoard();
}

void GameBoard::InitializeBoard()
{
	for (int y = 0; y <= m_maxYIndex; y++)
	{
		for (int x = 0; x <= m_maxXIndex; x++)
		{
			m_grid[y][x].Clear();
		}
	}

	for (int y = 0; y < 2; y++)
	{
		m_grid[y][0].Set(Tetromino::TYPE_NONE);
		m_grid[y][11].Set(Tetromino::TYPE_NONE);
	}

	for (int y = 2; y <= m_maxYIndex; y++) {
		m_grid[y][0].Set(Tetromino::TYPE_GRID);
		m_grid[y][11].Set(Tetromino::TYPE_GRID);
	}

	for (int x = 0; x <= m_maxXIndex; x++) {
		m_grid[22][x].Set(Tetromino::TYPE_GRID);
	}
	int screenWidth = 0;
	int screenHeight = 0;
	learning::GetScreenSize(screenWidth, screenHeight);

	PositioningOnScreen(screenWidth, screenHeight);
}

bool GameBoard::IsOccupied(int x, int y) const
{
	if (x < 0 || x >= m_maxXIndex || y < 0 || y >= m_maxYIndex)
	{
		return true;
	}

	return m_grid[y][x].m_occupied;
}

Tetromino::eBrickType GameBoard::GetBlockType(int x, int y) const
{
	return m_grid[y][x].m_blockType;
}

void GameBoard::SetCell(int x, int y, Tetromino::eBrickType type)
{
	if (x >= 0 && x <= m_maxXIndex && y >= 0 && y <= m_maxYIndex) {
		m_grid[y][x].Set(type);
	}
}

void GameBoard::PositioningOnScreen(int windowWidth, int windowHeight)
{
	int boardWidth = (m_maxXIndex + 1) * m_cellSize;
	int boardHeight = (m_maxYIndex + 1) * m_cellSize;

	m_boardX = (windowWidth - boardWidth) / 2;
	m_boardY = (windowHeight - boardHeight) / 2;

	m_boardY += 80;
}

void GameBoard::Draw(HDC hdc, SpriteSheet* pSpriteSheet, Tetromino* activeTetromino, Tetromino* holdTetromino, Tetromino** nextTetrominos, int nextTetrominosSize)
{
	for (int y = 0; y <= m_maxYIndex; y++)
	{
		for (int x = 0; x <= m_maxXIndex; x++)
		{
			if (IsOccupied(x, y))
			{
				int drawX = m_boardX + (x * m_cellSize);
				int drawY = m_boardY + (y * m_cellSize);;
				DrawBlock(drawX, drawY, GetBlockType(x, y), hdc, pSpriteSheet);
			}
		}
	}

	if (activeTetromino)
	{
		DrawGhostPiece(hdc, pSpriteSheet, activeTetromino);
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

					int drawX = m_boardX + (x * m_cellSize);
					int drawY = m_boardY + (y * m_cellSize);

					DrawBlock(drawX, drawY, activeTetromino->GetType(), hdc, pSpriteSheet);
				}
			}
		}
	}

	if (holdTetromino)
	{
		int holdX;
		int holdY;
		
		PositioningHoldBlock(holdX, holdY, holdTetromino);

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

					int drawX = holdX + (x * m_cellSize);
					int drawY = holdY + (y * m_cellSize);

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

				PositioningNextBlock(nextX, nextY, i, nextTetrominos[i]);

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

							int drawX = nextX + (x * m_cellSize);
							int drawY = nextY + (y * m_cellSize);

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
	pSpriteSheet->DrawSprite(hdc, spriteIndex, x, y, m_cellSize, m_cellSize, 255);
}

void GameBoard::DrawGhostPiece(HDC hdc, SpriteSheet* pSpriteSheet, Tetromino* activeTetromino)
{
	if (!activeTetromino) return;

	int originalX = activeTetromino->GetX();
	int originalY = activeTetromino->GetY();
	int rotation = activeTetromino->GetRotation();
	Tetromino::eBrickType type = activeTetromino->GetType();

	int topHeight[12];
	for (int x = 0; x < m_maxXIndex; x++) {
		topHeight[x] = m_maxYIndex; // 초기값은 보드 맨 아래
		// 위에서부터 아래로 검사하여 첫 블록 위치 찾기
		for (int y = 2; y < m_maxYIndex; y++) {
			if (IsOccupied(x, y)) {
				topHeight[x] = y;
				break;
			}
		}
	}

	// 테트로미노의 각 열마다 떨어질 위치 계산
	int dropDistance = 100; // 충분히 큰 값으로 초기화

	// 테트로미노의 가장 아래 블록 위치
	int lowestBlockByColumn[4] = { -1, -1, -1, -1 };

	// 테트로미노의 각 열에서 가장 아래 블록 위치 찾기
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j >= 0; j--) {
			if (activeTetromino->GetBlock(rotation, j, i) != 0) {
				lowestBlockByColumn[i] = j;
				break;
			}
		}
	}

	// 각 열에 대해 낙하 거리 계산
	for (int i = 0; i < 4; i++) {
		if (lowestBlockByColumn[i] != -1) { // 이 열에 블록이 있으면
			int x = originalX + i;

			if (x >= 0 && x <= m_maxXIndex) { // 유효한 x 좌표면
				int blockBottom = originalY + lowestBlockByColumn[i];
				int distance = topHeight[x] - blockBottom - 1;
				dropDistance = min(dropDistance, distance);
			}
		}
	}

	// 안전 체크
	if (dropDistance < 0) dropDistance = 0;
	if (dropDistance > m_maxYIndex) dropDistance = m_maxYIndex;

	// 고스트 피스의 Y 위치 계산
	int ghostY = originalY + dropDistance;

	// 고스트 피스 그리기
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			if (activeTetromino->GetBlock(rotation, j, i) != 0) {
				int x = originalX + i;
				int y = ghostY + j;

				// 화면 안에 있는지 확인
				if (y >= 0 && y <= m_maxYIndex && x >= 0 && x <= m_maxXIndex) {
					int drawX = m_boardX + (x * m_cellSize);
					int drawY = m_boardY + (y * m_cellSize);

					DrawGhostBlock(drawX, drawY, type, hdc, pSpriteSheet);
				}
			}
		}
	}
}

void GameBoard::DrawGhostBlock(int x, int y, Tetromino::eBrickType type, HDC hdc, SpriteSheet* pSpriteSheet)
{
	if (type < 0 || type >= Tetromino::TYPE_MAX || !pSpriteSheet)
		return;

	int spriteIndex = static_cast<int>(type);

	pSpriteSheet->DrawSprite(hdc, spriteIndex, x, y, m_cellSize, m_cellSize, 128);
}

void GameBoard::PositioningHoldBlock(int& x, int& y, Tetromino* holdTetromino)
{
	if (holdTetromino->GetType() == Tetromino::TYPE_I)
	{
		switch (holdTetromino->GetRotation())
		{
		case 0:
			x = 200;
			y = 275;
			break;
		case 1:
			x = 120;
			y = 325;
			break;
		case 2:
			x = 230;
			y = 250;
			break;
		case 3:
			x = 130;
			y = 325;
			break;
		}
	}
	else if (holdTetromino->GetType() == Tetromino::TYPE_O)
	{
		x = 170;
		y = 325;
	}
	else if (holdTetromino->GetType() == Tetromino::TYPE_J || holdTetromino->GetType() == Tetromino::TYPE_L)
	{
		switch (holdTetromino->GetRotation())
		{
		case 0:
			x = 185;
			y = 320;
			break;
		case 1:
			x = 170;
			y = 310;
			break;
		case 2:
			x = 185;
			y = 300;
			break;
		case 3:
			x = 200;
			y = 310;
			break;
		}
	}
	else if (holdTetromino->GetType() == Tetromino::TYPE_S || holdTetromino->GetType() == Tetromino::TYPE_Z)
	{
		switch (holdTetromino->GetRotation())
		{
		case 0:
			x = 185;
			y = 320;
			break;
		case 1:
			x = 165;
			y = 315;
			break;
		case 2:
			x = 185;
			y = 300;
			break;
		case 3:
			x = 195;
			y = 310;
			break;
		}
	}
	else
	{
		switch (holdTetromino->GetRotation())
		{
		case 0:
			x = 185;
			y = 320;
			break;
		case 1:
			x = 170;
			y = 310;
			break;
		case 2:
			x = 185;
			y = 300;
			break;
		case 3:
			x = 190;
			y = 310;
			break;
		}
	}
}

void GameBoard::PositioningNextBlock(int& x, int& y, int index, Tetromino* nextTetromino)
{
	if (nextTetromino->GetType() == Tetromino::TYPE_I)
	{
		x = 1078;
		y = 145 + index * 135;
	}
	else if (nextTetromino->GetType() == Tetromino::TYPE_O)
	{
		x = 1048;
		y = 185 + index * 135;
	}
	else if (nextTetromino->GetType() == Tetromino::TYPE_L || nextTetromino->GetType() == Tetromino::TYPE_J)
	{
		x = 1060;
		y = 195 + index * 135;
	}
	else if (nextTetromino->GetType() == Tetromino::TYPE_S || nextTetromino->GetType() == Tetromino::TYPE_Z)
	{
		x = 1060;
		y = 195 + index * 135;
	}
	else
	{
		x = 1060;
		y = 185 + index * 135;
	}
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

				if (boardX >= 0 && boardX <= m_maxXIndex && boardY >= 0 && boardY <= m_maxYIndex)
				{
					board.SetCell(boardX, boardY, tetromino->GetType());
				}
			}
		}
	}
}

bool GameBoard::CheckTSpin(Tetromino* tetromino)
{
	if (!tetromino || tetromino->GetType() != Tetromino::TYPE_T)
		return false;

	int tX = tetromino->GetX();
	int tY = tetromino->GetY();

	int centerX = tX + 1;
	int centerY = tY + 1;

	int cornerCoords[4][2] = {
		{centerX - 1, centerY - 1},
		{centerX - 1, centerY + 1},
		{centerX + 1, centerY - 1},
		{centerX + 1, centerY + 1},
	};

	int occupiedCorners = 0;
	for (int i = 0; i < 4; i++)
	{
		int x = cornerCoords[i][0];
		int y = cornerCoords[i][1];

		if (x < 0 || x > m_maxXIndex || y < 0 || y > m_maxYIndex || IsOccupied(x, y))
		{
			occupiedCorners++;
		}
	}

	return occupiedCorners >= 3;
}

bool GameBoard::CheckPerfectClear()
{
	for (int y = m_maxYIndex - 1; y >= 2; y--)
	{
		for (int x = 1; x < m_maxXIndex; x++)
		{
			if (IsOccupied(x, y))
			{
				return false;
			}
		}
	}
	return true;
}

bool GameBoard::CheckFullLine(int y) const
{
	for (int x = 1; x < m_maxXIndex; x++)
	{
		if (!IsOccupied(x, y))
		{
			return false;
		}
	}

	return true;
}

int GameBoard::RemoveFullLines()
{
	bool isFullLine[22] = { false };
	int clearedLines = 0;

	for (int y = m_maxYIndex - 1; y >= 2; y--)
	{
		if (CheckFullLine(y))
		{
			isFullLine[y] = true;
			clearedLines++;
		}
	}

	if (clearedLines == 0)
	{
		return clearedLines;
	}


	int targetY = m_maxYIndex - 1;

	for (int sourceY = m_maxYIndex - 1; sourceY >= 2; sourceY--)
	{
		if (isFullLine[sourceY])
		{
			continue;
		}

		if (sourceY != targetY)
		{
			for (int x = 1; x < m_maxXIndex; x++)
			{
				m_grid[targetY][x] = m_grid[sourceY][x];
				m_grid[sourceY][x].Clear();
			}
		}

		targetY--;
	}

	return clearedLines;
}

bool GameBoard::IsGameOver()
{
	for (int x = 1; x < m_maxXIndex; x++)
	{
		if (IsOccupied(x, 2))
		{ 
			return true;
		}
	}
	return false;
}

int GameBoard::GetMaxXIndex() const
{
	return m_maxXIndex;
}

int GameBoard::GetMaxYIndex() const
{
	return m_maxYIndex;
}
