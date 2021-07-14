#include <Pacman/game.hpp>
#include <string>

const std::string MAP = "res/field.maka";
const char FPS = 60;
const short MOVE_RATIO = 15;
const short ABILITY_DURATION = 60;

int main(int argc, char** argv) {
	auto game = Pacman::Game(MAP, ABILITY_DURATION);
	game.loop(FPS, MOVE_RATIO);

	return 0;
}