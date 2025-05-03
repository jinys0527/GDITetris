#pragma once
#pragma pack(push, 1)
class Ranking
{
public:
    Ranking();
    Ranking(const wchar_t* name, int lines, int score);
    ~Ranking();

    void SetName(const wchar_t* name);
    void SetLines(int lines);
    void SetScore(int score);

    const wchar_t* GetName() const;
    int GetLines() const;
    int GetScore() const;

    Ranking* GetNext() const;
    void SetNext(Ranking* next);

private:
    wchar_t m_name[4];
    int m_lines;
    int m_score;
    Ranking* m_pNext;
};
#pragma pack(pop)
