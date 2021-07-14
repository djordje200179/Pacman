#ifndef MISC_HPP
#define MISC_HPP


#define RED_COLOR "\033[0;31m"
#define GREEN_COLOR "\033[0;32m"
#define YELLOW_COLOR "\033[0;33m"
#define BLUE_COLOR "\033[0;34m"
#define PURPLE_COLOR "\033[0;35m"
#define CYAN_COLOR "\033[0;36m"
#define NO_COLOR "\033[0m"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

enum class Key : char {
	NONE = 0,
	ESCAPE = 27,
	ARROW_UP = 72,
	ARROW_DOWN = 80,
	ARROW_LEFT = 75,
	ARROW_RIGHT = 77
};

void setup_console();
void reset_cursor();
Key get_pressed_key();

#endif // MISC_HPP