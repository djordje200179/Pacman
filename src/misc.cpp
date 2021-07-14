#include "misc.hpp"
#include <Windows.h>
#include <conio.h>

void setup_console() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info = { 100, FALSE };

	SetConsoleCursorInfo(consoleHandle, &info);

	system("cls");
}

void reset_cursor() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
}

Key get_pressed_key() {
	if(!_kbhit())
		return Key::NONE;

	return static_cast<Key>(_getch());
}
