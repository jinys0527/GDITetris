#include "EndingScene.h"
#include "MyFirstWndGame.h"
#include <assert.h>

void EndingScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
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
}

void EndingScene::Render(HDC hDC)
{
    DrawText(hDC, m_szTitle, -1, &m_rect, DT_CENTER | DT_VCENTER);
}

void EndingScene::Finalize()
{
}

void EndingScene::Enter()
{
}

void EndingScene::Leave()
{
}

