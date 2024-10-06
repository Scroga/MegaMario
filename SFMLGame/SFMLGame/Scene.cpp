#include "GameEngine.h"
#include "Scene.h"

#include <iostream>

Scene::Scene() {}

Scene::Scene(GameEngine* gameEngine) 
	: m_game(gameEngine) {}

void Scene::setPaused(bool paused)
{
	m_paused = paused;
}

void Scene::simulate(const size_t frames)
{
	// TODO:
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
	if(m_actionMap.find(inputKey) == m_actionMap.end())
	{
		m_actionMap.insert({inputKey, actionName});
	}
}

size_t Scene::width() const
{
	return m_game->window().getSize().x;
}

size_t Scene::height() const
{
	return m_game->window().getSize().y;
}


size_t Scene::currentFrame() const
{
	return m_currentFrame;
}

bool Scene::hasEnded() const
{
	return m_hasEnded;
}

const ActionMap& Scene::getActionMap() const
{
	return m_actionMap;
}

void Scene::drawLine(const Vec2f& p1, const Vec2f& p2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};

	m_game->window().draw(line, 2, sf::Lines);
}

void Scene::doAction(const Action& action)
{
	//std::cout << action.toString() << "\n";

	sDoAction(action);
}
