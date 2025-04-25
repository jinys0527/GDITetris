#include "TitleScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>

using namespace learning;

void TitleScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(512);
    pNewObject->SetHeight(512);

    pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

    m_rect.left = width/2 - 175;
    m_rect.top = height/2 + 25;
    m_rect.right = m_rect.left + 350;
    m_rect.bottom = m_rect.top + 100;

    m_pBackground = pNewObject;
}

void TitleScene::Update(float deltaTime)
{
    static float time = 0.0f;
    time += deltaTime;

    if (isTrigger)
    {
        m_pGame->ChangeScene(SceneType::SCENE_PLAY);
    }
    wsprintf(m_szTitle, L"Press Any Key!");
}

void TitleScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    m_pBackground->Render(hDC, RGB(255, 0, 0));

    SetBkMode(hDC, TRANSPARENT);

    HFONT hFont = CreateFont(
        32, 0, 0, 0,
        FW_BOLD,
        FALSE, FALSE, FALSE,
        OEM_CHARSET,
        OUT_RASTER_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        FIXED_PITCH | FF_MODERN,
        L"Terminal"
    );

    SetTextColor(hDC, RGB(200, 200, 50));

    // 이전 폰트 백업
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    DrawText(hDC, m_szTitle, -1, &m_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    SelectObject(hDC, hOldFont); // 원래 폰트 복원
    DeleteObject(hFont);         // 새로 만든 폰트 해제
}

void TitleScene::SetTrigger(bool isTrigger)
{
    this->isTrigger = isTrigger;
}

void TitleScene::Finalize()
{
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }
}

void TitleScene::Enter()
{
}

void TitleScene::Leave()
{
}

