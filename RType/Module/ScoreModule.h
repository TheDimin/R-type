#pragma once
#include "PScore.h"
#include "../Extension/Interfaces/IModule.h"
#include "../Extension/Interfaces/IRender.h"

#define TOP_SCORE_COUNT 9

//Module that saves all data used between levels
class ScoreModule :
	public IModule, public IRender
{
public:
	ScoreModule();

	void OnLevelUnload(Level* OldLevel) override;
	int GetScore() const { return score; }
	void AddScore(int Amount);

	int GetLives() const { return lives; };
	void RemoveLife();

	bool HasHighScore()const;

	void Reset();
	void LoadTopScore();
	void SaveScore(const char* Name);
	const PScore* GetTopScore()const { return topScore; }

	void Render(Tmpl8::Surface* Surface) const override;
private:
	int score = 0;
	int lives = 3;

	PScore topScore[TOP_SCORE_COUNT];
	bool topScoreLoaded = false;
};