#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

#include <iostream>
#include <fstream>

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
	: Scene(gameEngine)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::Escape,	"MENU");
	registerAction(sf::Keyboard::P,			"PAUSE");
	registerAction(sf::Keyboard::T,			"TOGGLE_TEXTURE");		// Toggle drawing (T)extures
	registerAction(sf::Keyboard::C,			"TOGGLE_COLLISION");	// Toggle drawing (C)ollision Boxes
	registerAction(sf::Keyboard::G,			"TOGGLE_GRID");			// Toggle drawing (G)rid

	registerAction(sf::Keyboard::Space,		"UP");
	registerAction(sf::Keyboard::W,			"UP");
	registerAction(sf::Keyboard::S,			"DOWN");
	registerAction(sf::Keyboard::D,			"RIGHT");
	registerAction(sf::Keyboard::A,			"LEFT");

	registerAction(sf::Keyboard::Up,		"UP");
	registerAction(sf::Keyboard::Down,		"DOWN");
	registerAction(sf::Keyboard::Right,		"RIGHT");
	registerAction(sf::Keyboard::Left,		"LEFT");

	m_gridText.setCharacterSize(12);
	m_gridText.setFont(m_game->assets().getFont("CooperHewitt"));

	loadLevel(levelPath);
}

Vec2f Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
	if (gridX < 0) gridX *= -1;
	if (gridY < 0) gridY *= -1;

	float posX = (gridX + 1) * m_gridSize.x;
	float posY = (gridY + 1) * m_gridSize.y;

	posX -= (entity->getComponent<CAnimation>().animation.getSize().x / 2.0f);
	posY -= (entity->getComponent<CAnimation>().animation.getSize().y / 2.0f);

	posY = m_game->window().getSize().y - posY;

	return Vec2f(posX, posY);
}

void Scene_Play::loadLevel(const std::string& path)
{
	// reset the entity manager every time we load a level
	m_entityManager = EntityManager();
	m_currentFrame  = 0;

	std::ifstream fin(path);
	if (!fin.is_open())
	{
		std::cerr << "Failed to open file \"" << path << "\" \n";
		exit(-1);
	}

	std::string keyword;

	while (fin >> keyword)
	{
		if (keyword == "Player")
		{
			fin >> m_playerConfig.GX
				>> m_playerConfig.GY
				>> m_playerConfig.CW
				>> m_playerConfig.CH
				>> m_playerConfig.SPEED
				>> m_playerConfig.JUMP
				>> m_playerConfig.GRAVITY
				>> m_playerConfig.WEAPON;
		}

		if (keyword == "Tile")
		{
			std::string tileName;
			int			gridPosX;
			int			gridPosY;

			fin >> tileName >> gridPosX >> gridPosY;

			auto tile = m_entityManager.addEntity(keyword);

			tile->addComponent<CAnimation>(m_game->assets().getAnimation(tileName), true);
			tile->addComponent<CTransform>(gridToMidPixel(gridPosX, gridPosY, tile));
			tile->addComponent<CBoundingBox>(m_game->assets().getAnimation(tileName).getSize());

			// TODO: Check the type of object
		}

		if (keyword == "Dec")
		{
			// TODO:
		}
	}

	spawnPlayer();
}

void Scene_Play::update()
{
	m_entityManager.update();

	if(!m_paused)
	{
		sMovement();
		sCollision();
		sLifeSpan();
		sAnimation();

		m_currentFrame++;
	}
	sRender();
}

void Scene_Play::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
	sf::View view = m_game->window().getView();
	view.setCenter(m_game->window().getSize().x / 2.0f, m_game->window().getSize().y / 2.0f);
	m_game->window().setView(view);

	loadLevel(m_levelPath);
}


#pragma region Systems

void Scene_Play::sMovement()
{
	for(auto& e : m_entityManager.getEntities())
	{
		e->getComponent<CTransform>().position += e->getComponent<CTransform>().velocity;

		if (e->getComponent<CState>().state == "left")  e->getComponent<CTransform>().scale.x = -1;
		if (e->getComponent<CState>().state == "right") e->getComponent<CTransform>().scale.x = 1;
	}

	playerMovement();
}

void Scene_Play::sLifeSpan()
{
	// TODO: Check lifespawn of entities that have them, and destroy them if they go over
}

