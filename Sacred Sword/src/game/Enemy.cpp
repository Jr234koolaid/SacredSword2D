#include "pch.h"
#include "Enemy.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::Enemy::Enemy(const float x, const float y, const float width, const float height, const std::string group)
	: ss::Entity(x, y, "Enemy")
{
	setRectColor(sf::Color::Red);
	setSize(width, height);
	setAcceleration(10.15f);
	setDeceleration(accel - 3.55f);
	setMaxVelocity(305.f);

	enemyView.setSize(sf::Vector2f(600.f, 600.f));

	const float xPos = entityRect.getPosition().x - (enemyView.getSize().x / 2.f) + (entityRect.getSize().x / 2.f);
	const float yPos = entityRect.getPosition().y - (enemyView.getSize().y / 2.f) + (entityRect.getSize().y / 2.f);

	enemyView.setPosition(sf::Vector2f(xPos, yPos));
	enemyView.setFillColor(sf::Color::Transparent);
	enemyView.setOutlineColor(sf::Color::Magenta);
	enemyView.setOutlineThickness(1.5f);
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Enemy::~Enemy()
{
	std::cout << "~ENEMY::Deleted an enemy" << std::endl;
}



////////////////////////////// UPDATE METHODS //////////////////////////////

void ss::Enemy::FollowPlayer(ss::Entity* const& player, ss::Entity* const& enemy)
{
	if (enemy->getX() < player->getX())
		MoveEntity(1);
	else if (enemy->getX() > player->getX())
		MoveEntity(-1);
	else
		MoveEntity(0);
}

void ss::Enemy::Update(const float& deltaTime)
{
	calculateVelocity(deltaTime);
	updateHealthBarPosition();

	const float xPos = entityRect.getPosition().x - (enemyView.getSize().x / 2.f) + (entityRect.getSize().x / 2.f);
	const float yPos = entityRect.getPosition().y - (enemyView.getSize().y / 2.f) + (entityRect.getSize().y / 2.f);

	enemyView.setPosition(sf::Vector2f(xPos, yPos));
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::Enemy::Render(sf::RenderTarget* const& target)
{
	target->draw(entityRect);
	target->draw(healthBarFill);
	target->draw(healthBarBorder);
	target->draw(enemyView);
}