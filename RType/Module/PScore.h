#pragma once
#include <cstring>
#define MAX_PNAME 3
class CSVParser;

class PScore
{
public:
	PScore(const char* PName, int Score);
	PScore(CSVParser& fp);
	PScore() = default;
	~PScore();


	PScore(const PScore& Other)
	{
		score = Other.score;
		int l = strlen(Other.pName);
		pName = new char[l];

		strcpy(pName, Other.pName);
	}
	PScore& operator=(const PScore& Other)
	{
		score = Other.score;
		int l = strlen(Other.pName);
		pName = new char[l];

		strcpy(pName, Other.pName);
		return *this;
	}

	char* pName;
	int score = 0;

	void ToFileLine(char* buffer);

	void ToScore(int index,char* buffer) const;
};
