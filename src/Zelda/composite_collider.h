#ifndef TE_COMPOSITE_COLLIDER_H
#define TE_COMPOSITE_COLLIDER_H

#include "collider.h"
#include "box_collider.h"
#include "wall.h"

#include <vector>

namespace te
{
	class BoxCollider;

	class CompositeCollider : public Collider
	{
	public:
		void addCollider(const BoxCollider& collider);
		//virtual std::vector<Wall2f> getWalls() const;
		const std::vector<Wall2f>& getWalls() const;

		bool contains(float x, float y) const;
		bool intersects(const BoxCollider&) const;
		bool intersects(const BoxCollider&, sf::FloatRect&) const;
		bool intersects(const CompositeCollider&) const;
		bool intersects(const CompositeCollider&, sf::FloatRect&) const;

		CompositeCollider transform(const sf::Transform&) const;
		void createFixtures(b2Body& body, std::vector<b2Fixture*>& outFixtures) const;

	private:
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
		std::vector<BoxCollider> mBoxColliders;
		std::vector<Wall2f> mWalls;
	};
}

#endif
