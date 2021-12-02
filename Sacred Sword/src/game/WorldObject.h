#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "engine/Entity.h"

namespace ss
{
	class WorldObject :
		public ss::Entity
	{
	private:
		float startPos, pEffect;

	public:
		WorldObject(const float x, const float y, const float width = 1200.f, const float height = 35.f, const std::string tag = "Ground");
		~WorldObject();

		inline const float getParallaxEffect() const { return pEffect; }
		inline const float getStartPos() const { return startPos; }

		inline void setParallaxEffect(const float set) { pEffect = set; }

		void Update(const float& deltaTime) override;
		void Render(sf::RenderTarget* const& target) override;
	};
}

#endif