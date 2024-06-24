
#include "Play.h"
#include <iostream>
#include "CardStack.h"
#include "Deck.h"

#pragma comment(lib, ".\\CConsoleDll.lib")

int main(int argv, char *argc[])
{

	HWND m_hwnd = GetForegroundWindow();

	SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, IDI_SHIELD));

	ConsoleLib m_console;
	m_console.SetTitle("BridgeCard");

	Play m_play;

	Deck m_deck(&m_console, &m_play);

	while (GameExit != m_deck.GetStatus())
		m_deck.Process();

	return 0;
}
