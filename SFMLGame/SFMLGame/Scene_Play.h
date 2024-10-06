#pragma once

#include "Scene.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

class Scene_Play : public Scene
{
	struct PlayerConfig { float GX, GY, CW, CH, SPEED, JUMP, GRAVITY; std::string WEAPON; };

protected:

	std::shared_ptr<Entity> m_player;
	std::string				m_levelPath;
	PlayerConfig			m_playerConfig;
	bool					m_drawTextures = true;
	bool					m_drawCollision = false;
	bool					m_drawGrid = false;
	const Vec2f				m_gridSize = { 64.0f , 64.0f };
	sf::Text				m_gridText;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& path);
	void onEnd() override;

public:

	Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

	void update() override;

	Vec2f gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

	void sMovement();
	void sLifeSpan();
	void sCollision();
	void sAnimation();
	void sRender() override;
	void sDoAction(const Action& action) override;
	void sDebug();

	//void doAction(const Action& action) override;

	void spawnPlayer();
	void playerMovement();
	void spawnBullet(std::shared_ptr<Entity> entity);
};