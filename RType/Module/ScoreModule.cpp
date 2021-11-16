#include "../precomp.h"
#include "ScoreModule.h"
#include "../RType/Levels/MainMenu.h"
#include "../Extension/Files/CSVParser.h"

ScoreModule::ScoreModule()
{
	Reset();
}

void ScoreModule::OnLevelUnload(Level* OldLevel)
{
	LoadTopScore();

	if (dynamic_cast<MainMenu*>(OldLevel))
		Reset();
}

void ScoreModule::AddScore(int Amount)
{
	score += static_cast<int>(sqrtf(powf(static_cast<float>(lives), 1.15f)+0.4f) * abs(Amount));
}

void ScoreModule::RemoveLife()
{
	lives--;
}

bool ScoreModule::HasHighScore() const
{
	for (int i = 0; i < TOP_SCORE_COUNT; ++i)
	{
		if (topScore[i].score < score)
			return true;
	}
	return false;
}

void ScoreModule::Reset()
{
	score = 0;
	lives = 3;
}

void ScoreModule::LoadTopScore()
{
	auto parser = CSVParser("Levels/Score.csv");

	for (int i = 0; i < TOP_SCORE_COUNT && !parser.ReachedEndOfData(); ++i)
	{
		topScore[i] = PScore(parser);
		parser.NextColumn();
	}

}

void ScoreModule::SaveScore(const char* Name)
{
	for (int i = 0; i < TOP_SCORE_COUNT; ++i)
	{
		if (topScore[i].score < score)
		{
			int moveCount = TOP_SCORE_COUNT - i - 1;
			memmove(&topScore[i + 1], &topScore[i], sizeof(PScore) * moveCount);
			topScore[i] = PScore(Name, score);
			break;
		}
	}

	FILE* filePtr = fopen("Levels/Score.csv", "w");
	char buffer[100]{};
	buffer[99] = 0;
	for (int i = 0; i < TOP_SCORE_COUNT; ++i)
	{
		if (topScore[i].score <= 0)break;
		topScore[i].ToFileLine(buffer);
		fwrite(buffer, strlen(buffer), sizeof(char), filePtr);
	}
	fclose(filePtr);
}

void ScoreModule::Render(Tmpl8::Surface* Surface) const
{
	char ms[30]{};
	ms[29] = 0;

	sprintf(ms, "Score: %i      Lives: %i", score, lives);
	Surface->PrintCentre(ms, SCRHEIGHT - 40, 3, 0xff0000);
}
