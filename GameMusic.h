#ifndef DEF_GAMEMUSIC // Game Music Manager

#define DEF_GAMEMUSIC 1

#include <windows.h>
#include <stdio.h>
#include <mmsystem.h> // Include file

/* Game Music Class */
class __declspec(dllexport) GameMusic
{
protected:
	int m_ID;			 // ID
	static int m_nNewID; // Next available ID

	int m_nVolume; // Volume (0~1000)

public:
	GameMusic();

	GameMusic(LPTSTR szMusicPath);

	~GameMusic();

	BOOL GetPlayState(); // Get the music play state

	BOOL GetOpenState(); // Get the music file open state

	BOOL GetStopState(); // Get the music stop state

	int GetVolume(); // Get the volume level

	void SetVolume(int nVolume); // Set the volume

	void VolumeDown(); // Decrease volume

	void VolumeUp(); // Increase volume

	BOOL Open(LPTSTR szMusicPath); // Open music file

	BOOL Close(); // Close music file

	BOOL Stop(); // Stop playing

	BOOL Play(int nVolume = 300, BOOL bRepeat = FALSE, BOOL bReStart = FALSE); // Play music, can set whether to repeat and restart
};

#endif
