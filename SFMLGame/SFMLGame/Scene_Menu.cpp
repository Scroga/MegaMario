#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "GameEngine.h"

#include <fstream>

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine) 
{
	init();
}

void Scene_Menu::init()
{
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::Enter,  "PLAY");
	registerAction(sf::Keyboard::W,		 "UP");
	registerAction(sf::Keyboard::S,		 "DOWN");
	registerAction(sf::Keyboard::Up,	 "UP");
	registerAction(sf::Keyboard::Down,	 "DOWN");

	m_menuText.setFont(m_game->assets().getFont("SuperPlumberBrothers"));
	m_menuText.setCharacterSize(72);
	m_menuText.setScale(1.5f, 1.0f);

	m_menuStrings.push_back("Level 1");
	m_menuStrings.push_back("Level 2");
	m_menuStrings.push_back("Level 3");
	m_menuStrings.push_back("Level 4");

	m_levelPaths.push_back("bin/levels/level1.txt");
	m_levelPaths.push_back("bin/levels/level2.txt");
	m_levelPaths.push_back("bin/levels/level3.txt");
	m_levelPaths.push_back("bin/levels/level4.txt");
}

void Scene_Menu::update()
{
	sRender();

	m_currentFrame++;
}

void Scene_Menu::onEnd()
{
	exit(0);
}

void Scene_Menu::sRender()
{
	const int lineOffset = 50;

	m_game->window().clear(sf::Color(100, 100, 255));

	m_menuText.setString("MegaMario");
	m_menuText.setPosition(m_menuText.getCharacterSize(), lineOffset / 2);
	m_menuText.setFillColor(sf::Color(247, 235, 199));
	m_game->window().draw(m_menuText);

	for(size_t i = 0; i < m_menuStrings.size(); i++)
	{
		m_menuText.setString(m_menuStrings[i]);
		m_menuText.setPosition(m_menuText.getCharacterSize(), lineOffset + (m_menuText.getCharacterSize() + lineOffset) * (i + 1));
		m_menuText.setOutlineThickness(2);
		m_menuText.setOutlineColor(sf::Color((200, 200, 200)));

		// change the color of the text of the selected level
		if(i == m_selectedMenuIndex)	{ m_menuText.setFillColor(sf::Color(255, 107, 99)); }
		else							{ m_menuText.setFillColor(sf::Color(255, 227, 99)); }

		m_game->window().draw(m_menuText);
	}

	m_game->window().display();

}

void Scene_Menu::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
		if (action.name() == "UP")
		{
			if (m_selectedMenuIndex > 0) { m_selectedMenuIndex--; }
			else { m_selectedMenuIndex = m_menuStrings.size() - 1; }
		}
		else if (action.name() == "DOWN")
		{
			m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
		}
		else if (action.name() == "PLAY")
		{
			m_game->changeScene(m_menuStrings[m_selectedMenuIndex], std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]), true);
		}
		else if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}