#include "misc.hpp"
#include <Windows.h>

void setup_console() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info = { 100, FALSE };

	SetConsoleCursorInfo(consoleHandle, &info);

	system("cls");
}

void reset_cursor() {
	//SetCursorPos(0, 0);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
}