void Scene_Play::sCollision()
{
	auto& playerTransform	= m_player->getComponent<CTransform>();
	auto& playerState		= m_player->getComponent<CState>();
	auto& playerBBox		= m_player->getComponent<CBoundingBox>();

	for (auto& e : m_entityManager.getEntities("Tile"))
	{
		Vec2f overlap		= Physics::getOverlap(m_player, e);
		Vec2f prevOverlap	= Physics::getPreviousOverlap(m_player, e);

		auto& entityTransform = e->getComponent<CTransform>();

		if (overlap.x > 0 && overlap.y > 0)
		{
			// determine the axis of collision by comparing overlap
			if (overlap.x < overlap.y)
			{
				// handle horizontal collision
				if (playerTransform.prevPosition.x > entityTransform.position.x)
				{
					playerTransform.position.x += overlap.x;  // player moving left hits the right side of the tile
				}
				else if (playerTransform.prevPosition.x < entityTransform.position.x)
				{
					playerTransform.position.x -= overlap.x;  // player moving right hits the left side of the tile
				}

				playerTransform.velocity.x = 0; // stop horizontal movement upon collision
			}
			else
			{
				// handle vertical collision
				if (playerTransform.prevPosition.y > entityTransform.position.y)
				{
					playerTransform.position.y += overlap.y;  // player moving up hits the bottom of the tile
				}
				else if (playerTransform.prevPosition.y < entityTransform.position.y)
				{
					playerTransform.position.y -= overlap.y;  // player moving down hits the top of the tile

					// player is on the ground
					playerState.onGround = true;
				}

				playerTransform.velocity.y = 0; // stop vertical movement upon collision
			}
		}
	}

	if (playerTransform.position.y > height())
	{
		playerTransform.position = gridToMidPixel(m_playerConfig.GX, m_playerConfig.GY, m_player);
	}

	if ((playerTransform.position.x - playerBBox.halfSize.x) < 0)
	{
		playerTransform.position.x = playerBBox.halfSize.x;
	}

	//playerState.onGround = false;

	// TODO: Implement bullet / tile collisions
	//       Destroy the tile if it has a Brick animation
	// TODO: Implement player / tile collisions and resolutions
	//		 Update the CState component of the player to store whether
	//       it is currently on the ground or in the air. This will be
	//       used by the Animation system
	// TODO: Check to see if the player has fallen down a hole (y > height())
	// TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::sAnimation()
{
	// TODO: Complete the Animation class code first

	// TODO: set the animation of the player based on its CState component
	// TODO: for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
	//		 if the animation is not repeated, and it has ended, destroy the entity
}

void Scene_Play::sRender()
{
	// color the background darker so you know that the game is paused
	if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)); }
	else { m_game->window().clear(sf::Color(50, 50, 255)); }

	// set the viewport of the window to be centered on the player if it's far enough right
	float windowCenterX = m_game->window().getSize().x / 2.0f;
	if(m_player)
	{
		auto& pPos = m_player->getComponent<CTransform>().position;
		windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
	}
	sf::View view = m_game->window().getView();
	view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
	m_game->window().setView(view);

	// draw all Entity textures / animations
	if(m_drawTextures)
	{
		for(auto e : m_entityManager.getEntities())
		{
			auto& transform = e->getComponent<CTransform>();

			transform.prevPosition = transform.prevPosition;

			if(e->hasComponent<CAnimation>())
			{
				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.position.x, transform.position.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				m_game->window().draw(animation.getSprite());
			}
		}	
	}

	// draw all Entity collision bounding boxes with a rectangleshape
	if(m_drawCollision)
	{
		for(auto e : m_entityManager.getEntities())
		{
			if(e->hasComponent<CBoundingBox>())
			{
				auto& box = e->getComponent<CBoundingBox>();
				auto& transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
				rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
				rect.setPosition(transform.position.x, transform.position.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setOutlineColor(sf::Color(255, 255, 255, 255));
				rect.setOutlineThickness(1);
				m_game->window().draw(rect);
			}
		}
	}

	// draw the grid so that it will be easier to debug
	if(m_drawGrid)
	{
		float leftX = m_game->window().getView().getCenter().x - width() / 2;
		float rightX = leftX + width() + m_gridSize.x;
		float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

		for(float x = nextGridX; x < rightX; x += m_gridSize.x)
		{
			drawLine(Vec2f(x, 0), Vec2f(x, height()));
		}

		for (float y = 0; y < height(); y += m_gridSize.y)
		{
			drawLine(Vec2f(leftX, height() - y), Vec2f(rightX, height() - y));
		
			for(float x = nextGridX; x < rightX; x += m_gridSize.x)
			{
				std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
				std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
			
				m_gridText.setString("(" + xCell + "," + yCell + ")");
				m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
				m_game->window().draw(m_gridText);
			}
		}
	}

	m_game->window().display();
}

