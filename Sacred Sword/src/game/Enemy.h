#ifndef ENEMY_H
#define ENEMY_H

#include "engine/Entity.h"

namespace ss
{
	class Enemy :
		public ss::Entity
	{
	private:
		sf::RectangleShape enemyView;

	public:
		Enemy(const float x, const float y, const float width, const float height, const std::string group);
		~Enemy();

		inline sf::FloatRect getEnemyView() const { return sf::FloatRect(enemyView.getPosition().x, enemyView.getPosition().y, enemyView.getSize().x, enemyView.getSize().y); }

		void FollowPlayer(ss::Entity* const& player, ss::Entity* const& enemy);

		void Update(const float& deltaTime) override;
		void Render(sf::RenderTarget* const& target) override;
	};
}

#endif