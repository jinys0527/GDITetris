#include "Tetromino.h"
#include "Cell.h"
#include <iostream>

struct Offset
{
    int x;
    int y;
};

static const Offset WallKickDataNormal[5] = {
    {0, 0},
    {-1, 0},
    {-1, 1},
    {0, -2},
    {-1, -2}
};

static const Offset WallKickDataI[5] = {
    {0, 0},
    {-2, 0},
    {1, 0},
    {-2, -1},
    {1, 2}
};

Tetromino::Tetromino(eBrickType brickType) : type(brickType), rotation(0), x(4), y(-1)
{
    memset(blocks, 0, sizeof(blocks));
    //blocks[rotation][y][x]
    switch (brickType)
    {
    case TYPE_I:
        SetSize(0, 4, 1);
        blocks[0][1][0] = 1; blocks[0][1][1] = 1; blocks[0][1][2] = 1; blocks[0][1][3] = 1;

        SetSize(1, 1, 4);
        blocks[1][0][2] = 1; blocks[1][1][2] = 1; blocks[1][2][2] = 1; blocks[1][3][2] = 1; 

        SetSize(2, 4, 1); 
        blocks[2][2][0] = 1; blocks[2][2][1] = 1; blocks[2][2][2] = 1; blocks[2][2][3] = 1; 

        SetSize(3, 1, 4);
        blocks[3][0][1] = 1; blocks[3][1][1] = 1; blocks[3][2][1] = 1; blocks[3][3][1] = 1;
        break;

    case TYPE_J:
        SetSize(0, 3, 2);
        blocks[0][0][0] = 1; blocks[0][1][0] = 1; blocks[0][1][1] = 1; blocks[0][1][2] = 1;

        SetSize(1, 2, 3);
        blocks[1][0][1] = 1; blocks[1][0][2] = 1; blocks[1][1][1] = 1; blocks[1][2][1] = 1; 

        SetSize(2, 3, 2);
        blocks[2][1][0] = 1; blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][2][2] = 1; 

        SetSize(3, 2, 3);
        blocks[3][0][1] = 1; blocks[3][1][1] = 1; blocks[3][2][0] = 1; blocks[3][2][1] = 1;
        break;

    case TYPE_L:
        SetSize(0, 3, 2); 
        blocks[0][0][2] = 1; blocks[0][1][0] = 1; blocks[0][1][1] = 1; blocks[0][1][2] = 1;

        SetSize(1, 2, 3);
        blocks[1][0][1] = 1; blocks[1][1][1] = 1; blocks[1][2][1] = 1; blocks[1][2][2] = 1;

        SetSize(2, 3, 2);
        blocks[2][1][0] = 1; blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][2][0] = 1;

        SetSize(3, 2, 3);
        blocks[3][0][0] = 1; blocks[3][0][1] = 1; blocks[3][1][1] = 1; blocks[3][2][1] = 1;
        break;

    case TYPE_O:
        for (int i = 0; i < 4; i++)
        {
            SetSize(i, 2, 2);
            blocks[i][0][1] = 1; blocks[i][0][2] = 1; blocks[i][1][1] = 1; blocks[i][1][2] = 1;
        }
        break;

    case TYPE_T:
        SetSize(0, 3, 2);
        blocks[0][0][1] = 1; blocks[0][1][0] = 1; blocks[0][1][1] = 1; blocks[0][1][2] = 1;

        SetSize(1, 2, 3);
        blocks[1][0][1] = 1; blocks[1][1][1] = 1; blocks[1][1][2] = 1; blocks[1][2][1] = 1;

        SetSize(2, 3, 2);
        blocks[2][1][0] = 1; blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][2][1] = 1;

        SetSize(3, 2, 3);
        blocks[3][0][1] = 1; blocks[3][1][0] = 1; blocks[3][1][1] = 1; blocks[3][2][1] = 1;
        break;

    case TYPE_S:
        SetSize(0, 3, 2);
        blocks[0][0][1] = 1; blocks[0][0][2] = 1; blocks[0][1][0] = 1; blocks[0][1][1] = 1;
        
        SetSize(1, 2, 3);
        blocks[1][0][1] = 1; blocks[1][1][1] = 1; blocks[1][1][2] = 1; blocks[1][2][2] = 1;

        SetSize(2, 3, 2);
        blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][2][0] = 1; blocks[2][2][1] = 1;
        
        SetSize(3, 2, 3);
        blocks[3][0][0] = 1; blocks[3][1][0] = 1; blocks[3][1][1] = 1; blocks[3][2][1] = 1;
        break;

    case TYPE_Z:
        SetSize(0, 3, 2);
        blocks[0][0][0] = 1; blocks[0][0][1] = 1; blocks[0][1][1] = 1; blocks[0][1][2] = 1;

        SetSize(1, 2, 3);
        blocks[1][0][2] = 1; blocks[1][1][1] = 1; blocks[1][1][2] = 1; blocks[1][2][1] = 1;

        SetSize(2, 3, 2);
        blocks[2][1][0] = 1; blocks[2][1][1] = 1; blocks[2][2][1] = 1; blocks[2][2][2] = 1;

        SetSize(3, 2, 3);
        blocks[3][0][1] = 1; blocks[3][1][0] = 1; blocks[3][1][1] = 1; blocks[3][2][0] = 1;
        break;
    }
}

