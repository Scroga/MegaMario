#include "Assets.h"

#include <iostream>

Assets::Assets() {}

void Assets::addTexture(const std::string& name, const std::string& path)
{
	if (m_textures.find(name) != m_textures.end()) return;

	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "Failed to open texture \"" << path << "\"" << std::endl;
	}

	m_textures[name] = std::move(texture);
}

void Assets::addFont(const std::string& name, const std::string& path)
{
	if (m_fonts.find(name) != m_fonts.end()) return;

	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cerr << "Failed to open font \"" << path << "\"" << std::endl;
	}

	m_fonts[name] = std::move(font);
}

void Assets::addSound(const std::string& name, const std::string& path)
{
	if (m_sounds.find(name) != m_sounds.end()) return;

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path))
	{
		std::cerr << "Failed to open font \"" << path << "\"" << std::endl;
	}

	sf::Sound sound;
	sound.setBuffer(buffer);
	
	m_sounds[name] = std::move(sound);
}

void Assets::addAnimation(const std::string& name, Animation& animation)
{
	if (m_animation.find(name) != m_animation.end()) return;

	m_animation[name] = animation;
}

sf::Texture& Assets::getTexture(const std::string& name)
{
	return m_textures[name];
}

sf::Font& Assets::getFont(const std::string& name)
{
	return m_fonts[name];
}

sf::Sound& Assets::getSound(const std::string& name)
{
	return m_sounds[name];
}

Animation& Assets::getAnimation(const std::string& name)
{
	return m_animation[name];
}
