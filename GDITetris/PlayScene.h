#pragma once
#include "Scene.h"
#include "Tetromino.h"
#include <random>
#include "SoundManager.h"
#include "RankingManager.h"

class GameObjectBase;
class GameObject;
class Tetris;
class GameBoard;
class Background;

namespace renderHelp
{
    class SpriteSheet;
}

struct SevenBag
{
    int blocks[7];
    int currentIndex;
};

class PlayScene :public Scene
{
    using SpriteSheet = renderHelp::SpriteSheet;
 public:
     PlayScene() : Scene(), mt(std::random_device{}()), m_bgmStarted(false) {}
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
    void OnClicked(int x, int y) override;

    bool OnMove(int dx, int dy);
    bool OnRotate(bool clockwise, bool is180);

    bool Hold();
    void RandomGenerateTetromino();

    void SaveScore(const wchar_t* name);

    enum LastAction
    {
        ACTION_NONE,
        ACTION_MOVE,
        ACTION_ROTATE,
        ACTION_DROP
    };
private:
    void InitBag();
    void ShuffleBag();
    int GetNextBlockType();

    Tetris* m_pGame = nullptr;

    GameBoard* m_pGameBoard = nullptr;
    SpriteSheet* m_pSpriteSheet = nullptr;

    Tetromino* m_pTetromino = nullptr;
    Tetromino* m_pNextTetromino[4] = { nullptr, nullptr, nullptr, nullptr };
    Tetromino* m_pHoldTetromino = nullptr;

    GameObjectBase* m_pBackground = nullptr;
    Background* m_pGameover = nullptr;

    Background* m_pEnterName = nullptr;

    RankingManager m_rankingManager;
    wchar_t m_playerName[4] = { 0 };

    enum eGameState
    {
        PLAYING,
        GAMEOVER,
        ENTERINGNAME
    };

    eGameState m_GameState = PLAYING;

    SevenBag m_bag = { { 0, }, 0 };

    bool m_canHold = true;
    bool m_wasLastMoveRotation = false;
    bool m_isTSpin = false;
    bool m_isPerfectClear = false;
    bool m_isBackToBack = false;
    bool m_wasLastSpecialAction = false;
    bool m_isTetris = false;

    const int DAS_DELAY = 170;
    const int ARR_SPEED = 50;
    int m_dasTimer = 0;
    int m_arrTimer = 0;
    bool m_keyLeftPressed = false;
    bool m_keyRightPressed = false;
    bool m_dasActive = false;

    int m_level = 1;
    int m_linesCleared = 0;
    int m_score = 0;
    int m_combo = 0;
    int m_TSpinlinesCleared = 0;

    bool isInitialized = false;

    bool m_isGameover = false;

    bool m_isEnterClicked = false;

    bool m_bgmStarted;

    LastAction m_lastAction = ACTION_NONE;

    std::mt19937 mt;
};

