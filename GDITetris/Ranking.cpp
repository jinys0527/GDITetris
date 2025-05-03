#include "Ranking.h"

Ranking::Ranking() : m_lines(0), m_score(0), m_pNext(nullptr)
{
	m_name[0] = '\0';
}

Ranking::Ranking(const wchar_t* name, int lines, int score) : m_lines(lines), m_score(score), m_pNext(nullptr)
{
	SetName(name);
}

Ranking::~Ranking()
{
}

void Ranking::SetName(const wchar_t* name)
{
	int i = 0;
	while (i < 3 && name[i] != '\0')
	{
		m_name[i] = name[i];
		i++;
	}
	m_name[i] = '\0';
}

void Ranking::SetLines(int lines)
{
	m_lines = lines;
}

void Ranking::SetScore(int score)
{
	m_score = score;
}

const wchar_t* Ranking::GetName() const
{
	return m_name;
}

int Ranking::GetLines() const
{
	return m_lines;
}

int Ranking::GetScore() const
{
	return m_score;
}

Ranking* Ranking::GetNext() const
{
	return m_pNext;
}

void Ranking::SetNext(Ranking* next)
{
	m_pNext = next;
}