bool Tetromino::MoveLeft(const GameBoard& board)
{
    x -= 1;
    if (CheckCollision(board))
    {
        x += 1;
        return false;
    }
    return true;
}

bool Tetromino::MoveRight(const GameBoard& board)
{
    x += 1;
    if (CheckCollision(board))
    {
        x -= 1;
        return false;
    }
    return true;
}

bool Tetromino::MoveDown(const GameBoard& board)
{
    y += 1;
    if (CheckCollision(board))
    {
        y-=1;
        return false;
    }
    return true;
}

bool Tetromino::RotateCW(const GameBoard& board)
{
    return RotateWithCheckCollision(board, true);
}

bool Tetromino::RotateCCW(const GameBoard& board)
{
    return RotateWithCheckCollision(board, false);
}

bool Tetromino::HardDrop(GameBoard& board)
{
    while (MoveDown(board))
    {
        // 계속 아래로 이동
    }
    board.FixTetrominoToBoard(board, this);
    return true;
}

bool Tetromino::Rotate180(const GameBoard& board)
{
    rotation = (rotation + 1) % 4;
    return RotateWithCheckCollision(board, true);
}

bool Tetromino::TryWallKick(const GameBoard& board, int oldRotation, int newRotation)
{
    if (type == TYPE_O)
    {
        return false;
    }

    const Offset* wallKickTable = (type == TYPE_I) ? WallKickDataI : WallKickDataNormal;

    for (int i = 0; i < 5; i++)
    {
        int dx = wallKickTable[i].x;
        int dy = wallKickTable[i].y;

        x += dx;
        y += dy;
        rotation = newRotation;

        if (!CheckCollision(board)) // 충돌 없이 비어 있으면 성공
            return true;

        x -= dx;
        y -= dy;
        rotation = oldRotation;
    }

    return false; // 완전 실패
}

bool Tetromino::RotateWithCheckCollision(const GameBoard& board, bool clockwise)
{
    int oldRotation = rotation;
    int newRotation = (rotation + (clockwise ? 1 : 3)) % 4;

    rotation = newRotation;
    if (!CheckCollision(board))
    {
        return true;
    }

    rotation = oldRotation;
    return TryWallKick(board, oldRotation, newRotation);
}

bool Tetromino::CheckCollision(const GameBoard& board)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (blocks[rotation][j][i] != 0)
            {
                int boardX = x + i;
                int boardY = y + j;

                if (boardX < 0 || boardX >= board.GetMaxXIndex() || boardY < 0 || boardY >= board.GetMaxYIndex())
                    return true;

                if (board.IsOccupied(boardX, boardY))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

int Tetromino::GetBlock(int rotation, int y, int x) const
{
    return blocks[rotation][y][x];
}

void Tetromino::SetSize(int index, int width, int height)
{
    if (index >= 0 && index < 4)
    {
        this->width[index] = width;
        this->height[index] = height;
    }
}
