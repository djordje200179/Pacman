#include "misc.h"
#include <Windows.h>
#include <locale.h>
#include <stdio.h>

void setup_console() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info = { 100, FALSE };

	SetConsoleCursorInfo(consoleHandle, &info);

	system("cls");
}

void reset_cursor() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { 0, 0 });
}