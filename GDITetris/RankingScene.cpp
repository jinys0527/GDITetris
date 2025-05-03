#include "RankingScene.h"
#include "Tetris.h"
#include <assert.h>

void RankingScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<Tetris*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");
}

void RankingScene::Finalize()
{
}

void RankingScene::Enter()
{
}

void RankingScene::Leave()
{
}

void RankingScene::FixedUpdate()
{
}

void RankingScene::Update(float deltaTime)
{
}

void RankingScene::Render(HDC hDC)
{
}
