#ifndef TE_RENDERER_H
#define TE_RENDERER_H

#include "texture_atlas.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class BaseGameEntity;

	template<typename T>
	class Renderer : public sf::Drawable
	{
	public:
		static std::unique_ptr<Renderer<T>> make(BaseGameEntity& owner)
		{
			return std::unique_ptr<Renderer<T>>{new Renderer<T>{owner}};
		}

		//void setSprite(sf::Texture& texture, const TextureAtlas::Sprite& spriteInfo);
		void setDrawable(const T& drawable) { mDrawable = drawable; }
		void setDrawable(T&& drawable) { mDrawable = std::move(drawable); }

	private:
		Renderer(BaseGameEntity& owner)
			: mOwner{owner}
			, mDrawable{}
		{}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(mDrawable, states);
		}

		BaseGameEntity& mOwner;
		T mDrawable;
	};
}

#endif