#pragma once
#include "system.h"

#define BUFFER_SIZE 2

class DoubleBuffer
{
private:
	CONSOLE_CURSOR_INFO cci;
	COORD size{ CONSOLE_WIDTH,CONSOLE_HEIGHT };
	SMALL_RECT rect;
	HANDLE buffer[BUFFER_SIZE];
	int nowIndex = 0;

public:
	void Init();
	HANDLE GetBuffer();
	void ChangeBuffer();
	void WriteBuffer(int x, int y, char *c);
	void ClearBuffer();
	void CloseHandle();

	DoubleBuffer();
	~DoubleBuffer();
};

