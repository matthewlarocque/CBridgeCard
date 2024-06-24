#ifndef GAMEMUSICOPERATE
#define GAMEMUSICOPERATE 1

#include "GameMusic.h"

class GameMusicOperate
{
public:
	GameMusicOperate();
	~GameMusicOperate();

	GameMusic *GetBackgroupMusic()
	{
		return m_pBackgroundMusic;
	}

protected:
	GameMusic *m_pBackgroundMusic;
};

#endif