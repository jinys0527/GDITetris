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

	//ÃÊ°ú ·©Å·
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
	_wfopen_s(&file, L"ranking.txt", L"wb");
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
	_wfopen_s(&file, L"ranking.txt", L"rb");
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
