#pragma once
class GameBoard;

class Tetromino
{
public:
	enum eBrickType
	{
		TYPE_NONE = -1,      //ฐ๘น้
		TYPE_I,     //cyan
		TYPE_J,         //blue
		TYPE_T,         //pupple
		TYPE_S,         //green
		TYPE_L,         //orange
		TYPE_O,			//yellow
		TYPE_Z,			//red
		TYPE_GRID,		//gray	
		TYPE_MAX
	};

	Tetromino(eBrickType brickType);
	virtual ~Tetromino() {}

	bool MoveLeft(const GameBoard& board);
	bool MoveRight(const GameBoard& board);
	bool MoveDown(const GameBoard& board);
	bool RotateCW(const GameBoard& board);
	bool RotateCCW(const GameBoard& board);
	bool HardDrop(GameBoard& board);
	bool Rotate180(const GameBoard& board);

	bool TryWallKick(const GameBoard& board, int oldRotation, int newRotation);
	bool RotateWithCheckCollision(const GameBoard& board, bool clockwise);
	bool CheckCollision(const GameBoard& board);

	void SetPosition(int x, int y) { this->x = x; this->y = y; }

	int GetBlock(int rotation, int y, int x) const;
	int GetRotation() { return rotation; }
	int GetX() { return x; }
	int GetY() { return y; }
	eBrickType GetType() { return type; }

	void SetSize(int index, int width, int height);
protected:
	int m_blocks[4][4][4];
	int m_width[4] = { 0, };
	int m_height[4] = { 0, };
	eBrickType m_type;
	int m_rotation;
	int m_x, m_y;
};

