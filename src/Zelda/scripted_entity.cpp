#include "scripted_entity.h"
#include "utilities.h"
#include "texture_manager.h"
#include "scripted_game.h"
#include "message_dispatcher.h"
#include "tile_map.h"
#include "entity_manager.h"
#include "rigid_body.h"

#include <Box2D/Box2D.h>

namespace te
{
	std::unique_ptr<ScriptedEntity> ScriptedEntity::make(ScriptedGame& world, luabridge::LuaRef entityTable, luabridge::LuaRef argsTable, sf::Vector2f position)
	{
		return std::unique_ptr<ScriptedEntity>{new ScriptedEntity{world, entityTable, argsTable, position}};
	}

	ScriptedEntity::ScriptedEntity(ScriptedGame& world, luabridge::LuaRef entityTable, luabridge::LuaRef argsTable, sf::Vector2f position)
		: BaseGameEntity{world, position}
		, mWorld{world}
		, mUserData{luabridge::newTable(entityTable)}
		, mStateMachines{}
	{
		if (entityTable.isTable())
		{
			if (entityTable["init"].isFunction()) entityTable["init"](this, &mWorld, argsTable);
			else throw std::runtime_error{"Entity package must include init function."};
		}
		else
		{
			throw std::runtime_error{"Invalid entity package for ScriptedEntity."};
		}
	}

	void ScriptedEntity::onUpdate(const sf::Time& dt)
	{
		for (auto& fsm : mStateMachines) fsm.update(dt);
	}

	bool ScriptedEntity::handleMessage(const Telegram& msg)
	{
		if (dynamic_cast<ScriptedGame::ScriptedInfo*>(msg.pInfo.get()) == nullptr) return false;

		ScriptedGame::ScriptedTelegram scriptedGram{msg.dispatchTime, msg.sender, msg.receiver, ((ScriptedGame::ScriptedInfo*)msg.pInfo.get())->ref};
		bool result = false;
		for (auto& fsm : mStateMachines) result = fsm.handleMessage(scriptedGram) || result;
		return result;
	}

	luabridge::LuaRef ScriptedEntity::getUserData() const
	{
		return mUserData;
	}

	void ScriptedEntity::initMachine(luabridge::LuaRef state)
	{
		mStateMachines.push_back({state, *this});
	}

	//void ScriptedEntity::setPositionByTile(int x, int y, EntityID mapID)
	//{
	//	auto& map = getWorld().getEntityManager().getEntityFromID(mapID);
	//	assert(dynamic_cast<TileMap*>(&map) != nullptr);
	//	setPosition(static_cast<TileMap&>(map).getTileToWorldTransform().transformPoint(0.5f + x, 0.5f + y));
	//}
}
