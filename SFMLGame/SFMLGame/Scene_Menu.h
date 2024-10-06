#pragma once

#include "Scene.h"

#include <SFML/Graphics.hpp>
#include <memory>

class Scene_Menu : public Scene
{
protected:

	std::string					m_title;
	std::vector<std::string>	m_menuStrings;
	std::vector<std::string>	m_levelPaths;
	sf::Text					m_menuText;
	size_t						m_selectedMenuIndex = 0;

	void init();
	void onEnd() override;

public:

	Scene_Menu(GameEngine* gameEngine = nullptr);

	void update() override;

	void sRender() override;
	void sDoAction(const Action& action) override;

	//void doAction(const Action& action) override;
};


