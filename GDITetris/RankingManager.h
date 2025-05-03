#pragma once
#include "Ranking.h"

class RankingManager
{
public:
	RankingManager();
	~RankingManager();

	void AddRanking(const wchar_t* name, int lines, int score);
	
	void SaveToFile();
	void LoadFromFile();

	Ranking* GetTopRanking() const;
	int GetCount() const;

	void Clear();
private:
	Ranking* m_pHead;
	int m_count;
	void InsertRanking(Ranking* newRanking);
	void EnsureMaximumRankings(int maxCount = 5);
};

