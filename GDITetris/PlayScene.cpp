#include "PlayScene.h"
#include "Tetris.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include "RenderHelp.h"
#include "Cell.h"

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<Tetris*>(pWnd);
    assert(m_pGame != nullptr);
    m_pSoundManager = m_pGame->GetSoundManager();
}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

}

void PlayScene::Update(float deltaTime)
{
    //게임 오버 전까지 돌게 하고 오버 시 ChangeScene Ending
    static float time = 0.0f;

    float baseDropTime = 500.0f; // 기본 500ms
    float levelFactor = 30.0f;   // 레벨당 30ms씩 줄어든다.

    float currentDropTime = baseDropTime - ((GetLevel() - 1) * levelFactor);
    if (currentDropTime < 100.0f)
        currentDropTime = 100.0f;

    if (m_pTetromino) {
        time += deltaTime;

        if(keyLeftPressed)
        {
            if (!dasActive)
            {
                dasTimer += deltaTime;
                if (dasTimer >= DAS_DELAY)
                {
                    dasActive = true;
                    arrTimer = 0;
                }
            }
            else
            {
                arrTimer += deltaTime;
                if (arrTimer >= ARR_SPEED)
                {
                    OnMove(-1, 0);
                    if (m_pSoundManager)
                    {
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_MOVE, 0.4f);
                    }
                    m_lastAction = ACTION_MOVE;
                    arrTimer = 0;
                }
            }
        }

        if (keyRightPressed)
        {
            if (!dasActive)
            {
                dasTimer += deltaTime;
                if (dasTimer >= DAS_DELAY)
                {
                    dasActive = true;
                    arrTimer = 0;
                }
            }
            else
            {
                arrTimer += deltaTime;
                if (arrTimer >= ARR_SPEED)
                {
                    OnMove(1, 0);
                    if (m_pSoundManager)
                    {
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_MOVE, 0.4f);
                    }
                    m_lastAction = ACTION_MOVE;
                    arrTimer = 0;
                }
            }
        }

        if(time > currentDropTime)
        {
            time = 0.0f;

            if (!m_pTetromino->MoveDown(*m_pGameBoard)) {
                // 충돌 처리
                m_pGameBoard->FixTetrominoToBoard(*m_pGameBoard, m_pTetromino);
                int clearedLines = m_pGameBoard->RemoveFullLines();
                AddLinesCleard(clearedLines);

                if(m_lastAction == ACTION_ROTATE)
                {
                    m_isTSpin = m_pGameBoard->CheckTSpin(m_pTetromino);
                }
                else
                {
                    m_isTSpin = false;
                }

                AddScore(clearedLines, m_isTSpin, m_combo);

                if (m_pSoundManager)
                {
                    if (m_isPerfectClear)
                    {
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_PERFECTCLEAR, 0.4f);
                    }
                    else if (m_isTSpin)
                    {
                        if (m_isBackToBack)
                        {
                            m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_BACKTOBACK, 0.4f);
                        }
                        else
                        {
                            m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_TSPIN, 0.4f);
                        }
                    }
                    else if (m_isTetris)
                    {
                        if (m_isBackToBack)
                        {
                            m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_BACKTOBACK, 0.4f);
                        }
                        else
                        {
                            m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_TETRIS, 0.4f);
                        }
                    }
                    else if (clearedLines == 1)
                    {
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_SINGLE, 0.4f);
                    }
                    else if (clearedLines == 2)
                    {
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_DOUBLE, 0.4f);
                    }
                    else if (clearedLines == 3)
                    {
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_TRIPLE, 0.4f);
                    }
                    switch (m_combo)
                    {
                    case 0:
                        break;
                    case 1:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO1, 0.4f);
                        break;
                    case 2:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO2, 0.4f);
                        break;
                    case 3:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO3, 0.4f);
                        break;
                    case 4:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO4, 0.4f);
                        break;
                    case 5:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO5, 0.4f);
                        break;
                    case 6:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO6, 0.4f);
                        break;
                    default:
                        m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_COMBO7, 0.4f);
                        break;
                    }
                }

                delete m_pTetromino;
                m_pTetromino = nullptr;

                if (m_pGameBoard->IsGameOver())
                {
                    gameoverTimer += deltaTime;
                    if (!isGameover)
                    {
                        isGameover = true;
                        if (m_pSoundManager)
                        {
                            m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_GAMEOVER, 0.4f);
                        }

                    }
                    
                    if(gameoverTimer >= GAMEOVER_DELAY)
                    {
                        m_pGame->ChangeScene(SCENE_ENDING);
                    }
                }
            }
        }
    }
    else
    {
        RandomGenerateTetromino();
    }
}

void PlayScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    m_pBackground->Render(hDC, RGB(255, 0, 0));

    m_pGameBoard->Draw(hDC, m_pSpriteSheet, m_pTetromino, m_pHoldTetromino, m_pNextTetromino, 4);

    int level = GetLevel();
    int linesCleared = GetLinesCleared();
    int score = GetScore();
    // 텍스트로 출력
    wchar_t levelText[32];
    swprintf_s(levelText, L"%d", level);

    wchar_t linesText[32];
    swprintf_s(linesText, L"%d", linesCleared);

    wchar_t scoreText[32];
    swprintf_s(scoreText, L"%d", score);

    wchar_t comboText[32] = L"";
    if (m_combo > 0)
    {
        swprintf_s(comboText, L"Combo %d!", m_combo);
    }
    
    wchar_t tSpinText[32] = L"";
    if (m_isTSpin)
    {
        switch (m_TSpinlinesCleared)
        {
        case 0:
            swprintf_s(tSpinText, L"T-Spin Zero!");
            break;
        case 1:
            swprintf_s(tSpinText, L"T-Spin Mini!");
            break;
        case 2:
            swprintf_s(tSpinText, L"T-Spin Double!");
            break;
        case 3:
            swprintf_s(tSpinText, L"T-Spin Triple!");
            break;
        }
    }

    wchar_t perfectText[32] = L"";
    swprintf_s(perfectText, L"Perfect Clear!");

    wchar_t tetrisText[32] = L"";
    swprintf_s(tetrisText, L"Tetris!");

    wchar_t backtobackText[32] = L"";
    swprintf_s(backtobackText, L"Back To Back!");

    SetBkMode(hDC, TRANSPARENT);

    HFONT hFont = CreateFontW(
        40, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        OEM_CHARSET,
        OUT_RASTER_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        FIXED_PITCH | FF_MODERN,
        L"Terminal"
    );

    // 현재 DC에 폰트 적용
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);

    // 텍스트 색상 설정 (예: 흰색)
    SetTextColor(hDC, RGB(255, 255, 255));

    // 텍스트 출력 위치
    int startX = 183;
    int positioning = 15;
    int x = startX, y = 590;

    int levelLength = lstrlenW(levelText);
    x = startX - positioning * (levelLength - 1);
    
    // 레벨 출력
    TextOutW(hDC, x, y, levelText, levelLength);
    y += 152;  // 다음 줄로 내려서 출력

    int lineLength = lstrlenW(linesText);
    x = startX - positioning * (lineLength - 1);

    // 라인 출력
    TextOutW(hDC, x, y, linesText, lineLength);
    y += 149;

    int scoreLength = lstrlenW(scoreText);
    x = startX - positioning * (scoreLength - 1);

    TextOutW(hDC, x, y, scoreText, scoreLength);

    //콤보, T스핀, 퍼펙트, 테트리스(출력 후 다시 돌리기), 백투백
    int textX = 1060;
    
    int comboLength = lstrlenW(comboText);
    x = textX - positioning * (comboLength - 1);
    if (m_combo > 0)
    {
        TextOutW(hDC, x, 720, comboText, comboLength);
    }

    int tSpinLength = lstrlenW(tSpinText);
    x = textX - positioning * (tSpinLength - 1);

    if (m_isTSpin && m_TSpinlinesCleared != 0)
    {
        TextOutW(hDC, x, 765, tSpinText, tSpinLength);
    }

    int perfectClearLength = lstrlenW(perfectText);
    x = textX - positioning * (perfectClearLength - 1);

    if (m_isPerfectClear)
    {
        TextOutW(hDC, x, 855, perfectText, perfectClearLength);
    }

    int tetrisLength = lstrlenW(tetrisText);
    x = textX - positioning * (tetrisLength - 1);

    if (m_isTetris)
    {
        TextOutW(hDC, x, 810, tetrisText, tetrisLength);
    }

    int backtobackLength = lstrlenW(backtobackText);
    x = textX - positioning * (backtobackLength - 1);

    if (m_isBackToBack)
    {
        TextOutW(hDC, x, 900, backtobackText, backtobackLength);
    }

    // 폰트 복원
    SelectObject(hDC, hOldFont);

    // 폰트 객체 삭제
    DeleteObject(hFont);
}

int PlayScene::GetLevel() const
{
    return m_level;
}

void PlayScene::UpdateLevel()
{
    m_level = (m_linesCleared / 10) + 1;
}

int PlayScene::GetLinesCleared() const
{
    return m_linesCleared;
}

