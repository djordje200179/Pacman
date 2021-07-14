#include <Pacman/game.hpp>
#include <cxxopts.hpp>
#include <string>

int main(int argc, char** argv) {
	cxxopts::Options options("Pacman", "A console Pacman game");

	options.add_options()
		("f,fps", "Frames per second", cxxopts::value<Pacman::u16>()->default_value("60"))
		("m,map", "Map file name", cxxopts::value<std::string>()->default_value("res/field.maka"))
		("r,ratio", "Move ratio", cxxopts::value<Pacman::u16>()->default_value("15"))
		("a,ability", "Ability duration", cxxopts::value<Pacman::u16>()->default_value("60"))
		("h,help", "Print parameter info");

	auto parameters = options.parse(argc, argv);

	if(parameters["help"].as<bool>())
		std::cout << options.help() << std::endl;
	else {
		auto map_file_name = parameters["map"].as<std::string>();
		auto ability_duration = parameters["ability"].as<Pacman::u16>();
		auto fps = parameters["fps"].as<Pacman::u16>();
		auto move_ratio = parameters["ratio"].as<Pacman::u16>();

		Pacman::Game game(map_file_name, ability_duration);
		game.loop(fps, move_ratio);
	}

	return 0;
}