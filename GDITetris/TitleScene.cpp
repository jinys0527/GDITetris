#include "TitleScene.h"
#include "Tetris.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>

using namespace learning;

void TitleScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<Tetris*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    learning::SetScreenSize(width, height);

    pNewObject->SetWidth(512);
    pNewObject->SetHeight(512);

    pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

    m_rect.left = width/2 - 275;
    m_rect.top = height/2 + 25;
    m_rect.right = m_rect.left + 550;
    m_rect.bottom = m_rect.top + 100;

    m_pBackground = pNewObject;

    m_pKey = new Background(ObjectType::BACKGROUND);
    m_pKey->SetPosition(0.0f, 0.0f);

    m_pKey->SetWidth(465);
    m_pKey->SetHeight(520);

    m_pKey->SetBitmapInfo(m_pGame->GetKeyBitmapInfo());

    m_pButton = new Background(ObjectType::BACKGROUND);
    m_pButton->SetPosition(0.0f, 0.0f);

    m_pButton->SetWidth(285);
    m_pButton->SetHeight(225);

    m_pButton->SetBitmapInfo(m_pGame->GetButtonBitmapInfo());
}

void TitleScene::Update(float deltaTime)
{
    static float time = 0.0f;
    time += deltaTime;

    if (isClickedStart)
    {
        m_pGame->ChangeScene(SceneType::SCENE_PLAY);
    }
    wsprintf(m_szTitle, L"Press Any Key!");
}

void TitleScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    m_pBackground->Render(hDC, RGB(255, 0, 0));
    m_pButton->DrawBitmap(hDC, 490, 400, 285, 225);

    if(isClickedMethod)
    {
        m_pKey->DrawBitmap(hDC, 335, 230, 600, 600);
    }
}

void TitleScene::OnClicked(int x, int y)
{
    if (x >= 490 && x <= 775)
    {
        if(y>=400 && y<=500) 
        {
            isClickedStart = true;
        }

        if (y >= 525 && 625)
        {
            isClickedMethod = true;
        }
    }

    if (x >= 875 && x <= 930)
    {
        if (y >= 225 && y <= 280)
        {
            isClickedMethod = false;
        }
    }
}

void TitleScene::Finalize()
{
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }

    if (m_pKey)
    {
        delete m_pKey;
        m_pKey = nullptr;
    }

    if (m_pButton)
    {
        delete m_pButton;
        m_pButton = nullptr;
    }
}

void TitleScene::Enter()
{
    isClickedMethod = false;
    isClickedStart = false;
}

void TitleScene::Leave()
{
}

