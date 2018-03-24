#include "DoubleBuffer.h"



void DoubleBuffer::Init()
{
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = CONSOLE_WIDTH - 1;
	rect.Bottom = CONSOLE_HEIGHT - 1;

	for (int i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] =
			CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				CONSOLE_TEXTMODE_BUFFER,
				NULL
			);

		SetConsoleScreenBufferSize(buffer[i], size);
		SetConsoleWindowInfo(buffer[i], TRUE, &rect);
		SetConsoleCursorInfo(buffer[i], &cci);
		
	}

	cci.dwSize = 1;
	cci.bVisible = FALSE;

}

HANDLE DoubleBuffer::GetBuffer()
{
	return buffer[nowIndex];
}

void DoubleBuffer::ChangeBuffer()
{
	//Sleep(330);
	SetConsoleActiveScreenBuffer(buffer[nowIndex]);
	nowIndex = (nowIndex + 1) % BUFFER_SIZE;
}

void DoubleBuffer::WriteBuffer(int x, int y, char * c)
{
	DWORD dw;
	COORD CursorPos{ x,y };

	SetConsoleCursorPosition(buffer[nowIndex], CursorPos);
	WriteFile(buffer[nowIndex], c,strlen(c), &dw, NULL);
}

void DoubleBuffer::ClearBuffer()
{
	DWORD dw;
	COORD CursorPos{ 0,0};
	FillConsoleOutputCharacter(buffer[nowIndex], ' ', size.X*size.Y, CursorPos, &dw);
}

void DoubleBuffer::CloseHandle()
{
	for (int i = 0; i < BUFFER_SIZE; i++) {
		::CloseHandle(buffer[i]);
	}
}

DoubleBuffer::DoubleBuffer()
{
}


DoubleBuffer::~DoubleBuffer()
{

}
