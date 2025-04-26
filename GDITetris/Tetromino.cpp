#include "Tetromino.h"
#include "Cell.h"

Tetromino::Tetromino(eBrickType brickType) : type(brickType), rotation(0), x(4), y(4)
{
    memset(blocks, 0, sizeof(blocks));

    switch (brickType)
    {
    case TYPE_I:
        SetSize(0, 1, 4);
        blocks[0][1][0] = 1; blocks[0][1][1] = 1; blocks[0][1][2] = 1; blocks[0][1][3] = 1;

        SetSize(1, 4, 1);
        blocks[1][0][1] = 1; blocks[1][1][1] = 1; blocks[1][2][1] = 1; blocks[1][3][1] = 1;

        SetSize(2, 1, 4); 
        blocks[2][2][0] = 1; blocks[2][2][1] = 1; blocks[2][2][2] = 1; blocks[2][2][3] = 1;

        SetSize(3, 4, 1);
        blocks[3][0][2] = 1; blocks[3][1][2] = 1; blocks[3][2][2] = 1; blocks[3][3][2] = 1;
        break;

    case TYPE_J:
        SetSize(0, 2, 3);
        blocks[0][1][1] = 1; blocks[0][2][1] = 1; blocks[0][1][2] = 1; blocks[0][1][3] = 1;

        SetSize(1, 3, 2);
        blocks[1][0][2] = 1; blocks[1][1][2] = 1; blocks[1][2][2] = 1; blocks[1][2][3] = 1;

        SetSize(2, 2, 3);
        blocks[2][0][3] = 1; blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][1][3] = 1;

        SetSize(3, 3, 2);
        blocks[3][0][1] = 1; blocks[3][0][2] = 1; blocks[3][1][2] = 1; blocks[3][2][2] = 1;
        break;

    case TYPE_L:
        SetSize(0, 2, 3); 
        blocks[0][1][1] = 1; blocks[0][1][2] = 1; blocks[0][1][3] = 1; blocks[0][2][3] = 1;

        SetSize(1, 3, 2);
        blocks[1][0][2] = 1; blocks[1][1][2] = 1; blocks[1][2][2] = 1; blocks[1][0][3] = 1;

        SetSize(2, 2, 3);
        blocks[2][0][1] = 1; blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][1][3] = 1;

        SetSize(3, 3, 2);
        blocks[3][0][2] = 1; blocks[3][1][2] = 1; blocks[3][2][1] = 1; blocks[3][2][2] = 1;
        break;

    case TYPE_O:
        for (int i = 0; i < 4; i++)
        {
            SetSize(i, 2, 2);
            for (int j = 1; j < 3; j++)
            {
                for (int k = 1; k < 3; k++)
                {
                    blocks[i][j][k] = 1;
                }
            }
        }
        break;

    case TYPE_T:
        SetSize(0, 2, 3);
        blocks[0][1][1] = 1; blocks[0][1][2] = 1; blocks[0][1][3] = 1; blocks[0][2][2] = 1;

        SetSize(1, 3, 2);
        blocks[1][0][2] = 1; blocks[1][1][2] = 1; blocks[1][2][2] = 1; blocks[1][1][3] = 1;

        SetSize(2, 2, 3);
        blocks[2][0][2] = 1; blocks[2][1][1] = 1; blocks[2][1][2] = 1; blocks[2][1][3] = 1;

        SetSize(3, 3, 2);
        blocks[3][0][2] = 1; blocks[3][1][1] = 1; blocks[3][1][2] = 1; blocks[3][2][2] = 1;
        break;

    case TYPE_S:
        SetSize(0, 2, 3);
        blocks[0][1][1] = 1; blocks[0][1][2] = 1; blocks[0][2][2] = 1; blocks[0][2][3] = 1;
        
        SetSize(1, 3, 2);
        blocks[1][0][3] = 1; blocks[1][1][2] = 1; blocks[1][1][3] = 1; blocks[1][2][2] = 1;

        SetSize(2, 2, 3);
        blocks[2][0][1] = 1; blocks[2][0][2] = 1; blocks[2][1][2] = 1; blocks[2][1][3] = 1;
        
        SetSize(3, 3, 2);
        blocks[3][0][2] = 1; blocks[3][1][1] = 1; blocks[3][1][2] = 1; blocks[3][2][1] = 1;
        break;

    case TYPE_Z:
        SetSize(0, 2, 3);
        blocks[0][1][2] = 1; blocks[0][1][3] = 1; blocks[0][2][1] = 1; blocks[0][2][2] = 1;

        SetSize(1, 3, 2);
        blocks[1][1][1] = 1; blocks[1][2][1] = 1; blocks[1][2][2] = 1; blocks[1][3][2] = 1;

        SetSize(2, 2, 3);
        blocks[2][3][1] = 1; blocks[2][2][2] = 1; blocks[2][3][2] = 1; blocks[2][2][3] = 1;

        SetSize(3, 3, 2);
        blocks[3][1][2] = 1; blocks[3][2][2] = 1; blocks[3][2][3] = 1; blocks[3][3][3] = 1;
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
    int prevRotation = rotation;
    rotation = (rotation + 1) % 4;

    if (CheckCollision(board))
    {
        rotation = prevRotation;
        return false;
    }
    return true;
}

bool Tetromino::RotateCCW(const GameBoard& board)
{
    int prevRotation = rotation;
    rotation = (rotation - 1 + 4) % 4;

    if (CheckCollision(board))
    {
        rotation = prevRotation;
        return false;
    }
    return true;
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
    int prevRotation = rotation;
    rotation = (rotation + 2) % 4;

    if (CheckCollision(board))
    {
        rotation = prevRotation;
        return false;
    }
    return true;
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

                if (boardX < 0 || boardX >= 12 || boardY < 0 || boardY >= 21)
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
