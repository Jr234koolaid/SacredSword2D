#include "pch.h"
#include "Player.h"

////////////////////////////// LUA FUNCTIONS //////////////////////////////

ss::Player::Player(const float x, const float y, const std::string name)
	: ss::Entity(x, y, "Player"),
	minWidth(750.f), maxWidth(6000.f), minHeight(450.f), maxHeight(385.f), maxView(290.f),
	interp(0.f)
{
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

ss::Player::~Player()
{
}



////////////////////////////// UPDATE METHODS //////////////////////////////

void ss::Player::MoveCamera(sf::View& playerCamera)
{
	const float mult = 15.f;

	if (velocity.x < 0.f)
		interp -= mult;
	else if (velocity.x > 0.f)
		interp += mult;
	else
	{
		if (interp < 0.f)
		{
			interp += mult;
			interp = std::clamp(interp, -maxView, 0.f);
		}
		else if (interp > 0.f)
		{
			interp -= mult;
			interp = std::clamp(interp, 0.f, maxView);
		}
	}

	float posX = (getX() + getWidth()) - (getWidth() / 2.f) + interp;
	float posY = getY();

	interp = std::clamp(interp, -maxView, maxView);
	posX = std::clamp(posX, minWidth, maxWidth);
	posY = std::clamp(posY, maxHeight, minHeight);

	playerCamera.setCenter(sf::Vector2f(posX, posY));
}

void ss::Player::Update(const float& deltaTime)
{
	calculateVelocity(deltaTime);
	updateHealthBarPosition();

	/*if (facingLeft)
		entityRect.setRotation(10);
	else if (facingRight)
		entityRect.setRotation(-10);
	else
		entityRect.setRotation(0);*/

	//entitySprite.setPosition(sf::Vector2f(entityRect.getPosition().x, entityRect.getPosition().y));
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::Player::Render(sf::RenderTarget* const& target)
{
	target->draw(entityRect);
	target->draw(healthBarFill);
	target->draw(healthBarBorder);

	if (hitRectTimer != 0 && hitRect != nullptr)
		target->draw(*hitRect);
}