#include "Pacman/game.hpp"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

namespace Pacman {
	Game::Game(const std::string& mapFileName, u16 abilityDuration, float fps, u16 updateFrequency) :
		abilityDuration(abilityDuration), map(mapFileName), targetFrameTime(1.0f / fps), updateFrequency(updateFrequency) {
		sAppName = "Pacman";
	}

	Position Game::generateNewPosition(Position oldPosition, Direction direction) const {
		auto dimensions = map.getDimensions();
		
		auto new_position = oldPosition;
		new_position += dimensions;

		switch(direction) {
		case Direction::UP:
			new_position.y--;
			break;
		case Direction::DOWN:
			new_position.y++;
			break;
		case Direction::LEFT:
			new_position.x--;
			break;
		case Direction::RIGHT:
			new_position.x++;
			break;
		}

		new_position %= dimensions;

		return new_position;
	}

	void Game::moveEntity(Entity& entity) {
		if(entity.direction == Direction::STOP)
			return;

		auto oldPosition = entity.position;
		auto newPosition = generateNewPosition(oldPosition, entity.direction);

		auto underlyingField = entity.field;
		auto oldField = map.getField(oldPosition);
		auto newField = map.getField(newPosition);

		if(newField == Field::WALL)
			entity.direction = Direction::STOP;

		if(entity.direction != Direction::STOP) {
			entity.position = newPosition;

			map.setField(oldPosition, underlyingField);
			map.setField(newPosition, oldField);
			entity.field = newField;
		}
	}

	void Game::movePlayer() {
		moveEntity(player);

		switch(player.field) {
		case Field::ENEMY:
			if(abilityCounter) {
				enemies.erase(std::find_if(enemies.begin(), enemies.end(),
							  [&](const Entity& enemy) { return enemy.position == player.position; })
				);

				player.field = Field::SPACE;
			} else {
				isRunning = false;

				map.setField(player.position, Field::ENEMY);
			}

			break;
		case Field::ABILITY:
			abilityCounter = abilityDuration;
		case Field::FOOD:
			score++;
			player.field = Field::SPACE;

			break;
		}
	}

	void Game::moveEnemies() {
		for(auto& enemy : enemies) {
			moveEntity(enemy);

			if(enemy.field == Field::PLAYER)
				isRunning = false;
		}
	}

	void Game::setInitialPositions() {
		auto dimensions = map.getDimensions();

		for(u16 i = 0; i < dimensions.height; i++)
			for(u16 j = 0; j < dimensions.width; j++) {
				Position position { i, j };

				switch(map.getField(position)) {
				case Field::ENEMY:
					enemies.emplace_back(position, Direction::STOP, Field::SPACE);
					break;
				case Field::PLAYER:
					player = { position, Direction::STOP, Field::SPACE };
					break;
				}
			}
	}

	void Game::draw() const {
		auto dimensions = map.getDimensions();

		for(u16 i = 0; i < dimensions.height; i++)
			for(u16 j = 0; j < dimensions.width; j++) {
				auto position = Position { i, j };
				auto pixelColor = olc::WHITE;
				auto field = map.getField(position);

				switch(field) {
				case Field::SPACE:
					pixelColor = olc::BLACK;
					break;
				case Field::WALL:
					pixelColor = olc::WHITE;
					break;
				case Field::FOOD:
					pixelColor = olc::YELLOW;
					break;
				case Field::ABILITY:
					pixelColor = olc::DARK_YELLOW;
					break;
				case Field::ENEMY:
					pixelColor = abilityCounter ? olc::BLUE : olc::RED;
					break;
				case Field::PLAYER:
					pixelColor = olc::DARK_GREEN;
					break;
				}

				switch(field) {
				case Field::FOOD:
					Draw(j * 5 + 2, i * 5 + 2, pixelColor);

					break;
				case Field::SPACE:
				case Field::WALL:
					for(u8 k = 0; k < 5; k++)
						for(u8 l = 0; l < 5; l++)
							Draw(j * 5 + l, i * 5 + k, pixelColor);

					break;
				case Field::ABILITY:
				case Field::ENEMY:
				case Field::PLAYER:
					for(u8 k = 0; k < 3; k++)
						for(u8 l = 0; l < 3; l++)
							Draw(j * 5 + 1 + l, i * 5 + 1 + k, pixelColor);

					break;
				}
			}
	}

	void Game::update() {
		static u16 timeCounter = 0;
		if(timeCounter == updateFrequency) {
			movePlayer();
			moveEnemies();

			if(abilityCounter > 0)
				abilityCounter--;

			timeCounter = 0;
		}

		timeCounter++;
	}

	void Game::handleEvents() {
		if(GetKey(olc::Key::ESCAPE).bPressed)
			isRunning = false;

		if(GetKey(olc::Key::UP).bPressed)
			player.direction = Direction::UP;

		if(GetKey(olc::Key::DOWN).bPressed)
			player.direction = Direction::DOWN;

		if(GetKey(olc::Key::LEFT).bPressed)
			player.direction = Direction::LEFT;

		if(GetKey(olc::Key::RIGHT).bPressed)
			player.direction = Direction::RIGHT;
	}

	void Game::log() const {
		std::cout << player << std::endl;
	}

	bool Game::OnUserCreate() {
		setInitialPositions();
		isRunning = true;

		return true;
	}

	bool Game::OnUserUpdate(float elapsedTime) {
		if(!isRunning)
			return false;

		handleEvents();

		static float accumulatedTime = 0.0f;

		accumulatedTime += elapsedTime;
		if(accumulatedTime >= targetFrameTime) {
			accumulatedTime -= targetFrameTime;

			log();
			update();
			draw();
		}

		return true;
	}

	void Game::start() {
		Construct(240, 160, 4, 4);
		Start();
	}
}