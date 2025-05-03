#include "RankingScene.h"
#include "Tetris.h"
#include <assert.h>
#include "GameObject.h"

void RankingScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<Tetris*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");
}

void RankingScene::Finalize()
{
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }
}

void RankingScene::Enter()
{
    m_pBackground = new Background(ObjectType::BACKGROUND);
    m_pBackground->SetPosition(0.0f, 0.0f);

    m_pBackground->SetWidth(1280);
    m_pBackground->SetHeight(960);

    m_pBackground->SetBitmapInfo(m_pGame->GetRankingBitmapInfo());

    m_rakingManger.LoadFromFile();
}

void RankingScene::Leave()
{
    Finalize();
}

void RankingScene::FixedUpdate()
{
}

void RankingScene::Update(float deltaTime)
{
}

void RankingScene::Render(HDC hDC)
{
    m_pBackground->DrawBitmap(hDC, 0, 0, 1280, 960);
    
    m_rakingManger.DrawRanking(hDC);
}

void RankingScene::OnKeyDown(int key)
{
    if (key == VK_RETURN)
    {
        m_pGame->ChangeScene(SCENE_TITLE);
    }
}