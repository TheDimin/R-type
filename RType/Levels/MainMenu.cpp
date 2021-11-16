#include "../precomp.h"
#include "MainMenu.h"
#include "RTypeLevel.h"
#include "../RType/Module/ScoreModule.h"

void MainMenu::OnLoad()
{
	backGround.Set(new BackGround());
	sm = GetModule<ScoreModule>();
}

void MainMenu::Render(Tmpl8::Surface* Surface) const
{
	Level::Render(Surface);

	Surface->PrintCentre("D-Type", 50, 10, 0xFFFFFF);
	Surface->PrintCentre("By Damian", 120, 3, 0xFFFFFF);

	const Pixel colorLookup[] = { 0xd4af37,0xC0C0C0,0xCD7F32 };
	const Pixel other = 0xffffff;
	const short colorLookupSize = sizeof(colorLookup) / sizeof(Pixel);


	const short sizeLookup[] = { 3 };
	const short defaultSize = 2;
	const short sizeLookupSize = sizeof(sizeLookup) / sizeof(short);

	const PScore* ts = sm->GetTopScore();
	char buffer[50];

	int offset = 200;

	for (int i = 0; i < TOP_SCORE_COUNT; ++i)
	{
		if (ts[i].score <= 0) break;
		ts[i].ToScore(i, buffer);

		const Pixel c = colorLookupSize > i ? colorLookup[i] : other;
		const short size = sizeLookupSize > i ? sizeLookup[i] : defaultSize;
		offset += (6 * size) + 10;
		Surface->Print(buffer, 180, offset, size, c);
	}

	Surface->PrintCentre("Press SPACE to start", SCRHEIGHT - 80, 3, 0xFFFFFF);
}

void MainMenu::KeyDown(const int& Key)
{
	if (Key == 44)
		gameInstance->LoadLevel(new RTypeLevel(1));
}
