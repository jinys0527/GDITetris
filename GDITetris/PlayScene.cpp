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
    m_pGameBoard = new GameBoard(0, 0, 32);

    // SpriteSheet 가져오기
    m_pSpriteSheet = m_pGame->GetBricksSpriteSheet();
    if (!m_pSpriteSheet) return;
}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

}

void PlayScene::Update(float deltaTime)
{
    //게임 오버 전까지 돌게 하고 오버 시 ChangeScene Ending
    static float time = 0.0f;
    if (m_pTetromino) {
        time += deltaTime;

        if(time > 500.0f)
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

    // 배경 그리기
    RECT rcClient;
    GetClientRect(WindowFromDC(hDC), &rcClient);
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hDC, &rcClient, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    m_pGameBoard->Draw(hDC, m_pSpriteSheet, m_pTetromino);
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
        //홀드
        break;
    case 'A':
        m_pTetromino->Rotate180(*m_pGameBoard);
        break;
    case 'T':
        m_pGame->ChangeScene(SCENE_ENDING);
        break;
    }
}

void PlayScene::RandomGenerateTetromino()
{
    Tetromino::eBrickType randomType = static_cast<Tetromino::eBrickType>(m_dist(mt));

    m_pTetromino = new Tetromino(randomType);

    m_pTetromino->SetPosition(5, 0);
}

void PlayScene::Finalize()
{
    if (m_pGameBoard)
    {
        delete m_pGameBoard;
        m_pGameBoard = nullptr;
    }
    if (m_pTetromino)
    {
        delete m_pTetromino;
        m_pTetromino = nullptr;
    }
}

void PlayScene::Enter()
{
}

void PlayScene::Leave()
{
}