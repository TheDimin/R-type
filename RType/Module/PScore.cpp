#include "PScore.h"

#include <cstdio>

#include "../../Extension/Files/CSVParser.h"

PScore::PScore(const char* name, int Score) :
score(Score)
{
	int l = strlen(name);
	pName = new char[l + 1];
	pName[l] = 0;

	strcpy(pName, name);
}

PScore::PScore(CSVParser& fp)
{
	pName = fp.ReadChar();
	score = fp.ReadInt();
}

PScore::~PScore()
{
	delete[] pName;
}

void PScore::ToFileLine(char* buffer)
{
	sprintf(buffer, "%s,%i\n", pName, score);
}

void PScore::ToScore(int index, char* buffer) const
{
	sprintf(buffer, "%i:    %i      %s", index + 1, score, pName);
}
