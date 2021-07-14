#include "game.hpp"
#include "misc.hpp"

const std::string MAP = "res/field.maka";
const u16 FPS = 60;
const u16 MOVE_RATIO = 15;
const u16 ABILITY_DURATION = 60;

int main(int argc, char** argv) {
	setup_console();

	auto game = Game(MAP, ABILITY_DURATION);
	game.loop(FPS, MOVE_RATIO);

	return 0;
}