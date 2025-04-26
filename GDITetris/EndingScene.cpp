#include "EndingScene.h"
#include "Tetris.h"
#include <assert.h>

void EndingScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<Tetris*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    m_rect.left = width / 2 - 50;
    m_rect.top = height / 2 - 50;
    m_rect.right = m_rect.left + 100;
    m_rect.bottom = m_rect.top + 100;
}

void EndingScene::Update(float deltaTime)
{
    if (isTrigger)
    {
        m_pGame->ChangeScene(SceneType::SCENE_TITLE);
    }
}

void EndingScene::Render(HDC hDC)
{
    DrawText(hDC, m_szTitle, -1, &m_rect, DT_CENTER | DT_VCENTER);
}

void EndingScene::OnKeyDown(int key)
{
    isTrigger = true;
}

void EndingScene::Finalize()
{
}

void EndingScene::Enter()
{
    isTrigger = false;
}

void EndingScene::Leave()
{
}

