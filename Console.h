#pragma once
#include <windows.h>

/* Set colour code, the set enumeration values are powers of 2 for convenient bitwise operations */
enum ConColor
{
	ConRed = 1,
	ConGreen = 2,
	ConBlue = 4
};

class __declspec(dllexport) ConsoleLib
{
	// Define handles for the window and keyboard
	HANDLE m_screen;
	HANDLE m_keyboard;
	// Define colour attributes
	WORD m_textColor;
	WORD m_backgroundColor;

public:
	ConsoleLib();  // Constructor
	~ConsoleLib(); // Destructor

	void SetBackgroundColor(WORD color); // Set background colour
	void SetTextColor(WORD color);		 // Set text colour
	void SetTitle(char *title);			 // Set title
	void SetPosition(COORD Position);	 // Set cursor position

	void Clear(void);								   // Clear console
	void ClearChar(short x, short y);				   // Clear character at specified position
	void OutputString(char *string);				   // Output string
	void OutputString(char *string, int stringLength); // Output string with specified length

	void Read(char *buffer, DWORD bufferSize); // Read a character from the keyboard
	int GetKey(void);						   // Get the key pressed on the keyboard
};
