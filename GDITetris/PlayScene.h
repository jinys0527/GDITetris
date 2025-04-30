#pragma once
#include "Scene.h"
#include "Tetromino.h"
#include <random>
#include "SoundManager.h"

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
     PlayScene() : mt(std::random_device{}()), m_dist(0, Tetromino::TYPE_GRID - 1), m_bgmStarted(false) {}
    ~PlayScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override;
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    int GetLevel() const;
    void UpdateLevel();

    int GetLinesCleared() const;
    void AddLinesCleard(int clearedLine);

    void AddScore(int clearedLine, bool isTSpin, int combo);
    int GetScore() const;

    void Init();

    void OnKeyDown(int key) override;
    void OnKeyUp(int key) override; 

    void OnMove(int dx, int dy);
    void OnRotate(bool clockwise, bool is180);

    void Hold();
    void RandomGenerateTetromino();

    enum LastAction
    {
        ACTION_NONE,
        ACTION_MOVE,
        ACTION_ROTATE,
        ACTION_DROP
    };
private:
    Tetris* m_pGame = nullptr;

    GameBoard* m_pGameBoard = nullptr;
    SpriteSheet* m_pSpriteSheet = nullptr;

    Tetromino* m_pTetromino = nullptr;
    Tetromino* m_pNextTetromino[4] = { nullptr, nullptr, nullptr, nullptr };
    Tetromino* m_pHoldTetromino = nullptr;

    GameObjectBase* m_pBackground = nullptr;

    bool m_canHold = true;
    bool m_wasLastMoveRotation = false;
    bool m_isTSpin = false;
    bool m_isPerfectClear = false;
    bool m_isBackToBack = false;
    bool m_wasLastSpecialAction = false;
    bool m_isTetris = false;

    const int DAS_DELAY = 170;
    const int ARR_SPEED = 75;
    int dasTimer = 0;
    int arrTimer = 0;
    bool keyLeftPressed = false;
    bool keyRightPressed = false;
    bool dasActive = false;

    int m_level = 1;
    int m_linesCleared = 0;
    int m_score = 0;
    int m_combo = 0;
    int m_TSpinlinesCleared = 0;

    int gameoverTimer = 0;
    const int GAMEOVER_DELAY = 200;
    bool isGameover = false;

    SoundManager* m_pSoundManager;
    bool m_bgmStarted;

    LastAction m_lastAction = ACTION_NONE;

    std::mt19937 mt;
    std::uniform_int_distribution<int> m_dist;
};