void PlayScene::AddLinesCleard(int clearedLine)
{
    m_linesCleared += clearedLine;
    UpdateLevel();
}

void PlayScene::AddScore(int clearedLine, bool isTSpin, int combo)
{
    m_isPerfectClear = m_pGameBoard->CheckPerfectClear();
    m_TSpinlinesCleared = 0;

    if (clearedLine > 0)
    {
        int basePoints = 0;

        if (m_isPerfectClear)
        {
            if (clearedLine == 4)
            {
                basePoints += 1800;
                m_isTetris = true;
            }
            else
            {
                basePoints += 800;
            }
        }
        
        else
        {
            switch (clearedLine)
            {
            case 1:
                basePoints += 100;
                break;
            case 2:
                basePoints += 300;
                break;
            case 3:
                basePoints += 500;
                break;
            case 4:
                basePoints += 800;
                m_isTetris = true;
                break;
            }
        }

        if (isTSpin)
        {
            basePoints *= 2;
            m_TSpinlinesCleared = clearedLine;
        }

        bool isSpecialAction = (m_isTetris || isTSpin);

        if (m_wasLastSpecialAction && isSpecialAction)
        {
            basePoints  = basePoints * 3 / 2;
            m_isBackToBack = true;
        }
        else
        {
            m_isBackToBack = false;
        }

        m_wasLastSpecialAction = isSpecialAction;

        if (combo > 1)
        {
            basePoints += 50 * (combo - 1);
        }

        m_score += basePoints * m_level;

        m_combo++;
    }
    else
    {
        if (isTSpin)
        {
            int basePoints = 100;
            m_TSpinlinesCleared = 0;

            bool isSpecialAction;
            if (m_wasLastSpecialAction)
            {
                basePoints = basePoints * 3 / 2;
                m_isBackToBack = true;
            }
            else
            {
                m_isBackToBack = false;
            }

            m_wasLastSpecialAction = isSpecialAction;
            m_score += basePoints * m_level;
        }
        else
        {
            m_isBackToBack = false;
        }
        m_combo = 0;
        m_isTetris = false;
    }
}

int PlayScene::GetScore() const
{
    return m_score;
}

void PlayScene::Init() 
{
    m_canHold = true;
    m_wasLastMoveRotation = false;
    m_isTSpin = false;
    m_isPerfectClear = false;
    m_isBackToBack = false;
    m_wasLastSpecialAction = false;
    m_isTetris = false;

    dasTimer = 0;
    arrTimer = 0;
    keyLeftPressed = false;
    keyRightPressed = false;
    dasActive = false;

    m_level = 1;
    m_linesCleared = 0;
    m_score = 0;
    m_combo = 0;
    m_TSpinlinesCleared = 0;

    m_lastAction = ACTION_NONE;
}

void PlayScene::OnKeyDown(int key)
{
    switch (key)
    {
    case VK_UP:
    case 'X':
        OnRotate(true, false);
        m_lastAction = ACTION_ROTATE;
        break;
    case VK_LEFT:
        if (!keyLeftPressed)
        {
            if (OnMove(-1, 0))
            {
                if (m_pSoundManager)
                {
                    m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_MOVE, 0.4f);
                }
                keyLeftPressed = true;
                dasTimer = 0;
                dasActive = false;
                m_lastAction = ACTION_MOVE;
            }
        }
        break;
    case VK_RIGHT:
        if(!keyRightPressed)
        {
            if (OnMove(1, 0))
            {
                if (m_pSoundManager)
                {
                    m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_MOVE, 0.4f);
                }
                keyRightPressed = true;
                dasTimer = 0;
                dasActive = false;
                m_lastAction = ACTION_MOVE;
            }
        }
        break;
    case VK_DOWN:
        OnMove(0, 1);
        m_lastAction = ACTION_DROP;
        break;
    case VK_SPACE:
        m_lastAction = ACTION_DROP;
        if(m_pTetromino->HardDrop(*m_pGameBoard))
        {
            if (m_pSoundManager)
            {
                m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_HARDDROP, 0.4f);
            }
            int clearedLines = m_pGameBoard->RemoveFullLines();
            AddLinesCleard(clearedLines);
            m_isTSpin = false;
            AddScore(clearedLines, m_isTSpin, m_combo);

            delete m_pTetromino;
            m_pTetromino = nullptr;
        }

        if (m_pGameBoard->IsGameOver())
        {
            m_pGame->ChangeScene(SCENE_ENDING);
        }
        break;
    case VK_CONTROL:
    case 'Z':
        if (OnRotate(false, false))
        {
            if (m_pSoundManager)
            {
                m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_ROTATE, 0.4f);
            }
            m_lastAction = ACTION_ROTATE;
        }
        
        break;
    case VK_SHIFT:
    case 'C':
        if (Hold())
        {
            if (m_pSoundManager)
            {
                m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_HOLD, 0.4f);
            }
        }
        break;
    case 'A':
        if(OnRotate(true, true))
        {
            if (m_pSoundManager)
            {
                m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_ROTATE, 0.4f);
            }
            m_lastAction = ACTION_ROTATE;
        }
        break;
    }
}