void Scene_Play::sDoAction(const Action& action)
{
	if (action.type() == "START")
	{
			 if (action.name() == "TOGGLE_TEXTURE")		{ m_drawTextures = !m_drawTextures; }
		else if (action.name() == "TOGGLE_COLLISION")	{ m_drawCollision = !m_drawCollision; }
		else if (action.name() == "TOGGLE_GRID")		{ m_drawGrid = !m_drawGrid; }
		else if (action.name() == "PAUSE")				{ setPaused(!m_paused); }
		else if (action.name() == "UP")					{ m_player->getComponent<CInput>().up = true; }
		else if (action.name() == "DOWN")				{ m_player->getComponent<CInput>().down = true; }
		else if (action.name() == "RIGHT")				{ m_player->getComponent<CInput>().right = true; }
		else if (action.name() == "LEFT")				{ m_player->getComponent<CInput>().left = true; }
		else if (action.name() == "MENU")				{ onEnd(); }
	}
	else if (action.type() == "END")
	{
			 if (action.name() == "UP")					{ m_player->getComponent<CInput>().up = false; }
		else if (action.name() == "DOWN")				{ m_player->getComponent<CInput>().down = false; }
		else if (action.name() == "RIGHT")				{ m_player->getComponent<CInput>().right = false; }
		else if (action.name() == "LEFT")				{ m_player->getComponent<CInput>().left = false; }
	}
}

void Scene_Play::sDebug()
{

}

#pragma endregion


#pragma region Mechanics

void Scene_Play::spawnPlayer()
{
	m_player = m_entityManager.addEntity("Player");

	m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
	m_player->addComponent<CTransform>(gridToMidPixel(m_playerConfig.GX, m_playerConfig.GY, m_player));
	m_player->addComponent<CBoundingBox>(Vec2f(m_playerConfig.CW, m_playerConfig.CH));
	m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
	m_player->addComponent<CInput>();
	m_player->addComponent<CState>();
}

void Scene_Play::playerMovement()
{
	auto& playerTransform	= m_player->getComponent<CTransform>();
	auto& playerInput		= m_player->getComponent<CInput>();
	auto& playerState		= m_player->getComponent<CState>();

	const Vec2f velocityMax = { 80.0f , 100.0f };
	const Vec2f velocityMin = { 1.0f  , 1.0f   };
	const Vec2f drag		= { 0.85f , 0.95f  };

	if (playerInput.up && playerState.canJump)
	{
		playerTransform.velocity.y -= (m_playerConfig.JUMP);
		playerState.state = "up";
		playerState.onGround = false;
	}

	if (playerInput.right)
	{
		playerTransform.velocity.x += m_playerConfig.SPEED;
		playerState.state = "right";
	}

	else if (playerInput.left)
	{
		playerTransform.velocity.x -= m_playerConfig.SPEED;
		playerState.state = "left";
	}

	playerTransform.velocity.x *= drag.x;
	playerTransform.velocity.y *= drag.y;

	if (abs(playerTransform.velocity.x) < velocityMin.x)
		playerTransform.velocity.x = 0;

	if (abs(playerTransform.velocity.y) < velocityMin.y)
		playerTransform.velocity.y = 0;

	if (abs(playerTransform.velocity.x) > velocityMax.x)
		playerTransform.velocity.x = velocityMax.x * (playerTransform.velocity.x < 0) ? -1.0f : 1.0f;

	if (abs(playerTransform.velocity.y) > velocityMax.y)
		playerTransform.velocity.y = velocityMax.y * (playerTransform.velocity.y < 0) ? -1.0f : 1.0f;

	if (playerTransform.position != (playerTransform.position + playerTransform.velocity))
		playerTransform.prevPosition = playerTransform.position;

	playerTransform.velocity.y += m_playerConfig.GRAVITY;


	// TODO: count frame for jump
}

void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
	// TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing
}

#pragma endregion