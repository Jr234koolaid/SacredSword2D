#include "pch.h"
#include "Entity.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::Entity::Entity(const float x, const float y, const std::string tag, const float hp)
	: maxHealth(hp), gravity(600.f), jumpF(-450.f), fallM(5.f),
	accel(19.5f), decel(accel - 4.f), maxVel(350.f), hp(maxHealth),
	jumpT(90), hitRectT(25),
	level(1), jumpTimer(0), hitRectTimer(0),
	isGrounded(false), isColliding(false), isDead(false),
	tag(tag),
	velocity(0.f, 0.f)
{
	entityRect.setSize(sf::Vector2f(175.f, 385.f)); // 225.f, 475.f
	entityRect.setPosition(sf::Vector2f(x, y));
	entityRect.setFillColor(sf::Color::Transparent);
	entityRect.setOutlineColor(sf::Color::Green);
	entityRect.setOutlineThickness(1.5f);

	{
		healthBarBorder.setSize(sf::Vector2f(hp * 1.5f, 10.f));

		const float xPos = entityRect.getPosition().x + (entityRect.getSize().x / 2.f) - (healthBarBorder.getSize().x / 2.f);
		const float yPos = entityRect.getPosition().y - healthBarBorder.getSize().y - 25.f;

		healthBarBorder.setPosition(sf::Vector2f(xPos, yPos));
		healthBarBorder.setFillColor(sf::Color::Transparent);
		healthBarBorder.setOutlineColor(sf::Color::White);
		healthBarBorder.setOutlineThickness(1.5f);
	}

	healthBarFill.setSize(sf::Vector2f(hp * 1.5f, healthBarBorder.getSize().y));
	healthBarFill.setPosition(sf::Vector2f(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y));
	healthBarFill.setFillColor(sf::Color::Green);
	healthBarFill.setOutlineColor(sf::Color::Transparent);
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Entity::~Entity()
{
	if (hitRect != nullptr)
		deletePointers(hitRect);
}



////////////////////////////// METHODS //////////////////////////////

sf::FloatRect ss::Entity::getHitRect() const
{
	if (hitRect != nullptr)
		return sf::FloatRect(hitRect->getPosition().x, hitRect->getPosition().y, hitRect->getSize().x, hitRect->getSize().y);

	return sf::FloatRect(-10000.f, -10000.f, 0.f, 0.f);
}

void ss::Entity::createHitRect()
{
	if (hitRect == nullptr)
	{
		hitRect = new sf::RectangleShape();
		hitRectTimer = hitRectT;

		hitRect->setSize(sf::Vector2f(200.f, 150.f));

		float xPos;

		if (velocity.x < 0.f)
			xPos = (entityRect.getPosition().x - hitRect->getSize().x);
		else
			xPos = (entityRect.getPosition().x + entityRect.getSize().x);

		const float yPos = (entityRect.getPosition().y + 150.f);

		hitRect->setPosition(sf::Vector2f(xPos, yPos));
		hitRect->setFillColor(sf::Color::Transparent);
		hitRect->setOutlineColor(sf::Color::White);
		hitRect->setOutlineThickness(1.5f);
	}
}

void ss::Entity::setTexture(const std::string path, sf::RenderWindow* const &rWindow)
{
	if (!entityTexture.loadFromFile(path))
		std::cout << "ENTITY::ERROR::Error loading sprite from: " << path << std::endl;

	entitySprite.setTexture(entityTexture);
	entitySprite.setScale(sf::Vector2f(entityRect.getSize().x / entitySprite.getGlobalBounds().width, entityRect.getSize().y / entitySprite.getGlobalBounds().height));
	entitySprite.setPosition(sf::Vector2f(entityRect.getPosition().x, entityRect.getPosition().y));
	
	set_texture = true;
}

void ss::Entity::updateHealthBarPosition()
{
	if (hp <= 0.f)
		isDead = true;

	hp = std::clamp(hp, 0.f, maxHealth);

	const float xPos = entityRect.getPosition().x + (entityRect.getSize().x / 2.f) - (healthBarBorder.getSize().x / 2.f);
	const float yPos = entityRect.getPosition().y - healthBarBorder.getSize().y - 25.f;

	healthBarBorder.setPosition(sf::Vector2f(xPos, yPos));
	healthBarFill.setSize(sf::Vector2f(hp * 1.5f, healthBarBorder.getSize().y));
	healthBarFill.setPosition(sf::Vector2f(healthBarBorder.getPosition().x, healthBarBorder.getPosition().y));
}

const bool ss::Entity::checkCollision(ss::Entity* const& obj1, ss::Entity* const& obj2)
{
	sf::FloatRect bounds1(obj1->getX(), obj1->getY(), obj1->getWidth(), obj1->getHeight() + 0.1f);
	sf::FloatRect bounds2(obj2->getX(), obj2->getY(), obj2->getWidth(), obj2->getHeight());

	if (bounds1.intersects(bounds2))
	{
		if (obj2->tag.compare("Ground") == 0)
		{
			obj1->isGrounded = true;
			obj1->setPosition(obj1->getX(), obj2->getY() - obj1->getHeight());

			return true;
		}
		else if (obj2->tag.compare("Bounds") == 0)
		{
			if (obj1->getX() < 1000.f)
				obj1->setX(obj1->getX() + 15.f);
			else
				obj1->setX(obj1->getX() - 15.f);
			obj1->setXVelocity(0.f);

			return true;
		}

		return true;
	}
	else
		obj1->isGrounded = false;

	return false;
}

const bool ss::Entity::checkCollision(sf::FloatRect obj1, sf::FloatRect obj2)
{
	if (obj1.intersects(obj2))
		return true;

	return false;
}

void ss::Entity::calculateVelocity(const float& deltaTime)
{
	if (!isGrounded)
	{
		if (velocity.y > 0.f)
			velocity.y += gravity * (fallM - 1) * deltaTime;
		else
			velocity.y += gravity * deltaTime;

		velocity.y = std::clamp(velocity.y, jumpF, 500.f);
	}
	else
		velocity.y = 0.f;

	if (velocity.x > 0.f)
	{
		velocity.x -= decel;

		velocity.x = std::clamp(velocity.x, 0.f, maxVel);
	}
	else if (velocity.x < 0.f)
	{
		velocity.x += decel;

		velocity.x = std::clamp(velocity.x, -maxVel, 0.f);
	}

	entityRect.move(velocity * deltaTime);

	if (jumpTimer != 0)
	{
		jumpTimer--;

		jumpTimer = static_cast<int>(std::clamp(static_cast<float>(jumpTimer), 0.f, static_cast<float>(jumpT)));
	}

	if (hitRectTimer != 0 && hitRect != nullptr)
	{
		hitRectTimer--;

		float xPos;

		if (velocity.x < 0.f)
			xPos = (entityRect.getPosition().x - hitRect->getSize().x);
		else
			xPos = (entityRect.getPosition().x + entityRect.getSize().x);

		const float yPos = (entityRect.getPosition().y + 150.f);

		hitRect->setPosition(sf::Vector2f(xPos, yPos));
	}
	else if (hitRectTimer == 0 && hitRect != nullptr)
	{
		delete hitRect;
		hitRect = nullptr;
	}
}

void ss::Entity::Jump()
{
	if (jumpTimer <= 0)
	{
		velocity.y = jumpF;
		isGrounded = false;
		jumpTimer = jumpT;
	}
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::Entity::Update(const float& deltaTime)
{
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::Entity::Render(sf::RenderTarget* const& target)
{
}