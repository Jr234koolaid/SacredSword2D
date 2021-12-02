#ifndef PLAYER_H
#define PLAYER_H

#include "engine/Entity.h"

namespace ss
{
	class Player :
		public ss::Entity
	{
	private:
		const float minWidth, maxWidth, maxHeight, minHeight, maxView;
		float interp;

	public:
		Player(const float x, const float y, const std::string name);
		~Player();

		void MoveCamera(sf::View& playerCamera);

		void Update(const float& deltaTime) override;
		void Render(sf::RenderTarget* const& target) override;
	};
}

#endif