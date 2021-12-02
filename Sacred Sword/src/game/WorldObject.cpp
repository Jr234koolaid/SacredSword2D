#include "pch.h"
#include "WorldObject.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::WorldObject::WorldObject(const float x, const float y, const float width, const float height, const std::string tag)
	: ss::Entity(x, y, tag),
	startPos(x), pEffect(0.f)
{
	setSize(width, height);
	setRectColor(sf::Color::Blue);
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::WorldObject::~WorldObject()
{
	std::cout << "~WORLDOBJECT::Deleted a WorldObject" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::WorldObject::Update(const float& deltaTime)
{
	entitySprite.setPosition(sf::Vector2f(entityRect.getPosition().x, entityRect.getPosition().y));
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::WorldObject::Render(sf::RenderTarget* const& target)
{
	if (set_texture)
		target->draw(entitySprite);
	else
		target->draw(entityRect);
}