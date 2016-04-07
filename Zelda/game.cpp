#include "game.h"
#include "tile_map.h"
#include "vector_ops.h"
#include "entity_manager.h"
#include "message_dispatcher.h"
#include "scene_node.h"

namespace te
{
	Game::Game()
		: mpEntityManager(EntityManager::make())
		, mpMessageDispatcher(MessageDispatcher::make(*mpEntityManager))
		, mpWorld(new b2World(b2Vec2(0, 0)))
		, mTileMapID(-1)
		, mpTileMap(nullptr)
		, mpSceneGraph(SceneNode::make(*this, { 0, 0 }))
	{}

	Game::~Game() {}

	bool Game::isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius) const
	{
		throwIfNoMap();
		auto& walls = mpTileMap->getWalls();

		sf::Vector2f toB = normalize(b - a);
		sf::Vector2f currPos = a;

		while (distanceSq(currPos, b) > boundingRadius * boundingRadius)
		{
			currPos += toB * 0.5f * boundingRadius;
			for (auto& wall : walls)
				if (wall.intersects(currPos, boundingRadius))
					return true;
		}

		return false;
	}

	const TileMap& Game::getMap() const
	{
		throwIfNoMap();
		return *mpTileMap;
	}

	TileMap& Game::getMap()
	{
		throwIfNoMap();
		return *mpTileMap;
	}

	void Game::update(const sf::Time& dt)
	{
		mpMessageDispatcher->dispatchDelayedMessages(dt);
		mpWorld->Step(dt.asSeconds(), 8, 3);
		mpSceneGraph->update(dt);
	}

	EntityManager& Game::getEntityManager()
	{
		return *mpEntityManager;
	}

	MessageDispatcher& Game::getMessageDispatcher()
	{
		return *mpMessageDispatcher;
	}

	b2World& Game::getPhysicsWorld() { return *mpWorld; }
	const b2World& Game::getPhysicsWorld() const { return *mpWorld; }

	void Game::setTileMap(std::unique_ptr<TileMap>&& pTileMap)
	{
		if (pTileMap)
		{
			mTileMapID = pTileMap->getID();
			mpTileMap = pTileMap.get();
			mpSceneGraph->attachNode(std::move(pTileMap));
		}
		else
		{
			throw std::runtime_error("Must supply TileMap to Game.");
		}
	}

	void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		throwIfNoMap();
		states.transform *= getTransform();
		target.draw(*mpSceneGraph, states);
	}

	SceneNode& Game::getSceneGraph()
	{
		return *mpSceneGraph;
	}

	void Game::throwIfNoMap() const
	{
		if (!mpEntityManager->hasEntity(mTileMapID)) throw std::runtime_error("TileMap not set in Game.");
	}
}
