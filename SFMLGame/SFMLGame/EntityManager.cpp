#include "EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::update()
{
	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	for (auto& pair : m_entityMap)
	{
		auto& tag = pair.first;
		auto& entityVec = pair.second;

		removeDeadEntities(entityVec);
	}

	// add entities from m_entitiesToAdd to the proper location(s)
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);

		m_entityMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();

}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity>& e)
		{
			return !e->isActive();
		}), vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}