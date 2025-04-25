#pragma once
class GameBoard;

class Tetromino
{
public:
	enum eBrickType
	{
		TYPE_I = 0,     //cyan
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

	bool moveLeft();
	bool moveRight();
	bool moveDown();
	bool rotate();

	bool checkCollision(GameBoard board);
protected:
	int blocks[4][4][4];
	int width[4], height[4];
	eBrickType type;
	int rotation;
	int x, y;
};

