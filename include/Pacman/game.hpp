#pragma once

#include "misc.hpp"
#include "map.hpp"
#include "entity.hpp"
#include <vector>
#include <string>
#include <olcPixelGameEngine.hpp>
#include <ostream>

namespace Pacman {
	class Game : private olc::PixelGameEngine {
	public:
		Game(const std::string& mapFileName, u16 abilityDuration, float fps, u16 updateFrequency);

		void start();

	private:
		const float targetFrameTime;
		const u16 abilityDuration;
		const u16 updateFrequency;

		Map map;
		Entity player;
		std::vector<Entity> enemies;

		u16 score = 0;
		u16 abilityCounter = 0;
		bool isRunning = false;

		Position generateNewPosition(Position oldPosition, Direction direction) const;
		void moveEntity(Entity& entity);
		void movePlayer();
		void moveEnemies();

		void setInitialPositions();

		void draw() const;
		void update();
		void handleEvents();
		void log() const;

		bool OnUserCreate() override;
		bool OnUserUpdate(float elapsedTime) override;
	};
}