void PlayScene::OnKeyUp(int key)
{
    switch (key)
    {
    case VK_LEFT:
        keyLeftPressed = false;
        dasTimer = 0;
        dasActive = false;
        break;
    case VK_RIGHT:
        keyRightPressed = false;
        dasTimer = 0;
        dasActive = false;
        break;
    }
}

void PlayScene::OnMove(int dx, int dy)
{
    if (!m_pTetromino) return;

    bool moved = false;
    if (dx < 0)
    {
        moved = m_pTetromino->MoveLeft(*m_pGameBoard);
    }
    else if(dx > 0)
    {
        moved = m_pTetromino->MoveRight(*m_pGameBoard);
    }

    if (dy > 0)
    {
        moved = m_pTetromino->MoveDown(*m_pGameBoard);
    }

    if (moved)
    {
        m_wasLastMoveRotation = false;
    }
}

void PlayScene::OnRotate(bool clockwise, bool is180)
{
    if (!m_pTetromino) return;

    bool rotated = false;
    if (clockwise)
    {
        if (is180)
        {
            rotated = m_pTetromino->Rotate180(*m_pGameBoard);
        }
        else
        {
            rotated = m_pTetromino->RotateCW(*m_pGameBoard);
        }
    }
    else
    {
        rotated = m_pTetromino->RotateCCW(*m_pGameBoard);
    }

    if (rotated)
    {
        m_wasLastMoveRotation = true;
    }
}


void PlayScene::Hold()
{
    if (!m_canHold)
        return;

    if (m_pHoldTetromino == nullptr)
    {
        m_pHoldTetromino = m_pTetromino;
        RandomGenerateTetromino();
    }
    else
    {
        Tetromino* temp = m_pHoldTetromino;
        m_pHoldTetromino = m_pTetromino;
        m_pTetromino = temp;
    }

    if (m_pTetromino)
    {
        m_pTetromino->SetPosition(5, 2);
    }
    m_canHold = false;
}

void PlayScene::RandomGenerateTetromino()
{
    static bool isSet = false;
    m_canHold = true;

    if (isSet)
    {
        if (m_pNextTetromino[0] == nullptr)
        {
            for (int i = 0; i < 3; i++)
            {
                m_pNextTetromino[i] = m_pNextTetromino[i + 1];
            }
        }

        Tetromino::eBrickType randomType = static_cast<Tetromino::eBrickType>(m_dist(mt));
        m_pNextTetromino[3] = new Tetromino(randomType);
    }
    else
    {
        isSet = true;
        for (int i = 0; i < 4; i++)
        {
            Tetromino::eBrickType randomType = static_cast<Tetromino::eBrickType>(m_dist(mt));
            m_pNextTetromino[i] = new Tetromino(randomType);
        }
    }

    m_pTetromino = m_pNextTetromino[0];
    m_pNextTetromino[0] = nullptr;
}

void PlayScene::Finalize()
{
    if (m_pGameBoard)
    {
        delete m_pGameBoard;
        m_pGameBoard = nullptr;
    }
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }
    if (m_pTetromino)
    {
        delete m_pTetromino;
        m_pTetromino = nullptr;
    }

    if (m_pHoldTetromino)
    {
        delete m_pHoldTetromino;
        m_pHoldTetromino = nullptr;
    }

    for (int i = 0; i < 4; i++)
    {
        if (m_pNextTetromino[i])
        {
            delete m_pNextTetromino[i];
            m_pNextTetromino[i] = nullptr;
        }
    }
}

void PlayScene::Enter()
{
    Init();

    m_pGameBoard = new GameBoard(0, 0, 32);

    // SpriteSheet 가져오기
    m_pSpriteSheet = m_pGame->GetBricksSpriteSheet();
    if (!m_pSpriteSheet) return;

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(1280);
    pNewObject->SetHeight(960);

    pNewObject->SetBitmapInfo(m_pGame->GetUIBitmapInfo());

    m_pBackground = pNewObject;


    if (m_pSoundManager && !m_bgmStarted)
    {
        m_pSoundManager->PlayBGM(0.4f);
        m_bgmStarted = true;
    }
}

void PlayScene::Leave()
{
    Finalize();
}