#include "GameEngine.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"

#include <iostream>
#include <fstream>

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::debug()
{
	std::cout << "Current Scene: " + m_currentScene << "\n";
}

void GameEngine::init(const std::string& path)
{
	loadAssets(path);

	m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
	m_window.setFramerateLimit(60);

	changeScene("MENU", std::make_shared<Scene_Menu>(this), false);
}

void GameEngine::loadAssets(const std::string& path)
{
	std::ifstream fin(path);
	if (!fin.is_open())
	{
		std::cerr << "Failed to open file \"" << path << "\" \n";
		exit(-1);
	}

	std::string assetName;
	std::string assetPath;

	std::string keyword;

	while (fin >> keyword)
	{
		if (keyword == "Font")
		{
			fin >> assetName >> assetPath;
			m_assets.addFont(assetName, assetPath);
		}

		if (keyword == "Texture")
		{
			fin >> assetName >> assetPath;
			m_assets.addTexture(assetName, assetPath);
		}

		if (keyword == "Animation")
		{
			std::string tName;
			size_t		frameCount = 1;
			size_t		animSpeed = 0;

			fin >> assetName >> tName >> frameCount >> animSpeed;

			Animation animation(assetName, m_assets.getTexture(tName), frameCount, animSpeed);
			m_assets.addAnimation(assetName, animation);
		}

		if (keyword == "Sound")
		{
			// TODO:
		}
	}
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	while(isRunning())
	{
		sUserInput();
		update();
	}
}

void GameEngine::update()
{
	currentScene()->update();
}

void GameEngine::quit()
{
	exit(0);
}

Assets& GameEngine::assets()
{
	return m_assets;
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	if (m_sceneMap.find(sceneName) == m_sceneMap.end())
	{
		m_sceneMap.insert({ sceneName, scene });
	}

	if (endCurrentScene || m_currentScene == "NONE")
	{
		m_currentScene = sceneName;
	}
	debug();
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while(m_window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			quit();
		}

		if(event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "screenshot saved to " << "test.png" << std::endl;
				sf::Texture texture;
				texture.create(m_window.getSize().x, m_window.getSize().y);
				texture.update(m_window);
				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "screenshot saved to " << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// if the current scene does not have an action associated with this key, skip the event
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
			   { continue; }

			// determine start or end action bu whether it was key pres or release
			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			// look up the action and send the action to the scene
			currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}