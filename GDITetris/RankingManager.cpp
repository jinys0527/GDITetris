#include "RankingManager.h"
#include <iostream>

RankingManager::RankingManager() : m_pHead(nullptr), m_count(0)
{
}

RankingManager::~RankingManager()
{
	Clear();
}

void RankingManager::InsertRanking(Ranking* newRanking)
{
	if (m_pHead == nullptr || newRanking->GetScore() > m_pHead->GetScore())
	{
		newRanking->SetNext(m_pHead);
		m_pHead = newRanking;
		m_count++;
		return;
	}

	Ranking* current = m_pHead;
	while (current->GetNext() != nullptr && current->GetNext()->GetScore() >= newRanking->GetScore())
	{
		current = current->GetNext();
	}

	newRanking->SetNext(current->GetNext());
	current->SetNext(newRanking);
	m_count++;
}

void RankingManager::EnsureMaximumRankings(int maxCount)
{
	if (m_count <= maxCount)
	{
		return;
	}

	Ranking* current = m_pHead;
	for (int i = 1; i < maxCount; i++)
	{
		if (current == nullptr)
		{
			return;
		}
		current = current->GetNext();
	}

	if (current == nullptr)
	{
		return;
	}

	//초과 랭킹
	Ranking* temp = current->GetNext();
	current->SetNext(nullptr);

	while (temp != nullptr)
	{
		Ranking* toDelete = temp;
		temp = temp->GetNext();
		delete toDelete;
		m_count--;
	}
}

void RankingManager::AddRanking(const wchar_t* name, int lines, int score)
{
	Ranking* newRanking = new Ranking(name, lines, score);
	InsertRanking(newRanking);
	EnsureMaximumRankings();
}

void RankingManager::SaveToFile()
{
	FILE* file = nullptr;
	_wfopen_s(&file, L"ranking.dat", L"wb");
	if (file == nullptr)
	{
		return;
	}

	Ranking* current = m_pHead;
	while (current != nullptr)
	{
		int lines = current->GetLines();
		int score = current->GetScore();

		fwrite(current->GetName(), sizeof(wchar_t), 4, file);
		fwrite(&lines, sizeof(int), 1, file);
		fwrite(&score, sizeof(int), 1, file);

		current = current->GetNext();
	}

	fclose(file);
}

void RankingManager::LoadFromFile()
{
	Clear();

	FILE* file = nullptr;
	_wfopen_s(&file, L"ranking.dat", L"rb");
	if (file == nullptr)
	{
		return;
	}

	wchar_t name[4];
	int lines, score;

	while (feof(file) != 0)
	{
		if (fread(name, sizeof(wchar_t), 4, file) != 4) break;
		if (fread(&lines, sizeof(int), 1, file) != 1) break;
		if (fread(&score, sizeof(int), 1, file) != 1) break;

		AddRanking(name, lines, score);
	}

	fclose(file);
}

void RankingManager::DrawRanking(HDC hDC)
{
	Ranking* current = m_pHead;

	int x;
	int positioning = 15;
	int y = 300;

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

	while (current != nullptr)
	{
		TextOutW(hDC, 303, y, current->GetName(), 3);

		wchar_t linesText[32];
		swprintf_s(linesText, L"%d", current->GetLines());
		int linesLength = lstrlenW(linesText);
		x = 541 - positioning * (linesLength - 1);
		TextOutW(hDC, x, y, linesText, linesLength);

		wchar_t scoreText[32];
		swprintf_s(scoreText, L"%d", current->GetScore());
		int scoreLength = lstrlenW(scoreText);
		x = 815 - positioning * (scoreLength - 1);
		TextOutW(hDC, x, y, scoreText, scoreLength);
		
		current = current->GetNext();
		y += 81;
	}

		// 폰트 복원
	SelectObject(hDC, hOldFont);

	// 폰트 객체 삭제
	DeleteObject(hFont);
}

Ranking* RankingManager::GetTopRanking() const
{
	return m_pHead;
}

int RankingManager::GetCount() const
{
	return m_count;
}

void RankingManager::Clear()
{
	Ranking* current = m_pHead;
	while (current != nullptr)
	{
		Ranking* toDelete = current;
		current = current->GetNext();
		delete toDelete;
	}

	m_pHead = nullptr;
	m_count = 0;
}
