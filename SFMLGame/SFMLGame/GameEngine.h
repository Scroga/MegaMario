#pragma once

#include "Scene.h"
#include "Assets.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <map>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:
	sf::RenderWindow	m_window;
	Assets				m_assets;
	std::string			m_currentScene = "NONE";
	SceneMap			m_sceneMap;
	size_t				m_simulationSpeed = 1;
	bool				m_running = true;

	void init(const std::string& path);
	void update();

	void sUserInput();

	std::shared_ptr<Scene> currentScene();
	void loadAssets(const std::string& path);

public:
	GameEngine(const std::string& path);

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void quit();
	void run();

	void debug();

	sf::RenderWindow& window();
	Assets& assets();
	bool isRunning();
};