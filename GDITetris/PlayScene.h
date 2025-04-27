#pragma once
#include "Scene.h"
#include "Tetromino.h"
#include <random>

class GameObjectBase;
class GameObject;
class Tetris;
class GameBoard;

namespace renderHelp
{
    class SpriteSheet;
}

class PlayScene :public Scene
{
    using SpriteSheet = renderHelp::SpriteSheet;
 public:
     PlayScene() : mt(std::random_device{}()), m_dist(0, Tetromino::TYPE_GRID - 1) {}
    ~PlayScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override;
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    void OnKeyDown(int key) override;

    void Hold();
    void RandomGenerateTetromino();
private:
    Tetris* m_pGame = nullptr;

    GameBoard* m_pGameBoard = nullptr;
    SpriteSheet* m_pSpriteSheet = nullptr;

    Tetromino* m_pTetromino = nullptr;
    Tetromino* m_pNextTetromino[4] = { nullptr, nullptr, nullptr, nullptr };
    Tetromino* m_pHoldTetromino = nullptr;

    GameObjectBase* m_pBackground = nullptr;

    bool m_canHold = true;

    std::mt19937 mt;
    std::uniform_int_distribution<int> m_dist;
};

