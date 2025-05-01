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

	m_pBackground = new Background(ObjectType::BACKGROUND);
	m_pBackground->SetPosition(0.0f, 0.0f);

	int width = m_pGame->GetWidth();
	int height = m_pGame->GetHeight();

	learning::SetScreenSize(width, height);

	m_pBackground->SetWidth(1280);
	m_pBackground->SetHeight(960);

	m_pBackground->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

	m_rect.left = width / 2 - 275;
	m_rect.top = height / 2 + 25;
	m_rect.right = m_rect.left + 550;
	m_rect.bottom = m_rect.top + 100;

	m_pKey = new Background(ObjectType::BACKGROUND);
	m_pKey->SetPosition(0.0f, 0.0f);

	m_pKey->SetWidth(610);
	m_pKey->SetHeight(622);

	m_pKey->SetBitmapInfo(m_pGame->GetKeyBitmapInfo());

	m_pButton = new Background(ObjectType::BACKGROUND);
	m_pButton->SetPosition(0.0f, 0.0f);

	m_pButton->SetWidth(297);
	m_pButton->SetHeight(237);

	m_pButton->SetBitmapInfo(m_pGame->GetButtonBitmapInfo());

	m_pSoundManager = m_pGame->GetSoundManager();
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

	m_pBackground->DrawBitmap(hDC, 0, 0, 1280, 960);
	m_pButton->DrawBitmap(hDC, 490, 420, 297, 237);
	
	if (isClickedMethod)
	{
		m_pKey->DrawBitmap(hDC, 335, 230, 610, 622);
	}
}

void TitleScene::OnClicked(int x, int y)
{
	if (m_pSoundManager)
	{
		m_pSoundManager->PlaySFX(m_pSoundManager->SOUND_CLICK, 0.4f);
	}

	if (x >= 490 && x <= 780)
	{
		if (y >= 420 && y <= 530)
		{
			isClickedStart = true;
		}

		if (y >= 540 && 650)
		{
			isClickedMethod = true;
		}
	}

	if (x >= 883 && x <= 938)
	{
		if (y >= 230 && y <= 285)
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
	if (m_pSoundManager)
	{
		m_pSoundManager->StopAllSFX();
	}
}

