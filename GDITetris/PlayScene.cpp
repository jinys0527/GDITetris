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

    float currentDropTime = baseDropTime - ((m_pGameBoard->GetLevel() - 1) * levelFactor);
    if (currentDropTime < 100.0f)
        currentDropTime = 100.0f;

    if (m_pTetromino) {
        time += deltaTime;

        if(time > currentDropTime)
        {
            time = 0.0f;

            if (!m_pTetromino->MoveDown(*m_pGameBoard)) {
                // 충돌 처리
                m_pGameBoard->FixTetrominoToBoard(*m_pGameBoard, m_pTetromino);
                m_pGameBoard->RemoveFullLines();

                delete m_pTetromino;
                m_pTetromino = nullptr;

                if (m_pGameBoard->IsGameOver())
                {
                    m_pGame->ChangeScene(SCENE_ENDING);
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

    int level = m_pGameBoard->GetLevel();
    int linesCleared = m_pGameBoard->GetLinesCleared();
    int score = m_pGameBoard->GetScore();
    // 텍스트로 출력
    wchar_t levelText[32];
    wsprintf(levelText, L"%d", level);

    wchar_t linesText[32];
    wsprintf(linesText, L"%d", linesCleared);

    wchar_t scoreText[32];
    wsprintf(scoreText, L"%d", score);

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

    // 폰트 복원
    SelectObject(hDC, hOldFont);

    // 폰트 객체 삭제
    DeleteObject(hFont);
}

void PlayScene::OnKeyDown(int key)
{
    switch (key)
    {
    case VK_UP:
    case 'X':
        m_pTetromino->RotateCW(*m_pGameBoard);
        break;
    case VK_LEFT:
        m_pTetromino->MoveLeft(*m_pGameBoard);
        break;
    case VK_RIGHT:
        m_pTetromino->MoveRight(*m_pGameBoard);
        break;
    case VK_DOWN:
        m_pTetromino->MoveDown(*m_pGameBoard);
        break;
    case VK_SPACE:
        if(m_pTetromino->HardDrop(*m_pGameBoard))
        {
            m_pGameBoard->RemoveFullLines();
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
        m_pTetromino->RotateCCW(*m_pGameBoard);
        break;
    case VK_SHIFT:
    case 'C':
        Hold();
        break;
    case 'A':
        m_pTetromino->Rotate180(*m_pGameBoard);
        break;
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
    m_pGameBoard = new GameBoard(0, 0, 32);

    // SpriteSheet 가져오기
    m_pSpriteSheet = m_pGame->GetBricksSpriteSheet();
    if (!m_pSpriteSheet) return;

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(252);
    pNewObject->SetHeight(200);

    pNewObject->SetBitmapInfo(m_pGame->GetUIBitmapInfo());

    m_pBackground = pNewObject;
}

void PlayScene::Leave()
{
    Finalize();
}